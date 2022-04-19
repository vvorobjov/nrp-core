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

#include "nrp_simulation/simulation/simulation_manager.h"

#include "nrp_general_library/utils/nrp_logger.h"

void SimulationManager::validateConfig(jsonSharedPtr &config)
{
    json_utils::validateJson(*config, "https://neurorobotics.net/simulation.json#Simulation");

    // Set default values
    json_utils::setDefault<std::vector<nlohmann::json>>(*config, "EngineConfigs", std::vector<nlohmann::json>());
    json_utils::setDefault<std::vector<nlohmann::json>>(*config, "ExternalProcesses", std::vector<nlohmann::json>());
    json_utils::setDefault<std::vector<nlohmann::json>>(*config, "DataPackProcessingFunctions", std::vector<nlohmann::json>());
    json_utils::setDefault<std::vector<std::string>>(*config, "ComputationalGraph", std::vector<std::string>());
}

SimulationManager::SimulationManager(const jsonSharedPtr &simulationConfig)
    : _simConfig(simulationConfig),
    _simState(SimState::Created)
{
    validateConfig(_simConfig);
}

SimulationManager::RequestResult SimulationManager::initializeSimulation()
{
    NRP_LOGGER_TRACE("{} called", __FUNCTION__);
    return processRequest([&]() {
                              this->initializeCB();
                              changeState(SimState::Initialized);
                          },
                          {SimState::Created},
                          "Initializing",
                          "initialized");
}

SimulationManager::RequestResult SimulationManager::runSimulation(unsigned numIterations)
{
    NRP_LOGGER_TRACE("{} called", __FUNCTION__);
    return processRequest([&]() {
                              changeState(SimState::Running);
                              if(!this->runCB(numIterations))
                                  NRPLogger::debug("Simulation has been stopped before running the specified number of iterations");
                              changeState(SimState::Stopped);
                          },
                          {SimState::Initialized, SimState::Stopped},
                          "Running",
                          "run");
}

SimulationManager::RequestResult SimulationManager::runSimulationUntilTimeout()
{
    NRP_LOGGER_TRACE("{} called", __FUNCTION__);
    return processRequest([&]() {
                              changeState(SimState::Running);
                              if(!this->runUntilTimeOutCB())
                                  NRPLogger::debug("Simulation has been stopped before reaching timeout");
                              changeState(SimState::Stopped);
                          },
                          {SimState::Initialized, SimState::Stopped},
                          "Running",
                          "run");
}

SimulationManager::RequestResult SimulationManager::resetSimulation()
{
    NRP_LOGGER_TRACE("{} called", __FUNCTION__);
    return processRequest([&]() {
                              if(this->resetCB())
                                  changeState(SimState::Initialized);
                          },
                          {SimState::Initialized, SimState::Stopped},
                          "Resetting",
                          "reset");
}

SimulationManager::RequestResult SimulationManager::stopSimulation()
{
    NRP_LOGGER_TRACE("{} called", __FUNCTION__);
    return processRequest([&]() {
                              this->stopCB();
                          },
                          {SimState::Initialized, SimState::Stopped, SimState::Running},
                          "Stopping",
                          "stopped",
                          false);
}

SimulationManager::RequestResult SimulationManager::shutdownSimulation()
{
    NRP_LOGGER_TRACE("{} called", __FUNCTION__);
    return processRequest([&]() {
                       this->shutdownCB();
                       changeState(SimState::Created);
        },
                   {SimState::Initialized, SimState::Stopped, SimState::Running, SimState::Failed},
                   "Shutting down",
                   "shutdown");
}

SimulationManager::RequestResult SimulationManager::processRequest(
        std::function<void ()> action, std::vector<SimState> validSourceStates,
        std::string actionMsg1, std::string actionMsg2, bool lockMutex)
{
    NRPLogger::info(actionMsg1 + " Simulation");
    checkTransitionConstraints(std::move(validSourceStates), actionMsg2);

    if(lockMutex)
        _reqMutex.lock();

    RequestResult res;

    try {
        action();
    }
    catch (const std::exception &e) {
        changeState(SimState::Failed);
        NRPLogger::warn(actionMsg1 + " Simulation Failed:\n" + e.what());
        res.errorMessage = e.what();
    }

    res.currentState = _simState;

    if(lockMutex)
        _reqMutex.unlock();

    return res;
}

std::string SimulationManager::printSimState(const SimulationManager::SimState& simState)
{
    switch (simState) {
        case SimState::Created:
            return "Created";
        case SimState::Initialized:
            return "Initialized";
        case SimState::Running:
            return "Running";
        case SimState::Stopped:
            return "Stopped";
        case SimState::Failed:
            return "Failed";
        default:
            return "";
    }
}

SimulationManager::SimState SimulationManager::currentState()
{
    return _simState;
}

void SimulationManager::checkTransitionConstraints(std::vector<SimState> validSourceStates, std::string actionStr)
{
    // Check FSM constraints
    if(std::find(validSourceStates.begin(), validSourceStates.end(), _simState) == validSourceStates.end()) {
        std::string msg = "Simulation can't be " + actionStr + " from state \"" + printSimState(_simState) + "\"";
        throw std::logic_error(msg);
    }
}

void SimulationManager::changeState(SimState newState)
{
    _simState = newState;
}
