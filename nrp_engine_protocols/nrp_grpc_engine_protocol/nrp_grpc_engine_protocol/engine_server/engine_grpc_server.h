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

#include "nrp_protobuf/engine_grpc.grpc.pb.h"
#include "nrp_general_library/engine_interfaces/datapack_controller.h"
#include "nrp_general_library/utils/time_utils.h"
#include "nrp_protobuf/config/cmake_constants.h"
#include "nrp_protobuf/proto_python_bindings/proto_field_ops.h"
#include "nrp_protobuf/proto_ops/protobuf_ops.h"
#include "nrp_protobuf/proto_ops/proto_ops_manager.h"


using ProtoDataPackController = DataPackController<google::protobuf::Message>;

using EngineGrpc::EngineGrpcService;

/*!
 * \brief Abstract class for Engine server with gRPC support
 *
 * The class provides a base for implementing an Engine server with gRPC
 * as middleware. All RPC services are implemented. Derived classes are responsible
 * for implementing simulation initialization, shutdown and run step methods.
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
         * \param[in] engineName    Name of the simulation engine
         */
        EngineGrpcServer(const std::string serverAddress, const std::string &engineName,
                         const std::string &protobufPluginsPath,
                         const nlohmann::json &protobufPlugins)
                : _loggerCfg(engineName), _engineName(engineName)
        {
            this->_serverAddress   = serverAddress;
            this->_isServerRunning = false;

            grpc::EnableDefaultHealthCheckService(true);

            NRPLogger::info("EngineGrpcServer {} has been created", engineName);

            ProtoOpsManager::getInstance().addPluginPath(protobufPluginsPath);
            for (const auto &packageName: protobufPlugins) {
                auto packageNameStr = packageName.template get<std::string>();
                _protoOpsStr += packageNameStr + ",";
                std::stringstream pluginLibName;
                pluginLibName << "lib" << NRP_PROTO_OPS_LIB_PREFIX << packageNameStr <<
                              NRP_PROTO_OPS_LIB_SUFIX << ".so";
                auto pluginLib = ProtoOpsManager::getInstance().loadPlugin(pluginLibName.str());
                if(pluginLib)
                    _protoOps.template emplace_back(std::move(pluginLib));
                else
                    throw NRPException::logCreate("Failed to load ProtobufPackage \""+packageNameStr+"\"");
            }

            if(!_protoOpsStr.empty())
                _protoOpsStr.pop_back();
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
         * \brief Registers a datapack controller with the given name in the engine
         *
         * \param[in] datapackName       Name of the datapack to be registered
         * \param[in] datapackController Pointer to the datapack controller object that's supposed to be
         *                             registered in the engine
         */
        void registerDataPack(const std::string & datapackName, ProtoDataPackController *interface)
        {
            EngineGrpcServer::lock_t lock(this->_datapackLock);
            this->registerDataPackNoLock(datapackName, interface);
        }

        void registerDataPackNoLock(const std::string & datapackName, ProtoDataPackController *interface)
        {
            this->_datapacksControllers.emplace(datapackName, interface);
        }

        // TODO used only in tests, try to remove it?
        unsigned getNumRegisteredDataPacks()
        {
            return this->_datapacksControllers.size();
        }

        /*!
         * \brief Get the Engine name
         */
        const std::string &getEngineName() { return _engineName; }

    protected:
        mutex_t                       _datapackLock;

        void clearRegisteredDataPacks()
        {
            // TODO Check if it's true
            // Do not lock scope. This method is called from the route handlers, which should already have locked down access.
            //EngineJSONServer::lock_t lock(this->_datapackLock);

            this->_datapacksControllers.clear();
        }

        /*!
        * \brief Returns the pointer to the DataPackController of the Data Pack with the specified name
        */
        ProtoDataPackController* getDataPackController(const std::string & datapackName)
        {
            return this->_datapacksControllers.find(datapackName)->second;
        }


        /*!
        * \brief If true controllers are sent incoming DataPackMessages, if false only the contained data
        */
        bool _handleDataPackMessage = false;

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
         * in the datapack metadata, which allows for additional consistency checks.
         */
        std::string _engineName;

        /*!
         * \brief Pointer to the gRPC server object
         */
        std::unique_ptr<grpc::Server> _server;

        /*!
         * \brief Map of datapack names and datapack controllers used by the engine
         */
         std::map<std::string, ProtoDataPackController*> _datapacksControllers;

        /*!
         * \brief Initializes the simulation
         *
         * \param[in] data       Simulation configuration data
         * \param[in] datapackLock ???
         */
        virtual void initialize(const nlohmann::json &data, EngineGrpcServer::lock_t &datapackLock) = 0;

        /*!
         * \brief Resets the simulation
         */
        virtual void reset() = 0;

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
        grpc::Status initialize(      grpc::ServerContext           * /*context*/,
                                const EngineGrpc::InitializeRequest * request,
                                      EngineGrpc::InitializeReply   * /*reply*/) override
        {
            try
            {
                EngineGrpcServer::lock_t lock(this->_datapackLock);

                nlohmann::json requestJson = nlohmann::json::parse(request->json());

                // Run engine-specific initialization function

                this->initialize(requestJson, lock);
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
                EngineGrpcServer::lock_t lock(this->_datapackLock);

                // Run engine-specific reset function
                this->reset();
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
                EngineGrpcServer::lock_t lock(this->_datapackLock);

                nlohmann::json requestJson = nlohmann::json::parse(request->json());

                // Run engine-specifi shutdown function

                this->shutdown(requestJson);
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
                EngineGrpcServer::lock_t lock(this->_datapackLock);

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
                this->setDataPacksData(*request);
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
                this->getDataPacksData(*request, reply);
            }
            catch(const std::exception &e)
            {
                return handleGrpcError("Error while executing getDataPack", e.what());
            }

            return grpc::Status::OK;
        }

        virtual void setDataPacksData(const EngineGrpc::SetDataPacksRequest & data)
        {
            EngineGrpcServer::lock_t lock(this->_datapackLock);

            const auto numDataPacks = data.datapacks_size();

            for(int i = 0; i < numDataPacks; i++)
            {
                const auto &dataPack = data.datapacks(i);
                const auto &devInterface = this->_datapacksControllers.find(dataPack.datapackid().datapackname());

                if(devInterface != _datapacksControllers.end())
                {
                    if(_handleDataPackMessage)
                        devInterface->second->handleDataPackData(dataPack);
                    else
                    {
                        std::unique_ptr<gpb::Message> protoMsg;

                        for(auto& mod : _protoOps) {
                            try {
                                protoMsg = mod->getDataFromDataPackMessage(dataPack);
                            }
                            catch (NRPException &) {
                                // this just means that the module couldn't process the request, try with the next one
                            }
                        }

                        if(protoMsg)
                            devInterface->second->handleDataPackData(*protoMsg);
                        else
                            throw NRPException::logCreate("In Engine \"" + dataPack.datapackid().enginename() + "\", unable to deserialize datapack \"" +
                                                                  dataPack.datapackid().datapackname() + "\" using any of the NRP-Core Protobuf plugins specified in the"
                                                                  " engine configuration: [" + _protoOpsStr + "]. Ensure that the parameter "
                                                                  "\"ProtobufPackages\" is properly set in the Engine configuration");
                    }
                }
                else
                {
                    const auto errorMessage = "DataPack " + dataPack.datapackid().datapackname() + " is not registered in engine " + this->_engineName;
                    throw std::invalid_argument(errorMessage);
                }
            }
        }

        virtual void getDataPacksData(const EngineGrpc::GetDataPacksRequest & request, EngineGrpc::GetDataPacksReply * reply)
        {
            EngineGrpcServer::lock_t lock(this->_datapackLock);

            const auto numDataPacks = request.datapackids_size();

            for(int i = 0; i < numDataPacks; i++)
            {
                const auto &devInterface = this->_datapacksControllers.find(request.datapackids(i).datapackname());
                if(devInterface != _datapacksControllers.end())
                {
                    auto * protoDataPack = reply->add_datapacks();

                    protoDataPack->mutable_datapackid()->set_datapackname(request.datapackids(i).datapackname());
                    protoDataPack->mutable_datapackid()->set_enginename(this->_engineName);

                    // ask controller to fetch datapack data. nullptr means there is no new data available
                    auto d = devInterface->second->getDataPackInformation();
                    if(d != nullptr) {
                        bool isSet = false;
                        for(auto& mod : _protoOps) {
                            try {
                                mod->setDataPackMessageData(*d, protoDataPack);
                                isSet = true;
                            }
                            catch (NRPException &) {
                                // this just means that the module couldn't process the request, try with the next one
                            }
                        }

                        if(!isSet)
                            throw NRPException::logCreate("In Engine \"" + protoDataPack->datapackid().enginename() + "\", unable to serialize datapack \"" +
                                                                  protoDataPack->datapackid().datapackname() + "\" using any of the NRP-Core Protobuf plugins specified in the"
                                                                  " engine configuration: [" + _protoOpsStr + "]. Ensure that the parameter "
                                                                  "\"ProtobufPackages\" is properly set in the Engine configuration");
                    }
                }
                else
                {
                    const auto errorMessage = "DataPack " + request.datapackids(i).datapackname() + " is not registered in engine " + this->_engineName;
                    throw std::invalid_argument(errorMessage);
                }
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
            NRPLogger::error("context message: [ {} ]", contextMessage);
            NRPLogger::error("error message: [ {} ]", errorMessage);

            // Pass the error message to the client

            grpc::Status status(grpc::StatusCode::CANCELLED, errorMessage);

            return status;
        }

    protected:
        std::vector<std::unique_ptr<protobuf_ops::NRPProtobufOpsIface>> _protoOps;
        std::string _protoOpsStr = "";
};

#endif // ENGINE_GRPC_SERVER_H
