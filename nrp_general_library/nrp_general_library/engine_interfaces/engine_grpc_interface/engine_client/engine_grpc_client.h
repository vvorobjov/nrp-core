#ifndef ENGINE_GRPC_CLIENT_H
#define ENGINE_GRPC_CLIENT_H

#include <future>

#include <grpcpp/grpcpp.h>
#include <grpcpp/support/time.h>
#include <nlohmann/json.hpp>

#include <nrp_grpc_library/engine_grpc.grpc.pb.h>

#include "nrp_general_library/engine_interfaces/engine_interface.h"
#include "nrp_general_library/engine_interfaces/engine_json_interface/config/engine_json_config.h"

template<class ENGINE, ENGINE_CONFIG_C ENGINE_CONFIG, DEVICE_C ...DEVICES>
class EngineGrpcClient
    : public Engine<ENGINE, ENGINE_CONFIG>
{
    void prepareRpcContext(grpc::ClientContext * context)
    {
        // Set RPC timeout, if it has been specified by the user

        if(this->_rpcTimeout > SimulationTime::zero())
        {
            context->set_deadline(std::chrono::system_clock::now() + this->_rpcTimeout);
        }
    }

    public:

        EngineGrpcClient(EngineConfigConst::config_storage_t &config, ProcessLauncherInterface::unique_ptr &&launcher)
            : Engine<ENGINE, ENGINE_CONFIG>(config, std::move(launcher))
        {
            std::string serverAddress = this->engineConfig()->engineServerAddress();

            // Timeouts of less than 1ms will be rounded up to 1ms

            SimulationTime timeout = toSimulationTime<float, std::ratio<1>>(this->engineConfig()->engineCommandTimeout());
            this->_rpcTimeout      = (timeout > std::chrono::milliseconds(1)) ? timeout : std::chrono::milliseconds(1);

            _channel = grpc::CreateChannel(serverAddress, grpc::InsecureChannelCredentials());
            _stub    = EngineGrpc::EngineGrpcService::NewStub(_channel);
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
            EngineGrpc::InitRequest  request;
            EngineGrpc::InitReply    reply;
            grpc::ClientContext      context;

            prepareRpcContext(&context);

            request.set_json(data.dump());

            grpc::Status status = _stub->init(&context, request, &reply);

            if(!status.ok())
            {
                const auto errMsg = "Engine server initialization failed: " + status.error_message() + " (" + std::to_string(status.error_code()) + ")";
                throw std::runtime_error(errMsg);
            }
        }

        void sendShutdownCommand(const nlohmann::json & data)
        {
            EngineGrpc::ShutdownRequest request;
            EngineGrpc::ShutdownReply   reply;
            grpc::ClientContext         context;

            prepareRpcContext(&context);

            request.set_json(data.dump());

            grpc::Status status = _stub->shutdown(&context, request, &reply);

            if(!status.ok())
            {
                const auto errMsg = "Engine server shutdown failed: " + status.error_message() + " (" + std::to_string(status.error_code()) + ")";
                throw std::runtime_error(errMsg);
            }
        }

        SimulationTime sendRunLoopStepCommand(const SimulationTime timeStep)
        {
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

        SimulationTime getEngineTime() const override
        {
            return this->_engineTime;
        }

        virtual typename EngineInterface::step_result_t runLoopStep(SimulationTime timeStep) override
        {
            this->_loopStepThread = std::async(std::launch::async, std::bind(&EngineGrpcClient::sendRunLoopStepCommand, this, timeStep));
            return EngineInterface::SUCCESS;
        }

        virtual typename EngineInterface::RESULT waitForStepCompletion(float timeOut) override
        {
            // If thread state is invalid, loop thread has completed and waitForStepCompletion was called once before
            if(!this->_loopStepThread.valid())
            {
                return EngineInterface::SUCCESS;
            }

            // Wait until timeOut has passed
            if(timeOut > 0)
            {
                if(this->_loopStepThread.wait_for(std::chrono::duration<double>(timeOut)) != std::future_status::ready)
                    return EngineInterface::ERROR;
            }
            else
                this->_loopStepThread.wait();

            this->_engineTime = this->_loopStepThread.get();
            return EngineInterface::SUCCESS;
        }

        virtual typename EngineInterface::RESULT handleInputDevices(const typename EngineInterface::device_inputs_t &inputDevices) override
        {
            EngineGrpc::SetDeviceRequest request;
            EngineGrpc::SetDeviceReply   reply;
            grpc::ClientContext          context;

            prepareRpcContext(&context);

            for(const auto &device : inputDevices)
            {
                if(device->engineName().compare(this->engineName()) == 0)
                {
                    auto r = request.add_request();
                    this->getProtoFromSingleDeviceInterface<DEVICES...>(*device, r);
                }
            }

            grpc::Status status = _stub->setDevice(&context, request, &reply);

            if(!status.ok())
            {
                const auto errMsg = "Engine server handleInputDevices failed: " + status.error_message() + " (" + std::to_string(status.error_code()) + ")";
                throw std::runtime_error(errMsg);
            }

            return EngineInterface::SUCCESS;
        }

        template<class DEVICE, class ...REMAINING_DEVICES>
        inline void getProtoFromSingleDeviceInterface(const DeviceInterface &device, EngineGrpc::SetDeviceMessage * request) const
        {
            if(DEVICE::TypeName.compare(device.type()) == 0)
            {
                request->mutable_deviceid()->set_devicename(device.name());
                request->mutable_deviceid()->set_devicetype(device.type());
                request->mutable_deviceid()->set_enginename(device.engineName());

                device.serialize(request);

                // Early return

                return;
            }

            // If device classess are left to check, go through them. If all device classes have been checked without proper result, throw an error

            if constexpr (sizeof...(REMAINING_DEVICES) > 0)
            {
                this->getProtoFromSingleDeviceInterface<REMAINING_DEVICES...>(device, request);
            }
            else
            {
                throw std::logic_error("Could not process given device of type " + device.type());
            }
        }

        typename EngineInterface::device_outputs_set_t getDeviceInterfacesFromProto(const EngineGrpc::GetDeviceReply & reply)
        {
            typename EngineInterface::device_outputs_set_t interfaces;

            for(int i = 0; i < reply.reply_size(); i++)
            {
                // Check whether the requested device has new data
				if(reply.reply(i).has_deviceid())
					interfaces.insert(this->getSingleDeviceInterfaceFromProto<DEVICES...>(reply.reply(i)));
            }

            return interfaces;
        }

        template<class DEVICE, class ...REMAINING_DEVICES>
        inline DeviceInterfaceConstSharedPtr getSingleDeviceInterfaceFromProto(const EngineGrpc::GetDeviceMessage &deviceData) const
        {
            if(DEVICE::TypeName.compare(deviceData.deviceid().devicetype()) == 0)
            {
                DeviceIdentifier devId(deviceData.deviceid().devicename(),
                                       deviceData.deviceid().enginename(),
                                       deviceData.deviceid().devicetype());

                DeviceInterfaceSharedPtr newDevice(new DEVICE(devId, deviceData));

                return newDevice;
            }

            // If device classess are left to check, go through them. If all device classes have been checked without proper result, throw an error
            if constexpr (sizeof...(REMAINING_DEVICES) > 0)
            {
                return this->getSingleDeviceInterfaceFromProto<REMAINING_DEVICES...>(deviceData);
            }
            else
            {
                throw std::logic_error("Could not process given device of type " + deviceData.deviceid().devicetype());
            }
        }

	protected:
		virtual typename EngineInterface::device_outputs_set_t requestOutputDeviceCallback(const typename EngineInterface::device_identifiers_t &deviceIdentifiers) override
		{
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
				const auto errMsg = "Engine server getOutputDevices failed: " + status.error_message() + " (" + std::to_string(status.error_code()) + ")";
				throw std::runtime_error(errMsg);
			}

			return this->getDeviceInterfacesFromProto(reply);
		}

    private:

        std::shared_ptr<grpc::Channel>                       _channel;
        std::unique_ptr<EngineGrpc::EngineGrpcService::Stub> _stub;

        std::future<SimulationTime> _loopStepThread;
        SimulationTime _prevEngineTime = SimulationTime::zero();
        SimulationTime _engineTime     = SimulationTime::zero();
        SimulationTime _rpcTimeout     = SimulationTime::zero();
};

#endif // ENGINE_GRPC_CLIENT_H

// EOF
