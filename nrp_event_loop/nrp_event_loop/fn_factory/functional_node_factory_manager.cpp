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

#include "nrp_event_loop/fn_factory/functional_node_factory_manager.h"

#include "nrp_general_library/utils/nrp_exceptions.h"

#include <dlfcn.h>
#include <iostream>

using load_factory_fcn_t = void();
using create_fn_fcn_t = FunctionalNodeBase* (const std::string&, const std::string&, FunctionalNodePolicies::ExecutionPolicy);

std::unique_ptr<FunctionalNodeFactoryManager> FunctionalNodeFactoryManager::_instance = nullptr;

void FunctionalNodeFactoryManager::loadFNFactoryPlugin(const std::string &pluginLibFile)
{
    // Load plugin, only if not loaded already
    if(this->_loadedLibs.count(pluginLibFile))
        return;

    // If it can't be loaded throw exception
    if(!this->loadPlugin(pluginLibFile))
        throw NRPException::logCreate("Plugin Library \"" + pluginLibFile + "\" could not be loaded");

    auto pLibHandle = this->_loadedLibs.at(pluginLibFile);

    // Find loadFNFactory function in library and load it
    load_factory_fcn_t *pLoadFNFactory = reinterpret_cast<load_factory_fcn_t*>(dlsym(pLibHandle, "loadFNFactory"));
    if(pLoadFNFactory == nullptr)
        throw NRPException::logCreate("Plugin Library \"" + pluginLibFile + "\" does not contain a loadFNFactory function");
    else
        pLoadFNFactory();
}

FunctionalNodeBase* FunctionalNodeFactoryManager::createFunctionalNode(const std::string& functionName, const std::string &nodeName,
                                                                       FunctionalNodePolicies::ExecutionPolicy policy)
{
    for (auto & [key, value] : this->_loadedLibs) {
        // Find createFunctionalNode function in library
        create_fn_fcn_t *pCreateFN = reinterpret_cast<create_fn_fcn_t*>(dlsym(value, "createFunctionalNode"));
        if(pCreateFN == nullptr)
            NRPLogger::error("Loaded Plugin Library \"" + key + "\" does not contain a createFunctionalNode function");
        else {
            auto fn = pCreateFN(functionName, nodeName, policy);
            if(fn)
                return fn;
        }
    }

    throw NRPException::logCreate("Failed to instantiate Functional Node \"" + nodeName + "\" from function \"" +
                                        functionName + "\" after trying with all loaded plugins");
}

FunctionalNodeFactoryManager &FunctionalNodeFactoryManager::getInstance()
{
    if(!FunctionalNodeFactoryManager::_instance)
        FunctionalNodeFactoryManager::resetInstance();

    return *(FunctionalNodeFactoryManager::_instance);
}

void FunctionalNodeFactoryManager::resetInstance()
{
    FunctionalNodeFactoryManager::_instance.reset(new FunctionalNodeFactoryManager());
}
