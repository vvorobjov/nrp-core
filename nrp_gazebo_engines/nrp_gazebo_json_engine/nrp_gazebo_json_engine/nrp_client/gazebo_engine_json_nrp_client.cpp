//
// NRP Core - Backend infrastructure to synchronize simulations
//
// Copyright 2020-2021 NRP Team
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//   http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//
// This project has received funding from the European Union’s Horizon 2020
// Framework Programme for Research and Innovation under the Specific Grant
// Agreement No. 945539 (Human Brain Project SGA3).
//

#include "nrp_gazebo_json_engine/nrp_client/gazebo_engine_json_nrp_client.h"

#include <signal.h>
#include <sys/prctl.h>
#include <sys/types.h>
#include <sys/wait.h>

#include "nrp_gazebo_json_engine/config/cmake_constants.h"
#include "nrp_general_library/config/cmake_constants.h"

#include <chrono>

GazeboEngineJSONNRPClient::GazeboEngineJSONNRPClient(nlohmann::json &config, ProcessLauncherInterface::unique_ptr &&launcher)
    : EngineJSONNRPClient(config, std::move(launcher))
{
    setDefaultProperty<std::vector<std::string>>("GazeboPlugins", std::vector<std::string>());
}

void GazeboEngineJSONNRPClient::initialize()
{
    NRP_LOGGER_TRACE("{} called", __FUNCTION__);

    try
    {
        // Wait for Gazebo to load world
        const nlohmann::json initRes = this->sendInitCommand(this->engineConfig());
        if(!initRes[0].get<bool>())
            throw NRPExceptionNonRecoverable("Received initialization fail message from Engine \"" + this->engineName() + "\"");
    }
    catch(std::exception &e)
    {
        throw NRPException::logCreate(e, "Engine \"" + this->engineName() + "\" initialization failed");
    }

    NRPLogger::debug("GazeboEngineJSONNRPClient::initialize(...) completed with no errors.");
}

void GazeboEngineJSONNRPClient::reset()
{
    NRP_LOGGER_TRACE("{} called", __FUNCTION__);

    try
    {
        const nlohmann::json resetRes = this->sendResetCommand(nlohmann::json());
        
        if(!resetRes[0].get<bool>()){
            throw NRPExceptionNonRecoverable("Received reset fail message from Engine \"" + this->engineName() + "\"");
        }

        this->resetEngineTime();
    }
    catch(std::exception &e)
    {
        throw NRPException::logCreate(e, "Engine \"" + this->engineName() + "\" reset failed");
    }
}

void GazeboEngineJSONNRPClient::shutdown()
{
    NRP_LOGGER_TRACE("{} called", __FUNCTION__);

    try
    {
        this->sendShutdownCommand(nlohmann::json());
    }
    catch(std::exception &e)
    {
        throw NRPException::logCreate(e, "Engine \"" + this->engineName() + "\" shutdown failed");
    }
}

const std::vector<std::string> GazeboEngineJSONNRPClient::engineProcEnvParams() const
{
    NRP_LOGGER_TRACE("{} called", __FUNCTION__);

    std::vector<std::string> envVars = this->EngineJSONNRPClient::engineProcEnvParams();

    // Add NRP and Gazebo plugins dir
    envVars.push_back("GAZEBO_PLUGIN_PATH=" NRP_GAZEBO_PLUGINS_DIR ":" DEFAULT_GAZEBO_PLUGIN_DIR ":$GAZEBO_PLUGIN_PATH");

    // Add NRP and Gazebo library paths
    envVars.push_back("LD_LIBRARY_PATH=" NRP_LIB_INSTALL_DIR ":" DEFAULT_GAZEBO_LIB_DIRS ":" NRP_GAZEBO_PLUGINS_DIR ":" DEFAULT_GAZEBO_PLUGIN_DIR ":$LD_LIBRARY_PATH");

    // Add Gazebo models path
    envVars.push_back("GAZEBO_MODEL_PATH=" DEFAULT_GAZEBO_MODEL_DIR ":${GAZEBO_MODEL_PATH}");

    return envVars;
}

const std::vector<std::string> GazeboEngineJSONNRPClient::engineProcStartParams() const
{
    NRP_LOGGER_TRACE("{} called", __FUNCTION__);

    std::vector<std::string> startParams = this->EngineJSONNRPClient::engineProcStartParams();

    // Add gazebo plugins
    for(const auto &curPlugin : this->engineConfig().at("GazeboPlugins"))
    {
        NRPLogger::debug("Adding Gazebo plugin: {}.", curPlugin.dump());
        startParams.push_back(GazeboJSONConfigConst::GazeboPluginArg.data());
        startParams.push_back(curPlugin);
    }

    // Add gazebo communication system plugin
    NRPLogger::debug("Adding Gazebo communication system plugin.");
    startParams.push_back(GazeboJSONConfigConst::GazeboPluginArg.data());
    startParams.push_back(NRP_GAZEBO_COMMUNICATION_PLUGIN);

    // Add RNG Seed
    int seed = this->engineConfig().at("GazeboRNGSeed");
    NRPLogger::debug("Adding Gazebo RNG seed: {0:d}.", seed);
    startParams.push_back(GazeboJSONConfigConst::GazeboRNGSeedArg.data());
    startParams.push_back(std::to_string(seed));

    // Add world file
    NRPLogger::debug("Adding world file {}.", this->engineConfig().at("GazeboWorldFile"));
    startParams.push_back(this->engineConfig().at("GazeboWorldFile"));

    return startParams;
}
