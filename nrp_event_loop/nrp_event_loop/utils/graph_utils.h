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

#include <filesystem>
#include <nlohmann/json.hpp>

#include "nrp_event_loop/computational_graph/computational_graph_manager.h"

inline void createPythonGraphFromConfig(const nlohmann::json &config, const ComputationalGraph::ExecMode& execMode,
                                        const boost::python::dict &globalDict)
{
    // Load Computation Graph
    ComputationalGraphManager::resetInstance();
    ComputationalGraphManager& gm = ComputationalGraphManager::getInstance();
    gm.setExecMode(execMode);

    for(const auto &fn : config) {
        auto fileName = fn.get<std::string>();
        if(std::filesystem::exists(fileName)) {
            try {
                boost::python::exec_file(fileName.c_str(), globalDict, globalDict);
                NRPLogger::info("Loaded computation graph file \"" + fileName + "\"");
            }
            catch (boost::python::error_already_set &) {
                throw NRPException::logCreate(
                        "Loading of computation graph file \"" + fileName + "\" failed: " + handle_pyerror());
            }
        }
        else
            throw NRPException::logCreate("Loading of computation graph file \"" + fileName + "\" failed: the file doesn't exist.");
    }

    gm.configure();
}

/*!
 * \brief Parses a computational node address returning the node id and the port (if any) contained in the address
 *
 * \param address string containing a computational graph connection address with the format /node_id/port_id
 * \param hasPort if the address contains a port id or just a node id
 * \return a pair with the node and port ids parsed from 'address'
 */
inline std::pair<std::string, std::string> parseCGAddress(const std::string& address, bool hasPort = true)
{
    if(address.at(0) != '/')
        throw NRPException::logCreate("Error while parsing address \""+ address +"\". Computational Graph addresses must start with '/'");

    auto n = address.find('/',1);
    if(n == std::string::npos && hasPort)
        throw NRPException::logCreate("Error while parsing address \""+ address +"\". Expected format is '/node_id/port_id'");

    auto node = hasPort ? address.substr(1, n-1) : address.substr(1);
    auto port = hasPort ? address.substr(n+1, address.size()) : node;

    return std::make_pair(node, port);
}

#endif //GRAPH_UTILS_H
