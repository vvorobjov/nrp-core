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

#ifndef SIMULATION_LOOP_H
#define SIMULATION_LOOP_H

#include "nrp_general_library/transceiver_function/transceiver_function_interpreter.h"
#include "nrp_general_library/transceiver_function/transceiver_function_manager.h"
#include "nrp_general_library/transceiver_function/transceiver_function_sorted_results.h"

#include "nrp_general_library/engine_interfaces/engine_client_interface.h"

#include "nrp_general_library/utils/json_schema_utils.h"

/*!
 * \brief Manages simulation loop. Runs physics and brain interface, and synchronizes them via Transfer Functions
 */
class SimulationLoop
        : public PtrTemplates<SimulationLoop>
{
	public:
		using engine_interfaces_t = std::vector<EngineClientInterfaceSharedPtr>;

		SimulationLoop() = default;
		SimulationLoop(jsonSharedPtr config, engine_interfaces_t engines);

		/*!
		 * \brief Initialize engines before running loop
		 */
		void initLoop();

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
		{	return this->_simTime;	}

	private:
		/*!
		 * \brief Configuration of simulation
		 */
        jsonSharedPtr _config;

		/*!
		 * \brief Engines
		 */
		engine_interfaces_t _engines;

		using engine_queue_t = std::multimap<SimulationTime, EngineClientInterfaceSharedPtr>;

		/*!
		 * \brief Engine Queue. Contains all engines, sorted by completion time of their last step
		 */
		engine_queue_t _engineQueue;

		/*!
		 * \brief TF Manager containing all TFs associated with this simulation
		 */
		TransceiverFunctionManager _tfManager;

		/*!
		 * \brief Simulated time (in seconds)
		 */
		SimulationTime _simTime = SimulationTime::zero();

		/*!
		 * \brief Initialize the TF Manager. Reads the TF Configurations from the Simulation Config, and registers the TFs
		 * \param simConfig Simulation Config
		 * \param engines Loaded Engines
		 * \return Returns initialized TransceiverFunctionManager
		 */
		static TransceiverFunctionManager initTFManager(const jsonSharedPtr &simConfig, const engine_interfaces_t &engines);

		/*!
		 * \brief Handle device intputs of specified interface
		 * \param interfacePtr Shared Pointer to interface
		 * \param results Results to be processes
		 * \return Returns result of device handling inputs
		 */
		void sendDevicesToEngine(const EngineClientInterfaceSharedPtr &engine, const TransceiverFunctionSortedResults &results);

		friend class SimulationLoopTest_InitTFManager_Test;
};

using SimulationLoopSharedPtr = SimulationLoop::shared_ptr;
using SimulationLoopConstSharedPtr = SimulationLoop::const_shared_ptr;

#endif // SIMULATION_LOOP_H
