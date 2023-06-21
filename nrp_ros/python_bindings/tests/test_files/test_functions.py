#!/usr/bin/python3

# NRP Core - Backend infrastructure to synchronize simulations
#
# Copyright 2020-2023 NRP Team
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#   http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#
# This project has received funding from the European Union’s Horizon 2020
# Framework Programme for Research and Innovation under the Specific Grant
# Agreement No. 945539 (Human Brain Project SGA3).

import unittest
import numpy as np
import nrp_core.data.nrp_ros.rostime_boost_python as rbt
import nrp_core.data.nrp_ros.std_msgs as smb
import nrp_core.data.nrp_ros.geometry_msgs as gmb
import nrp_core.data.nrp_ros.nrp_ros_msgs as nrm



def test_input(input_rosmsg):

    # Create a test case object, so that we can use unittest assertions

    tc = unittest.TestCase()

    # Test that all basic types of objects were translated
    # correctly from test.msg into python objects


    tc.assertTrue       (input_rosmsg.bool_msg)

    tc.assertEqual      (input_rosmsg.int8_msg, 20)
    tc.assertEqual      (input_rosmsg.uint8_msg, 21)
    tc.assertEqual      (input_rosmsg.int16_msg, 22)
    tc.assertEqual      (input_rosmsg.uint16_msg, 23)
    tc.assertEqual      (input_rosmsg.int32_msg, 24)
    tc.assertEqual      (input_rosmsg.uint32_msg, 25)
    tc.assertEqual      (input_rosmsg.int64_msg, 26)
    tc.assertEqual      (input_rosmsg.uint64_msg, 27)

    tc.assertAlmostEqual(input_rosmsg.float32_msg, np.float32(100.13))
    tc.assertAlmostEqual(input_rosmsg.float64_msg, 56.56)
    tc.assertEqual      (input_rosmsg.string_msg, "Armageddon")

    tc.assertEqual      (input_rosmsg.byte_array_msg[0], 1)
    tc.assertEqual      (input_rosmsg.byte_array_msg[1], 0)
    tc.assertEqual      (input_rosmsg.byte_array_msg[2], 1)
    tc.assertEqual      (input_rosmsg.byte_array_msg[3], 0)

    tc.assertEqual      (input_rosmsg.float_array_msg[0], 1)
    tc.assertEqual      (input_rosmsg.float_array_msg[1], 0)
    tc.assertEqual      (input_rosmsg.float_array_msg[2], 2)

    duration = rbt.Duration()
    #duration.sec = 1
    tc.assertEqual      (input_rosmsg.duration_msg.secs, duration.secs)
    tc.assertEqual      (input_rosmsg.duration_msg.nsecs, duration.nsecs)

    time = rbt.Time()
    tc.assertEqual      (input_rosmsg.time_msg.secs, time.secs)
    tc.assertEqual      (input_rosmsg.time_msg.nsecs, time.nsecs)

    tc.assertEqual      (input_rosmsg.std_string_msg.data, "TestString")

    pose = gmb.Pose()
    tc.assertEqual      (input_rosmsg.pose_msg[0].position.x, pose.position.x)
    tc.assertEqual      (input_rosmsg.pose_msg[0].position.y, pose.position.x)
    tc.assertEqual      (input_rosmsg.pose_msg[0].position.z, pose.position.x)

    tc.assertEqual      (input_rosmsg.pose_msg[0].orientation.x, pose.orientation.x)
    tc.assertEqual      (input_rosmsg.pose_msg[0].orientation.y, pose.orientation.y)
    tc.assertEqual      (input_rosmsg.pose_msg[0].orientation.z, pose.orientation.z)
    tc.assertEqual      (input_rosmsg.pose_msg[0].orientation.w, pose.orientation.w)



def test_output():

    # Fill the test datapack with different types of data
    # The C++ caller code will test that they were translated
    # correctly from python objects into test.msg

    test_datapack = nrm.Test()

    # Standard python types

    test_datapack.bool_msg       = True
    test_datapack.int8_msg       = 1
    test_datapack.uint8_msg      = 2
    test_datapack.int16_msg      = 3
    test_datapack.uint16_msg     = 4
    test_datapack.int32_msg      = 5
    test_datapack.uint32_msg     = 6
    test_datapack.int64_msg      = 7
    test_datapack.uint64_msg     = 8

    test_datapack.float32_msg             = np.float32(43.21)
    test_datapack.float64_msg             = 41.21
    test_datapack.string_msg              = "string"
    test_datapack.std_string_msg.data     = "test"

    test_datapack.byte_array_msg.append(1)
    test_datapack.byte_array_msg.append(0)
    test_datapack.byte_array_msg.append(1)

    test_datapack.float_array_msg.append(4)
    test_datapack.float_array_msg.append(1)
    test_datapack.float_array_msg.append(6)

    #duration = rbt.Duration()
    test_datapack.duration_msg.secs = 60
    test_datapack.duration_msg.nsecs = 0

    #time = rbt.Time()
    test_datapack.time_msg.secs = 0
    test_datapack.time_msg.nsecs = 1000000

    pose = gmb.Pose()
    pose.position.x = 1
    pose.position.y = 2
    pose.position.z = 3

    pose.orientation.x = 1
    pose.orientation.y = 2
    pose.orientation.z = 3
    pose.orientation.w = 4

    test_datapack.pose_msg.append(pose)

    return test_datapack

def test_unsupported_not_default_constructor_duration(input_datapack):
    # Try to use not default constructor
    # Should throw. The exception should be caught by C++ part of the test

    input_datapack.data.duration_msg = rbt.Duration(60)

def test_unsupported_not_default_constructor_time(input_datapack):
    # Try to use not default constructor
    # Should throw. The exception should be caught by C++ part of the test

    input_datapack.data.time_msg = rbt.Time(1)





# EOF
