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

#include "nrp_simulation/simulation/simulation_manager_fti.h"

#include "nrp_general_library/utils/file_finder.h"
#include "nrp_general_library/utils/nrp_exceptions.h"
#include "nrp_simulation/config/cmake_conf.h"
#include "nrp_general_library/utils/nrp_logger.h"

#include <iostream>
#include <fstream>
#include <string>
#include <future>
#include <filesystem>


FTILoopSimManager::FTILoopSimManager(const jsonSharedPtr &simulationConfig, const EngineLauncherManagerConstSharedPtr& engineLauncherManager,
                  const MainProcessLauncherManager::const_shared_ptr &processLauncherManager) :
                  SimulationManager(simulationConfig),
                  _engineLauncherManager(engineLauncherManager),
                  _processLauncherManager(processLauncherManager),
                  _stopLoop(false)
{}

FTILoopSimManager::~FTILoopSimManager()
{
    NRP_LOGGER_TRACE("{} called", __FUNCTION__);
    try {
        this->shutdownSimulation();
    }
    catch (const std::exception &e) {
        // It's ok, the simulation manager is being destroyed anyways
    }
}

bool FTILoopSimManager::hasSimulationTimedOut() const
{
    return (this->_simTimeout >= SimulationTime::zero() && this->_loop->getSimTime() >= this->_simTimeout);
}

void FTILoopSimManager::initializeCB()
{
    if(this->_loop == nullptr)
        this->_loop.reset(new FTILoop(this->createSimLoop()));
    else
        throw NRPException::logCreate("FTILoop already initialized");

    this->_loop->initLoop();
}

bool FTILoopSimManager::resetCB()
{
    try{
        if(this->_loop != nullptr)
            this->_loop->resetLoop();
        else
            throw NRPException::logCreate("Cannot reset simulation, FTI loop has not been created");

        this->_simulationDataManager.clear();
    }
    catch(NRPException &e) {
        throw NRPException::logCreate(e, "Reset simulation failed");
    }

    return true;
}

void FTILoopSimManager::stopCB()
{
    _stopLoop = true;
}

bool FTILoopSimManager::runUntilDoneOrTimeoutCB()
{
    std::function condition = [&] () {
        return this->_simulationDataManager.getDoneFlag() ||
               hasSimTimedOut(this->_loop->getSimTime(),
                              toSimulationTime<unsigned, std::ratio<1>>(int(this->_simConfig->at("SimulationTimeout"))));
    };

    return runSimulationUntilCondition(condition);
}

bool FTILoopSimManager::runCB(unsigned numIterations)
{
    unsigned iteration = 0;
    std::function condition = [&] () {
        return (numIterations > 0 && iteration++ >= numIterations);
    };

    return runSimulationUntilCondition(condition);
}

void FTILoopSimManager::shutdownCB()
{
    if(this->_loop != nullptr) {
        this->_loop->shutdownLoop();
        this->_loop.reset();
    }
}

bool FTILoopSimManager::runSimulationUntilCondition(std::function<bool ()> condition)
{
    if(this->_loop == nullptr)
        return false;

    bool hasCondition = false;
    _stopLoop = false;

    while(!_stopLoop)
    {
        hasCondition = condition();
        if(hasCondition)
            break;

        this->runSimulationOnce();
    }

    this->_loop->waitForEngines();

    return hasCondition;
}

void FTILoopSimManager::runSimulationOnce()
{
    if(this->_loop != nullptr)
        this->_loop->runLoop(this->_timeStep);
    else
        throw NRPException::logCreate("Simulation must be initialized before calling runLoop");
}

FTILoop FTILoopSimManager::createSimLoop()
{
    this->_timeStep = SimulationTime::max();

    DataPackProcessor::engine_interfaces_t engines;
    auto &engineConfigs = this->_simConfig->at("EngineConfigs");
    std::set<std::string> engineNames;

    // Create all engines required by simConfig
    engines.reserve(engineConfigs.size());
    for(auto &engineConfig : engineConfigs)
    {
        // Check that name is not repeated
        auto engineName = engineConfig.at("EngineName").get<std::string>();
        if(! engineNames.count(engineName))
            engineNames.insert(engineName);
        else
            throw NRPException::logCreate("Error while processing experiment configuration. The experiment contains two engines with the same name: "
                                          + engineName);
        
        // Get engine launcher associated with type
        const std::string engineType = engineConfig.at("EngineType");
        auto engineLauncher = _engineLauncherManager->findLauncher(engineType);
        if(engineLauncher == nullptr)
        {
            const auto errMsg = "Failed to find engine interface \"" + engineType + "\"";
            NRPLogger::error(errMsg);

            throw std::invalid_argument(errMsg);
        }

        // Create and launch engine
        try
        {
            engines.push_back(engineLauncher->launchEngine(engineConfig, _processLauncherManager->createProcessLauncher(this->_simConfig->at("ProcessLauncherType"))));
        }
        catch(std::exception &e)
        {
            throw NRPException::logCreate(e, "Failed to launch engine:  \"" + engineName + "\"");
        }

        if(this->_timeStep > toSimulationTimeFromSeconds(engineConfig.at("EngineTimestep").get<double>()))
        {
            this->_timeStep = toSimulationTimeFromSeconds(engineConfig.at("EngineTimestep").get<double>());
        }
    }
    NRPLogger::debug("Simulation Timestep is: "+ std::to_string(fromSimulationTime<double, std::ratio<1>>(this->_timeStep)));
    return FTILoop(this->_simConfig, engines, &this->_simulationDataManager);
}

bool FTILoopSimManager::hasSimTimedOut(const SimulationTime &simTime, const SimulationTime &simTimeout)
{
    return (simTimeout >= SimulationTime::zero() && simTime >= simTimeout);
}
