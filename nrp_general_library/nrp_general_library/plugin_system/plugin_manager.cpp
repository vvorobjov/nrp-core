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

#include "nrp_general_library/plugin_system/plugin_manager.h"
#include "nrp_general_library/utils/nrp_exceptions.h"

#include <dlfcn.h>
#include <iostream>

bool PluginManager::loadPlugin(const std::string &pluginLibFile)
{
    NRP_LOGGER_TRACE("{} called [ pluginLibFile: {} ]", __FUNCTION__, pluginLibFile);

    if(this->_loadedLibs.count(pluginLibFile)) {
        NRPLogger::info("Plugin library \"" + pluginLibFile + "\" has been already loaded, skipping request");
        return false;
    }

    dlerror();  // Clear previous error msgs

    // Try loading plugin with given paths
    void *pLibHandle = nullptr;
    for(const auto &path : this->_pluginPaths)
    {
        NRPLogger::debug("Looking for Plugin {} at {}", pluginLibFile, path.string());
        const std::string fileName = path.empty() ? pluginLibFile : (path/pluginLibFile).c_str();
        if(!std::filesystem::exists(fileName))
            continue;

        pLibHandle = dlopen(fileName.c_str(), RTLD_LAZY | RTLD_GLOBAL);
        if(pLibHandle != nullptr)
            NRPLogger::debug("Plugin {} found at {}", pluginLibFile, fileName);

        break;
    }

    // Print error if opening failed
    if(pLibHandle == nullptr)
    {
        const auto dlerr = dlerror();

        NRPLogger::error("Unable to load plugin library \"" + pluginLibFile + "\"" + (dlerr ? std::string(": ")+dlerr : ""));

        return false;
    }

    // Save stored library
    this->_loadedLibs.emplace(pluginLibFile, pLibHandle);

    return true;
}

PluginManager::~PluginManager()
{
    // Unload all plugins
    while(!this->_loadedLibs.empty())
    {
        auto curLibIt = --this->_loadedLibs.end();
        if(dlclose(curLibIt->second) != 0)
        {
            const auto errStr = dlerror();
            NRPLogger::error("Couldn't unload plugin \"" + curLibIt->first + "\": " + errStr);
        }

        this->_loadedLibs.erase(curLibIt);
    }
}

void PluginManager::addPluginPath(const std::string &pluginPath)
{
    this->_pluginPaths.insert(--this->_pluginPaths.end(), pluginPath);
}
