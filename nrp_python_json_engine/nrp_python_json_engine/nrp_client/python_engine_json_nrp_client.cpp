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

#include "nrp_python_json_engine/nrp_client/python_engine_json_nrp_client.h"

#include "nrp_python_json_engine/config/cmake_constants.h"
#include "nrp_python_json_engine/config/python_config.h"
#include "nrp_json_engine_protocol/config/engine_json_config.h"

#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/prctl.h>

#include <chrono>

PythonEngineJSONNRPClient::PythonEngineJSONNRPClient(nlohmann::json &config, ProcessLauncherInterface::unique_ptr &&launcher)
    : EngineJSONNRPClient(config, std::move(launcher))
{
    setDefaultProperty<std::string>("EngineProcCmd", NRP_PYTHON_EXECUTABLE_PATH);
}

PythonEngineJSONNRPClient::~PythonEngineJSONNRPClient()
{}

void PythonEngineJSONNRPClient::initialize()
{
	nlohmann::json resp = this->sendInitCommand(this->engineConfig());
	if(!resp.at(PythonConfigConst::InitFileExecStatus.data()).get<bool>())
	{
		// Write the error message
		this->_initErrMsg = resp.at(PythonConfigConst::InitFileErrorMsg.data());
		NRPLogger::SPDErrLogDefault(this->_initErrMsg);

		throw NRPException::logCreate("Initialization failed: " + this->_initErrMsg);
	}
}

void PythonEngineJSONNRPClient::shutdown()
{
	this->sendShutdownCommand(nlohmann::json());
}

const std::vector<std::string> PythonEngineJSONNRPClient::engineProcStartParams() const
{
    std::vector<std::string> startParams = this->EngineJSONNRPClient::engineProcStartParams();

    // Add JSON Server address (will be used by plugin)
    std::string server_address = this->engineConfig().at("ServerAddress");
    startParams.push_back(std::string("--") + EngineJSONConfigConst::EngineServerAddrArg.data() + "=" + server_address);

    return startParams;
}
