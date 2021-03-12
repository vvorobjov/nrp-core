//
// NRP Core - Backend infrastructure to synchronize simulations
//
// Copyright 2020-2021 NRP Team
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//   http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//
// This project has received funding from the European Union’s Horizon 2020
// Framework Programme for Research and Innovation under the Specific Grant
// Agreement No. 945539 (Human Brain Project SGA3).
//

#include "nrp_simulation/simulation/simulation_loop.h"
#include "nrp_general_library/utils/nrp_exceptions.h"
#include "nrp_general_library/device_interface/device.h"

#include <iostream>

static void executePreprocessingFunctions(TransceiverFunctionManager & tfManager, const std::vector<EngineClientInterfaceSharedPtr> & engines)
{
	for(auto &engine : engines)
	{
		// Execute all preprocessing functions for this engine

		auto results = tfManager.executeActiveLinkedPreprocessingTFs(engine->engineName());

		// Extract devices from the function results
		// The devices are stack objects, but we want to store pointers to them in engines cache
		// We have to convert them into heap-allocated objects

		EngineClientInterface::devices_set_t devicesHeap;
		for(const auto & result : results)
		{
			for(const auto &device : result.Devices)
			{
				devicesHeap.emplace(device->moveToSharedPtr());
			}
		}

		// Store pointers to devices from preprocessing functions in the engines cache

		engine->updateCachedDevices(std::move(devicesHeap));
	}
}

SimulationLoop::SimulationLoop(jsonSharedPtr config, engine_interfaces_t engines)
    : _config(config),
      _engines(engines),
      _tfManager(SimulationLoop::initTFManager(config, _engines))
{
	TransceiverDeviceInterface::setTFInterpreter(&(this->_tfManager.getInterpreter()));

	for(const auto &curEnginePtr : this->_engines)
	{	this->_engineQueue.emplace(0, curEnginePtr);	}
}

void SimulationLoop::initLoop()
{
	for(const auto &engine : this->_engines)
	{
		try
		{
			engine->initialize();
		}
		catch(std::exception &e)
		{
			throw NRPException::logCreate(e, "Failed to initialize engine \"" + engine->engineName() + "\"");
		}
	}
}

void SimulationLoop::runLoop(SimulationTime runLoopTime)
{
	const auto loopStopTime = this->_simTime + runLoopTime;
	
	if(this->_engineQueue.empty())
	{
		this->_simTime = loopStopTime;
		return;
	}

	// Compute all engines which should finish before loopStopTime
	std::vector<EngineClientInterfaceSharedPtr> processedEngines;
	while(this->_engineQueue.begin()->first < loopStopTime)
	{
		// Get the next batch of engines which should finish next
		// _engineQueue is sorted by completion time of engine last step
		const auto nextCompletionTime = this->_engineQueue.begin()->first;
		do
		{
			this->_simTime = this->_engineQueue.begin()->first;
			processedEngines.push_back(this->_engineQueue.begin()->second);

			this->_engineQueue.erase(this->_engineQueue.begin());
		}
		while(!this->_engineQueue.empty() && this->_engineQueue.begin()->first <= nextCompletionTime);

		// Wait for processed engines to complete execution
		for(const auto &engine : processedEngines)
		{
            float timeout = engine->engineConfig().at("EngineCommandTimeout");
			try
			{
				engine->waitForStepCompletion(timeout);
			}
			catch(std::exception &e)
			{
				throw NRPException::logCreate(e, "Engine \"" + engine->engineName() +"\" loop exceeded timeout of " +
				                              std::to_string(timeout) + "s");
			}
		}

		// Retrieve devices required by TFs from processed engines
		const auto requestedDeviceIDs = this->_tfManager.updateRequestedDeviceIDs();
		try
		{
			for(auto &engine : processedEngines)
			{
				engine->updateDevicesFromEngine(requestedDeviceIDs);
			}
		}
		catch(std::exception &)
		{
			// TODO: Handle failure on device retrieval
			throw;
		}

		// Execute preprocessing TFs

		executePreprocessingFunctions(this->_tfManager, processedEngines);

		// Execute TFs, and sort results according to engine
		TransceiverFunctionSortedResults results;
		for(const auto &engine : processedEngines)
		{
			auto curResults = this->_tfManager.executeActiveLinkedTFs(engine->engineName());
			results.addResults(curResults);
		}

		// Send tf output devices to corresponding engines
		for(const auto &engine : processedEngines)
		{
			try
			{
				this->sendDevicesToEngine(engine, results);
			}
			catch(std::exception &e)
			{
				throw NRPException::logCreate(e, "Failed to send devices to engine \"" + engine->engineName() + "\"");
			}
		}

		// Restart engines
		for(auto &engine : processedEngines)
		{
			const auto trueRunTime = this->_simTime - engine->getEngineTime() + engine->getEngineTimestep();

			if(trueRunTime >= SimulationTime::zero())
			{
				try
				{
					engine->runLoopStep(trueRunTime);
				}
				catch(std::exception &e)
				{
					throw NRPException::logCreate(e, "Failed to start loop of engine \"" + engine->engineName() + "\"");
				}

				// Reinsert engines into queue
				this->_engineQueue.emplace(this->_simTime + engine->getEngineTimestep(), engine);
			}
			else
			{
				const auto timeDiff = fromSimulationTime<float, std::ratio<1>>(engine->getEngineTime() - this->_simTime);

				NRPLogger::SPDWarnLogDefault("Engine \"" + engine->engineName() + "\" is ahead of simulation time by " +
				                             std::to_string(timeDiff) + "s\n");

				// Wait for rest of simulation to catch up to engine
				this->_engineQueue.emplace(engine->getEngineTime(), engine);
			}

			engine = nullptr;
		}

		processedEngines.clear();
	}

	this->_simTime = loopStopTime;
}

TransceiverFunctionManager SimulationLoop::initTFManager(const jsonSharedPtr &simConfig, const engine_interfaces_t &engines)
{
	TransceiverFunctionManager newManager;

	{
		TransceiverFunctionInterpreter::engines_devices_t engineDevs;
		for(const auto &engine : engines)
			engineDevs.emplace(engine->engineName(), &(engine->getCachedDevices()));

		newManager.getInterpreter().setEngineDevices(std::move(engineDevs));
	}

	TransceiverDeviceInterface::setTFInterpreter(&newManager.getInterpreter());

	// Load all preprocessing functions specified in the config

	const auto &preprocessingFunctions = simConfig->at("PreprocessingFunctionConfigs");
	for(const auto &tf : preprocessingFunctions)
	{
		newManager.loadTF(tf, true);
	}

	// Load all transceiver functions specified in the config

	const auto &transceiverFunctions = simConfig->at("TransceiverFunctionConfigs");
	for(const auto &tf : transceiverFunctions)
	{
        newManager.loadTF(tf, false);
	}

	return newManager;
}

void SimulationLoop::sendDevicesToEngine(const EngineClientInterfaceSharedPtr &engine, const TransceiverFunctionSortedResults &results)
{
	// Find corresponding devices
	const auto interfaceResultIterator = results.find(engine->engineName());
	if(interfaceResultIterator != results.end())
		engine->sendDevicesToEngine(interfaceResultIterator->second);

	// If no devices are available, have interface handle empty device input list
	// TODO: be sure that this is right
	engine->sendDevicesToEngine(typename EngineClientInterface::devices_ptr_t());
}

/*! \page simulation_loop Simulation Loop
The SimulationLoop is the class responsible for managing the execution of a simulation.

On initialization, it creates a TransceiverFunctionManager to manage all user-generated TransceiverFunctions. Additionally, it runs the initialization routines of all engines.

During the simulation, several components are managed by the SimulationLoop:
- The timestep of each engine is checked, and execution is ordered accordingly
- The loop waits for the engines with the next expected completion time. Should mulitple Engines complete at the same time, wait for all of them
- All TransceiverFunctions linked to these engines are identified
- The devices requested by these TFs are identified. Only those devices linked to newly completed engines are requested. For all others, use the data available in the buffer
- Newly received devices are stored in engine-local buffers
- Devices are retrieved from the local buffers, the linked TFs are executed
- The TFs return arrays with devices that should be sent to engines
- Received devices are sent to all engines
- Should this NRP instance be running as a server, the NRPClient can now communicate with the SimulationLoop and request updates/changes to the simulation
- A timeout is checked. Should it have occured or should the NRPClient requests a shutdown, the SimulationLoop is stopped. Otherwise, continue from the top

On shutdown, each engine is issued a shutdown command to close gracefully.

The entire loop is executed within one function, SimulationLoop::runLoop. Should any of the above steps fail, an exception is thrown.
 */
