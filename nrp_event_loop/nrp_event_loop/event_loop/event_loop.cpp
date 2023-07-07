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

#ifdef ROS_ON
#include "ros/ros.h"
#endif

#include "nrp_event_loop/event_loop/event_loop.h"
#include "nrp_general_library/utils/nrp_exceptions.h"

#include "nrp_event_loop/utils/graph_utils.h"

EventLoop::EventLoop(const nlohmann::json &graph_config, std::chrono::milliseconds timestep, std::chrono::milliseconds rtDeltaThres,
                     ComputationalGraph::ExecMode execMode, bool ownGIL, bool spinROS, bool logRTInfo, bool syncTimeRef) :
        EventLoopInterface(timestep, rtDeltaThres, false, logRTInfo, syncTimeRef),
    _graph_config(graph_config),
    _execMode(execMode),
    _ownGIL(ownGIL),
    _spinROS(spinROS)
{
    _isTimeSyncMaster = true;
    this->initialize();
}

EventLoop::~EventLoop()
{ this->shutdown(); }

void EventLoop::initializeCB()
{
    if(!_ownGIL)
        _pyGILState = PyGILState_Ensure();

    try {
        boost::python::dict globalDict;
        createPythonGraphFromConfig(_graph_config, _execMode, globalDict);
    }
    catch (std::exception& e) {
        if(!_ownGIL)
            PyGILState_Release(_pyGILState);

        throw;
    }

    std::tie(_clock, _iteration) = findTimeNodes();

    if(!_ownGIL)
        PyGILState_Release(_pyGILState);
}

void EventLoop::runLoopCB()
{
    if(!_ownGIL)
        _pyGILState = PyGILState_Ensure();

#ifdef ROS_ON
    if(_spinROS)
        ros::spinOnce();
#endif

    if(_clock)
        _clock->updateClock(_currentTime);

    if(_iteration)
        _iteration->updateIteration(_iterations);

    try {
        ComputationalGraphManager::getInstance().compute();
    }
    catch (std::exception& e) {
        if(!_ownGIL)
            PyGILState_Release(_pyGILState);

        throw;
    }

    if(!_ownGIL)
        PyGILState_Release(_pyGILState);
}

void EventLoop::shutdownCB()
{
    ComputationalGraphManager::getInstance().clear();
}

std::string EventLoop::getLoggerBaseName()
{
    return "computational_graph";
}
