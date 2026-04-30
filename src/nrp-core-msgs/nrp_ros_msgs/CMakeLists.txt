cmake_minimum_required(VERSION 3.5)
set(PROJECT_NAME "nrp_ros_msgs")
# Must enable both C and CXX: rosidl_generate_interfaces emits C
# targets for the typesupport / introspection layers; with CXX only,
# cmake fails with "Cannot determine link language for target
# nrp_ros_msgs__rosidl_typesupport_c__pyext".
project("${PROJECT_NAME}" VERSION 1.0 LANGUAGES C CXX)

# ─────────────────────────────────────────────────────────────────────
# ROS 2 message package for nrp-core (built via colcon)
#
# This file is built OUTSIDE the main NRPCore cmake tree by the colcon
# workspace set up in src/nrp_ros/CMakeLists.txt. It uses ament_cmake +
# rosidl_generate_interfaces to emit C++, Python, and typesupport
# artefacts for the messages in msg/.
# ─────────────────────────────────────────────────────────────────────

message(STATUS "[${PROJECT_NAME}] Configuring ROS 2 message package (ament_cmake + rosidl)")

find_package(ament_cmake REQUIRED)
find_package(rosidl_default_generators REQUIRED)
find_package(builtin_interfaces REQUIRED)
find_package(std_msgs REQUIRED)
find_package(geometry_msgs REQUIRED)

set(MSG_FILES
    "msg/Test.msg"
)

message(STATUS "[${PROJECT_NAME}] Generating ROS 2 interfaces: ${MSG_FILES}")

rosidl_generate_interfaces(${PROJECT_NAME}
    ${MSG_FILES}
    DEPENDENCIES
        builtin_interfaces
        std_msgs
        geometry_msgs
)

ament_export_dependencies(rosidl_default_runtime)
ament_package()
