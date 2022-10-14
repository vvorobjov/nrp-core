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

#ifndef SIMULATION_MANAGER_EVENT_LOOP_H
#define SIMULATION_MANAGER_EVENT_LOOP_H

#include "nrp_general_library/engine_interfaces/engine_launcher_manager.h"
#include "nrp_general_library/process_launchers/process_launcher_manager.h"
#include "nrp_event_loop/event_loop/event_loop.h"
#include "nrp_simulation/simulation/simulation_manager_fti.h"
#include "nrp_simulation/simulation/simulation_manager.h"

/*!
 * \brief Implementation of SimulationManager which manages an EventLoop
 */
class EventLoopSimManager
        : public SimulationManager
{
    public:

        /*!
         * \brief Constructor
         * \param simulationConfig Simulation configuration
         * \param engineLauncherManager Engine launchers
         * \param processLaunchers Process launchers
         */
        EventLoopSimManager(const jsonSharedPtr &simulationConfig, const EngineLauncherManagerConstSharedPtr &engineLauncherManager,
                          const MainProcessLauncherManager::const_shared_ptr &processLauncherManager);


        ~EventLoopSimManager() = default;

        const std::string & getStatus() override
        {
            return _status;
        }

    private:

        // Callback functions for the different simulation control requests.
        void initializeCB() override;
        bool resetCB() override;
        void stopCB() override;
        bool runUntilTimeOutCB() override;
        bool runCB(unsigned numIterations, const nlohmann::json & clientData) override;
        void shutdownCB() override;

        bool runUntilMilliseconds(const std::chrono::milliseconds& eTout);

        /*! \brief SimulationManager fTILoopSimManager*/
        std::shared_ptr<FTILoopSimManager> _fTILoopSimManager;
        /*! \brief engineLauncherManager */
        EngineLauncherManagerConstSharedPtr  _engineLauncherManager;
        /*! \brief processLauncherManager */
        MainProcessLauncherManager::const_shared_ptr  _processLauncherManager;

        /*! \brief Simulation loop */
        std::shared_ptr<EventLoop> _loop;

        std::string _status = "";

        std::chrono::milliseconds _timeout;
        std::chrono::milliseconds _timestep;
};

#endif
