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
#include "nrp_general_library/engine_interfaces/engine_client_interface.h"
#include "nrp_protobuf/engine_grpc.grpc.pb.h"
#include "nrp_general_library/datapack_interface/datapack.h"
#include "proto_python_bindings/proto_python_bindings.h"
#include "nrp_protobuf/protobuf_utils.h"

template<class ENGINE, const char* SCHEMA, class ...MSG_TYPES>
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
            static_assert((std::is_base_of_v<google::protobuf::Message, MSG_TYPES> && ...), "Parameter MSG_TYPES must derive from protobuf::Message");

            NRP_LOGGER_TRACE("{} called", __FUNCTION__);

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
        }

        grpc_connectivity_state getChannelStatus()
        {
            return _channel->GetState(false);
        }

        grpc_connectivity_state connect()
        {
            NRP_LOGGER_TRACE("{} called", __FUNCTION__);

            _channel->GetState(true);
            _channel->WaitForConnected(gpr_time_add(
            gpr_now(GPR_CLOCK_REALTIME), gpr_time_from_seconds(10, GPR_TIMESPAN)));
            return _channel->GetState(false);
        }

        void sendInitializeCommand(const nlohmann::json & data)
        {
            NRP_LOGGER_TRACE("{} called", __FUNCTION__);
            sleep(1);
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

    virtual typename EngineClientInterface::datapacks_set_t getDataPacksFromEngine(const typename EngineClientInterface::datapack_identifiers_set_t &datapackIdentifiers) override
    {
        NRP_LOGGER_TRACE("{} called", __FUNCTION__);

        EngineGrpc::GetDataPacksRequest request;
        EngineGrpc::GetDataPacksReply   reply;
        grpc::ClientContext             context;

        for(const auto &devID : datapackIdentifiers)
        {
            if(this->engineName().compare(devID.EngineName) == 0)
            {
                auto dataPackId = request.add_datapackids();

                dataPackId->set_datapackname(devID.Name);
                dataPackId->set_datapacktype(devID.Type);
                dataPackId->set_enginename(devID.EngineName);
            }
        }

        grpc::Status status = _stub->getDataPacks(&context, request, &reply);

        if(!status.ok())
        {
            const auto errMsg = "Engine client getDataPacksFromEngine failed: " + status.error_message() + " (" + std::to_string(status.error_code()) + ")";
            throw std::runtime_error(errMsg);
        }

        typename EngineClientInterface::datapacks_set_t interfaces;
        for(int i = 0; i < reply.datapacks_size(); i++) {
            auto datapackData = reply.datapacks(i);
            DataPackInterfaceConstSharedPtr datapack;

            if constexpr(sizeof...(MSG_TYPES) > 0)
                datapack = protobuf_utils::getDataPackInterfaceFromMessageSubset<MSG_TYPES...>(this->engineName(), datapackData);
            else
                datapack = protobuf_utils::getDataPackInterfaceFromMessage(this->engineName(), datapackData);

            interfaces.insert(datapack);
        }

        return interfaces;
    }

        virtual void sendDataPacksToEngine(const typename EngineClientInterface::datapacks_ptr_t &datapacksArray) override
        {
            NRP_LOGGER_TRACE("{} called", __FUNCTION__);

            EngineGrpc::SetDataPacksRequest request;
            EngineGrpc::SetDataPacksReply   reply;
            grpc::ClientContext             context;

            prepareRpcContext(&context);

            for(const auto &datapack : datapacksArray)
            {
                if(datapack->engineName().compare(this->engineName()) == 0)
                {
                    if(datapack->isEmpty())
                        throw NRPException::logCreate("Attempt to send empty datapack " + datapack->name() + " to Engine " + this->engineName());
                    else {
                        auto protoDataPack = request.add_datapacks();

                        if constexpr(sizeof...(MSG_TYPES) > 0)
                            protobuf_utils::setDataPackMessageFromInterfaceSubset<MSG_TYPES...>(*datapack, protoDataPack);
                        else
                            protobuf_utils::setDataPackMessageFromInterface(*datapack, protoDataPack);
                    }
                }
                else
                    NRPLogger::warn("Attempting to send DataPack \"" + datapack->name() + "\" linked to engine \"" + datapack->engineName() +
                    "\" to Engine \"" + this->engineName() + "\". It won't be sent.");
            }

            grpc::Status status = _stub->setDataPacks(&context, request, &reply);

            if(!status.ok())
            {
                const auto errMsg = "Engine server sendDataPacksToEngine failed: " + status.error_message() + " (" + std::to_string(status.error_code()) + ")";
                throw std::runtime_error(errMsg);
            }
        }

        virtual const std::vector<std::string> engineProcStartParams() const override
        {
            NRP_LOGGER_TRACE("{} called", __FUNCTION__);

            std::vector<std::string> startParams = this->engineConfig().at("EngineProcStartParams");

            std::string name = this->engineConfig().at("EngineName");
            startParams.push_back(std::string("--") + EngineGRPCConfigConst::EngineNameArg.data() + "=" + name);

            // Add JSON Server address (will be used by EngineGrpcServer)
            std::string address = this->engineConfig().at("ServerAddress");
            startParams.push_back(std::string("--") + EngineGRPCConfigConst::EngineServerAddrArg.data() + "=" + address);

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
};


#endif // ENGINE_GRPC_CLIENT_H

// EOF
