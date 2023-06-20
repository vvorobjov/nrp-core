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

#include "${engine_name_lowercase}_json_engine/engine_client/${engine_name_lowercase}_json_client.h"
#include "${engine_name_lowercase}_json_engine/config/cmake_constants.h"

${engine_name}EngineJSONClient::${engine_name}EngineJSONClient(nlohmann::json &config, ProcessLauncherInterface::unique_ptr &&launcher)
    : EngineJSONNRPClient(config, std::move(launcher))
{
    setDefaultProperty<std::string>("EngineProcCmd", ${engine_name_uppercase}_JSON_ENGINE_EXECUTABLE_PATH);
}

void ${engine_name}EngineJSONClient::initialize()
{
    nlohmann::json resp = this->sendInitCommand(this->engineConfig());
}

void ${engine_name}EngineJSONClient::shutdown()
{
    nlohmann::json resp = this->sendShutdownCommand(nlohmann::json());
}

void ${engine_name}EngineJSONClient::reset()
{
    nlohmann::json resp = sendResetCommand(nlohmann::json());
}

// EOF
