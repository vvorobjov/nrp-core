
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

#ifndef PYTHON_ENGINE_GRPC_NRP_CLIENT_H
#define PYTHON_ENGINE_GRPC_NRP_CLIENT_H

#include "nrp_general_library/plugin_system/plugin.h"

#include "nrp_general_library/engine_interfaces/engine_client_interface.h"

#include "nrp_grpc_engine_protocol/engine_client/engine_grpc_client.h"
#include "nrp_grpc_engine_protocol/config/engine_grpc_config.h"

#include "nrp_python_grpc_engine/config/cmake_constants.h"
#include "nrp_python_grpc_engine/config/python_grpc_config.h"
#include "nrp_protobuf/engine_grpc.pb.h"

#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/prctl.h>
#include <chrono>
#include <unistd.h>


/*!
 * \brief PythonGRPCEngine client
 */
class PythonEngineGRPCNRPClient
: public EngineGrpcClient<PythonEngineGRPCNRPClient, PythonGrpcConfigConst::EngineSchema>
{
    public:

    PythonEngineGRPCNRPClient(nlohmann::json &config, ProcessLauncherInterface::unique_ptr &&launcher)
            : EngineGrpcClient(config, std::move(launcher))
    {
        NRP_LOGGER_TRACE("{} called", __FUNCTION__);

        setDefaultProperty<std::string>("EngineProcCmd", NRP_PYTHON_GRPC_EXECUTABLE_PATH);
    }

    virtual ~PythonEngineGRPCNRPClient() override
    {
            NRP_LOGGER_TRACE("{} called", __FUNCTION__);
    }

    virtual void initialize() override
    {
        NRP_LOGGER_TRACE("{} called", __FUNCTION__);

        // Pass the ratio used by SimulationTime to the server
        // Based on the ratio, the server should be able to assert that it's using correct time units

        nlohmann::json config = this->engineConfig();
        config[PythonGrpcConfigConst::SimulationTimeRatio.data()] = { SimulationTime::period::num, SimulationTime::period::den };

        try
        {
            this->sendInitializeCommand(config);
        }
        catch(std::exception &e)
        {
            // Write the error message
            this->_initErrMsg = e.what();
            NRPLogger::error(this->_initErrMsg);

            throw NRPException::logCreate("Initialization failed: " + this->_initErrMsg);
        }


        NRPLogger::debug("PythonEngineGRPCNRPClientBase::initialize(...) completed with no errors.");
    }

    virtual void reset() override
    {
        NRP_LOGGER_TRACE("{} called", __FUNCTION__);

        try
        {
            this->sendResetCommand();
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

    virtual const std::vector<std::string> engineProcStartParams() const override
    {
        NRP_LOGGER_TRACE("{} called", __FUNCTION__);

        std::vector<std::string> startParams = this->engineConfig().at("EngineProcStartParams");

        std::string name = this->engineConfig().at("EngineName");
        startParams.push_back(std::string("--") + EngineGRPCConfigConst::EngineNameArg.data() + "=" + name);

        std::string address = this->engineConfig().at("ServerAddress");
        startParams.push_back(std::string("--") + EngineGRPCConfigConst::EngineServerAddrArg.data() + "=" + address);

        return startParams;
    }

    private:
        /*!
         * \brief Error message returned by init command
         */
        std::string _initErrMsg = "";
};

using PythonEngineGRPCLauncher = PythonEngineGRPCNRPClient::EngineLauncher<PythonGrpcConfigConst::EngineType>;


CREATE_NRP_ENGINE_LAUNCHER(PythonEngineGRPCLauncher);


#endif // PYTHON_ENGINE_GRPC_NRP_CLIENT_H
