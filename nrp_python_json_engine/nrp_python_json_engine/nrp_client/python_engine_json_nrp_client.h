
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

#ifndef PYTHON_ENGINE_JSON_NRP_CLIENT_H
#define PYTHON_ENGINE_JSON_NRP_CLIENT_H

#include "nrp_python_json_engine/nrp_client/python_engine_json_nrp_client_base.h"
#include "nrp_general_library/plugin_system/plugin.h"


/*!
 * \brief PythonJSONEngine client
 */
class PythonEngineJSONNRPClient: public PythonEngineJSONNRPClientBase<PythonEngineJSONNRPClient, PythonConfigConst::EngineSchema>
{
public:

    PythonEngineJSONNRPClient(nlohmann::json &config, ProcessLauncherInterface::unique_ptr &&launcher)
            : PythonEngineJSONNRPClientBase(config, std::move(launcher))
    {
        NRP_LOGGER_TRACE("{} called", __FUNCTION__);

        setDefaultProperty<std::string>("EngineProcCmd", NRP_PYTHON_EXECUTABLE_PATH);
    }
};

using PythonEngineJSONLauncher = PythonEngineJSONNRPClient::EngineLauncher<PythonConfigConst::EngineType>;


CREATE_NRP_ENGINE_LAUNCHER(PythonEngineJSONLauncher);


#endif // PYTHON_ENGINE_JSON_NRP_CLIENT_H
