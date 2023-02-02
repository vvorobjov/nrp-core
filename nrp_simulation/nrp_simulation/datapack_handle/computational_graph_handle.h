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

#ifndef COMPUTATION_GRAPH_HANDLE_H
#define COMPUTATION_GRAPH_HANDLE_H

#ifdef ROS_ON
#include "ros/ros.h"
#endif

#include "nrp_event_loop/computational_graph/computational_graph_manager.h"
#include "nrp_event_loop/nodes/engine/input_node.h"
#include "nrp_event_loop/nodes/engine/output_node.h"

#include "nrp_simulation/datapack_handle/datapack_handle.h"

#include "nrp_event_loop/utils/graph_utils.h"

#include "nrp_general_library/utils/python_interpreter_state.h"

/*!
 * \brief Uses a Computation Graph to execute datapack transformation operations
 */
struct ComputationalGraphHandle : public DataPackProcessor {

    ComputationalGraphHandle(SimulationDataManager * simulationDataManager, bool slaveMode = false, bool spinROS = false) :
        DataPackProcessor(simulationDataManager),
        _slaveMode(slaveMode),
        _spinROS(spinROS)
    {}

    bool _slaveMode;
    bool _spinROS;
    PyGILState_STATE _pyGILState;

    /*!
     * \brief Map containing all InputEngineNodes associated with this simulation
     */
    std::map<std::string, InputEngineNode*> _inputs;

    /*!
     * \brief Map containing all OutputEngineNodes associated with this simulation
     */
    std::map<std::string, OutputEngineNode*> _outputs;

    void init(const jsonSharedPtr &simConfig, const engine_interfaces_t &engines) override
    {
        // Load Computation Graph
        if(!_slaveMode) {
            boost::python::dict globalDict;
            // When controlling the graph set output driven mode to optimize on graph node execution
            createPythonGraphFromConfig(simConfig->at("ComputationalGraph"),
                                        ComputationalGraph::ExecMode::OUTPUT_DRIVEN, globalDict);
        }

        ComputationalGraphManager& gm = ComputationalGraphManager::getInstance();

        // Find Engine nodes in graph
        for(const auto &engine : engines) {
            std::string input_id = engine->engineName()+"_input";
            std::string output_id = engine->engineName()+"_output";

            if(gm.getNode(input_id) && dynamic_cast<InputEngineNode *>(gm.getNode(input_id)))
                _inputs[engine->engineName()] = dynamic_cast<InputEngineNode *>(gm.getNode(input_id));

            if(gm.getNode(output_id) && dynamic_cast<OutputEngineNode *>(gm.getNode(output_id))) {
                auto o_node = dynamic_cast<OutputEngineNode *>(gm.getNode(output_id));
                // setting computePeriod to 0 so node is only executed when linked Engine is synced
                if(!_slaveMode)
                    o_node->setComputePeriod(0);
                _outputs[engine->engineName()] = o_node;
            }
        }
    }

    void updateDataPacksFromEngines(const std::vector<EngineClientInterfaceSharedPtr> &engines) override
    {
        if(_slaveMode)
            _pyGILState = PyGILState_Ensure();

        try
        {
            for(auto &engine : engines)
                if(_inputs.count(engine->engineName())) {
                    _inputs[engine->engineName()]->setDataPacks(
                            engine->getDataPacksFromEngine(_inputs[engine->engineName()]->requestedDataPacks()));
                }
        }
        catch(std::exception &)
        {
            // TODO: Handle failure on datapack retrieval
            throw;
        }

        if(_slaveMode)
            PyGILState_Release(_pyGILState);
    }

    void compute(const std::vector<EngineClientInterfaceSharedPtr> & engines) override
    {
        if(!_slaveMode) {
#ifdef ROS_ON
            if(_spinROS)
                ros::spinOnce();
#endif
            for(auto &engine : engines)
                if(_outputs.count(engine->engineName()))
                    _outputs[engine->engineName()]->setDoCompute(true);

            ComputationalGraphManager::getInstance().compute();
        }
    }

    void sendDataPacksToEngines(const std::vector<EngineClientInterfaceSharedPtr> &engines) override
    {
        if(_slaveMode)
            _pyGILState = PyGILState_Ensure();

        for(const auto &engine : engines)
            if(_outputs.count(engine->engineName())) {
                try {
                    auto devs = _outputs[engine->engineName()]->getDataPacks();
                    engine->sendDataPacksToEngine(devs);
                }
                catch (std::exception &e) {
                    throw NRPException::logCreate(e,
                                                  "Failed to send datapacks to engine \"" + engine->engineName() + "\"");
                }
            }

        if(_slaveMode)
            PyGILState_Release(_pyGILState);
    }
};

#endif // COMPUTATION_GRAPH_HANDLE_H
