//
// NRP Core - Backend infrastructure to synchronize simulations
//
// Copyright 2020-2023 NRP Team
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

#include "nrp_simulation/simulation/simulation_manager_event_loop.h"

#include "nrp_general_library/utils/file_finder.h"
#include "nrp_general_library/utils/nrp_exceptions.h"
#include "nrp_simulation/config/cmake_conf.h"
#include "nrp_general_library/utils/nrp_logger.h"

#include <iostream>
#include <fstream>
#include <string>
#include <future>
#include <filesystem>


EventLoopSimManager::EventLoopSimManager(const jsonSharedPtr &simulationConfig, const EngineLauncherManagerConstSharedPtr& engineLauncherManager,
                  const MainProcessLauncherManager::const_shared_ptr &processLauncherManager) :
                  SimulationManager(simulationConfig),
                  _engineLauncherManager(engineLauncherManager),
                  _processLauncherManager(processLauncherManager)
{}

void EventLoopSimManager::initializeCB()
{
    if(this->_loop == nullptr) {
        // Validate configuration
        auto ELoopConf = this->_simConfig->contains("EventLoop") ?
                (*(this->_simConfig))["EventLoop"].get<nlohmann::json>() : nlohmann::json::object();
        json_utils::validateJson(ELoopConf, "json://nrp-core/event_loop.json#/event_loop");

        // Configure Event Loop
        auto eTstep = ELoopConf.at("Timestep").get<float>();
        auto eTout  = ELoopConf.at("Timeout").get<float>();
        auto eTstepWarn = ELoopConf.at("TimestepWarnThreshold").get<float>();

        _timestep = std::chrono::milliseconds((int)(1000 * eTstep));
        _timeout = std::chrono::milliseconds((int)(1000 * eTout));
        auto timestepWarn = std::chrono::milliseconds((int)(1000 * eTstepWarn));

        std::stringstream info_msg;
        info_msg << "Creating Event Loop with configuration: timestep=" << _timestep.count() << "(ms), timeout=" << _timeout.count() << "(ms)";
        NRPLogger::info(info_msg.str());

        // Create fti loop
        if(this->_simConfig->at("DataPackProcessor") != "cg")
            throw NRPException::logCreate("Event Loop can only run a Computational Graph, not TFs. Please modify your experiment to use a Computational Graph and set the \"DataPackProcessor\" config parameter to \"cg\".");
        // TODO: little hack to pass execution mode to ComputationalGraphHandle
        (*(this->_simConfig))["ExecutionMode"] = ELoopConf.at("ExecutionMode");
        auto [ftiLoop, ftiTimestep] = createFTILoop(this->_simConfig, nullptr, 
                                                               this->_engineLauncherManager,
                                                               this->_processLauncherManager);

        // Create and initialize EventLoop
        this->_loop.reset(new EventLoop(_timestep, timestepWarn, ftiLoop, ftiTimestep));
    }
    else
        throw NRPException::logCreate("EventLoop already initialized");
}

bool EventLoopSimManager::resetCB()
{
    throw NRPException::logCreate("Reset is not implemented in the EventLoop");
}

void EventLoopSimManager::stopCB()
{
    this->_loop->stopLoop();
}

bool  EventLoopSimManager::runUntilMilliseconds(const std::chrono::milliseconds& eTout)
{
    // run EventLoop
    _loop->runLoop(eTout);

    return true;
}

bool EventLoopSimManager::runUntilDoneOrTimeoutCB()
{
    // TODO Is there a 'done' condition?
    return runUntilMilliseconds(_timeout);
}

bool EventLoopSimManager::runCB(unsigned numIterations)
{
    return runUntilMilliseconds(_timestep * numIterations);
}

void EventLoopSimManager::shutdownCB()
{
    if(this->_loop != nullptr) {
        this->_loop->shutdown();
        this->_loop.reset();
    }
}
