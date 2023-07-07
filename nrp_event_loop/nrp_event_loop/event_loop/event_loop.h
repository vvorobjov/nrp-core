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

#ifndef EVENT_LOOP_H
#define EVENT_LOOP_H

#include "nrp_event_loop/event_loop/event_loop_interface.h"

#include "nrp_event_loop/computational_graph/computational_graph_manager.h"
#include "nrp_event_loop/computational_graph/input_node.h"
#include "nrp_event_loop/computational_graph/output_node.h"

#include "nrp_general_library/utils/time_utils.h"
#include "nrp_event_loop/nodes/time/input_time.h"

#include "nrp_general_library/utils/python_interpreter_state.h"

#include <nlohmann/json.hpp>

/*!
 * \brief EventLoop implementation which runs a Computational Graph
 */
class EventLoop : public EventLoopInterface
{
    public:

        /*!
         * \brief Constructor
         */
        EventLoop(const nlohmann::json &graph_config, std::chrono::milliseconds timestep, std::chrono::milliseconds rtDeltaThres,
                  ComputationalGraph::ExecMode execMode = ComputationalGraph::ExecMode::ALL_NODES,
                  bool ownGIL = true, bool spinROS = false, bool logRTInfo = false, bool syncTimeRef = false);

        ~EventLoop();

    protected:

        void initializeCB() override;

        void runLoopCB() override;

        void shutdownCB() override;

        // no corrective actions are implemented yet
        void realtimeDeltaCB(std::chrono::milliseconds /*deviation*/) override
        { }

    private:
    
        /*! \brief Configuration of the Computational Graph run by this EventLoop  */
        nlohmann::json _graph_config;
        /*! \brief Execution mode the event loop will use */
        ComputationalGraph::ExecMode _execMode;
        /*! \brief true if the EventLoop is assumed to always owns the GIL, false if it is shared with other threads  */
        bool _ownGIL;
        /*! \brief if true ros::spin is called in every loop  */
        bool _spinROS;
        /*! \brief GIL state object used to request the GIL ownership when needed  */
        PyGILState_STATE _pyGILState;
        /*! \brief Pointer to the clock_node of the graph */
        InputClockNode* _clock = nullptr;
        /*! \brief Pointer to the iteration_node of the graph */
        InputIterationNode* _iteration = nullptr;

};


#endif // EVENT_LOOP_H
