cmake_minimum_required(VERSION 3.16)
project(NRPROS VERSION ${NRP_VERSION})

##########################################
## Copy the nrp message definitions to a catkin workspace which path is specified by var CATKIN_WS
## and build that workspace
set(CATKIN_WS $ENV{CATKIN_WS})
if ("${CATKIN_WS}" STREQUAL "")
    message(STATUS "Setting catkin_ws path to: $ENV{HOME}/catkin_ws")
    set(CATKIN_WS $ENV{HOME}/catkin_ws)
endif()

if (NOT EXISTS ${CATKIN_WS})
    message(STATUS "Creating catkin_ws folder: ${CATKIN_WS}")
    file(MAKE_DIRECTORY ${CATKIN_WS})
    file(MAKE_DIRECTORY ${CATKIN_WS}/src)
endif()

execute_process(
        WORKING_DIRECTORY ${CATKIN_WS}
        COMMAND cp -rf ${CMAKE_CURRENT_SOURCE_DIR}/nrp_ros_msgs src
        COMMAND bash -c "cd ${CATKIN_WS} && catkin_make && source devel/setup.bash && rospack profile"
)

set(ENV{ROS_PACKAGE_PATH} "${CATKIN_WS}/src:$ENV{ROS_PACKAGE_PATH}")
set(ENV{CMAKE_PREFIX_PATH} "${CATKIN_WS}/devel:$ENV{CMAKE_PREFIX_PATH}")

##########################################
## Build nrp ros python bindings
add_subdirectory(python_bindings)