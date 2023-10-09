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

#ifndef ENGINE_GRPC_SERVER_H
#define ENGINE_GRPC_SERVER_H

#include <string>
#include <map>
#include <type_traits>

#include <grpcpp/grpcpp.h>
#include <grpcpp/health_check_service_interface.h>
#include <nlohmann/json.hpp>

#include "nrp_protobuf/engine_grpc.grpc.pb.h"
#include "nrp_general_library/engine_interfaces/datapack_controller.h"
#include "nrp_general_library/utils/time_utils.h"
#include "nrp_protobuf/config/cmake_constants.h"
#include "nrp_protobuf/proto_python_bindings/proto_field_ops.h"
#include "nrp_protobuf/proto_ops/protobuf_ops.h"
#include "nrp_protobuf/proto_ops/proto_ops_manager.h"

#include "nrp_grpc_engine_protocol/engine_server/engine_proto_wrapper.h"


using EngineGrpc::EngineGrpcService;

/*!
 * \brief class for Engine server with gRPC support
 *
 * The class provides an Engine server with gRPC
 * as middleware. All RPC services are implemented. EngineProtoWrapper is used to perform
 * the actual simulation initialization, shutdown, reset, data exchange and run step operations.
 */

class EngineGrpcServer : public EngineGrpcService::Service
{
    public:

        using mutex_t = std::timed_mutex;
        using lock_t  = std::unique_lock<EngineGrpcServer::mutex_t>;

        /*!
         * \brief No dummy servers, only those with name and url
         */
        EngineGrpcServer() = delete;

        /*!
         * \brief Constructor
         *
         * \param[in] serverAddress Address of the gRPC server
         * \param[in] engineWrapper Class processing requests to the Engine
         */
        EngineGrpcServer(const std::string serverAddress, EngineProtoWrapper* engineWrapper)
                :  _engineWrapper(engineWrapper)
        {
            this->_serverAddress   = serverAddress;
            this->_isServerRunning = false;

            grpc::EnableDefaultHealthCheckService(true);
        }

        /*!
         * \brief Destructor
         */
        ~EngineGrpcServer()
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
                NRPLogger::debug("Using server address: "+ this->_serverAddress);

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
         * \brief Indicates if the simulation was initialized and is running
         */
         virtual bool initRunFlag() const  { return this->_engineWrapper->initRunFlag(); }

        /*!
         * \brief Indicates if shutdown was requested by the client
         */
        virtual bool shutdownFlag() const  { return this->_engineWrapper->shutdownFlag(); }

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

    private:

        mutex_t _engineCallLock;

        /*!
         * \brief Address of the gRPC server
         */
        std::string _serverAddress;

        /*!
         * \brief Indicates whether the gRPC server is currently running
         */
        bool _isServerRunning;

        /*!
         * \brief Pointer to the gRPC server object
         */
        std::unique_ptr<grpc::Server> _server;

        /*!
         * \brief Pointer to the Engine Wrapper object
         */
        std::unique_ptr<EngineProtoWrapper> _engineWrapper;

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
        grpc::Status initialize(      grpc::ServerContext           * /*context*/,
                                const EngineGrpc::InitializeRequest * request,
                                      EngineGrpc::InitializeReply   * /*reply*/) override
        {
            try
            {
                EngineGrpcServer::lock_t lock(this->_engineCallLock);

                nlohmann::json requestJson = nlohmann::json::parse(request->json());

                // Run engine-specific initialization function

                this->_engineWrapper->initialize(requestJson);
            }
            catch(const std::exception &e)
            {
                return handleGrpcError("Error while executing initialization", e.what());
            }
            NRPLogger::debug("init command returns OK");
            return grpc::Status::OK;
        }


        /*!
         * \brief Resets the simulation
         *
         * The function implements the reset method of the EngineGrpcService.
         * It acts as a wrapper around the virtual reset method, which should reset the simulation.
         * On error, it will return a status object with error message and grpc::StatusCode::CANCELLED error code.
         *
         * \param      context Pointer to gRPC server context structure
         * \param[in]  request Pointer to protobuf init request message. Contains simulation config in JSON format.
         * \param[out] reply   Pointer to protobuf init reply message. Currently no data is returned.
         *
         * \return gRPC request status
         */
        grpc::Status reset(      grpc::ServerContext      * /*context*/,
                           const EngineGrpc::ResetRequest * /*request*/,
                                 EngineGrpc::ResetReply   * /*reply*/) override
        {
            NRP_LOGGER_TRACE("{} called", __FUNCTION__);
            try
            {
                EngineGrpcServer::lock_t lock(this->_engineCallLock);

                // Run engine-specific reset function
                this->_engineWrapper->reset();
            }
            catch(const std::exception &e)
            {
                return handleGrpcError("Error while executing initialization", e.what());
            }

            NRPLogger::debug("reset command returns OK");
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
        grpc::Status shutdown(      grpc::ServerContext         * /*context*/,
                              const EngineGrpc::ShutdownRequest * request,
                                    EngineGrpc::ShutdownReply   * /*reply*/) override
        {
            try
            {
                EngineGrpcServer::lock_t lock(this->_engineCallLock);

                // TODO: we don't have any instance where json data is actually sent to an engine for shutdown,
                //  it can be removed from EngineGrpc::ShutdownRequest definition
                nlohmann::json requestJson = nlohmann::json::parse(request->json());

                // Run engine-specifi shutdown function

                this->_engineWrapper->shutdown();
            }
            catch(const std::exception &e)
            {
                return handleGrpcError("Error while executing shutdown", e.what());
            }

            NRPLogger::debug("shutdown command returns OK");
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
        grpc::Status runLoopStep(      grpc::ServerContext            * /*context*/,
                                 const EngineGrpc::RunLoopStepRequest * request,
                                       EngineGrpc::RunLoopStepReply   * reply) override
        {
            try
            {
                EngineGrpcServer::lock_t lock(this->_engineCallLock);

                int64_t engineTime = (this->_engineWrapper->runLoopStep(SimulationTime(request->timestep()))).count();

                reply->set_enginetime(engineTime);
            }
            catch(const std::exception &e)
            {
                return handleGrpcError("Error while executing runLoopStep", e.what());
            }

            return grpc::Status::OK;
        }

        /*!
         * \brief Sets received data into proper datapacks
         *
         * The function implements the setDataPack method of the EngineGrpcService.
         * It acts as a wrapper around the virtual setDataPackData method, which should set received data into proper datapacks.
         * On error, it will return a status object with error message and grpc::StatusCode::CANCELLED error code.
         *
         * \param      context Pointer to gRPC server context structure
         * \param[in]  request Pointer to protobuf setDataPack request message. Contains datapack data and metadata
         * \param[out] reply   Pointer to protobuf setDataPack reply message. Currently no data is returned.
         *
         * \return gRPC request status
         */
        grpc::Status setDataPacks(      grpc::ServerContext             * /*context*/,
                                  const EngineGrpc::SetDataPacksRequest * request,
                                        EngineGrpc::SetDataPacksReply   * /*reply*/) override
        {
            try
            {
                EngineGrpcServer::lock_t lock(this->_engineCallLock);

                this->_engineWrapper->setDataPacks(*request);
            }
            catch(const std::exception &e)
            {
                return handleGrpcError("Error while executing setDataPack", e.what());
            }

            return grpc::Status::OK;
        }

        /*!
         * \brief Gets data from requested datapacks
         *
         * The function implements the getDataPack method of the EngineGrpcService.
         * It acts as a wrapper around the virtual getDataPackData method, which should get data from requested datapacks.
         * On error, it will return a status object with error message and grpc::StatusCode::CANCELLED error code.
         *
         * \param      context Pointer to gRPC server context structure
         * \param[in]  request Pointer to protobuf setDataPack request message. Contains metadata of requested datapacks.
         * \param[out] reply   Pointer to protobuf setDataPack reply message. Contains datapack data and metadata.
         *
         * \return gRPC request status
         */
        grpc::Status getDataPacks(      grpc::ServerContext             * /*context*/,
                                  const EngineGrpc::GetDataPacksRequest * request,
                                        EngineGrpc::GetDataPacksReply   * reply) override
        {
            try
            {
                EngineGrpcServer::lock_t lock(this->_engineCallLock);

                this->_engineWrapper->getDataPacks(*request, reply);
            }
            catch(const std::exception &e)
            {
                return handleGrpcError("Error while executing getDataPack", e.what());
            }

            return grpc::Status::OK;
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
            NRPLogger::error("context message: [ {} ]", contextMessage);
            NRPLogger::error("error message: [ {} ]", errorMessage);

            // Pass the error message to the client

            grpc::Status status(grpc::StatusCode::CANCELLED, errorMessage);

            return status;
        }
};

#endif // ENGINE_GRPC_SERVER_H
