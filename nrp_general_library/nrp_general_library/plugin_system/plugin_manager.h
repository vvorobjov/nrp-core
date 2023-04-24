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

#include <filesystem>
#include <map>
#include <vector>

class PluginManager
{
public:
    /*!
     * \brief Load a Plugin from a given library
     * \param pluginLibFile Plugin library file (.so)
     * \return Returns true if the plugin was loaded successfully, false otherwise
     */
    bool loadPlugin(const std::string &pluginLibFile);

    /*!
     *  \brief Destructor. Unloads all plugin libraries
     */
    ~PluginManager();

    /*!
     * \brief Adds search path under which to look for plugins
     * \param pluginPath Path to plugins
     */
    void addPluginPath(const std::string &pluginPath);

protected:

    /*!
     * \brief Loaded Libraries
     */
    std::map<std::string, void*> _loadedLibs;

private:

    /*!
     * \brief Plugin paths. The last element should always be an empty path,
     * which signifies using the standard linux search method
     *
     * The current working directory is always the 1st element in the vector
     */
    std::vector<std::filesystem::path> _pluginPaths = {std::filesystem::current_path(), std::filesystem::path()};
};

#endif // PLUGIN_MANAGER_H
