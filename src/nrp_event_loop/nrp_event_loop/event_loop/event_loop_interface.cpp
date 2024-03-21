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

#include "nrp_event_loop/event_loop/event_loop_interface.h"
#include "nrp_general_library/utils/nrp_exceptions.h"


EventLoopInterface::EventLoopInterface(std::chrono::milliseconds timestep, std::chrono::milliseconds timestepThres) :
    _timestep(timestep),
    _timestepThres(timestepThres)
{ }

void EventLoopInterface::runLoopOnce(const std::chrono::time_point<std::chrono::steady_clock>& startTime)
{
    this->runLoopCB();
    std::this_thread::sleep_until(startTime + _timestep);
}

void EventLoopInterface::runLoop(std::chrono::milliseconds timeout)
{
    if(!_isInitialized)
        throw NRPException::logCreate("EventLoop has not been initialized. It can't be run");

    NRPLogger::debug("Starting Event Loop");

    _doRun = true;
    bool useTimeout = timeout != std::chrono::milliseconds(0);

    _iterations = 0L;
    auto startLoopTime = std::chrono::steady_clock::now();
    auto startStepTime = startLoopTime;
    auto lastStartStepTime = startLoopTime;
    auto lastStepDuration = startStepTime - lastStartStepTime;

    while(_doRun) {
        startStepTime = std::chrono::steady_clock::now();
        lastStepDuration = startStepTime - lastStartStepTime;
        _currentTime = std::chrono::duration_cast<std::chrono::milliseconds>(startStepTime - startLoopTime);

        if(useTimeout && _currentTime >= timeout)
            break;

        if(lastStepDuration > _timestep + _timestepThres) {
            NRPLogger::warn("Event Loop can't run at the target frequency. Actual step duration: " +
            std::to_string(std::chrono::duration_cast<std::chrono::milliseconds>(lastStepDuration).count()) +
            " (ms). Target step duration: " + std::to_string(_timestep.count()) + " (ms).");
        }

        runLoopOnce(startStepTime);

        lastStartStepTime = startStepTime;
        _iterations++;
    }

    _doRun = false;

    NRPLogger::debug("Completed Event Loop");
}

void EventLoopInterface::runLoopAsync(std::chrono::milliseconds timeout, bool doInit)
{
    if(!this->isRunning()) {
        NRPLogger::debug("EventLoop was started");
        _runFuture = std::async([&, timeout, doInit] () {
            if(doInit)
                this->initialize();

            this->runLoop(timeout);
        });
    }
    else
        NRPLogger::info("EventLoop is already running. You must stop it before running it again");
}

void EventLoopInterface::initialize()
{
    if(_isInitialized)
        throw NRPException::logCreate("EventLoop has been initialized already. You must shut it down before initializing it again");
    
    this->initializeCB();
    _isInitialized = true;
}

void EventLoopInterface::shutdown()
{
    if(!_isInitialized)
        return;

    NRPLogger::debug("Shutting down EventLoop");
    
    this->stopLoop();
    this->shutdownCB();

    NRPLogger::debug("EventLoop was shut down");

    _isInitialized = false;
}

void EventLoopInterface::stopLoop()
{
    NRPLogger::debug("Stopping EventLoop");
    _doRun = false;
    this->waitForLoopEnd();
    NRPLogger::debug("EventLoop stopped");
}

void EventLoopInterface::waitForLoopEnd()
{
    if(this->isRunning())
        _runFuture.wait();
}

bool EventLoopInterface::isRunning()
{
    if(!_runFuture.valid())
        return false;
    else
        return _runFuture.wait_for(std::chrono::milliseconds(0)) != std::future_status::ready;
}

bool EventLoopInterface::isRunningNotAsync()
{ return _doRun; }
