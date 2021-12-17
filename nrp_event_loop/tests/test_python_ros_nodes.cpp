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

#include <signal.h>
#include <thread>

#include <gtest/gtest.h>
#include <nlohmann/json.hpp>

#include "ros/ros.h"
#include "nrp_ros_msgs/Test.h"
#include "nrp_ros_proxy/nrp_ros_proxy.h"

#include "nrp_event_loop/computational_graph/computational_node.h"
#include "nrp_event_loop/computational_graph/computational_graph_manager.h"

#include "nrp_event_loop/computational_graph/input_port.h"

#include "nrp_event_loop/config/cmake_constants.h"

#include "nrp_general_library/utils/utils.h"

namespace bpy = boost::python;

TEST(ComputationalGraphPythonNodes, ROS_NODES) {
    // Start roscore in a child process
    // TODO: find a better way to test these nodes without actually running roscore
    const auto pid = fork();
    if(pid == 0) {
        execlp("roscore", "roscore", (char*) NULL);
    }
    else {
        // Setup required elements
        namespace bpy = boost::python;
        ros::init(std::map<std::string, std::string>(), "nrp_core");
        NRPROSProxy::resetInstance();
        ComputationalGraphManager::resetInstance();
        Py_Initialize();
        PyImport_ImportModule(EVENT_LOOP_MODULE_NAME_STR);
        appendPythonPath(TEST_EVENT_LOOP_PYTHON_FUNCTIONS_MODULE_PATH);

        //// Test normal case
        // load and configure nodes
        try {
            bpy::import("test_ros_nodes");
        }
        catch (const boost::python::error_already_set &) {
            NRPLogger::error("Test failed when loading test_engine_nodes.py");
            PyErr_Print();
            boost::python::throw_error_already_set();
        }

        ComputationalGraphManager::getInstance().configure();

        // check results
        boost::shared_ptr<nrp_ros_msgs::Test const> msg_got;
        boost::function<void (const boost::shared_ptr<nrp_ros_msgs::Test const>&)> callback =
                [&](const boost::shared_ptr<nrp_ros_msgs::Test const>& a) { msg_got = std::move(a); };
        NRPROSProxy::getInstance().subscribe("/test_pub/test", callback);

        nrp_ros_msgs::Test msg_sent;
        msg_sent.string_msg = "hi";
        NRPROSProxy::getInstance().publish("/test_sub/test", msg_sent);
        // TODO: these sleep commands are required since msgs are actually sent and received through ROS.
        //  Find a way to mock the ROS side of this test
        sleep(1);
        ros::spinOnce();
        ComputationalGraphManager::getInstance().compute();
        sleep(1);
        ros::spinOnce();

        ASSERT_EQ(msg_got->string_msg, "hi");

        kill(pid,SIGTERM);
        int status;
        waitpid(pid, &status, 0);
    }

    // ROS decorator with an incorrect msg type
    ASSERT_THROW(bpy::import("test_ros_wrong_type"), boost::python::error_already_set);
}

// EOF
