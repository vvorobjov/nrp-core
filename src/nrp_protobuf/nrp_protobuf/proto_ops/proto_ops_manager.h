/* * NRP Core - Backend infrastructure to synchronize simulations
 *
 * Copyright 2020-2023 NRP Team
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

#ifndef PROTO_PLUGIN_MANAGER_H
#define PROTO_PLUGIN_MANAGER_H

#include "nrp_general_library/plugin_system/plugin_manager.h"
#include "nrp_protobuf/proto_ops/protobuf_ops.h"

/*!
 * \brief Singleton class which loads Protobuf conversion libraries.
 *
 * Also in charge of unloading then when the object is destructed.
 *
 * \warning Lifetime contract. `getInstance()` returns a reference into
 *          a `std::unique_ptr`-owned storage slot. `resetInstance()`
 *          destroys whatever is currently in that slot and replaces it
 *          with a fresh instance. Any caller-held reference or pointer
 *          obtained from a prior `getInstance()` call therefore DANGLES
 *          after the next `resetInstance()`. Callers must refresh by
 *          calling `getInstance()` again, or simply avoid caching the
 *          reference across a reset. See EBR2-25.
 */
class ProtoOpsManager : public PluginManager
{
public:

    // Delete move and copy operators. This ensures this class is a singleton
    ProtoOpsManager(const ProtoOpsManager &) = delete;
    ProtoOpsManager(ProtoOpsManager &&) = delete;

    ProtoOpsManager &operator=(const ProtoOpsManager &) = delete;
    ProtoOpsManager &operator=(ProtoOpsManager &&) = delete;

    /*!
     * \brief Get singleton instance of ProtoOpsManager
     *
     * \warning The returned reference is valid until the next
     *          `resetInstance()` call. Do not cache across that
     *          boundary. See EBR2-25.
     */
    static ProtoOpsManager &getInstance();

    /*!
     * \brief Reset singleton instance
     *
     * \warning Invalidates every reference and pointer previously
     *          obtained from `getInstance()`. After calling this, all
     *          cached handles must be refreshed. See EBR2-25.
     */
    static void resetInstance();
    
    /*!
     * \brief Load a Protobuf conversion plugin from a given library
     * \param pluginLibFile Plugin library file (.so)
     * \return Returns ptr to loaded protobuf_ops::NRPProtobufOpsIface if found, nullptr otherwise
     */
    std::unique_ptr<protobuf_ops::NRPProtobufOpsIface> loadProtobufPlugin(const std::string &pluginLibFile);


private:

    ProtoOpsManager() = default;
    static std::unique_ptr<ProtoOpsManager> _instance;
    
};

#endif // PROTO_PLUGIN_MANAGER_H
