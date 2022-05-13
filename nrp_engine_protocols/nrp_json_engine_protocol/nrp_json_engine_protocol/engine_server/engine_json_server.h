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

#ifndef ENGINE_JSON_SERVER_H
#define ENGINE_JSON_SERVER_H

#include "nrp_json_engine_protocol/config/engine_json_config.h"
#include "nrp_json_engine_protocol/engine_server/json_datapack_controller.h"

#include "nrp_general_library/datapack_interface/datapack.h"
#include "nrp_general_library/utils/time_utils.h"
#include "nrp_general_library/utils/nrp_logger.h"

#include <map>
#include <memory>
#include <pistache/router.h>
#include <pistache/endpoint.h>
#include <nlohmann/json.hpp>

/*!
 *  \brief Manages communication with the NRP. Uses a REST server to send/receive data. Singleton class.
 */
class EngineJSONServer
{
        /*!
         * \brief Time to wait for shutdown before raising an exception (in milliseconds)
         */
        static constexpr auto ShutdownWaitTime = std::chrono::milliseconds(10*1000);

    protected:
        static constexpr std::string_view GetDataPackInformationRoute = EngineJSONConfigConst::EngineServerGetDataPacksRoute;
        static constexpr std::string_view SetDataPackRoute = EngineJSONConfigConst::EngineServerSetDataPacksRoute;
        static constexpr std::string_view RunLoopStepRoute = EngineJSONConfigConst::EngineServerRunLoopStepRoute;
        static constexpr std::string_view InitializeRoute = EngineJSONConfigConst::EngineServerInitializeRoute;
        static constexpr std::string_view ResetRoute = EngineJSONConfigConst::EngineServerResetRoute;
        static constexpr std::string_view ShutdownRoute = EngineJSONConfigConst::EngineServerShutdownRoute;

    public:
        using mutex_t = std::timed_mutex;
        using lock_t = std::unique_lock<EngineJSONServer::mutex_t>;

        /*!
         * \brief Constructor. Tries to bind to a port and register itself with clientAddress
         * \param engineAddress Server Address. If it contains a port, will try to bind to said port. If that fails, will increment port number and try again. This will continue for at most EngineJSONConfigConst::MaxAddrBindTries times
         * \param engineName Engine Name
         * \param clientAddress Client Address. The server will try to register itself under this address
         */
        EngineJSONServer(const std::string &engineAddress, const std::string &engineName, const std::string &clientAddress);

        /*!
         * \brief Constructor. Will try to bind to engineAddress
         * \param engineAddress Server address
         */
        EngineJSONServer(const std::string &engineAddress);
        EngineJSONServer();
        virtual ~EngineJSONServer();

        // Delete copy mechanisms
        EngineJSONServer(const EngineJSONServer&) = delete;
        EngineJSONServer &operator=(const EngineJSONServer&) = delete;


        /*!
         * \brief Is the server running?
         * \return Returns true if the server is running
         */
        bool isServerRunning() const;

        /*!
         * \brief Start the server in asynchronous mode
         */
        void startServerAsync();

        /*!
         * \brief Start the server synchronously
         */
        void startServer();

        /*!
         * \brief Stop running server
         */
        void shutdownServer();

        /*!
         * \brief Get running server port
         * \return Returns port of running server, 0 if server is not running
         */
        uint16_t serverPort() const;

        /*!
         * \brief Get server address
         */
        std::string serverAddress() const;

        /*!
         * \brief Registers a datapack
         * \param datapackName Name of datapack
         * \param interface Pointer to interface
         */
        void registerDataPack(const std::string &datapackName, JsonDataPackController *interface);

        /*!
         * \brief Registers a datapack. Skips locking the mutex.
         * Should only be used if thread-safe access to _datapacksControllers can be guaranteed
         * \param datapackName Name of datapack
         * \param interface Pointer to interface
         */
        void registerDataPackNoLock(const std::string &datapackName, JsonDataPackController *interface);

        /*!
         * \brief Run a single loop step
         * \param timeStep Step to take
         * \return Returns the time registered by this engine at the end of the loop
         */
        virtual SimulationTime runLoopStep(SimulationTime timeStep) = 0;

        /*!
         * \brief Engine Initialization routine
         * \param data Initialization data
         * \param datapackLock DataPack Lock. Prevents access to _datapacksControllers
         * \return Returns data about initialization status
         */
        virtual nlohmann::json initialize(const nlohmann::json &data, EngineJSONServer::lock_t &datapackLock) = 0;

        /*!
         * \brief Engine reset routine
         * \param datapackLock DataPack Lock. Prevents access to _datapacksControllers
         * \return Returns data about initialization status
         */
        virtual nlohmann::json reset(EngineJSONServer::lock_t &datapackLock) = 0;

        /*!
         * \brief Engine Shutdown routine
         * \param data Shutdown data
         * \return Returns data about shutdown status
         */
        virtual nlohmann::json shutdown(const nlohmann::json &data) = 0;

        /*!
         * \brief Has a shutdown command been received?
         * \return Returns true if a shutdown command has been received
         */
        bool shutdownFlag();

    protected:
        /*!
         * \brief Lock access to _datapacks to make execution thread-safe
         */
        mutex_t _datapackLock;

        /*!
         * \brief Remove all registered datapacks
         */
        void clearRegisteredDataPacks();


        /*!
         * \brief Retrieves datapack data. Takes an array of datapack names for which to get data.
         * \param reqData A JSON array containing datapack names.
         * \return DataPack data, formatted as a JSON array
         */
        virtual nlohmann::json getDataPackData(const nlohmann::json &reqData);

        /*!
         * \brief Set datapack data
         * \param reqData A JSON array containing datapack names linked to the individual datapack's data
         * \return Execution result
         */
        virtual void setDataPackData(const nlohmann::json &reqData);

    private:
        /*!
         * \brief Is the server running?
         */
        bool _serverRunning = false;

        /*!
         * \brief Shutdown Flag. Set to true once the shutdown signal has been received
         */
        bool _shutdownFlag = false;

        /*!
         * \brief Mutex used to access the shutdown flag in a thread-safe manner
         */
        std::mutex _shutdown_mutex;

        /*!
         *  \brief ServerURL
         */
        std::string _serverAddress;

        /*!
         * \brief Routes
         */
        Pistache::Rest::Router _router;

        using enpoint_ptr_t = std::unique_ptr<Pistache::Http::Endpoint>;

        /*!
         * \brief Server Endpoint
         */
        enpoint_ptr_t _pEndpoint = nullptr;

        /*!
         * \brief Available datapacks
         */
        std::map<std::string, JsonDataPackController*> _datapacksControllers;

        NRPLogger _loggerCfg;

        /*!
         * \brief Set routes used by server
         */
        static Pistache::Rest::Router setRoutes(EngineJSONServer *server);

        /*!
         * \brief Parse request
         * \param req Request
         * \paragraph res Response Writer. Sends back failure message if parse failed
         * \return Returns JSON object
         */
        static nlohmann::json parseRequest(const Pistache::Rest::Request &req, Pistache::Http::ResponseWriter &res);

        /*!
         * \brief Callback function to retrieve datapack data. Takes an array of datapack names for which to get data.
         * \param req DataPack data request. A JSON array containing datapack names.
         * \param res Response writer that sends datapack data back to NRP, formatted as a JSON array.
         */
        void getDataPackDataHandler(const Pistache::Rest::Request &req, Pistache::Http::ResponseWriter res);

        /*!
         * \brief Callback function to set datapack data
         * \param req DataPack Data. A JSON array containing datapack names linked to the individual datapack's data
         * \param res Response writer that sends execution result back to the NRP
         */
        void setDataPackProcessorr(const Pistache::Rest::Request &req, Pistache::Http::ResponseWriter res);

        /*!
         * \brief Try to get iterator key, print error message and throw exception if no key is available
         * \param jsonIterator JSON object iterator
         * \return Returns key
         */
        static const std::string &getIteratorKey(const nlohmann::json::const_iterator &jsonIterator);

        /*!
         * \brief Callback function to run loop step
         * \param req Loop Data. Only contains the simulation time to be executed (in s)
         * \param res Response writer that sends OK on completion
         */
        void runLoopStepHandler(const Pistache::Rest::Request &req, Pistache::Http::ResponseWriter res);

        /*!
         * \brief Callback function to initialize
         * \param req Initialization Data
         * \param res Response writer. Contains initialization status
         */
        void initializeHandler(const Pistache::Rest::Request &req, Pistache::Http::ResponseWriter res);

        /*!
         * \brief Callback function to reset
         * \param req Reset Data
         * \param res Response writer. Contains initialization status
         */
        void resetHandler(const Pistache::Rest::Request &req, Pistache::Http::ResponseWriter res);

        /*!
         * \brief Callback function for shutdown routine
         * \param req Shutdown Data
         * \param res Response writer. Contains shutdown data
         */
        void shutdownHandler(const Pistache::Rest::Request &req, Pistache::Http::ResponseWriter res);

        /*!
         * \brief Creates a REST server. Tries to bind to a port and register itself with clientAddress
         * \param engineAddress Server Address. If it contains a port, will try to bind to said port. If that fails, will increment port number and try again. This will continue for at most EngineJSONConfigConst::MaxAddrBindTries times
         * \param engineName Engine Name
         * \param clientAddress Client Address. The server will try to register itself under this address
         */
        static Pistache::Http::Endpoint createEndpoint(std::string *engineAddress, const std::string &engineName);

        friend class EngineJSONServerTest_Functions_Test;
        friend class TestEngineJSONServer;
};

#endif // ENGINE_JSON_SERVER_H
