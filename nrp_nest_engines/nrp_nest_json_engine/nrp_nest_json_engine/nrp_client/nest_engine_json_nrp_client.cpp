//
// NRP Core - Backend infrastructure to synchronize simulations
//
// Copyright 2020-2023 NRP Team
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

#include "nrp_nest_json_engine/nrp_client/nest_engine_json_nrp_client.h"
#include "nrp_json_engine_protocol/config/engine_json_config.h"

#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/prctl.h>

#include <chrono>

NestEngineJSONNRPClient::NestEngineJSONNRPClient(nlohmann::json &config, ProcessLauncherInterface::unique_ptr &&launcher)
    : EngineJSONNRPClient(config, std::move(launcher))
{
    NRP_LOGGER_TRACE("{} called", __FUNCTION__);
    
    setDefaultProperty<std::string>("EngineProcCmd", NRP_NEST_EXECUTABLE_PATH);
}

NestEngineJSONNRPClient::~NestEngineJSONNRPClient()
{}

void NestEngineJSONNRPClient::initialize()
{
    NRP_LOGGER_TRACE("{} called", __FUNCTION__);

    nlohmann::json resp = this->sendInitCommand(this->engineConfig());
    if(!resp.at(NestConfigConst::InitFileExecStatus.data()).get<bool>())
    {
        // Write the error message
        this->_initErrMsg = resp.at(NestConfigConst::ErrorMsg.data());
        NRPLogger::error(this->_initErrMsg);

        throw NRPException::logCreate("Engine \"" + this->engineName() + "\" initialization failed: " + this->_initErrMsg);
    }

    NRPLogger::debug("NestEngineJSONNRPClient::initialize(...) completed with no errors.");
}

void NestEngineJSONNRPClient::reset()
{
    NRP_LOGGER_TRACE("{} called", __FUNCTION__);

    nlohmann::json resp = this->sendResetCommand(nlohmann::json("reset"));

    if(!resp.at(NestConfigConst::ResetExecStatus.data()).get<bool>())
    {
        // Write the error message
        std::string msg = resp.at(NestConfigConst::ErrorMsg.data());
        NRPLogger::error("NestEngineJSONNRPClient::reset() failed");

        throw NRPException::logCreate("Engine \"" + this->engineName() + "\" reset failed: " + msg);
    }

    this->resetEngineTime();
    
    NRPLogger::debug("NestEngineJSONNRPClient::reset() engine time: {}", this->getEngineTime().count());
}

void NestEngineJSONNRPClient::shutdown()
{
    NRP_LOGGER_TRACE("{} called", __FUNCTION__);
    
    this->sendShutdownCommand(nlohmann::json());
}
