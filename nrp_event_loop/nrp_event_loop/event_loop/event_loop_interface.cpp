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
#include <algorithm>

EventLoopInterface::EventLoopInterface(std::chrono::milliseconds timestep, std::chrono::milliseconds rtDeltaThres,
                                       bool delegateRTControl, bool logRTInfo, bool syncTimeRef) :
        _timestep(timestep),
        _rtDeltaThres(rtDeltaThres),
        _delegateRTControl(delegateRTControl),
        _syncTimeRef(syncTimeRef),
        _logRTInfo(logRTInfo)
{ }

void EventLoopInterface::runLoop(std::chrono::milliseconds timeout)
{
    if(!_isInitialized)
        throw NRPException::logCreate("EventLoop has not been initialized. It can't be run");

    NRPLogger::debug("Starting Event Loop");

    _doRun = true;
    bool useTimeout = timeout != std::chrono::milliseconds(0);

    _iterations = 0L;
    _currentTime = std::chrono::milliseconds(0);
    auto startLoopTime = std::chrono::system_clock::now();
    if(_syncTimeRef) {
#ifdef MQTT_ON
        if(_isTimeSyncMaster)
            sendTimeRef(startLoopTime);
        else
            startLoopTime = waitForTimeRef();
#else
        throw NRPException::logCreate("EventLoop was requested to use a global time reference, which requires MQTT for synchronization. But nrp-core has been compiled without MQTT support.");
#endif
    }

    auto startStepTime = startLoopTime;
    auto endStepTime = startLoopTime;
    auto stepDuration = endStepTime - startStepTime;

    long int stepDurationInt = 0;
    long int stepDurationAverage = 0;
    long int stepDurationMin = _timestep.count();
    long int stepDurationMax = 0;

    if(_logRTInfo) {
        initFileLogger();
        (*_fileLogger) << "Iteration CurrentTime StepDuration\n";
    }

    while(_doRun) {
        // Mark step start time
        startStepTime = std::chrono::system_clock::now();
        // Run loop computations
        this->runLoopCB();
        // Control step duration, threshold and rt deviation
        endStepTime = std::chrono::system_clock::now();
        _currentTime = std::chrono::duration_cast<std::chrono::milliseconds>(endStepTime - startLoopTime);
        _iterations++;
        stepDuration = endStepTime -  startStepTime;
        auto rtDev = stepDuration - _timestep;

        if(_logRTInfo) {
            stepDurationInt =  std::chrono::duration_cast<std::chrono::milliseconds>(stepDuration).count();
            stepDurationAverage += stepDurationInt;
            stepDurationMax = std::max(stepDurationMax, stepDurationInt);
            stepDurationMin = std::min(stepDurationMin, stepDurationInt);
            // TODO: it would be better to do the logging in a separate thread, to avoid delays when ofstream buffer flush
            (*_fileLogger) << _iterations << " " << _currentTime.count() << " " << stepDurationInt << "\n";
        }

        if(useTimeout && _currentTime >= timeout)
            break;

//        if(rtDev > _rtDeltaThres) {
//            NRPLogger::warn("Event Loop can't run at the target frequency. Actual step duration: " +
//                            std::to_string(std::chrono::duration_cast<std::chrono::milliseconds>(stepDuration).count()) +
//                            " (ms). Target step duration: " + std::to_string(_timestep.count()) + " (ms).");
//        }

        if(!_delegateRTControl) {
            // TODO: add average window parameter and probably change sleep_until by sleep_for and move to derived classes?
            realtimeDeltaCB(std::chrono::duration_cast<std::chrono::milliseconds>(rtDev));
            std::this_thread::sleep_until(startStepTime + _timestep);
        }
    }

    if(_logRTInfo && _iterations) {
        NRPLogger::info("Event Loop Step Duration stats. average: " +
                        std::to_string(stepDurationAverage/_iterations) + " (ms). max: " +
                        std::to_string(stepDurationMax) + " (ms). min: " +
                        std::to_string(stepDurationMin) + " (ms).");
        // TODO: if the process is stopped with signint is the log file closed properly?
        closeFileLogger();
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

#ifdef MQTT_ON

void EventLoopInterface::sendTimeRef(const std::chrono::time_point<std::chrono::system_clock>& timeRef)
{
    auto mqttProxy = &(NRPMQTTProxy::getInstance());
    if(!mqttProxy || !mqttProxy->isConnected())
        throw NRPException::logCreate("EventLoop failed to connect to MQTT. Ensure that the MQTT broker is running and check the engine configuration");

    auto timeRefStr = std::to_string(std::chrono::duration_cast<std::chrono::microseconds>(
            timeRef.time_since_epoch()).count());

    NRPLogger::debug("Sent time reference: " + timeRefStr);

    mqttProxy->publish(EVENT_LOOP_TIME_REF_MQTT_TOPIC,
                       timeRefStr, true);
}

std::chrono::time_point<std::chrono::system_clock> EventLoopInterface::waitForTimeRef()
{
    auto mqttProxy = &(NRPMQTTProxy::getInstance());
    if(!mqttProxy || !mqttProxy->isConnected())
        throw NRPException::logCreate("EventLoop failed to connect to MQTT. Ensure that the MQTT broker is running and check the engine configuration");

    std::string timeRefStr = "";
    bool gotTimeRef = false;
    mqttProxy->subscribe(EVENT_LOOP_TIME_REF_MQTT_TOPIC, [&] (const std::string& msg) {
        timeRefStr = msg;
        gotTimeRef = true;
    });

    while(!gotTimeRef) {
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
        if(!_doRun)
            return std::chrono::system_clock::now();
    }

    NRPLogger::debug("Got time reference: " + timeRefStr);

    long timeRefLong = std::stol(timeRefStr);
    return std::chrono::time_point<std::chrono::system_clock>(std::chrono::microseconds(timeRefLong));
}

#endif
