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

#ifndef ENGINE_CLIENT_INTERFACE_H
#define ENGINE_CLIENT_INTERFACE_H

#include "nrp_general_library/process_launchers/process_launcher.h"
#include "nrp_general_library/utils/fixed_string.h"
#include "nrp_general_library/utils/ptr_templates.h"
#include "nrp_general_library/utils/time_utils.h"
#include "nrp_general_library/utils/json_schema_utils.h"
#include "nrp_general_library/datapack_interface/datapack_interface.h"

#include <set>
#include <vector>
#include <future>

class EngineClientInterface;
class EngineLauncherInterface;


/*!
 * \brief Interface to engines
 */
class EngineClientInterface
        : public PtrTemplates<EngineClientInterface>
{
        /*! \brief DataPackInterfaceConstSharedPtr comparison. Used for set sorting */
        struct CompareDevInt : public std::less<>
        {
            public: bool operator()(const DataPackInterfaceConstSharedPtr &lhs, const DataPackInterfaceConstSharedPtr &rhs) const
            {   return lhs->name() < rhs->name();   }

            public: bool operator()(const DataPackInterfaceConstSharedPtr &lhs, const std::string &name) const
            {   return lhs->name() < name;  }

            public: bool operator()(const std::string &name, const DataPackInterfaceConstSharedPtr &rhs) const
            {   return name < rhs->name();  }
        };

    public:
        using datapack_identifiers_set_t = std::set<DataPackIdentifier>;
        using datapacks_t = std::vector<DataPackInterfaceConstSharedPtr>;
        using datapacks_set_t = std::set<DataPackInterfaceConstSharedPtr, CompareDevInt>;
        using datapacks_ptr_t = std::vector<DataPackInterface*>;

        explicit EngineClientInterface(ProcessLauncherInterface::unique_ptr &&launcher);
        virtual ~EngineClientInterface();

        /*!
         * \brief Get Engine Name
         * \return Returns engine name
         */
        virtual const std::string engineName() const = 0;

        /*!
         * \brief Get engine config data
         */
        virtual const nlohmann::json &engineConfig() const = 0;

        /*!
        * \brief Get engine config data
        */
        virtual nlohmann::json &engineConfig() = 0;

        /*!
         * \brief Get all Engine Process Startup parameters.
         */
        virtual const std::vector<std::string> engineProcStartParams() const = 0;

        /*!
         * \brief Get all Engine Process Environment variables.
         */
        virtual const std::vector<std::string> engineProcEnvParams() const = 0;

        /*!
         * \brief Launch the engine
         * \return Returns engine process ID on success, throws on failure
         */
        virtual pid_t launchEngine();

        /*!
         * \brief Initialize engine
         * \return Returns SUCCESS if no error was encountered
         * \throw Throws on error
         */
        virtual void initialize() = 0;

        /*!
         * \brief Reset engine
         * \return Returns SUCCESS if no error was encountered
         * \throw Throws on error
         */
        virtual void reset() = 0;

        /*!
         * \brief Shutdown engine
         * \return Return SUCCESS if no error was encountered
         * \throw Throws on error
         */
        virtual void shutdown() = 0;

        /*!
         * \brief Get engine timestep
         * \throw Throws on error
         */
        virtual SimulationTime getEngineTimestep() const = 0;

        /*!
         * \brief Get current engine time
         * \return Returns engine time
         */
        virtual SimulationTime getEngineTime() const = 0;

        /*!
         * \brief Get json schema for this specific engine type.
         * \return Returns URI of engine schema
         * \throw Throws on error
         */
        virtual const std::string engineSchema() const = 0;

        /*!
         * \brief Starts a single loop step in a separate thread
         *
         * The function should be called in tandem with EngineClientInterface::runLoopStepAsyncGet(),
         * which will join the worker thread and retrieve the results of the loop step.
         *
         * \param[in] timeStep Requested duration of the simulation loop step.
         */
        virtual void runLoopStepAsync(SimulationTime timeStep) = 0;

        /*!
         * \brief Waits and gets the results of the loop step started by EngineClientInterface::runLoopStepAsync()
         *
         * The function should be called after calling EngineClientInterface::runLoopStepAsync().
         * It should join the worker thread and retrieve the results of the loop step.
         *
         * \param timeOut Timeout of the loop step. If it's less or equal to 0, the function will wait indefinitely.
         */
        virtual void runLoopStepAsyncGet(SimulationTime timeOut) = 0;

        /*!
         * \brief Gets requested datapacks from engine and updates _datapackCache with the results
         * Uses getDataPacksFromEngine override for the actual communication
         * \param datapackNames All requested names. NOTE: can also include IDs of other engines. A check must be added that only the corresponding IDs are retrieved
         * \return Returns all datapacks returned by the engine
         */
        const datapacks_t &updateDataPacksFromEngine(const datapack_identifiers_set_t &datapackIdentifiers);

        /*!
         * \brief get cached engine datapacks
         */
        constexpr const datapacks_t &getCachedDataPacks() const
        {   return this->_datapackCache;    }

        /*!
         * \brief Sends datapacks to engine
         * \param datapacksArray Array of datapacks that will be send to the engine
         * \return Returns SUCCESS if all datapacks could be handles, ERROR otherwise
         * \throw Throws on error
         */
        virtual void sendDataPacksToEngine(const datapacks_ptr_t &datapacksArray) = 0;

        /*!
         * \brief Update _datapackCache from datapacks
         *
         * If the datapack with a particular name is already in the cache, the function will
         * replace it. If the datapack isn't in the cache, the function will insert it.
         *
         * \param devs DataPacks to insert
         */
        void updateCachedDataPacks(datapacks_set_t &&devs);

        /*!
         * \brief Gets requested datapacks from engine
         * \param datapackNames All requested datapack ids
         * \return Returns all requested datapacks
         * \throw Throws on error
         */
        virtual datapacks_set_t getDataPacksFromEngine(const datapack_identifiers_set_t &datapackIdentifiers) = 0;

protected:

        /*!
         * \brief Process Launcher. Will be used to stop process at end
         */
        ProcessLauncherInterface::unique_ptr _process;

        /*!
         * \brief Engine datapack cache. Stores retrieved datapacks
         */
        datapacks_t _datapackCache;
};

using EngineClientInterfaceSharedPtr = EngineClientInterface::shared_ptr;
using EngineClientInterfaceConstSharedPtr = EngineClientInterface::const_shared_ptr;

class EngineLauncherInterface
        : public PtrTemplates<EngineLauncherInterface>
{
    public:
        using engine_type_t = decltype(DataPackIdentifier::Type);

        EngineLauncherInterface(const engine_type_t &engineType);
        virtual ~EngineLauncherInterface() = default;

        const engine_type_t &engineType() const;
        virtual EngineClientInterfaceSharedPtr launchEngine(nlohmann::json  &engineConfig, ProcessLauncherInterface::unique_ptr &&launcher) = 0;

    private:
        /*!
         * \brief Engine Type
         */
        const engine_type_t _engineType;
};

using EngineLauncherInterfaceSharedPtr = EngineLauncherInterface::shared_ptr;
using EngineLauncherInterfaceConstSharedPtr = EngineLauncherInterface::const_shared_ptr;

/*!
 *  \brief Base class for all Engines
 *  \tparam ENGINE Final derived engine class
 */
template<class ENGINE, const char *SCHEMA>
class EngineClient
        : public EngineClientInterface
{
    public:
        using engine_t = ENGINE;

        /*!
         * \brief Class for launching engine
         * \tparam ENGINE_TYPE Default engine type
         */
        template<const char *ENGINE_TYPE>
        class EngineLauncher
                : public EngineLauncherInterface
        {
            public:
                EngineLauncher()
                    : EngineLauncherInterface(ENGINE_TYPE)
                {}

                EngineLauncher(const engine_type_t &engineType)
                    : EngineLauncherInterface(engineType)
                {}

                ~EngineLauncher() override = default;

                /*!
                 * \brief Launches an engine. Configures config and forks a new child process for the engine
                 * \param engineConfig Engine Configuration
                 * \param launcher Process Forker
                 * \return Returns pointer to EngineClientInterface
                 */
                EngineClientInterfaceSharedPtr launchEngine(nlohmann::json &engineConfig, ProcessLauncherInterface::unique_ptr &&launcher) override
                {
                    auto launcherName = launcher->launcherName();

                    EngineClientInterfaceSharedPtr engine(new ENGINE(engineConfig, std::move(launcher)));

                    switch (engine->launchEngine())
                    {
                        case 0: { // TODO: process not forked (error)
                                NRPLogger::error(
                                    "\"{}\" EngineClient (type: \"{}\") could NOT launch an EngineServer using \"{}\" launcher.", engine->engineName(), this->engineType(), launcherName);
                            }; break;
                        case -1: { // process not forked (empty launcher, expected behaviour)
                                NRPLogger::info(
                                    "\"{}\" EngineClient (type: \"{}\") won't launch an EngineServer. Assume it's already been launched.", engine->engineName(), this->engineType());
                            }; break;
                        default: { // success
                                NRPLogger::info(
                                    "\"{}\" EngineServer (type: \"{}\") launched successfully by \"{}\" launcher.", engine->engineName(), this->engineType(), launcherName);
                            }; break;
                    };

                    return engine;
                }
        };

        /*!
         * \brief Constructor
         * \param engineConfig Engine Configuration
         * \param launcher Process Forker
         */
        EngineClient(nlohmann::json  &engineConfig, ProcessLauncherInterface::unique_ptr &&launcher)
            : EngineClientInterface(std::move(launcher)),
              engineConfig_(engineConfig)
        {
            // validate engine config
            json_utils::validateJson(this->engineConfig(), this->engineSchema());

            // setting process start and env params to an empty vector since this can't be done from json schema
            setDefaultProperty<std::vector<std::string>>("EngineProcStartParams", std::vector<std::string>());
            setDefaultProperty<std::vector<std::string>>("EngineEnvParams", std::vector<std::string>());
        }

        ~EngineClient() override = default;

        const std::string engineName() const override final
        { return this->engineConfig().at("EngineName"); }

        SimulationTime getEngineTimestep() const override final
        {
            // We need to cast floating-point seconds to integers with units of SimulationTime type
            return toSimulationTime<float, std::ratio<1>>(this->engineConfig().at("EngineTimestep"));
        }

        /*!
         * \brief Get Engine Configuration
         */
        const nlohmann::json &engineConfig() const override final
        {   return engineConfig_;   }

        /*!
         * \brief Get Engine Configuration
         */
        nlohmann::json &engineConfig() override final
        {   return engineConfig_;   }

        /*!
         * \brief Get json schema for this engine type
         */
        const std::string engineSchema() const override final
        { return schema;   }

        /*!
         * \brief Returns current engine (simulation) time
         *
         * The time is updated by EngineClient::runLoopStepAsyncGet() method.
         *
         * \return Current engine (simulation) time
         */
        SimulationTime getEngineTime() const override
        {
            return this->_engineTime;
        }

        /*!
         * \brief Concrete implementation of EngineClientInterface::runLoopStepAsync()
         *
         * The function starts EngineClient::runLoopStepCallback() asynchronously using std::async.
         * The callback function should be provided by concrete engine implementation.
         * The result of the callback is going to be retrieved using an std::future object
         * in EngineClient::runLoopStepAsyncGet().
         *
         * \param[in] timeStep Requested duration of the simulation loop step.
         * \throw NRPException If the future object is still valid (EngineClient::runLoopStepAsyncGet() was not called)
         */
        void runLoopStepAsync(SimulationTime timeStep) override
        {
            if(this->_loopStepThread.valid())
            {
                throw NRPException::logCreate("Engine \"" + this->engineName() + "\" runLoopStepAsync has overrun");
            }

            this->_loopStepThread = std::async(std::launch::async, std::bind(&EngineClient::runLoopStepCallback, this, timeStep));
        }

        /*!
         * \brief Concrete implementation of EngineClientInterface::runLoopStepAsyncGet()
         *
         * The function should be called after EngineClient::runLoopStepAsync(). It will wait for the
         * worker thread to finish and retrieve the results from the future object. The value returned
         * by the future should be the simulation (engine) time after running the loop step. It will
         * be saved in the engine object, and can be accessed with EngineClient::getEngineTime().
         *
         * \param[in] timeOut Timeout of the loop step. If it's less or equal to 0, the function will wait indefinitely.
         * \throw NRPException On timeout
         */
        void runLoopStepAsyncGet(SimulationTime timeOut) override
        {
            NRP_LOGGER_TRACE("{} called", __FUNCTION__);

            // If thread state is invalid, loop thread has completed and runLoopStepAsyncGet was called once before
            if(!this->_loopStepThread.valid())
                return;

            // Wait until timeOut has passed
            if(timeOut > SimulationTime::zero())
            {
                if(this->_loopStepThread.wait_for(timeOut) != std::future_status::ready)
                    throw NRPException::logCreate("Engine \"" + this->engineName() + "\" loop is taking too long to complete");
            }

            // Retrieve the engine time returned from the loop step

            this->_engineTime = this->_loopStepThread.get();
        }

    protected:

        virtual void resetEngineTime()
        {
            this->_engineTime = SimulationTime(0);
        }

        /*!
        * \brief Attempts to set a default value for a property in the engine configuration. If the property has been already
         * set either in the engine configuration file or from the engine schema, its value is not overwritten.
        * \param key Name of the property to be set
        * \param value Default value to set
        */
        template<class T>
        void setDefaultProperty(std::string key, T value)
        {
            json_utils::setDefault<T>(this->engineConfig(), key, value);
        }

        /*!
         * \brief Executes a single loop step
         *
         * This function is going to be called by runLoopStep using std::async.
         * It will be executed by a worker thread, which allows for runLoopStepFunction
         * from multiple engines to run simultaneously.
         *
         * \param[in] timeStep A time step by which the simulation should be advanced
         * \return Engine time after loop step execution
         */
        virtual SimulationTime runLoopStepCallback(SimulationTime timeStep) = 0;

    private:

        nlohmann::json engineConfig_;
        std::string schema = std::string(SCHEMA);
        /*!
         * \brief Future of thread running a single loop. Used by runLoopStep and waitForStepCompletion to execute the thread
         */
        std::future<SimulationTime> _loopStepThread;

        /*!
         * \brief Engine Time
         */
        SimulationTime _engineTime = SimulationTime::zero();
};

#endif // ENGINE_CLIENT_INTERFACE_H
