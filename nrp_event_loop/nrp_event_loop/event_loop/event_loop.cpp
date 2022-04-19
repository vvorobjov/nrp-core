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

#ifdef ROS_ON
#include "ros/ros.h"
#endif

#include "nrp_event_loop/event_loop/event_loop.h"
#include "nrp_general_library/utils/nrp_exceptions.h"

#include "nrp_event_loop/utils/graph_utils.h"

EventLoop::EventLoop(const nlohmann::json &graph_config, std::chrono::milliseconds timestep, bool ownGIL, bool spinROS) :
    _graph_config(graph_config),
    _timestep(timestep),
    _ownGIL(ownGIL),
    _spinROS(spinROS)
{
    this->initialize();
}

EventLoop::~EventLoop()
{
    this->shutdown();
}

void EventLoop::runLoopOnce()
{
    auto now = std::chrono::steady_clock::now();

    if(!_ownGIL)
        _pyGILState = PyGILState_Ensure();

#ifdef ROS_ON
    if(_spinROS)
        ros::spinOnce();
#endif

    ComputationalGraphManager::getInstance().compute();


    if(!_ownGIL)
        PyGILState_Release(_pyGILState);

    std::this_thread::sleep_until(now + _timestep);
}

void EventLoop::runLoop(std::chrono::milliseconds timeout)
{
    NRPLogger::debug("in loop");

    _doRun = true;
    bool useTimeout = timeout != std::chrono::milliseconds(0);

    auto start = std::chrono::steady_clock::now();

    while(_doRun && (!useTimeout || std::chrono::steady_clock::now() - start < timeout))
        runLoopOnce();

    NRPLogger::debug("out loop");
}

void EventLoop::runLoopAsync(std::chrono::milliseconds timeout)
{
    if(!this->isRunning()) {
        NRPLogger::debug("EventLoop was started");
        _runFuture = std::async(&EventLoop::runLoop, this, timeout);
    }
    else
        NRPLogger::info("EventLoop is already running. You must shut it down before running it again");
}

void EventLoop::initialize()
{
    if(!_ownGIL)
        _pyGILState = PyGILState_Ensure();

    boost::python::dict globalDict;
    createPythonGraphFromConfig(_graph_config, globalDict);

    if(!_ownGIL)
        PyGILState_Release(_pyGILState);
}

void EventLoop::shutdown()
{
    NRPLogger::debug("Shutting down EventLoop");
    this->stopLoop();
    ComputationalGraphManager::getInstance().clear();
    NRPLogger::debug("EventLoop was shut down");
}

void EventLoop::stopLoop()
{
    NRPLogger::debug("Stopping EventLoop");
    _doRun = false;
    this->waitForLoopEnd();
    NRPLogger::debug("EventLoop stopped");
}

void EventLoop::waitForLoopEnd()
{
    if(this->isRunning())
        _runFuture.wait();
}

bool EventLoop::isRunning()
{
    if(!_runFuture.valid())
        return false;
    else
        return _runFuture.wait_for(std::chrono::milliseconds(0)) != std::future_status::ready;
}
