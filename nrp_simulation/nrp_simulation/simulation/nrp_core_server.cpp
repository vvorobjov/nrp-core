/* * NRP Core - Backend infrastructure to synchronize simulations
 *
 * Copyright 2020-2023 NRP Team
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
#include "nrp_general_library/engine_interfaces/engine_client_interface.h"
#include "nrp_json_engine_protocol/datapack_interfaces/json_datapack.h"
#include "nrp_protobuf/proto_ops/proto_ops_manager.h"
#include "nrp_protobuf/proto_libraries.h"

NrpCoreServer::NrpCoreServer(const std::string & address, std::shared_ptr<SimulationManager> && manager)
    : _lock(_mutex, std::defer_lock_t()),
    _manager(std::move(manager))
{

    ProtoOpsManager::getInstance().addPluginPath(NRP_PLUGIN_INSTALL_DIR);

    // Split semicolon-delimited string into a vector of string with library names

    std::vector<std::string> libs;
    std::string str(nrp_protobuf::PROTO_LIBRARIES);
    std::string delimiter = ";";
    size_t pos = 0;
    std::string token;
    while((pos = str.find(delimiter)) != std::string::npos)
    {
        token = str.substr(0, pos);
        libs.push_back(token);
        str.erase(0, pos + delimiter.length());
    }
    libs.push_back(str);

    // Load the libraries with protobuf conversion operations

    for(const auto & packageName : libs)
    {
        auto pluginLib = ProtoOpsManager::getInstance().loadProtobufPlugin(packageName);
        if(pluginLib)
            _protoOps.template emplace_back(std::move(pluginLib));
        else
            throw NRPException::logCreate("Failed to load ProtobufPackage \""+packageName+"\"");
    }


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
                    break;
                case NrpCoreServer::RequestType::RunUntilTimeout:
                    result = _manager->runSimulationUntilDoneOrTimeout();
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

        this->markRequestAsProcessed(result);

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

void NrpCoreServer::markRequestAsProcessed(const SimulationManager::RequestResult & result)
{
    this->_requestResult = result;
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

grpc::Status NrpCoreServer::requestHelper(std::unique_lock<std::mutex> & lock, RequestType requestType, NrpCore::SimStateMessage * stateMessage)
{
    // Set the event type

    this->_requestType = requestType;

    // Notify the main thread that a new event is pending

    this->_consumerConditionalVar.notify_one();

    // Wait for the main thread to finish processing of the latest event

    this->_producerConditionalVar.wait(lock, [this] { return !this->isRequestPending(); });

    // In case of a failure during request processing, send back the error message and status
    if(this->_requestResult.currentState == SimulationManager::SimState::NotSet) {
        return grpc::Status(grpc::StatusCode::CANCELLED, this->_requestResult.errorMessage);
    }
    else {
        // Set simulation state after processing the request
        setReturnMessageContent(this->_requestResult, stateMessage);

        return grpc::Status::OK;
    }
}

void NrpCoreServer::setReturnMessageContent(const SimulationManager::RequestResult& res, NrpCore::SimStateMessage * returnMessage)
{
    returnMessage->set_currentstate(_manager->printSimState(res.currentState));
    returnMessage->set_errormsg(res.errorMessage);
}

grpc::Status NrpCoreServer::initialize(grpc::ServerContext * , const NrpCore::EmptyMessage * , NrpCore::InitializeResponse * returnMessage)
{
    std::unique_lock<std::mutex> lock(this->_mutex);

    grpc::Status status = requestHelper(lock, RequestType::Initialize, returnMessage->mutable_simstate());
    if(status.ok())
    {
        this->prepareTrajectory(returnMessage->mutable_trajectory());
    }

    return status;
}


void NrpCoreServer::prepareTrajectory(NrpCore::Trajectory * trajectory)
{
    auto & trajectoryVector = this->_manager->getSimulationDataManager().getTrajectory();

    unsigned trajectoryIndex = 0;
    for(auto trajectoryElement = trajectoryVector.begin(); trajectoryElement != trajectoryVector.end(); trajectoryElement++)
    {
        if((**trajectoryElement).type() == JsonDataPack::getType())
        {
            NrpCore::TrajectoryMessage * trajectoryMessage = trajectory->add_jsontrajectorymessages();
            const nlohmann::json & data = (dynamic_cast<const JsonDataPack *>((*trajectoryElement).get()))->getData();
            NrpCore::JsonMessage strdata;
            strdata.set_data(data.dump());

            trajectoryMessage->set_dataindex(trajectoryIndex);
            trajectoryMessage->mutable_data()->PackFrom(strdata);
        }
        else
        {
            // We assume that it's a proto DataPack
            NrpCore::TrajectoryMessage * trajectoryMessage = trajectory->add_prototrajectorymessages();

            for(auto& mod : _protoOps) {
                try {
                    mod->setTrajectoryMessageFromInterface(*((*trajectoryElement).get()), trajectoryMessage);
                }
                catch (NRPException &) {
                    // this just means that the module couldn't process the request, try with the next one
                }
            }

            // TODO How to check for failure in message packing?

            trajectoryMessage->set_dataindex(trajectoryIndex);
        }

        trajectoryIndex++;
    }

    this->_manager->getSimulationDataManager().clearTrajectory();
}


std::vector<std::shared_ptr<const DataPackInterface>> NrpCoreServer::extractExternalDataPacks(const NrpCore::DataPacks & message)
{
    std::vector<std::shared_ptr<const DataPackInterface>> externalDataPacks;

    // Extract all JSON DataPacks

    for(int i = 0; i < message.jsondatapacks_size(); i++)
    {
        auto datapackData = message.jsondatapacks(i);
        DataPackInterface::shared_ptr datapack;

        NrpCore::JsonMessage data;
        message.jsondatapacks(i).data().UnpackTo(&data);
        nlohmann::json * data_json = new nlohmann::json(nlohmann::json::parse(data.data()));
        datapack = DataPackInterface::shared_ptr(new JsonDataPack(message.jsondatapacks(i).datapackid().datapackname(),
                                                                  message.jsondatapacks(i).datapackid().enginename(),
                                                                  data_json));

        externalDataPacks.push_back(datapack);
    }

    // Extract all proto DataPacks

    for(int i = 0; i < message.protodatapacks_size(); i++)
    {
        auto datapackData = message.protodatapacks(i);
        DataPackInterface::const_shared_ptr datapack;

        for(auto& mod : _protoOps) {
            datapack = mod->getDataPackInterfaceFromMessage(message.protodatapacks(i).datapackid().enginename(), datapackData);

            if(datapack != nullptr)
                break;
        }

        if(!datapack)
        {
            NRPException::logCreate("Failed to unpack External DataPack '" +
                                    message.protodatapacks(i).datapackid().datapackname() + "' for engine '" +
                                    message.protodatapacks(i).datapackid().enginename() + "'");
        }

        externalDataPacks.push_back(datapack);
    }

    return externalDataPacks;
}


grpc::Status NrpCoreServer::runLoop(grpc::ServerContext * , const NrpCore::RunLoopMessage * message, NrpCore::RunLoopResponse * returnMessage)
{
    std::unique_lock<std::mutex> lock(this->_mutex);

    this->_manager->getSimulationDataManager().updateExternalPool(extractExternalDataPacks(message->datapacks()));
    this->_numIterations = message->numiterations();

    grpc::Status status = requestHelper(lock, RequestType::RunLoop, returnMessage->mutable_simstate());
    if(status.ok())
    {
        const bool done_flag = this->_manager->getSimulationDataManager().getDoneFlag();

        returnMessage->set_doneflag(done_flag);
        // TODO Implement
        returnMessage->set_timeoutflag(false);

        if(done_flag)
        {
            this->prepareTrajectory(returnMessage->mutable_trajectory());
        }
    }

    return status;
}

grpc::Status NrpCoreServer::runUntilTimeout(grpc::ServerContext * , const NrpCore::EmptyMessage * , NrpCore::RunLoopResponse * returnMessage)
{
    std::unique_lock<std::mutex> lock(this->_mutex);

    grpc::Status status = requestHelper(lock, RequestType::RunUntilTimeout, returnMessage->mutable_simstate());
    if(status.ok())
    {
        const bool done_flag = this->_manager->getSimulationDataManager().getDoneFlag();

        returnMessage->set_doneflag(done_flag);
        returnMessage->set_timeoutflag(this->_manager->hasSimulationTimedOut());

        this->prepareTrajectory(returnMessage->mutable_trajectory());
    }

    return status;
}

grpc::Status NrpCoreServer::shutdown(grpc::ServerContext * , const NrpCore::EmptyMessage * , NrpCore::Response * returnMessage)
{
    std::unique_lock<std::mutex> lock(this->_mutex);
    return requestHelper(lock, RequestType::Shutdown, returnMessage->mutable_simstate());
}

grpc::Status NrpCoreServer::reset(grpc::ServerContext * , const NrpCore::ResetMessage * message, NrpCore::ResetResponse * returnMessage)
{
    std::unique_lock<std::mutex> lock(this->_mutex);

    this->_manager->getSimulationDataManager().updateExternalPool(extractExternalDataPacks(message->datapacks()));

    grpc::Status status = requestHelper(lock, RequestType::Reset, returnMessage->mutable_simstate());
    if(status.ok())
    {
        this->prepareTrajectory(returnMessage->mutable_trajectory());
    }

    return status;
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
