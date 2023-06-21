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

#include "nrp_event_loop/nodes/ros/output_node.h"

#include "nrp_event_loop/config/cmake_constants.h"

#include "nrp_general_library/utils/utils.h"

namespace bpy = boost::python;

TEST(ComputationalGraphPythonNodes, ROS_NODES) {
    // Start roscore in a child process
    // TODO: find a better way to test these nodes without actually running roscore
    pid_t ppid_before_fork = getpid();
    const auto pid = fork();
    if(pid) {
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
        bpy::object test_module(bpy::import("test_ros_nodes"));
        bpy::dict test_module_dict(test_module.attr("__dict__"));

        ComputationalGraphManager::getInstance().configure();

        // check results
        boost::shared_ptr<nrp_ros_msgs::Test const> msg_got;
        boost::function<void (const boost::shared_ptr<nrp_ros_msgs::Test const>&)> callback =
                [&](const boost::shared_ptr<nrp_ros_msgs::Test const>& a) { msg_got = std::move(a); };
        NRPROSProxy::getInstance().subscribe("/test_pub/test", callback);

        boost::shared_ptr<nrp_ros_msgs::Test const> msg_got_2;
        boost::function<void (const boost::shared_ptr<nrp_ros_msgs::Test const>&)> callback_2 =
                [&](const boost::shared_ptr<nrp_ros_msgs::Test const>& a) { msg_got_2 = std::move(a); };
        NRPROSProxy::getInstance().subscribe("/test_pub/test_2", callback);

        nrp_ros_msgs::Test msg_sent;
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
        // TODO: these sleep commands are required since msgs are actually sent and received through ROS.
        //  Find a way to mock the ROS side of this test
        sleep(1);
        ros::spinOnce();
        ComputationalGraphManager::getInstance().compute();
        sleep(1);
        ros::spinOnce();
        std::string s = "second";

        ASSERT_EQ(msg_got->string_msg, "second");
        ASSERT_EQ(bpy::len(test_module_dict["msgs_all"]), 2);
        ASSERT_EQ(bpy::len(test_module_dict["msgs_clear"]), 2);

        // 'keep' policy
        msg_got.reset();
        ComputationalGraphManager::getInstance().compute();
        sleep(1);
        ros::spinOnce();
        ASSERT_EQ(msg_got->string_msg, "second");
        ASSERT_EQ(bpy::len(test_module_dict["msgs_all"]), 4);

        // 'clear' policy
        ASSERT_EQ(bpy::len(test_module_dict["msgs_clear"]), 2);

        kill(pid,SIGTERM);
        int status;
        waitpid(pid, &status, 0);

        // check compute period and publish from cache
        auto output_p = dynamic_cast<OutputROSNode<nrp_ros_msgs::Test>*>(ComputationalGraphManager::getInstance().getNode("/test_pub/test"));
        ASSERT_EQ(output_p->getComputePeriod(), 1);
        ASSERT_EQ(output_p->publishFromCache(), true);
        auto output_p2 = dynamic_cast<OutputROSNode<nrp_ros_msgs::Test>*>(ComputationalGraphManager::getInstance().getNode("/test_pub/test_2"));
        ASSERT_EQ(output_p2->getComputePeriod(), 2);
        ASSERT_EQ(output_p2->publishFromCache(), false);
    }
    else {
        int r = prctl(PR_SET_PDEATHSIG, SIGTERM);
        if (r == -1) { perror(0); exit(1); }
        if (getppid() != ppid_before_fork)
            exit(1);
        execlp("roscore", "roscore", (char*) NULL);
    }


    // ROS decorator with an incorrect msg type
    ASSERT_THROW(bpy::import("test_ros_wrong_type"), boost::python::error_already_set);
}

// EOF
