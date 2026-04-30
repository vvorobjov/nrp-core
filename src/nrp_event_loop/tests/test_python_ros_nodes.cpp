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

// ─────────────────────────────────────────────────────────────────────
// ROS 2 port of the ROS Python-nodes integration test.
//
// Under ROS 1 this test fork()ed a `roscore` child process to act as a
// broker. ROS 2 is brokerless (DDS) — a rclcpp::init() is enough to
// publish/subscribe intra-process, so the fork is gone.
// ─────────────────────────────────────────────────────────────────────

#include <memory>
#include <thread>

#include <gtest/gtest.h>
#include <nlohmann/json.hpp>

#include "rclcpp/rclcpp.hpp"
#include "nrp_ros_msgs/msg/test.hpp"
#include "nrp_ros_proxy/nrp_ros_proxy.h"

#include "nrp_event_loop/computational_graph/computational_node.h"
#include "nrp_event_loop/computational_graph/computational_graph_manager.h"

#include "nrp_event_loop/computational_graph/input_port.h"

#include "nrp_event_loop/nodes/ros/output_node.h"

#include "nrp_event_loop/config/cmake_constants.h"

#include "nrp_general_library/utils/utils.h"

namespace bpy = boost::python;
using TestMsg = nrp_ros_msgs::msg::Test;

TEST(ComputationalGraphPythonNodes, ROS_NODES) {
    // ROS 2 is brokerless (DDS): rclcpp::init() is all we need.
    if(!rclcpp::ok())
        rclcpp::init(0, nullptr);
    NRPROSProxy::resetInstance();
    ComputationalGraphManager::resetInstance();
    Py_Initialize();
    PyImport_ImportModule(EVENT_LOOP_MODULE_NAME_STR);
    appendPythonPath(TEST_EVENT_LOOP_PYTHON_FUNCTIONS_MODULE_PATH);

    //// Test normal case
    // load and configure nodes
    bpy::object test_module(bpy::import("test_ros_nodes"));
    bpy::dict test_module_dict(test_module.attr("__dict__"));

    ComputationalGraphManager::getInstance().graphLoadComplete();
    ComputationalGraphManager::getInstance().configure();

    // Subscribe from the test directly to observe what the graph publishes.
    std::shared_ptr<const TestMsg> msg_got;
    std::function<void(std::shared_ptr<const TestMsg>)> callback =
            [&](std::shared_ptr<const TestMsg> a) { msg_got = std::move(a); };
    NRPROSProxy::getInstance().subscribe<TestMsg>("/test_pub/test", callback);

    std::shared_ptr<const TestMsg> msg_got_2;
    std::function<void(std::shared_ptr<const TestMsg>)> callback_2 =
            [&](std::shared_ptr<const TestMsg> a) { msg_got_2 = std::move(a); };
    NRPROSProxy::getInstance().subscribe<TestMsg>("/test_pub/test_2", callback_2);

    TestMsg msg_sent;
    msg_sent.string_msg = "first";
    NRPROSProxy::getInstance().publish("/test_sub/test", msg_sent);
    NRPROSProxy::getInstance().publish("/test_sub/test_all", msg_sent);
    NRPROSProxy::getInstance().publish("/test_sub/test_clear", msg_sent);
    NRPROSProxy::getInstance().publish("/test_sub/test_clear_all", msg_sent);
    msg_sent.string_msg = "second";
    NRPROSProxy::getInstance().publish("/test_sub/test", msg_sent);
    NRPROSProxy::getInstance().publish("/test_sub/test_all", msg_sent);
    NRPROSProxy::getInstance().publish("/test_sub/test_clear", msg_sent);
    NRPROSProxy::getInstance().publish("/test_sub/test_clear_all", msg_sent);
    // DDS still needs a brief moment for subscriptions to be discovered
    // and for the publish -> subscribe transport to drain.
    sleep(1);
    NRPROSProxy::getInstance().spinSome();
    ComputationalGraphManager::getInstance().compute();
    sleep(1);
    NRPROSProxy::getInstance().spinSome();

    ASSERT_EQ(msg_got->string_msg, "second");
    ASSERT_EQ(bpy::len(test_module_dict["msgs_all"]), 2);
    ASSERT_EQ(bpy::len(test_module_dict["msgs_clear"]), 2);

    // 'keep' policy
    msg_got.reset();
    ComputationalGraphManager::getInstance().compute();
    sleep(1);
    NRPROSProxy::getInstance().spinSome();
    ASSERT_EQ(msg_got->string_msg, "second");
    ASSERT_EQ(bpy::len(test_module_dict["msgs_all"]), 4);

    // 'clear' policy
    ASSERT_EQ(bpy::len(test_module_dict["msgs_clear"]), 2);

    // check compute period and publish from cache
    auto output_p = dynamic_cast<OutputROSNode<TestMsg>*>(ComputationalGraphManager::getInstance().getNode("/test_pub/test"));
    ASSERT_EQ(output_p->getComputePeriod(), 1);
    ASSERT_EQ(output_p->publishFromCache(), true);
    auto output_p2 = dynamic_cast<OutputROSNode<TestMsg>*>(ComputationalGraphManager::getInstance().getNode("/test_pub/test_2"));
    ASSERT_EQ(output_p2->getComputePeriod(), 2);
    ASSERT_EQ(output_p2->publishFromCache(), false);

    // ROS decorator with an incorrect msg type
    ASSERT_THROW(bpy::import("test_ros_wrong_type"), boost::python::error_already_set);

    if(rclcpp::ok())
        rclcpp::shutdown();
}

// EOF
