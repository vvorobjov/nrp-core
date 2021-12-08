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

#include "nrp_general_library/engine_interfaces/engine_client_interface.h"

EngineClientInterface::EngineClientInterface(std::unique_ptr<ProcessLauncherInterface> &&launcher)
    : _process(std::move(launcher))
{}

EngineClientInterface::~EngineClientInterface() = default;

pid_t EngineClientInterface::launchEngine()
{
    // Launch engine
    return this->_process->launchEngineProcess(this->engineConfig(), this->engineProcEnvParams(), this->engineProcStartParams());
}

const EngineClientInterface::datapacks_t &EngineClientInterface::updateDataPacksFromEngine(const EngineClientInterface::datapack_identifiers_set_t &datapackIdentifiers)
{
    // Merge cached datapacks into new list
    this->updateCachedDataPacks(this->getDataPacksFromEngine(datapackIdentifiers));

    return this->_datapackCache;
}

inline const int &setCmp(int &ref, int val)
{   return ref=val; }

void EngineClientInterface::updateCachedDataPacks(EngineClientInterface::datapacks_set_t &&devs)
{
    size_t i = 0;
    for(const auto &dev : devs)
    {
        int cmp = -1;
        while(i < this->_datapackCache.size() && setCmp(cmp, this->_datapackCache[i]->name().compare(dev->name())) < 0)
            ++i;

        // If there's no datapack with the name in the cache - insert it
        // It there already is one - replace it, but only with a non-empty datapack

        if(cmp != 0)
            this->_datapackCache.insert(this->_datapackCache.begin()+i, dev);
        else if(!dev->isEmpty())
            this->_datapackCache[i] = dev;

        ++i;
    }
}

EngineLauncherInterface::EngineLauncherInterface(const EngineLauncherInterface::engine_type_t &engineType)
    : _engineType(engineType)
{}

const EngineLauncherInterface::engine_type_t &EngineLauncherInterface::engineType() const
{
    return this->_engineType;
}
