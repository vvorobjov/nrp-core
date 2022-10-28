
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

#ifndef PYTHON_ENGINE_JSON_NRP_CLIENT_BASE_H
#define PYTHON_ENGINE_JSON_NRP_CLIENT_BASE_H

#include "nrp_general_library/engine_interfaces/engine_client_interface.h"

#include "nrp_json_engine_protocol/nrp_client/engine_json_nrp_client.h"
#include "nrp_json_engine_protocol/config/engine_json_config.h"

#include "nrp_python_json_engine/config/cmake_constants.h"
#include "nrp_python_json_engine/config/python_config.h"

#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/prctl.h>
#include <chrono>
#include <unistd.h>

/*!
 * \brief NRP - Python Communicator on the NRP side. Converts DataPackInterface classes from/to JSON objects
 */
template<class ENGINE, const char* SCHEMA>
class PythonEngineJSONNRPClientBase
        : public EngineJSONNRPClient<ENGINE, SCHEMA>
{
        /*!
         * \brief Time (in seconds) to wait for Python to exit cleanly after first SIGTERM signal. Afterwards, send a SIGKILL
         */
        static constexpr size_t _killWait = 10;

    public:

        PythonEngineJSONNRPClientBase(nlohmann::json &config, ProcessLauncherInterface::unique_ptr &&launcher)
                : EngineJSONNRPClient<ENGINE, SCHEMA>(config, std::move(launcher))
        {
            NRP_LOGGER_TRACE("{} called", __FUNCTION__);
        }

        virtual ~PythonEngineJSONNRPClientBase() override
        {
            NRP_LOGGER_TRACE("{} called", __FUNCTION__);
        }

        virtual void initialize() override
        {
            NRP_LOGGER_TRACE("{} called", __FUNCTION__);

            // Pass the ratio used by SimulationTime to the server
            // Based on the ratio, the server should be able to assert that it's using correct time units

            nlohmann::json config = this->engineConfig();
            config[PythonConfigConst::SimulationTimeRatio.data()] = { SimulationTime::period::num, SimulationTime::period::den };

            try
            {
                nlohmann::json resp = this->sendInitCommand(config);
            }
            catch(std::exception &e)
            {
                // Write the error message
                this->_initErrMsg = e.what();
                NRPLogger::error(this->_initErrMsg);

                throw NRPException::logCreate("Initialization failed: " + this->_initErrMsg);
            }


            NRPLogger::debug("PythonEngineJSONNRPClientBase::initialize(...) completed with no errors.");
        }

        virtual void reset() override
        {
            NRP_LOGGER_TRACE("{} called", __FUNCTION__);

            try
            {
                nlohmann::json resp = this->sendResetCommand(nlohmann::json("reset"));
                NRPLogger::debug("NestEngineJSONNRPClient:reset()::resp [ {} ]", resp.dump());
            }
            catch(std::exception &e)
            {
                // Write the error message
                std::string msg = e.what();
                NRPLogger::error(msg);

                throw NRPException::logCreate("Reset failed: " + msg);
            }

            this->resetEngineTime();
        }

        virtual void shutdown() override
        {
            NRP_LOGGER_TRACE("{} called", __FUNCTION__);

            this->sendShutdownCommand(nlohmann::json());
        }

    private:
        /*!
         * \brief Error message returned by init command
         */
        std::string _initErrMsg = "";

        virtual const std::vector<std::string> engineProcStartParams() const override
        {
            NRP_LOGGER_TRACE("{} called", __FUNCTION__);

            std::vector<std::string> startParams = EngineJSONNRPClient<ENGINE, SCHEMA>::engineProcStartParams();

            // Pass any extra server options that were specififed by the user
            std::string serverOptions = this->engineConfig().at("ServerOptions");
            startParams.push_back(std::string("--") + PythonConfigConst::ExtraServerOptionsArg.data() + "=" + serverOptions);

            return startParams;
        }
};

#endif // PYTHON_ENGINE_JSON_NRP_CLIENT_BASE_H