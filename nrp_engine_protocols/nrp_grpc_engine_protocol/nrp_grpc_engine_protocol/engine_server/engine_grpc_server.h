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

#ifndef ENGINE_GRPC_SERVER_H
#define ENGINE_GRPC_SERVER_H

#include <string>
#include <map>
#include <type_traits>

#include <grpcpp/grpcpp.h>
#include <grpcpp/health_check_service_interface.h>
#include <nlohmann/json.hpp>

#include "nrp_grpc_engine_protocol/grpc_server/engine_grpc.grpc.pb.h"
#include "nrp_general_library/engine_interfaces/data_device_controller.h"
#include "nrp_general_library/utils/time_utils.h"
#include "proto_python_bindings/proto_field_ops.h"


using ProtoDeviceController = DataDeviceController<google::protobuf::Message>;

using EngineGrpc::EngineGrpcService;

/*!
 * \brief Abstract class for Engine server with gRPC support
 *
 * The class provides a base for implementing an Engine server with gRPC
 * as middleware. All RPC services are implemented. Derived classes are responsible
 * for implementing simulation initialization, shutdown and run step methods.
 */
template<PROTO_MSG_C ...MSG_TYPES>
class EngineGrpcServer : public EngineGrpcService::Service
{
    public:

        using mutex_t = std::timed_mutex;
        using lock_t  = std::unique_lock<EngineGrpcServer::mutex_t>;

        EngineGrpcServer(const std::string address)
        {
            this->_serverAddress   = address;
            this->_isServerRunning = false;

            grpc::EnableDefaultHealthCheckService(true);
        }

        /*!
         * \brief Constructor
         *
         * \param[in] serverAddress Address of the gRPC server
         * \param[in] engineName    Name of the simulation engine
         * \param[in] registrationAddress Should be removed
         */
        // TODO registrationAddress isn't needed
        EngineGrpcServer(const std::string &serverAddress, const std::string &engineName, const std::string &/*registrationAddress*/)
                : EngineGrpcServer(serverAddress)
        {
            this->_engineName = engineName;
        }

        /*!
         * \brief Destructor
         */
        virtual ~EngineGrpcServer()
        {
            this->shutdownServer();
        }

        /*!
         * \brief Starts the gRPC server in synchronous mode
         */
        void startServer()
        {
            if(!this->_isServerRunning)
            {
                grpc::ServerBuilder builder;
                builder.AddListeningPort(_serverAddress, grpc::InsecureServerCredentials());
                builder.RegisterService(this);

                this->_server = builder.BuildAndStart();
                // TODO Should we use a memory barrier here?
                this->_isServerRunning = true;
            }
        }

        /*!
         * \brief Starts the gRPC server in asynchronous mode
         */
        void startServerAsync()
        {
            this->startServer();
        }

        /*!
         * \brief Shutdowns the gRPC server
         */
        void shutdownServer()
        {
            if(this->_isServerRunning)
            {
                this->_server->Shutdown();
                // TODO Should we use a memory barrier here?
                this->_isServerRunning = false;
            }
        }

        /*!
         * \brief Indicates whether the gRPC server is currently running
         */
        bool isServerRunning() const
        {
            return this->_isServerRunning;
        }

        /*!
         * \brief Returns address of the gRPC server
         */
        const std::string serverAddress() const
        {
            return this->_serverAddress;
        }

        /*!
         * \brief Registers a device controller with the given name in the engine
         *
         * \param[in] deviceName       Name of the device to be registered
         * \param[in] deviceController Pointer to the device controller object that's supposed to be
         *                             registered in the engine
         */
        void registerDevice(const std::string & deviceName, ProtoDeviceController *interface)
        {
            EngineGrpcServer::lock_t lock(this->_deviceLock);
            this->_devicesControllers.emplace(deviceName, interface);
        }

        // TODO used only in tests, try to remove it?
        unsigned getNumRegisteredDevices()
        {
            return this->_devicesControllers.size();
        }

    protected:
        mutex_t                       _deviceLock;
        void clearRegisteredDevices()
        {
            // TODO Check if it's true
            // Do not lock scope. This method is called from the route handlers, which should already have locked down access.
            //EngineJSONServer::lock_t lock(this->_deviceLock);

            this->_devicesControllers.clear();
        }

    private:

        /*!
         * \brief Address of the gRPC server
         */
        std::string _serverAddress;

        /*!
         * \brief Indicates whether the gRPC server is currently running
         */
        bool _isServerRunning;

        /*!
         * \brief Conbfiguration of the process logger
         */
		NRPLogger _loggerCfg;

        /*!
         * \brief Name of the simulation engine
         *
         * Must be the same on the server and the client side. It should be imprinted
         * in the device metadata, which allows for additional consistency checks.
         */
        std::string _engineName;

        /*!
         * \brief Pointer to the gRPC server object
         */
        std::unique_ptr<grpc::Server> _server;

        /*!
         * \brief Map of device names and device controllers used by the engine
         */
         std::map<std::string, ProtoDeviceController*> _devicesControllers;

        /*!
         * \brief Initializes the simulation
         *
         * \param[in] data       Simulation configuration data
         * \param[in] deviceLock ???
         */
        virtual void initialize(const nlohmann::json &data, EngineGrpcServer::lock_t &deviceLock) = 0;

        /*!
         * \brief Shutdowns the simulation
         *
         * \param[in] data Additional data
         */
        virtual void shutdown(const nlohmann::json &data) = 0;

        /*!
         * \brief Runs a single simulation loop step
         *
         * \param[in] timeStep Time step by which the simulation should be advanced
         *
         * \return Engine time after running the step
         */
        virtual SimulationTime runLoopStep(const SimulationTime timeStep) = 0;

        /*!
         * \brief Initializes the simulation
         *
         * The function implements the init method of the EngineGrpcService.
         * It acts as a wrapper around the virtual initialize method, which should initialize the simulation.
         * On error, it will return a status object with error message and grpc::StatusCode::CANCELLED error code.
         *
         * \param      context Pointer to gRPC server context structure
         * \param[in]  request Pointer to protobuf init request message. Contains simulation config in JSON format.
         * \param[out] reply   Pointer to protobuf init reply message. Currently no data is returned.
         *
         * \return gRPC request status
         */
        grpc::Status init(grpc::ServerContext * /*context*/, const EngineGrpc::InitRequest * request, EngineGrpc::InitReply * /*reply*/) override
        {
            try
            {
                EngineGrpcServer::lock_t lock(this->_deviceLock);

                nlohmann::json requestJson = nlohmann::json::parse(request->json());

                // Run engine-specific initialization function

                this->initialize(requestJson, lock);
            }
            catch(const std::exception &e)
            {
                return handleGrpcError("Error while executing initialization", e.what());
            }

            return grpc::Status::OK;
        }

        /*!
         * \brief Shutdowns the simulation
         *
         * The function implements the shutdown method of the EngineGrpcService.
         * It acts as a wrapper around the virtual shutdown method, which should shutdown the simulation.
         * On error, it will return a status object with error message and grpc::StatusCode::CANCELLED error code.
         *
         * \param      context Pointer to gRPC server context structure
         * \param[in]  request Pointer to protobuf shutdown request message. Contains shutdown parameters in JSON format.
         * \param[out] reply   Pointer to protobuf shutdown reply message. Currently no data is returned.
         *
         * \return gRPC request status
         */
        grpc::Status shutdown(grpc::ServerContext * /*context*/, const EngineGrpc::ShutdownRequest * request, EngineGrpc::ShutdownReply * /*reply*/) override
        {
            try
            {
                EngineGrpcServer::lock_t lock(this->_deviceLock);

                nlohmann::json requestJson = nlohmann::json::parse(request->json());

                // Run engine-specifi shutdown function

                this->shutdown(requestJson);
            }
            catch(const std::exception &e)
            {
                return handleGrpcError("Error while executing shutdown", e.what());
            }

            return grpc::Status::OK;
        }

        /*!
         * \brief Runs a single loop step of the simulation
         *
         * The function implements the runLoopStep method of the EngineGrpcService.
         * It acts as a wrapper around the virtual runLoopStep method, which should run a single step of the simulation.
         * On error, it will return a status object with error message and grpc::StatusCode::CANCELLED error code.
         *
         * \param      context Pointer to gRPC server context structure
         * \param[in]  request Pointer to protobuf runLoopStep request message. Contains simulation time step.
         * \param[out] reply   Pointer to protobuf runLoopStep reply message. Contains engine time.
         *
         * \return gRPC request status
         */
        grpc::Status runLoopStep(grpc::ServerContext * /*context*/, const EngineGrpc::RunLoopStepRequest * request, EngineGrpc::RunLoopStepReply * reply) override
        {
            try
            {
                EngineGrpcServer::lock_t lock(this->_deviceLock);

                int64_t engineTime = (this->runLoopStep(SimulationTime(request->timestep()))).count();

                reply->set_enginetime(engineTime);
            }
            catch(const std::exception &e)
            {
                return handleGrpcError("Error while executing runLoopStep", e.what());
            }

            return grpc::Status::OK;
        }

        /*!
         * \brief Sets received data into proper devices
         *
         * The function implements the setDevice method of the EngineGrpcService.
         * It acts as a wrapper around the virtual setDeviceData method, which should set received data into proper devices.
         * On error, it will return a status object with error message and grpc::StatusCode::CANCELLED error code.
         *
         * \param      context Pointer to gRPC server context structure
         * \param[in]  request Pointer to protobuf setDevice request message. Contains device data and metadata
         * \param[out] reply   Pointer to protobuf setDevice reply message. Currently no data is returned.
         *
         * \return gRPC request status
         */
        grpc::Status setDevice(grpc::ServerContext * /*context*/, const EngineGrpc::SetDeviceRequest * request, EngineGrpc::SetDeviceReply * /*reply*/) override
        {
            try
            {
                this->setDeviceData(*request);
            }
            catch(const std::exception &e)
            {
                return handleGrpcError("Error while executing setDevice", e.what());
            }

            return grpc::Status::OK;
        }

        /*!
         * \brief Gets data from requested devices
         *
         * The function implements the getDevice method of the EngineGrpcService.
         * It acts as a wrapper around the virtual getDeviceData method, which should get data from requested devices.
         * On error, it will return a status object with error message and grpc::StatusCode::CANCELLED error code.
         *
         * \param      context Pointer to gRPC server context structure
         * \param[in]  request Pointer to protobuf setDevice request message. Contains metadata of requested devices.
         * \param[out] reply   Pointer to protobuf setDevice reply message. Contains device data and metadata.
         *
         * \return gRPC request status
         */
        grpc::Status getDevice(grpc::ServerContext * /*context*/, const EngineGrpc::GetDeviceRequest * request, EngineGrpc::GetDeviceReply * reply) override
        {
            try
            {
                this->getDeviceData(*request, reply);
            }
            catch(const std::exception &e)
            {
                return handleGrpcError("Error while executing getDevice", e.what());
            }

            return grpc::Status::OK;
        }

        virtual void setDeviceData(const EngineGrpc::SetDeviceRequest & data)
        {
            EngineGrpcServer::lock_t lock(this->_deviceLock);

            const auto numDevices = data.request_size();

            for(int i = 0; i < numDevices; i++)
            {
                const auto &r = data.request(i);
                const auto &devInterface = this->_devicesControllers.find(r.deviceid().devicename());

                if(devInterface != _devicesControllers.end())
                {
                    const google::protobuf::OneofDescriptor *fieldOne = r.GetDescriptor()->FindOneofByName("data");
                    const google::protobuf::FieldDescriptor *field = r.GetReflection()->GetOneofFieldDescriptor(r,fieldOne);
                    if(!field) {
                        const auto errorMessage = "Device " + r.deviceid().devicename() + " has been sent with no data";
                        throw std::invalid_argument(errorMessage);
                    }
                    devInterface->second->handleDeviceData(r.GetReflection()->GetMessage(r,field));
                }
                else
                {
                    const auto errorMessage = "Device " + r.deviceid().devicename() + " is not registered in engine " + this->_engineName;
                    throw std::invalid_argument(errorMessage);
                }
            }
        }
        virtual void getDeviceData(const EngineGrpc::GetDeviceRequest & request, EngineGrpc::GetDeviceReply * reply)
        {
            EngineGrpcServer::lock_t lock(this->_deviceLock);

            const auto numDevices = request.deviceid_size();

            for(int i = 0; i < numDevices; i++)
            {
                const auto &devInterface = this->_devicesControllers.find(request.deviceid(i).devicename());
                if(devInterface != _devicesControllers.end())
                {
                    auto r = reply->add_reply();
                    r->mutable_deviceid()->set_devicename(request.deviceid(i).devicename());
                    // ask controller to fetch device data. nullptr means there is no new data available
                    auto d = devInterface->second->getDeviceInformation();
                    if(d != nullptr)
                        setDeviceMessageData<MSG_TYPES...>(request.deviceid(i).devicename(), d, r);
                }
                else
                {
                    const auto errorMessage = "Device " + request.deviceid(i).devicename() + " is not registered in engine " + this->_engineName;
                    throw std::invalid_argument(errorMessage);
                }
            }
        }

        template<class MSG_TYPE, class ...REMAINING_MSG_TYPES>
        void setDeviceMessageData(const std::string &dev_name, google::protobuf::Message *dev_data, EngineGrpc::DeviceMessage *m)
        {
            if(dynamic_cast< MSG_TYPE *>(dev_data)) {
                auto d = dynamic_cast< MSG_TYPE *>(dev_data);
                auto n = m->GetDescriptor()->field_count();
                auto device_type = d->GetDescriptor()->full_name();

                for(int i=0;i<n;++i) {
                    auto field_type = m->GetDescriptor()->field(i)->message_type()->full_name();
                    if (device_type == field_type) {
                        m->GetReflection()->SetAllocatedMessage(m, d, m->GetDescriptor()->field(i));
                        return;
                    }
                }
            }

            if constexpr (sizeof...(REMAINING_MSG_TYPES) > 0)
                return setDeviceMessageData<REMAINING_MSG_TYPES...>(dev_name, dev_data, m);
            else {
                const auto errorMessage = "Device " + dev_name + " has type not supported by engine " + this->_engineName;
                throw std::invalid_argument(errorMessage);
            }

        }

        /*!
         * \brief Helper function for handling errors inside Remote Procedure Calls (RPCs)
         *
         * \param[in] contextMessage Name of the RPC that called the helper
         * \param[in] errorMessage   Error message that will be passed to the gRPC client
         *
         * \return gRPC request status, containing the error message and grpc::StatusCode::CANCELLED error code
         */
        grpc::Status handleGrpcError(const std::string & contextMessage, const std::string & errorMessage)
        {
            std::cerr << contextMessage << std::endl;
            std::cerr << errorMessage   << std::endl;

            // Pass the error message to the client

            grpc::Status status(grpc::StatusCode::CANCELLED, errorMessage);

            return status;
        }
};

#endif // ENGINE_GRPC_SERVER_H
