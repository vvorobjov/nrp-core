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
#include "nrp_grpc_engine_protocol/device_interfaces/grpc_device_serializer.h"
#include "nrp_grpc_engine_protocol/grpc_server/service.grpc.pb.h"

namespace EngineGrpc = communication;

template<class ENGINE, FixedString SCHEMA, DEVICE_C ...DEVICES>
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
            _stub    = EngineGrpc::CommunicationService::NewStub(_channel);
        }

        grpc_connectivity_state getChannelStatus()
        {
            return _channel->GetState(false);
        }

        grpc_connectivity_state connect()
        {
            _channel->GetState(true);
            _channel->WaitForConnected(gpr_time_add(
            gpr_now(GPR_CLOCK_REALTIME), gpr_time_from_seconds(10, GPR_TIMESPAN)));
            return _channel->GetState(false);
        }

        void sendInitCommand(const nlohmann::json & data)
        {
            EngineGrpc::InitializeRequest  request;
            EngineGrpc::InitializeReply    reply;
            grpc::ClientContext      context;

            prepareRpcContext(&context);

            request.set_json("1");

            grpc::Status status = _stub->initialize(&context, request, &reply);

            if(!status.ok())
            {
                const auto errMsg = "Engine server initialization failed: " + status.error_message() + " (" + std::to_string(status.error_code()) + ")";
                throw std::runtime_error(errMsg);
            }
        }

        void sendShutdownCommand(const nlohmann::json & )
        {
            EngineGrpc::NoParams request;
            EngineGrpc::ShutdownReply   reply;
            grpc::ClientContext         context;

            prepareRpcContext(&context);

            //request.set_json(data.dump());

            grpc::Status status = _stub->shutdown(&context, request, &reply);

            if(!status.ok())
            {
                const auto errMsg = "Engine server shutdown failed: " + status.error_message() + " (" + std::to_string(status.error_code()) + ")";
                throw std::runtime_error(errMsg);
            }
        }

        SimulationTime sendRunLoopStepCommand(const SimulationTime timeStep)
        {
            EngineGrpc::RunGameRequest request;
            EngineGrpc::RunGameReply   reply;
            grpc::ClientContext            context;

            prepareRpcContext(&context);

            request.set_time(fromSimulationTime<float, std::ratio<1,1>>(timeStep));

            grpc::Status status = _stub->run_game(&context, request, &reply);

            if(!status.ok())
            {
               const auto errMsg = "Engine server runLoopStep failed: " + status.error_message() + " (" + std::to_string(status.error_code()) + ")";
               throw std::runtime_error(errMsg);
            }

            const SimulationTime engineTime = toSimulationTime<float, std::ratio<1, 1>>(reply.totalsimulatedtime());

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

        SimulationTime getEngineTime() const override
        {
            return this->_engineTime;
        }

        virtual void runLoopStep(SimulationTime timeStep) override
        {
            this->_loopStepThread = std::async(std::launch::async, std::bind(&EngineGrpcClient::sendRunLoopStepCommand, this, timeStep));
        }

        virtual void waitForStepCompletion(float timeOut) override
        {
            // If thread state is invalid, loop thread has completed and waitForStepCompletion was called once before
            if(!this->_loopStepThread.valid())
            {
                return;
            }

            // Wait until timeOut has passed
            if(timeOut > 0)
            {
                if(this->_loopStepThread.wait_for(std::chrono::duration<double>(timeOut)) != std::future_status::ready)
					throw NRPException::logCreate("Engine \"" + this->engineName() + "\" loop is taking too long to complete");
            }
            else
                this->_loopStepThread.wait();

            this->_engineTime = this->_loopStepThread.get();
        }

		virtual void sendDevicesToEngine(const typename EngineClientInterface::devices_ptr_t &devicesArray) override
        {
            EngineGrpc::SetInfoRequest request;
            EngineGrpc::SetInfoReply   reply;
            grpc::ClientContext        context;

            prepareRpcContext(&context);

            for(const auto &device : devicesArray)
            {
                if(device->engineName().compare(this->engineName()) == 0)
                {

                    EngineGrpc::DeviceMessage message;
                    this->getProtoFromSingleDeviceInterface<DEVICES...>(*device, &message);

                    request.CopyFrom(message.setinfo());

                    grpc::Status status = _stub->set_info(&context, request, &reply);

                    if(!status.ok())
                    {
                        const auto errMsg = "Engine server sendDevicesToEngine failed: " + status.error_message() + " (" + std::to_string(status.error_code()) + ")";
                        throw std::runtime_error(errMsg);
                    }
                }
            }
        }

        template<class DEVICE, class ...REMAINING_DEVICES>
		inline void getProtoFromSingleDeviceInterface(const DeviceInterface &device, EngineGrpc::DeviceMessage * request) const
        {
            if(DEVICE::TypeName.compare(device.type()) == 0)
            {
				*request = GRPCDeviceSerializerMethods::template serialize<DEVICE>(dynamic_cast<const DEVICE&>(device));

                return;
            }

            // If device classess are left to check, go through them. If all device classes have been checked without proper result, throw an error

            if constexpr (sizeof...(REMAINING_DEVICES) > 0)
            {
                this->getProtoFromSingleDeviceInterface<REMAINING_DEVICES...>(device, request);
            }
            else
            {
				throw std::logic_error("Could not serialize given device of type \"" + device.type() + "\"");
            }
        }

        template<class DEVICE, class ...REMAINING_DEVICES>
        inline DeviceInterfaceConstSharedPtr getSingleDeviceInterfaceFromProto(const EngineGrpc::DeviceMessage &deviceData) const
        {
            if(DEVICE::TypeName.compare(deviceData.deviceid().devicetype()) == 0)
            {
                DeviceIdentifier devId(deviceData.deviceid().devicename(),
				                       this->engineName(),
                                       deviceData.deviceid().devicetype());

				return DeviceInterfaceConstSharedPtr(new DEVICE(DeviceSerializerMethods<GRPCDevice>::template deserialize<DEVICE>(std::move(devId), &deviceData)));
            }

            // If device classess are left to check, go through them. If all device classes have been checked without proper result, throw an error
            if constexpr (sizeof...(REMAINING_DEVICES) > 0)
            {
                return this->getSingleDeviceInterfaceFromProto<REMAINING_DEVICES...>(deviceData);
            }
            else
            {
				throw std::logic_error("Could not deserialize given device of type \"" + deviceData.deviceid().devicetype() + "\"");
            }
        }

        virtual const std::vector<std::string> engineProcStartParams() const override
        {
            std::vector<std::string> startParams = this->engineConfig().at("EngineProcStartParams");

            std::string name = this->engineConfig().at("EngineName");
            startParams.push_back(std::string("--") + EngineGRPCConfigConst::EngineNameArg.data() + "=" + name);

            // Add JSON Server address (will be used by EngineGrpcServer)
            std::string address = this->engineConfig().at("ServerAddress");
            startParams.push_back(std::string("--") + EngineGRPCConfigConst::EngineServerAddrArg.data() + "=" + address);

            // Add JSON registration Server address (will be used by EngineGrpcServer)
            std::string reg_address = this->engineConfig().at("RegistrationServerAddress");
            startParams.push_back(std::string("--") + EngineGRPCConfigConst::EngineRegistrationServerAddrArg.data() + "=" + reg_address);

            return startParams;
        }

        virtual const std::vector<std::string> engineProcEnvParams() const override
        {
            return this->engineConfig().at("EngineEnvParams");
        }

	protected:
		virtual typename EngineClientInterface::devices_set_t getDevicesFromEngine(const typename EngineClientInterface::device_identifiers_set_t &) override
		{
			EngineGrpc::NoParams     request;
			EngineGrpc::GetInfoReply reply;
			grpc::ClientContext      context;

			grpc::Status status = _stub->get_info(&context, request, &reply);

			if(!status.ok())
			{
				const auto errMsg = "Engine client getDevicesFromEngine failed: " + status.error_message() + " (" + std::to_string(status.error_code()) + ")";
				throw std::runtime_error(errMsg);
			}

            typename EngineClientInterface::devices_set_t interfaces;

            EngineGrpc::DeviceMessage infoMessage;
            infoMessage.mutable_deviceid()->set_devicename("GetInfo");
            infoMessage.mutable_deviceid()->set_devicetype("GetInfo");
            infoMessage.mutable_deviceid()->set_enginename("unity_grpc");
            infoMessage.mutable_getinfo()->CopyFrom(reply);
            interfaces.insert(this->getSingleDeviceInterfaceFromProto<DEVICES...>(infoMessage));

			EngineGrpc::GetCameraReply replyCamera;
            EngineGrpc::NoParams       requestCamera;
			grpc::ClientContext        contextCamera;

            status = _stub->get_camera(&contextCamera, requestCamera, &replyCamera);

			if(!status.ok())
			{
				const auto errMsg = "Engine client getDevicesFromEngine failed: " + status.error_message() + " (" + std::to_string(status.error_code()) + ")";
				throw std::runtime_error(errMsg);
			}

            EngineGrpc::DeviceMessage cameraMessage;
            cameraMessage.mutable_deviceid()->set_devicename("GetCamera");
            cameraMessage.mutable_deviceid()->set_devicetype("GetCamera");
            cameraMessage.mutable_deviceid()->set_enginename("unity_grpc");
            cameraMessage.mutable_getcamera()->CopyFrom(replyCamera);
            interfaces.insert(this->getSingleDeviceInterfaceFromProto<DEVICES...>(cameraMessage));

            return interfaces;
		}

    private:

        std::shared_ptr<grpc::Channel>                       _channel;
        std::unique_ptr<EngineGrpc::CommunicationService::Stub> _stub;

        std::future<SimulationTime> _loopStepThread;
        SimulationTime _prevEngineTime = SimulationTime::zero();
        SimulationTime _engineTime     = SimulationTime::zero();
        SimulationTime _rpcTimeout     = SimulationTime::zero();
};

/*! \defgroup GRPC Engine Protocol

\section engine_grpc_config_section Engine Configuration Options

Engines constructed on this protocol have the following additional configuration options available to them:

<table>
<caption id="grpc_engine_config_table">GRPC Engine Configuration Options</caption>
<tr><th>Name                       <th>Description                                                                <th>Type                <th>Default
<tr><td>ServerAddress              <td>GRPC Server address. Should this address already be in use, simulation initialization will fail   <td>string    <td>"localhost:9004"
</table>



 */


#endif // ENGINE_GRPC_CLIENT_H

// EOF
