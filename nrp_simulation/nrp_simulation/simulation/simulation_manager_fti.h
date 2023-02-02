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

#ifndef SIMULATION_MANAGER_FTI_H
#define SIMULATION_MANAGER_FTI_H

#include "nrp_general_library/engine_interfaces/engine_launcher_manager.h"
#include "nrp_general_library/process_launchers/process_launcher_manager.h"
#include "nrp_simulation/simulation/fti_loop.h"
#include "nrp_simulation/simulation/simulation_manager.h"

/*!
 * \brief Implementation of SimulationManager which manages an FTILoop
 */
class FTILoopSimManager
        : public SimulationManager
{
    public:

        /*!
         * \brief Constructor
         * \param simulationConfig Simulation configuration
         * \param engineLauncherManager Engine launchers
         * \param processLaunchers Process launchers
         */
        FTILoopSimManager(const jsonSharedPtr &simulationConfig, const EngineLauncherManagerConstSharedPtr &engineLauncherManager,
                          const MainProcessLauncherManager::const_shared_ptr &processLauncherManager);

        /*!
         * \brief Destructor. Will shutdown the simulation if it was initialized
         */
        ~FTILoopSimManager();

        bool hasSimulationTimedOut() const override;

    private:

        // Callback functions for the different simulation control requests.
        void initializeCB() override;
        bool resetCB() override;
        void stopCB() override;
        bool runUntilDoneOrTimeoutCB() override;
        bool runCB(unsigned numIterations) override;
        void shutdownCB() override;

        /*! \brief engineLauncherManager */
        EngineLauncherManagerConstSharedPtr  _engineLauncherManager;
        /*! \brief processLauncherManager */
        MainProcessLauncherManager::const_shared_ptr  _processLauncherManager;
        /*! \brief Simulation loop */
        FTILoopSharedPtr _loop;
        /*! \brief Timestep size of each simulation loop */
        SimulationTime _timeStep;
        /*! \brief Used to process stop loop requests */
        std::atomic<bool> _stopLoop;

        /*!
         * \brief Creates a simulation loop using the engines specified in the config file
         */
        FTILoop createSimLoop();

        /*!
         * \brief Run the Simulation Loop for one timestep
         */
        void runSimulationOnce();

        /*!
         * \brief Run the Simulation Loop until condition becomes true
         * \param condition function evaluating the stop condition
         * \param clientData Extra data coming from the NRP Client, will be passed to status function
         * \return true if condition was met, false otherwise
         */
        bool runSimulationUntilCondition(std::function<bool ()> condition);

        /*!
         * \brief Checks whether simulation has timed out. If simTimeout <=0, always returns false
         * \param simTime Simulation time (in seconds)
         * \param simTimeout Simulation timeout (in seconds)
         * \return Returns true if simulation has timed out, false otherwise
         */
        bool hasSimTimedOut(const SimulationTime &simTime, const SimulationTime &simTimeout);
};

#endif
