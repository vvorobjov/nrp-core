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

#ifndef ENGINE_GRPC_CLIENT_H
#define ENGINE_GRPC_CLIENT_H

#include <future>

#include <grpcpp/grpcpp.h>
#include <grpcpp/support/time.h>
#include <nlohmann/json.hpp>

#include "nrp_grpc_engine_protocol/config/engine_grpc_config.h"
#include "nrp_general_library/config/cmake_constants.h"
#include "nrp_general_library/engine_interfaces/engine_client_interface.h"
#include "nrp_general_library/utils/json_schema_utils.h"
#include "nrp_protobuf/engine_grpc.grpc.pb.h"
#include "nrp_protobuf/config/cmake_constants.h"
#include "nrp_general_library/datapack_interface/datapack.h"

#include "nrp_protobuf/proto_ops/protobuf_ops.h"
#include "nrp_protobuf/proto_ops/proto_ops_manager.h"

template<class ENGINE, const char* SCHEMA>
class EngineGrpcClient
    : public EngineClient<ENGINE, SCHEMA>
{
    void prepareRpcContext(grpc::ClientContext * context)
    {
        // Let client wait for server ready
        // TODO It happens that gRPC call is performed before the server is fully initialized.
        // This line was supposed to fix it, but it's breaking some of the tests. The issue will be addressed in NRRPLT-8187.
        // context->set_wait_for_ready(true);

        // Set RPC timeout (in absolute time), if it has been specified by the user

        if(this->_rpcTimeout > SimulationTime::zero())
        {
            context->set_deadline(std::chrono::system_clock::now() + this->_rpcTimeout);
        }
    }

    public:

        EngineGrpcClient(nlohmann::json &config, ProcessLauncherInterface::unique_ptr &&launcher)
            : EngineClient<ENGINE, SCHEMA>(config, std::move(launcher))
        {

            NRP_LOGGER_TRACE("{} called", __FUNCTION__);

            this->template setDefaultProperty<std::vector<std::string>>("ProtobufPackages", std::vector<std::string>());
            this->template setDefaultProperty<std::string>("ProtobufPluginsPath", NRP_PLUGIN_INSTALL_DIR);

            std::string serverAddress = this->engineConfig().at("ServerAddress");

            // Timeouts of less than 1ms will be rounded up to 1ms

            SimulationTime timeout = toSimulationTime<float, std::ratio<1>>(this->engineConfig().at("EngineCommandTimeout"));

            if(timeout != SimulationTime::zero())
            {
                this->_rpcTimeout = (timeout > std::chrono::milliseconds(1)) ? timeout : std::chrono::milliseconds(1);
            }
            else
            {
                this->_rpcTimeout = SimulationTime::zero();
            }

            _channel = grpc::CreateChannel(serverAddress, grpc::InsecureChannelCredentials());
            _stub    = EngineGrpc::EngineGrpcService::NewStub(_channel);

            ProtoOpsManager::getInstance().addPluginPath(this->engineConfig().at("ProtobufPluginsPath"));
            for(const auto& packageName : this->engineConfig().at("ProtobufPackages")) {
                auto packageNameStr = packageName.template get<std::string>();
                _protoOpsStr += packageNameStr + ",";
                std::stringstream pluginLibName;
                pluginLibName << "lib" << NRP_PROTO_OPS_LIB_PREFIX << packageNameStr <<
                        NRP_PROTO_OPS_LIB_SUFIX << ".so";
                auto pluginLib = ProtoOpsManager::getInstance().loadProtobufPlugin(pluginLibName.str());
                if(pluginLib)
                    _protoOps.template emplace_back(std::move(pluginLib));
                else
                    throw NRPException::logCreate("Failed to load ProtobufPackage \""+packageNameStr+"\"");
            }

            if(!_protoOpsStr.empty())
                _protoOpsStr.pop_back();
        }

        virtual pid_t launchEngine() override
        {
            NRP_LOGGER_TRACE("{} called", __FUNCTION__);

            // Launch engine process.
            // enginePID == -1 means that the launcher hasn't launched a process
            auto enginePID = this->EngineClientInterface::launchEngine();

            this->connectToServer();

            return enginePID;
        }

        void connectToServer()
        {
            NRP_LOGGER_TRACE("{} called", __FUNCTION__);

            if(!_channel->WaitForConnected(gpr_time_add(
                gpr_now(GPR_CLOCK_REALTIME), gpr_time_from_seconds(20, GPR_TIMESPAN))))
            {
                throw NRPException::logCreate("Timeout while connecting to engine server (" + this->engineName() + ")");
            }
        }

        void sendInitializeCommand(const nlohmann::json & data)
        {
            NRP_LOGGER_TRACE("{} called", __FUNCTION__);

            EngineGrpc::InitializeRequest request;
            EngineGrpc::InitializeReply   reply;
            grpc::ClientContext           context;

            prepareRpcContext(&context);

            request.set_json(data.dump());

            NRPLogger::debug("Sending init command to server [ {} ]", this->engineName());
            grpc::Status status = _stub->initialize(&context, request, &reply);

            if(!status.ok())
            {
                const auto errMsg = "Engine server initialization failed: " + status.error_message() + " (" + std::to_string(status.error_code()) + ")";
                throw std::runtime_error(errMsg);
            }
        }

        void sendResetCommand()
        {
            NRP_LOGGER_TRACE("{} called", __FUNCTION__);
            EngineGrpc::ResetRequest  request;
            EngineGrpc::ResetReply    reply;
            grpc::ClientContext       context;

            prepareRpcContext(&context);

            NRPLogger::debug("Sending reset command to server [ {} ]", this->engineName());
            grpc::Status status = _stub->reset(&context, request, &reply);

            if(!status.ok())
            {
                const auto errMsg = "Engine server reset failed: " + status.error_message() + " (" + std::to_string(status.error_code()) + ")";
                throw std::runtime_error(errMsg);
            }

            resetEngineTime();
        }

        void sendShutdownCommand(const nlohmann::json & data)
        {
            NRP_LOGGER_TRACE("{} called", __FUNCTION__);

            EngineGrpc::ShutdownRequest request;
            EngineGrpc::ShutdownReply   reply;
            grpc::ClientContext         context;

            prepareRpcContext(&context);

            request.set_json(data.dump());

            NRPLogger::debug("Sending shutdown command to server [ {} ]", this->engineName());
            grpc::Status status = _stub->shutdown(&context, request, &reply);

            if(!status.ok())
            {
                const auto errMsg = "Engine server shutdown failed: " + status.error_message() + " (" + std::to_string(status.error_code()) + ")";
                throw std::runtime_error(errMsg);
            }
        }

        SimulationTime runLoopStepCallback(const SimulationTime timeStep) override
        {
            NRP_LOGGER_TRACE("{} called", __FUNCTION__);

            EngineGrpc::RunLoopStepRequest request;
            EngineGrpc::RunLoopStepReply   reply;
            grpc::ClientContext            context;

            prepareRpcContext(&context);

            request.set_timestep(timeStep.count());

            grpc::Status status = _stub->runLoopStep(&context, request, &reply);

            if(!status.ok())
            {
               const auto errMsg = "Engine server runLoopStep failed: " + status.error_message() + " (" + std::to_string(status.error_code()) + ")";
               throw std::runtime_error(errMsg);
            }

            const SimulationTime engineTime(reply.enginetime());

            if(engineTime < SimulationTime::zero())
            {
               const auto errMsg = "Invalid engine time (should be greater than 0): " + std::to_string(engineTime.count());
               throw std::runtime_error(errMsg);
            }

            if(engineTime < this->_prevEngineTime)
            {
                const auto errMsg = "Invalid engine time (should be greater than previous time): "
                                  + std::to_string(engineTime.count())
                                  + ", previous: "
                                  + std::to_string(this->_prevEngineTime.count());

                throw std::runtime_error(errMsg);
            }

            this->_prevEngineTime = engineTime;

            return engineTime;
        }

    virtual datapacks_vector_t getDataPacksFromEngine(const datapack_identifiers_set_t &requestedDataPackIds) override
    {
        NRP_LOGGER_TRACE("{} called", __FUNCTION__);

        EngineGrpc::GetDataPacksRequest request;
        EngineGrpc::GetDataPacksReply   reply;
        grpc::ClientContext             context;

        for(const auto & requestedId: requestedDataPackIds)
        {
            if(this->engineName().compare(requestedId.EngineName) == 0)
            {
                auto dataPackId = request.add_datapackids();

                dataPackId->set_datapackname(requestedId.Name);
                dataPackId->set_datapacktype(requestedId.Type);
                dataPackId->set_enginename(requestedId.EngineName);
            }
        }

        grpc::Status status = _stub->getDataPacks(&context, request, &reply);

        if(!status.ok())
        {
            const auto errMsg = "In Engine \"" + this->engineName() + "\", getDataPacksFromEngine failed: " + status.error_message() + " (" + std::to_string(status.error_code()) + ")";
            throw std::runtime_error(errMsg);
        }

        datapacks_vector_t interfaces;
        for(int i = 0; i < reply.datapacks_size(); i++) {
            auto datapackData = reply.datapacks(i);
            DataPackInterfaceConstSharedPtr datapack;

            for(auto& mod : _protoOps) {
                datapack = mod->getDataPackInterfaceFromMessage(this->engineName(), datapackData);

                if(datapack != nullptr)
                    break;
            }

            if(datapack)
                interfaces.push_back(datapack);
            else
                throw NRPException::logCreate("In Engine \"" + this->engineName() + "\", unable to deserialize datapack \"" +
                                                      datapackData.datapackid().datapackname() + "\" using any of the NRP-Core Protobuf plugins specified in the"
                                                      " engine configuration: [" + _protoOpsStr + "]. Ensure that the parameter "
                                                      "\"ProtobufPackages\" is properly set in the Engine configuration");
        }

        return interfaces;
    }

        virtual void sendDataPacksToEngine(const datapacks_set_t &dataPacks) override
        {
            NRP_LOGGER_TRACE("{} called", __FUNCTION__);

            EngineGrpc::SetDataPacksRequest request;
            EngineGrpc::SetDataPacksReply   reply;
            grpc::ClientContext             context;

            prepareRpcContext(&context);

            for(const auto &datapack : dataPacks)
            {
                assert(datapack->engineName() == this->engineName());

                if(datapack->isEmpty())
                    throw NRPException::logCreate("Attempt to send empty datapack " + datapack->name() + " to Engine " + this->engineName());
                else {
                    auto protoDataPack = request.add_datapacks();

                    bool isSet = false;
                    for(auto& mod : _protoOps) {
                        try {
                            mod->setDataPackMessageFromInterface(*datapack, protoDataPack);
                            isSet = true;
                        }
                        catch (NRPException &) {
                            // this just means that the module couldn't process the request, try with the next one
                        }
                    }

                    if(!isSet)
                        throw NRPException::logCreate("In Engine \"" + this->engineName() + "\", unable to serialize datapack \"" +
                                                      datapack->name() + "\" using any of the NRP-Core Protobuf plugins specified in the"
                                                      " engine configuration: [" + _protoOpsStr + "]. Ensure that the parameter "
                                                      "\"ProtobufPackages\" is properly set in the Engine configuration");
                }
            }

            grpc::Status status = _stub->setDataPacks(&context, request, &reply);

            if(!status.ok())
            {
                const auto errMsg = "In Engine \"" + this->engineName() + "\", sendDataPacksToEngine failed: " + status.error_message() + " (" + std::to_string(status.error_code()) + ")";
                throw std::runtime_error(errMsg);
            }
        }

        virtual const std::vector<std::string> engineProcStartParams() const override
        {
            NRP_LOGGER_TRACE("{} called", __FUNCTION__);

            std::vector<std::string> startParams = this->engineConfig().at("EngineProcStartParams");

            std::string name = this->engineConfig().at("EngineName");
            startParams.push_back(std::string("--") + EngineGRPCConfigConst::EngineNameArg.data() + "=" + name);

            // Add Server address (will be used by EngineGrpcServer)
            std::string address = this->engineConfig().at("ServerAddress");
            startParams.push_back(std::string("--") + EngineGRPCConfigConst::EngineServerAddrArg.data() + "=" + address);

            // Information needed to load protobuf plugins
            startParams.push_back(std::string("--") + EngineGRPCConfigConst::ProtobufPluginsPathArg.data() + "=" +
                                    this->engineConfig().at("ProtobufPluginsPath").template get<std::string>());

            startParams.push_back(std::string("--") + EngineGRPCConfigConst::ProtobufPluginsArg.data() + "=" +
                                  this->engineConfig().at("ProtobufPackages").dump());


            return startParams;
        }

    protected:

        void resetEngineTime() override
        {
            EngineClient<ENGINE, SCHEMA>::resetEngineTime();
            this->_prevEngineTime = SimulationTime::zero();
        }

    private:

        std::shared_ptr<grpc::Channel>                       _channel;
        std::unique_ptr<EngineGrpc::EngineGrpcService::Stub> _stub;

        SimulationTime _prevEngineTime = SimulationTime::zero();
        SimulationTime _rpcTimeout     = SimulationTime::zero();

    std::vector<std::unique_ptr<protobuf_ops::NRPProtobufOpsIface>> _protoOps;
    std::string _protoOpsStr = "";
};


#endif // ENGINE_GRPC_CLIENT_H

// EOF
