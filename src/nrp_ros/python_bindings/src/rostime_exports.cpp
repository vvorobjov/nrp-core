/* * NRP Core - Backend infrastructure to synchronize simulations
 *
 * Copyright 2020-2023 NRP Team
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * This project has received funding from the European Union’s Horizon 2020
 * Framework Programme for Research and Innovation under the Specific Grant
 * Agreement No. 945539 (Human Brain Project SGA3).
 *
 * This file is a modified version of another one originally authored by:
 * Bhaskara Marthi with license BSD.
 * The original file can be found at: https://github.com/bhaskara/boost_python_ros
 */

#include <boost/python.hpp>
#include "builtin_interfaces/msg/time.hpp"
#include "builtin_interfaces/msg/duration.hpp"
#include "nrp_general_library/datapack_interface/datapack.h"


BOOST_PYTHON_MODULE(rostime_boost_python)
{
  // ROS 2: ros::Time / ros::Duration are replaced by
  // builtin_interfaces::msg::Time / builtin_interfaces::msg::Duration.
  // Field names also changed: sec/nsec -> sec/nanosec. The Python
  // attribute names follow the ROS 2 IDL convention.
  using namespace boost::python;
  using builtin_interfaces::msg::Time;
  using builtin_interfaces::msg::Duration;

  class_<Time, std::shared_ptr<Time> > ("Time", "ROS 2 builtin_interfaces/Time")
    .def_readwrite("sec", &Time::sec)
    .def_readwrite("nanosec", &Time::nanosec)
    ;

  DataPack<Time>::create_python("TimeDataPack");

  class_<Duration, std::shared_ptr<Duration> > ("Duration", "ROS 2 builtin_interfaces/Duration")
    .def_readwrite("sec", &Duration::sec)
    .def_readwrite("nanosec", &Duration::nanosec)
    ;

  DataPack<Duration>::create_python("DurationDataPack");
};
