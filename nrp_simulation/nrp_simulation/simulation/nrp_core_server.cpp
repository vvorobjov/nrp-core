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

NrpCoreServer::NrpCoreServer(const std::string & address)
    : _lock(_mutex, std::defer_lock_t())
{
    grpc::ServerBuilder builder;
    builder.AddListeningPort(address, grpc::InsecureServerCredentials());
    builder.RegisterService(this);

    this->_server = builder.BuildAndStart();
}

NrpCoreServer::RequestType NrpCoreServer::getRequestType() const
{
    return this->_requestType;
}

unsigned NrpCoreServer::getNumIterations() const
{
    return this->_numIterations;
}

void NrpCoreServer::waitForRequest()
{
    this->_lock.lock();
    this->_consumerConditionalVar.wait(_lock, [this]{ return this->isRequestPending(); });
}

void NrpCoreServer::markRequestAsProcessed()
{
    this->resetRequest();
    this->_producerConditionalVar.notify_one();
    this->_lock.unlock();
}

void NrpCoreServer::markRequestAsFailed(const std::string & message)
{
    this->_requestStatus.failed       = true;
    this->_requestStatus.errorMessage = message;
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

grpc::Status NrpCoreServer::requestHelper(std::unique_lock<std::mutex> & lock, RequestType requestType)
{
    // Set the event type

    this->_requestType = requestType;

    // Notify the main thread that a new event is pending

    this->_consumerConditionalVar.notify_one();

    // Wait for the main thread to finish processing of the new event

    this->_producerConditionalVar.wait(lock, [this] { return !this->isRequestPending(); });

    // In case of a failure during request processing, send back the error message and status

    if(this->_requestStatus.failed)
    {
        this->_requestStatus.failed = false;
        return grpc::Status(grpc::StatusCode::CANCELLED, this->_requestStatus.errorMessage);
    }

    return grpc::Status::OK;
}

grpc::Status NrpCoreServer::initialize(grpc::ServerContext * , const NrpCore::EmptyMessage * , NrpCore::EmptyMessage *)
{
    std::unique_lock<std::mutex> lock(this->_mutex);
    return requestHelper(lock, RequestType::Init);
}

grpc::Status NrpCoreServer::runLoop(grpc::ServerContext * , const NrpCore::RunLoopMessage * message, NrpCore::EmptyMessage *)
{
    std::unique_lock<std::mutex> lock(this->_mutex);

    this->_numIterations = message->numiterations();
    return requestHelper(lock, RequestType::RunLoop);
}

grpc::Status NrpCoreServer::shutdown(grpc::ServerContext * , const NrpCore::EmptyMessage * , NrpCore::EmptyMessage *)
{
    std::unique_lock<std::mutex> lock(this->_mutex);
    return requestHelper(lock, RequestType::Shutdown);
}

// EOF
