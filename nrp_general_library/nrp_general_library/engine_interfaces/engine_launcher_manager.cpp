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

#include "nrp_general_library/engine_interfaces/engine_launcher_manager.h"

#include "nrp_general_library/utils/nrp_logger.h"

void EngineLauncherManager::registerLauncher(const EngineLauncherInterfaceSharedPtr &launcher)
{
    const auto &sameLauncherPtr = this->findLauncher(launcher->engineType());
    if(sameLauncherPtr != nullptr)
    {
        // TODO: Handle error where two launchers have same name
        NRPLogger::error("Launcher with same name \"" + sameLauncherPtr->engineType() + "\"already registered");
    }

    this->_launchers.emplace(launcher);
}

EngineLauncherInterfaceSharedPtr EngineLauncherManager::findLauncher(const EngineLauncherInterface::engine_type_t &type) const
{
    NRP_LOGGER_TRACE("{} called", __FUNCTION__);
    
    for(const auto &launcher : this->_launchers)
    {
        if(launcher->engineType() == type)
        {
            NRPLogger::debug("Found launcher for engine " + type);
            return launcher;
        }
    }

    return EngineLauncherInterfaceSharedPtr(nullptr);
}
