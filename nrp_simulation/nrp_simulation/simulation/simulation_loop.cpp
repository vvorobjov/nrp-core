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
#include "nrp_general_library/utils/python_error_handler.h"
#include "nrp_general_library/utils/time_utils.h"

#include "nrp_simulation/device_handle/tf_manager_handle.h"

#include <iostream>

SimulationLoop::SimulationLoop(jsonSharedPtr config, DeviceHandle::engine_interfaces_t engines)
    : _config(config),
      _engines(engines),
      _devHandler(new TFManagerHandle())
{
	NRP_LOGGER_TRACE("{} called", __FUNCTION__);
}

void SimulationLoop::initLoop()
{
	NRP_LOGGER_TRACE("{} called", __FUNCTION__);

	// Init Device handle
    this->_devHandler->init(_config, _engines);

    // Init Engine
	for(const auto &engine : this->_engines)
	{
		try
		{
            this->_engineQueue.emplace(0, engine);
			engine->initialize();
		}
		catch(std::exception &e)
		{
			throw NRPException::logCreate(e, "Failed to initialize engine \"" + engine->engineName() + "\"");
		}
	}
}

void SimulationLoop::shutdownLoop()
{
	NRP_LOGGER_TRACE("{} called", __FUNCTION__);
	
	for(const auto &engine : this->_engines)
	{
		try
		{
			engine->shutdown();
		}
		catch(std::exception &e)
		{
			throw NRPException::logCreate(e, "Failed to shutdown engine \"" + engine->engineName() + "\"");
		}
	}
}

void SimulationLoop::runLoop(SimulationTime runLoopTime)
{
	NRP_LOGGER_TRACE("{} called [ runLoopTime: {} ]", __FUNCTION__, runLoopTime.count());
    NRP_LOG_TIME_BLOCK("sim_loop");

	const auto loopStopTime = this->_simTime + runLoopTime;

	if(this->_engineQueue.empty())
	{
		this->_simTime = loopStopTime;
		return;
	}

	// Continue processing engines until all engines next step completion time is greater than loopStopTime
    // _engineQueue is sorted by completion time of engine last step
	while(this->_engineQueue.begin()->first < loopStopTime)
	{
		// Get the next batch of engines which should finish next
        std::vector<EngineClientInterfaceSharedPtr> idleEngines;
		const auto nextCompletionTime = this->_engineQueue.begin()->first;
		do
		{
			this->_simTime = this->_engineQueue.begin()->first;
			idleEngines.push_back(this->_engineQueue.begin()->second);

			this->_engineQueue.erase(this->_engineQueue.begin());
		}
		while(!this->_engineQueue.empty() && this->_engineQueue.begin()->first <= nextCompletionTime);

        NRP_LOG_TIME("begin");

		// Wait for engines which will be processed to complete execution
		for(const auto &engine : idleEngines)
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

        NRP_LOG_TIME("wait");

		// Retrieve devices required by TFs from completed engines
		// Execute preprocessing TFs and TFs sequentially
        // Send tf output devices to corresponding engines
        this->_devHandler->deviceCycle(idleEngines);

		// Restart engines
		for(auto &engine : idleEngines)
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

				NRPLogger::warn("Engine \"" + engine->engineName() + "\" is ahead of simulation time by " +
				                             std::to_string(timeDiff) + "s\n");

				// Wait for rest of simulation to catch up to engine
				this->_engineQueue.emplace(engine->getEngineTime(), engine);
			}

			engine = nullptr;
		}

        NRP_LOG_TIME("start");
	}

	this->_simTime = loopStopTime;
}
