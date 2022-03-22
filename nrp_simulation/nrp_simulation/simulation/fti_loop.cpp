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

#include "nrp_simulation/simulation/fti_loop.h"
#include "nrp_general_library/utils/nrp_exceptions.h"
#include "nrp_general_library/datapack_interface/datapack.h"
#include "nrp_general_library/utils/python_error_handler.h"
#include "nrp_general_library/utils/time_utils.h"

#include "nrp_simulation/datapack_handle/tf_manager_handle.h"
#include "nrp_simulation/datapack_handle/computational_graph_handle.h"

#include <iostream>

// Helper function which reads simulation config and returns the correct DataPackProcessor
static DataPackProcessor* makeHandleFromConfig(jsonSharedPtr config)
{
    std::string dev_p = config->at("DataPackProcessor").get<std::string>();
    bool spinROS = config->contains("ConnectROS");
    bool slaveMode = config->at("SimulationLoop") == "EventLoop";
    if(dev_p == "cg" || slaveMode)
        return new ComputationalGraphHandle(slaveMode, spinROS);
    else if(dev_p == "tf")
        return new TFManagerHandle();
    else
        throw NRPException::logCreate("Unsupported DataPackProcessor: " + dev_p);
}

static void runLoopStepAsyncGet(EngineClientInterfaceSharedPtr engine)
{
    const SimulationTime timeout = toSimulationTimeFromSeconds(engine->engineConfig().at("EngineCommandTimeout"));

    try
    {
        // Wait for run loop step completion and get the results

        engine->runLoopStepAsyncGet(timeout);
    }
    catch(std::exception &e)
    {
        throw NRPException::logCreate(e, "Engine \"" + engine->engineName() +"\" loop exceeded timeout of " +
                                        std::to_string(timeout.count()));
    }
}


FTILoop::FTILoop(jsonSharedPtr config, DataPackProcessor::engine_interfaces_t engines)
    : _config(config),
      _engines(engines),
      _devHandler(makeHandleFromConfig(config))
{
    NRP_LOGGER_TRACE("{} called", __FUNCTION__);
}

void FTILoop::initLoop()
{
    NRP_LOGGER_TRACE("{} called", __FUNCTION__);

    // Init DataPack handle
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

void FTILoop::resetLoop()
{
    NRP_LOGGER_TRACE("{} called", __FUNCTION__);

    // reset engine queue
    if (!this->_engineQueue.empty())
    {
        // Get the next batch of engines which should finish next
        std::vector<EngineClientInterfaceSharedPtr> idleEngines;
        const auto nextCompletionTime = this->_engineQueue.begin()->first;
        do
        {
            this->_simTime = this->_engineQueue.begin()->first;
            idleEngines.push_back(this->_engineQueue.begin()->second);

            this->_engineQueue.erase(this->_engineQueue.begin());

        } while (!this->_engineQueue.empty());

        // Wait for engines which will be processed to complete execution
        for (const auto &engine : idleEngines)
        {
            runLoopStepAsyncGet(engine);
        }
    }

    for(const auto &engine : this->_engines)
    {
        try
        {
            engine->reset();
        }
        catch(std::exception &e)
        {
            throw NRPException::logCreate(e, "Failed to reset engine \"" + engine->engineName() + "\"");
        }
    }

    for (const auto &curEnginePtr : this->_engines)
    {
        this->_engineQueue.emplace(0, curEnginePtr);
    }

    this->_simTime = SimulationTime::zero();
}

void FTILoop::shutdownLoop()
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

void FTILoop::runLoop(SimulationTime runLoopTime)
{
    NRP_LOGGER_TRACE("{} called [ runLoopTime: {} ]", __FUNCTION__, runLoopTime.count());
    NRP_LOG_TIME_BLOCK("sim_loop");

    const auto loopStopTime = this->_simTime + runLoopTime;

    if(this->_engineQueue.empty())
    {
        NRPLogger::debug("FTILoop::runLoop: _engineQueue is empty");
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
            runLoopStepAsyncGet(engine);
        }

        NRP_LOG_TIME("wait");

        // Retrieve datapacks required by TFs from completed engines
        // Execute preprocessing TFs and TFs sequentially
        // Send tf output datapacks to corresponding engines
        this->_devHandler->datapackCycle(idleEngines);

        // Restart engines
        for(auto &engine : idleEngines)
        {
            const auto trueRunTime = this->_simTime - engine->getEngineTime() + engine->getEngineTimestep();

            if(trueRunTime >= SimulationTime::zero())
            {
                try
                {
                    // Execute run loop step in a worker thread
                    // The results will be checked before running the transceiver functions

                    engine->runLoopStepAsync(trueRunTime);
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
