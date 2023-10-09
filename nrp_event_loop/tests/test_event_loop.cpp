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

#include <functional>
#include <mutex>
#include <chrono>
#include "nlohmann/json.hpp"

#include <gtest/gtest.h>

#include "nrp_event_loop/event_loop/event_loop.h"
#include "nrp_event_loop/computational_graph/computational_node.h"
#include "nrp_event_loop/computational_graph/computational_graph_manager.h"
#include "nrp_event_loop/nodes/dummy/output_dummy.h"
#include "tests/test_files/helper_classes.h"

#include "nrp_event_loop/config/cmake_constants.h"


//// EVENT LOOP

TEST(EventLoop, EVENT_LOOP) {
    Py_Initialize();

    std::chrono::milliseconds timestep(10);
    std::chrono::milliseconds timestepThres(1);
    nlohmann::json graph_config;
    std::stringstream py_file;
    py_file << TEST_EVENT_LOOP_PYTHON_FUNCTIONS_MODULE_PATH << "/test_time_nodes.py";
    graph_config.push_back(py_file.str());
    EventLoop e_l(graph_config, timestep, timestepThres, ComputationalGraph::ALL_NODES, true, false);

    // run loop once
    auto now = std::chrono::steady_clock::now();
    e_l.runLoopOnce(now);
    auto time_lapse = std::chrono::steady_clock::now() - now;

    auto clockOut = dynamic_cast<OutputDummy*>(ComputationalGraphManager::getInstance().getNode("clock_out"));
    auto iterOut = dynamic_cast<OutputDummy*>(ComputationalGraphManager::getInstance().getNode("iteration_out"));
    ASSERT_TRUE(time_lapse.count() >= 10000000);
    ASSERT_EQ(clockOut->call_count, 1);
    ASSERT_EQ(iterOut->call_count, 1);

    // run loop async
    ASSERT_FALSE(e_l.isRunning());
    now = std::chrono::steady_clock::now();
    e_l.runLoopAsync();
    ASSERT_TRUE(e_l.isRunning());
    std::this_thread::sleep_until(now + std::chrono::seconds(1));
    e_l.stopLoop();
    ASSERT_FALSE(e_l.isRunning());

    // run loop with timeout
    now = std::chrono::steady_clock::now();
    e_l.runLoopAsync(std::chrono::seconds(1));
    e_l.waitForLoopEnd();
    time_lapse = std::chrono::steady_clock::now() - now;

    ASSERT_GE(time_lapse.count(), 1000000000);
    ASSERT_GE(std::chrono::duration_cast<std::chrono::milliseconds>(time_lapse).count(), bpy::extract<ulong>(*(clockOut->lastData)) + timestep.count());
    ASSERT_GE(bpy::extract<ulong>(*(clockOut->lastData)), bpy::extract<ulong>(*(iterOut->lastData)) * timestep.count());
}

// EOF
