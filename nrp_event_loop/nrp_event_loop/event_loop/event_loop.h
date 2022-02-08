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

#ifndef EVENT_LOOP_H
#define EVENT_LOOP_H

#include <chrono>
#include <thread>
#include <future>

#include "nrp_event_loop/computational_graph/computational_graph_manager.h"
#include "nrp_event_loop/computational_graph/input_node.h"
#include "nrp_event_loop/computational_graph/output_node.h"

#include "nrp_general_library/utils/python_interpreter_state.h"

#include <nlohmann/json.hpp>

/*!
 * \brief Manages simulation loop. Runs physics and brain interface, and synchronizes them via Transfer Functions
 */
class EventLoop
{

    // TODO: implement runasync and runonce

    public:

        virtual ~EventLoop();

        EventLoop() = delete;

        /*!
         * \brief Constructor
         */
        EventLoop(const nlohmann::json &graph_config, std::chrono::milliseconds timestep, bool ownGIL = true, bool spinROS = false);

        /*!
         * \brief Run a single loop
         */
        void runLoopOnce();

        /*!
         * \brief Run loop in a thread
         */
        void runLoopAsync(std::chrono::milliseconds timeout = std::chrono::milliseconds(0));

        /*!
         * \brief Stop loop
         */
        void stopLoop();

        /*!
         * \brief Shutdown loop
         */
        void shutdown();

        /*!
         * \brief Returns true if the event loop is currently running, false otherwise
         */
        bool isRunning();

        /*!
         * \brief Blocks execution until the loop reaches timeout
         */
        void waitForLoopEnd();

    private:

        /*!
         * \brief Initialize loop
         */
        void initialize();

        /*!
         * \brief Run loop
         *
         * This method is kept private. 'runLoopAsync' should be used instead.
         */
        void runLoop(std::chrono::milliseconds timeout);

        /*! \brief future state of the event loop thread run async  */
        std::future<void> _runFuture;
        /*! \brief Configuration of the Computational Graph run by this EventLoop  */
        nlohmann::json _graph_config;
        /*! \brief timestep of the event loop  */
        std::chrono::milliseconds _timestep;
        /*! \brief boolean variable used to step the event loop from parent thread */
        std::atomic<bool> _doRun;
        /*! \brief true if the EventLoop is assumed to always owns the GIL, false if it is shared with other threads  */
        bool _ownGIL;
        /*! \brief if true ros::sping is called in every loop  */
        bool _spinROS;
        /*! \brief GIL state object used to request the GIL ownership when needed  */
        PyGILState_STATE _pyGILState;
};


#endif // EVENT_LOOP_H
