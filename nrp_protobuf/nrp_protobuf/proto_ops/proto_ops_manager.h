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

#ifndef PLUGIN_MANAGER_H
#define PLUGIN_MANAGER_H

#include "nrp_protobuf/proto_ops/protobuf_ops.h"
#include <filesystem>

#include <map>

/*!
 * \brief Singleton class which loads Protobuf conversion libraries.
 *
 * Also in charge of unloading then when the object is destructed
 *
 */
class ProtoOpsManager
{
public:

    // Delete move and copy operators. This ensures this class is a singleton
    ProtoOpsManager(const ProtoOpsManager &) = delete;
    ProtoOpsManager(ProtoOpsManager &&) = delete;

    ProtoOpsManager &operator=(const ProtoOpsManager &) = delete;
    ProtoOpsManager &operator=(ProtoOpsManager &&) = delete;

    /*!
     * \brief Get singleton instance of ProtoOpsManager
     */
    static ProtoOpsManager &getInstance();

    /*!
     * \brief Reset singleton instance
     */
    static void resetInstance();
    
    /*!
     * \brief Load a Protobuf conversion plugin from a given library
     * \param pluginLibFile Plugin library file (.so)
     * \return Returns ptr to loaded protobuf_ops::NRPProtobufOpsIface if found, nullptr otherwise
     */
    std::unique_ptr<protobuf_ops::NRPProtobufOpsIface> loadPlugin(const std::string &pluginLibFile);

    /*!
     *  \brief Destructor. Unloads all plugin libraries
     */
    ~ProtoOpsManager();

    /*!
     * \brief Adds search path under which to look for plugins
     * \param pluginPath Path to plugins
     */
    void addPluginPath(const std::string &pluginPath);


private:
    /*!
     * \brief Loaded Libraries
     */
    std::map<std::string, void*> _loadedLibs;

    /*!
     * \brief Plugin paths. The last element should always be an empty path,
     * which signifies using the standard linux search method
     */
    std::vector<std::filesystem::path> _pluginPaths = {std::filesystem::path()};

    ProtoOpsManager() = default;
    static std::unique_ptr<ProtoOpsManager> _instance;
    
};

#endif // PLUGIN_MANAGER_H
