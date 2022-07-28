/* * NRP Core - Backend infrastructure to synchronize simulations
 *
 * Copyright 2020-2021 NRP Team
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * This project has received funding from the European Union’s Horizon 2020
 * Framework Programme for Research and Innovation under the Specific Grant
 * Agreement No. 945539 (Human Brain Project SGA3).
 */

#include "nrp_simulation/simulation/nrp_core_server.h"

NrpCoreServer::NrpCoreServer(const std::string & address, std::shared_ptr<SimulationManager> && manager)
    : _lock(_mutex, std::defer_lock_t()),
    _manager(std::move(manager))
{
    grpc::ServerBuilder builder;
    builder.AddListeningPort(address, grpc::InsecureServerCredentials());
    builder.RegisterService(this);

    this->_server = builder.BuildAndStart();
}

void NrpCoreServer::runServerLoop()
{
    bool isShutdown = false;

    while(true)
    {
        this->waitForRequest();
        SimulationManager::RequestResult result;
        std::string data("");
        // Check the request type and handle it accordingly
        try
        {
            switch(this->getRequestType())
            {
                case NrpCoreServer::RequestType::Initialize:
                    result = _manager->initializeSimulation();
                    break;
                case NrpCoreServer::RequestType::RunLoop:
                    result = _manager->runSimulation(this->getRequestNumIterations());
                    data   = _manager->getStatus();
                    break;
                case NrpCoreServer::RequestType::RunUntilTimeout:
                    result = _manager->runSimulationUntilTimeout();
                    break;
                case NrpCoreServer::RequestType::StopLoop:
                    result = _manager->stopSimulation();
                    break;
                case NrpCoreServer::RequestType::Reset:
                    result = _manager->resetSimulation();
                    break;
                case NrpCoreServer::RequestType::Shutdown:
                    try {
                        result = _manager->shutdownSimulation();
                    }
                    catch (std::logic_error&) {
                        // it's ok the server is going to be closed anyways
                    }

                    isShutdown = true;
                    break;
                default:
                    throw NRPException::logCreate("Unknown request received");
            }

        }
        // Usually because an illegal transition was requested,
        // exceptions in executing requests are handled by SimulatorManager by transitioning to "Failed" state
        catch(std::exception &e)
        {
            result.errorMessage = e.what();
        }

        this->markRequestAsProcessed(result, data);

        // Break out of the loop, if shutdown was requested
        if(isShutdown)
            break;
    }
}

void NrpCoreServer::stopServerLoop()
{
    std::lock_guard<std::mutex> lock(this->_mutex);
    this->_requestType = RequestType::Shutdown;
    this->_consumerConditionalVar.notify_one();
}

NrpCoreServer::RequestType NrpCoreServer::getRequestType() const
{
    return this->_requestType;
}

unsigned NrpCoreServer::getRequestNumIterations() const
{
    return this->_numIterations;
}

void NrpCoreServer::waitForRequest()
{
    this->_lock.lock();
    this->_consumerConditionalVar.wait(_lock, [this]{ return this->isRequestPending(); });
}

void NrpCoreServer::markRequestAsProcessed(const SimulationManager::RequestResult & result,
                                           const std::string                      & data)
{
    this->_requestResult = result;
    this->_jsonData = data;
    this->resetRequest();
    this->_producerConditionalVar.notify_one();
    this->_lock.unlock();
}

void NrpCoreServer::resetRequest()
{
    this->_requestType   = RequestType::None;
    this->_numIterations = 0;
}

bool NrpCoreServer::isRequestPending() const
{
    return (this->_requestType != RequestType::None);
}

grpc::Status NrpCoreServer::requestHelper(std::unique_lock<std::mutex> & lock, RequestType requestType, NrpCore::Response * returnMessage)
{
    // Set the event type

    this->_requestType = requestType;

    // Notify the main thread that a new event is pending

    this->_consumerConditionalVar.notify_one();

    // Wait for the main thread to finish processing of the new event

    this->_producerConditionalVar.wait(lock, [this] { return !this->isRequestPending(); });

    // In case of a failure during request processing, send back the error message and status
    if(this->_requestResult.currentState == SimulationManager::SimState::NotSet) {
        return grpc::Status(grpc::StatusCode::CANCELLED, this->_requestResult.errorMessage);
    }
    else {
        // Set simulation state after processing the request
        setReturnMessageContent(this->_requestResult, returnMessage->mutable_simstate());

        // Set the JSON data field if there's anything pending in the _jsonData string

        if(!this->_jsonData.empty())
        {
            returnMessage->set_json(this->_jsonData);
        }
        return grpc::Status::OK;
    }
}

void NrpCoreServer::setReturnMessageContent(const SimulationManager::RequestResult& res, NrpCore::SimStateMessage * returnMessage)
{
    returnMessage->set_currentstate(_manager->printSimState(res.currentState));
    returnMessage->set_errormsg(res.errorMessage);
}

grpc::Status NrpCoreServer::initialize(grpc::ServerContext * , const NrpCore::EmptyMessage * , NrpCore::Response * returnMessage)
{
    std::unique_lock<std::mutex> lock(this->_mutex);
    return requestHelper(lock, RequestType::Initialize, returnMessage);
}

grpc::Status NrpCoreServer::runLoop(grpc::ServerContext * , const NrpCore::RunLoopMessage * message, NrpCore::Response * status)
{
    std::unique_lock<std::mutex> lock(this->_mutex);

    this->_numIterations = message->numiterations();
    return requestHelper(lock, RequestType::RunLoop, status);
}

grpc::Status NrpCoreServer::runUntilTimeout(grpc::ServerContext * , const NrpCore::EmptyMessage * , NrpCore::Response * returnMessage)
{
    std::unique_lock<std::mutex> lock(this->_mutex);
    return requestHelper(lock, RequestType::RunUntilTimeout, returnMessage);
}

grpc::Status NrpCoreServer::shutdown(grpc::ServerContext * , const NrpCore::EmptyMessage * , NrpCore::Response * returnMessage)
{
    std::unique_lock<std::mutex> lock(this->_mutex);
    return requestHelper(lock, RequestType::Shutdown, returnMessage);
}

grpc::Status NrpCoreServer::reset(grpc::ServerContext * , const NrpCore::EmptyMessage * , NrpCore::Response * returnMessage)
{
    std::unique_lock<std::mutex> lock(this->_mutex);
    return requestHelper(lock, RequestType::Reset, returnMessage);
}

grpc::Status NrpCoreServer::stopLoop(grpc::ServerContext * , const NrpCore::EmptyMessage * , NrpCore::Response * returnMessage)
{
    // Stop is the only request which is allowed to be processed in thread and without lock
    SimulationManager::RequestResult res;
    try {
        res = this->_manager->stopSimulation();
        setReturnMessageContent(res, returnMessage->mutable_simstate());
        return grpc::Status::OK;
    }
    catch(std::exception &e)
    {
        return grpc::Status(grpc::StatusCode::CANCELLED, e.what());
    }
}

// EOF
