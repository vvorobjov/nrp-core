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

#include <dlfcn.h>

#include "nrp_general_library/plugin_system/engine_plugin_manager.h"

#include "nrp_general_library/plugin_system/plugin.h"
#include "nrp_general_library/utils/nrp_exceptions.h"

using engine_launch_fcn_t = NRP_ENGINE_LAUNCH_FCN_T;

EngineLauncherInterface::unique_ptr EnginePluginManager::loadEnginePlugin(const std::string &pluginLibFile)
{
    // Load plugin
    if(!this->loadPlugin(pluginLibFile))
        return nullptr;

    auto pLibHandle = this->_loadedLibs.at(pluginLibFile);

    // Find EngineLauncherInterface function in library
    engine_launch_fcn_t *pLaunchFcn = reinterpret_cast<engine_launch_fcn_t*>(dlsym(pLibHandle, CREATE_NRP_ENGINE_LAUNCHER_FCN_STR));
    if(pLaunchFcn == nullptr)
    {
        NRPLogger::error("Plugin Library \"" + pluginLibFile + "\" does not contain an engine load creation function");
        NRPLogger::error("Register a plugin using CREATE_NRP_ENGINE_LAUNCHER(engine_launcher_name)");

        return nullptr;
    }

    return EngineLauncherInterface::unique_ptr((*pLaunchFcn)());
}
