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
#include "nrp_general_library/device_interface/device.h"
#include "proto_python_bindings/proto_python_bindings.h"

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

        void sendInitCommand(const nlohmann::json & data)
        {
		    NRP_LOGGER_TRACE("{} called", __FUNCTION__);

            EngineGrpc::InitRequest  request;
            EngineGrpc::InitReply    reply;
            grpc::ClientContext      context;

            prepareRpcContext(&context);

            request.set_json(data.dump());

            NRPLogger::debug("Sending init command to server [ {} ]", this->engineName());
            grpc::Status status = _stub->init(&context, request, &reply);

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
            grpc::Status status = _stub->resetHandle(&context, request, &reply);

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

		virtual void sendDevicesToEngine(const typename EngineClientInterface::devices_ptr_t &devicesArray) override
        {
		    NRP_LOGGER_TRACE("{} called", __FUNCTION__);

            EngineGrpc::SetDeviceRequest request;
            EngineGrpc::SetDeviceReply   reply;
            grpc::ClientContext          context;

            prepareRpcContext(&context);

            for(const auto &device : devicesArray)
            {
                if(device->engineName().compare(this->engineName()) == 0)
                {
                    if(device->isEmpty())
                        throw NRPException::logCreate("Attempt to send empty device " + device->name() + " to Engine " + this->engineName());
                    else {
                        auto r = request.add_request();
                        setProtoFromDeviceInterface<MSG_TYPES...>(r, device);
                    }
                }
            }

            grpc::Status status = _stub->setDevice(&context, request, &reply);

            if(!status.ok())
            {
                const auto errMsg = "Engine server sendDevicesToEngine failed: " + status.error_message() + " (" + std::to_string(status.error_code()) + ")";
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

        virtual const std::vector<std::string> engineProcEnvParams() const override
        {
            return this->engineConfig().at("EngineEnvParams");
        }

        template<class MSG_TYPE, class ...REMAINING_MSG_TYPES>
        DeviceInterfaceConstSharedPtr getDeviceInterfaceFromProto(Engine::DeviceMessage &deviceData) const
        {
            const google::protobuf::OneofDescriptor *fieldOne = deviceData.GetDescriptor()->FindOneofByName("data");
            const google::protobuf::FieldDescriptor *field = deviceData.GetReflection()->GetOneofFieldDescriptor(deviceData,fieldOne);
            if(field && std::strstr(typeid(MSG_TYPE).name(), field->message_type()->name().data())) {
                return DeviceInterfaceConstSharedPtr(
                        new Device<MSG_TYPE>(deviceData.deviceid().devicename(), this->engineName(),
                                               dynamic_cast<MSG_TYPE *>(deviceData.GetReflection()->ReleaseMessage(&deviceData, field))));
            }
            // There's no data set in the message, so create an empty device with device ID only
            else if(!field) {
                // NOTE: deviceData.deviceid().devicetype() becomes useless, it should always be set internally in Device
                //  constructor
                return DeviceInterfaceConstSharedPtr(new DeviceInterface(deviceData.deviceid().devicename(),
                                                     this->engineName(), deviceData.deviceid().devicetype()));
            }

            if constexpr (sizeof...(REMAINING_MSG_TYPES) > 0)
                return getDeviceInterfaceFromProto<REMAINING_MSG_TYPES...>(deviceData);
            else
                throw NRPException::logCreate("Data type: \"" + field->name() + "\" is not supported by engine" +
                this->engineName());
        }

        template<class MSG_TYPE, class ...REMAINING_MSG_TYPES>
        void setProtoFromDeviceInterface(Engine::DeviceMessage *deviceData, DeviceInterface* device)
        {
            if(dynamic_cast< Device<MSG_TYPE> *>(device)) {
                deviceData->mutable_deviceid()->set_devicename(device->name());
                MSG_TYPE* d = dynamic_cast< Device<MSG_TYPE> *>(device)->releaseData();
                auto n = deviceData->GetDescriptor()->field_count();
                auto device_type = d->GetDescriptor()->full_name();
                for(int i=0;i<n;++i) {
                    auto field_type = deviceData->GetDescriptor()->field(i)->message_type()->full_name();
                    if (device_type == field_type) {
                        deviceData->GetReflection()->SetAllocatedMessage(deviceData, d,
                                                                         deviceData->GetDescriptor()->field(i));
                        return;
                    }
                }

                throw NRPException::logCreate("Data type \"" + d->GetDescriptor()->name() + "\" is not supported by engine" +
                                        this->engineName());
            }

            if constexpr (sizeof...(REMAINING_MSG_TYPES) > 0)
                return setProtoFromDeviceInterface<REMAINING_MSG_TYPES...>(deviceData, device);
            else
                throw NRPException::logCreate("Device " + device->name() + " is not supported by engine" +
                                        this->engineName());
        }


		virtual typename EngineClientInterface::devices_set_t getDevicesFromEngine(const typename EngineClientInterface::device_identifiers_set_t &deviceIdentifiers) override
		{
		    NRP_LOGGER_TRACE("{} called", __FUNCTION__);

			EngineGrpc::GetDeviceRequest request;
			EngineGrpc::GetDeviceReply   reply;
			grpc::ClientContext          context;

			for(const auto &devID : deviceIdentifiers)
			{
				if(this->engineName().compare(devID.EngineName) == 0)
				{
					auto r = request.add_deviceid();

					r->set_devicename(devID.Name);
					r->set_devicetype(devID.Type);
					r->set_enginename(devID.EngineName);
				}
			}

			grpc::Status status = _stub->getDevice(&context, request, &reply);

			if(!status.ok())
			{
				const auto errMsg = "Engine client getDevicesFromEngine failed: " + status.error_message() + " (" + std::to_string(status.error_code()) + ")";
				throw std::runtime_error(errMsg);
			}

            typename EngineClientInterface::devices_set_t interfaces;
            for(int i = 0; i < reply.reply_size(); i++)
                interfaces.insert(this->getDeviceInterfaceFromProto<MSG_TYPES...>(*reply.mutable_reply(i)));

            return interfaces;
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
