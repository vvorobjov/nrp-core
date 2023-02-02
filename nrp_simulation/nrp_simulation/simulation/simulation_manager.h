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

#ifndef SIMULATION_MANAGER_H
#define SIMULATION_MANAGER_H

#include "nrp_general_library/utils/json_schema_utils.h"
#include "nrp_general_library/utils/ptr_templates.h"
#include "nrp_general_library/engine_interfaces/engine_client_interface.h"
#include "nrp_simulation/datapack_handle/simulation_data_manager.h"

#include <mutex>
#include <functional>

/*!
 * \brief Manages simulation execution
 *
 * Maintain the current state of the simulation and process control requests according to this state.
 *
 * Control request methods are processed sequentially guarded by a lock, with the exception of "stopSimulation",
 * which can be processed in parallel.
 *
 * Control requests are forwarded to Implementations of this interface via a set of callbacks. Exceptions occurring
 * in these callbacks are handled by SimulationManager by transitioning the simulation to state "Failed", from which the
 * only possible action is to shutdown the simulation.
 *
 * Calling a control request from the wrong state triggers an exception
 */
class SimulationManager
        : public PtrTemplates<SimulationManager>
{
public:

    /*!
     * \brief States the simulation can be in
     */
    enum class SimState { Created, Initialized, Running, Stopped, Failed, NotSet};

    /*! \brief Helper structure for storing request result and error messages */
    struct RequestResult
    {
        SimState currentState = SimState::NotSet;
        std::string errorMessage = "";
    };

    /*!
     * \brief Validates config against Simulation schema
     * \param config Pointer to a config
     */
    static void validateConfig(jsonSharedPtr &config);

    /*!
     * \brief Constructor
     * \param simulationConfig Simulation configuration
     */
    SimulationManager(const jsonSharedPtr &simulationConfig);

    SimulationManager() = delete;

    virtual ~SimulationManager() = default;

    /*!
     * \brief Initialize the simulation
     * \return Simulation state after processing the request
     */
    RequestResult initializeSimulation();

    /*!
     * \brief Reset the currently running simulation
     * \return Simulation state after processing the request
     */
    RequestResult resetSimulation();

    /*!
     * \brief Request to stop the simulation if it was running, if it wasn't the call has no effect.
     *
     * It is not blocking, i.e. it is not guaranteed that the simulation is in state "Stopped" after the call returns.
     * The simulation will stop after the the "runSimulation" request that started it returns.
     *
     * \return Simulation state after processing the request
     */
    RequestResult stopSimulation();

    /*!
     * \brief Runs the simulation until a separate thread stops it or simTimeout (defined in SimulationConfig) is reached. If simTimeout is zero or negative, ignore it
     * \return Simulation state after processing the request
     */
    RequestResult runSimulationUntilDoneOrTimeout();

    virtual bool hasSimulationTimedOut() const = 0;

    /*!
     * \brief Run the Simulation for specified amount of timesteps
     * \param numIterations Number of iterations (i.e timesteps) to run simulation
     * \return Simulation state after processing the request
     */
    RequestResult runSimulation(unsigned numIterations);

    /*!
     * \brief Shuts down the simulation.
     * \return Simulation state after processing the request
     */
    RequestResult shutdownSimulation();

    /*!
     * \brief returns the current state of the simulation
     */
    SimState currentState();

    /*!
     * \brief returns a simulation state as a string
     */
    std::string printSimState(const SimState& simState);

    SimulationDataManager & getSimulationDataManager();

protected:

    // Callback functions for the different simulation control requests.
    /*! \brief Initialize the simulation */
    virtual void initializeCB() = 0;
    /*! \brief Resets the simulation, returns true if the simulation was reset false otherwise */
    virtual bool resetCB() = 0;
    /*! \brief Forward request to stop the simulation. */
    virtual void stopCB() = 0;
    /*! \brief Run the simulation. */
    virtual bool runUntilDoneOrTimeoutCB() = 0;
    /*! \brief Run the simulation. */
    virtual bool runCB(unsigned numIterations) = 0;
    /*! \brief Shutdown the simulation. */
    virtual void shutdownCB() = 0;

    /*!
     * \brief Simulation Configuration
     */
    jsonSharedPtr _simConfig;

    SimulationDataManager _simulationDataManager;

    SimulationTime _simTimeout = SimulationTime::zero();

private:

    /*!
     * \brief Stores the simulation state
     */
    SimState _simState;

    std::mutex _reqMutex;

    RequestResult processRequest(std::function<void ()> action, std::vector<SimState> validSourceStates,
                            std::string actionMsg1, std::string actionMsg2, bool lockMutex = true);

    void checkTransitionConstraints(std::vector<SimState> validSourceStates, std::string actionStr);
    void changeState(SimState newState);
};


#endif
