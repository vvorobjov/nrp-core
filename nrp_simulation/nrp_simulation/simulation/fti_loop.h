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

#ifndef FTI_LOOP_H
#define FTI_LOOP_H

#include "nrp_general_library/engine_interfaces/engine_client_interface.h"

#include "nrp_general_library/utils/json_schema_utils.h"

#include "nrp_simulation/datapack_handle/datapack_handle.h"

/*!
 * \brief Manages simulation loop. Runs physics and brain interface, and synchronizes them via Transfer Functions
 */
class FTILoop
        : public PtrTemplates<FTILoop>
{
    public:

        FTILoop() = default;
        FTILoop(jsonSharedPtr config, DataPackProcessor::engine_interfaces_t engines);

        /*!
         * \brief Initialize engines before running loop
         */
        void initLoop();

        /*!
         * \brief Reset engines of the loop
         */
        void resetLoop();

        /*!
         * \brief Shutdown engines
         */
        void shutdownLoop();

        /*!
         * \brief Runs a single loop step
         * \param timeStep How long the single components should run (in seconds)
         */
        //void runLoopStep(float timeStep);

        /*!
         * \brief Runs simulation for a total of runLoopTime (in s)
         * \param runLoopTime Time (in s) to run simulation. At end, will run TransceiverFunctions
         */
        void runLoop(SimulationTime runLoopTime);

        /*!
         * \brief Get Simulation Time (in seconds)
         * \return Returns time passed in simulation (in seconds)
         */
        inline SimulationTime getSimTime() const
        {   return this->_simTime;  }

    private:
        /*!
         * \brief Configuration of simulation
         */
        jsonSharedPtr _config;

        /*!
         * \brief Engines
         */
        DataPackProcessor::engine_interfaces_t _engines;

        using engine_queue_t = std::multimap<SimulationTime, EngineClientInterfaceSharedPtr>;

        /*!
         * \brief Engine Queue. Contains all engines, sorted by completion time of their last step
         */
        engine_queue_t _engineQueue;

        /*!
         * \brief Simulated time (in seconds)
         */
        SimulationTime _simTime = SimulationTime::zero();

        /*!
         * \brief Used to handle datapack operations in engines
         */
        std::unique_ptr<DataPackProcessor> _devHandler;


        friend class FTILoopTest_InitTFManager_Test;
};

using FTILoopSharedPtr = FTILoop::shared_ptr;
using FTILoopConstSharedPtr = FTILoop::const_shared_ptr;

#endif // FTI_LOOP_H
