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

#ifndef GRAPH_UTILS_H
#define GRAPH_UTILS_H

#include <filesystem>
#include <nlohmann/json.hpp>

#include "nrp_event_loop/computational_graph/computational_graph_manager.h"

#include "nrp_event_loop/nodes/time/input_time.h"

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

inline std::pair<InputClockNode*, InputIterationNode*> findTimeNodes()
{
    ComputationalGraphManager& gm = ComputationalGraphManager::getInstance();
    InputClockNode* _clock = nullptr;
    InputIterationNode* _iteration = nullptr;

    // Find Clock and Iteration nodes
    if(gm.getNode("clock_node")) {
        _clock = dynamic_cast<InputClockNode *>(gm.getNode("clock_node"));
        if (!_clock)
            throw NRPException::logCreate(
                    "Exception in the ComputationalGraph: found Node with id 'clock_node', which is reserved for the input Clock node");
    }
    if(gm.getNode("iteration_node")) {
        _iteration = dynamic_cast<InputIterationNode*>(gm.getNode("iteration_node"));
        if(!_iteration)
            throw NRPException::logCreate("Exception in the ComputationalGraph: found Node with id 'iteration_node', which is reserved for the input Iteration node");
    }

    return std::make_pair(_clock, _iteration);
}

#endif //GRAPH_UTILS_H
