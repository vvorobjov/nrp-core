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

#include "nrp_protobuf/proto_ops/proto_ops_manager.h"

#include "nrp_general_library/utils/nrp_exceptions.h"

#include <dlfcn.h>
#include <iostream>


using protobuf_ops_fcn_t = protobuf_ops::NRPProtobufOpsIface*();

std::unique_ptr<ProtoOpsManager> ProtoOpsManager::_instance = nullptr;

std::unique_ptr<protobuf_ops::NRPProtobufOpsIface> ProtoOpsManager::loadProtobufPlugin(const std::string &pluginLibFile)
{
    // Load plugin, only if not loaded already
    if(!this->_loadedLibs.count(pluginLibFile) && !this->loadPlugin(pluginLibFile))
        return nullptr;

    auto pLibHandle = this->_loadedLibs.at(pluginLibFile);

    // Find protobuf_ops::NRPProtobufOpsIface function in library
    protobuf_ops_fcn_t *pLaunchFcn = reinterpret_cast<protobuf_ops_fcn_t*>(dlsym(pLibHandle, CREATE_PROTOBUF_OPS_FCN_STR));
    if(pLaunchFcn == nullptr)
    {
        NRPLogger::error("Plugin Library \"" + pluginLibFile + "\" does not contain an protobuf operations creation function");

        return nullptr;
    }

    return std::unique_ptr<protobuf_ops::NRPProtobufOpsIface>((*pLaunchFcn)());
}

ProtoOpsManager &ProtoOpsManager::getInstance()
{
    if(!ProtoOpsManager::_instance)
        ProtoOpsManager::resetInstance();

    return *(ProtoOpsManager::_instance);
}

void ProtoOpsManager::resetInstance()
{
    ProtoOpsManager::_instance.reset(new ProtoOpsManager());
}
