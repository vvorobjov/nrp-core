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

#ifndef GRAPH_UTILS_H
#define GRAPH_UTILS_H

#include <nlohmann/json.hpp>

#include "nrp_event_loop/computational_graph/computational_graph_manager.h"

inline void createPythonGraphFromConfig(const nlohmann::json &config, const boost::python::dict &globalDict)
{
    // Load Computation Graph
    ComputationalGraphManager::resetInstance();
    ComputationalGraphManager& gm = ComputationalGraphManager::getInstance();

    for(const auto &fn : config) {
        auto fileName = fn.get<std::string>();
        try
        {
            boost::python::exec_file(fileName.c_str(), globalDict, globalDict);
        }
        catch(boost::python::error_already_set &) {
            const auto err = NRPException::logCreate(
                    "Loading of computation graph file \"" + fileName + "\" failed: " + handle_pyerror());
        }

    }

    gm.configure();
}

inline std::pair<std::string, std::string> extractNodePortFromAddress(const std::string& address)
{
    auto n = address.find('/',1);
    auto name = address.substr(1, n-1);
    auto property = address.substr(n+1, address.size());

    return std::make_pair(name, property);
}

#endif //GRAPH_UTILS_H
