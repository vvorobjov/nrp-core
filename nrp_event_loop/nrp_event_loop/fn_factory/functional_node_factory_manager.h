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

#ifndef FUNCTIONAL_NODE_FACTORY_MANAGER_H
#define FUNCTIONAL_NODE_FACTORY_MANAGER_H

#include "nrp_general_library/plugin_system/plugin_manager.h"

#include "nrp_event_loop/computational_graph/functional_node.h"

/*!
 * \brief Singleton class which loads Functional Node factory plugins and instantiates Functional Nodes using the former.
 *
 * Functional Node factory plugins are generated using the skeleton "fn_factory_module.cpp.template"
 *
 */
class FunctionalNodeFactoryManager : public PluginManager
{
public:

    // Delete move and copy operators. This ensures this class is a singleton
    FunctionalNodeFactoryManager(const FunctionalNodeFactoryManager &) = delete;
    FunctionalNodeFactoryManager(FunctionalNodeFactoryManager &&) = delete;

    FunctionalNodeFactoryManager &operator=(const FunctionalNodeFactoryManager &) = delete;
    FunctionalNodeFactoryManager &operator=(FunctionalNodeFactoryManager &&) = delete;

    /*!
     * \brief Get singleton instance of FunctionalNodeFactoryManager
     */
    static FunctionalNodeFactoryManager &getInstance();

    /*!
     * \brief Reset singleton instance
     */
    static void resetInstance();
    
    /*!
     * \brief Load a Functional Node factory plugin
     * \param pluginLibFile Plugin library file (.so)
     */
    void loadFNFactoryPlugin(const std::string &pluginLibFile);

    /*!
     * \brief Attempts to instantiate a Functional Node from the requested functionName using all loaded plugins
     * \param functionName Name of the function to instantiate the FN from
     * \param nodeName Name of the FN
     * \param policy Execution policy of the FN
     * \return Returns a FunctionalNodeBase pointer
     */
    FunctionalNodeBase* createFunctionalNode(const std::string& functionName, const std::string &nodeName,
                                             FunctionalNodePolicies::ExecutionPolicy policy = FunctionalNodePolicies::ExecutionPolicy::ON_NEW_INPUT);


private:

    FunctionalNodeFactoryManager() = default;

    static std::unique_ptr<FunctionalNodeFactoryManager> _instance;
    
};

#endif // FUNCTIONAL_NODE_FACTORY_MANAGER_H
