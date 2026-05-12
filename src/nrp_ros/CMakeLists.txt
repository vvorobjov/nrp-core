cmake_minimum_required(VERSION 3.16)
project(NRPROS VERSION ${NRP_VERSION})

# ─────────────────────────────────────────────────────────────────────
# ROS 2 integration for nrp-core
#
# Build steps executed at cmake-configure time:
#   1. Resolve a colcon workspace path (ROS2_WS, defaults to ~/ros2_ws)
#      and create its src/ directory if needed.
#   2. Copy the in-tree nrp_ros_msgs package into that workspace.
#   3. Build it with `colcon build --packages-select nrp_ros_msgs`.
#   4. Expose the resulting install prefix via CMAKE_PREFIX_PATH so
#      downstream find_package(nrp_ros_msgs) succeeds.
#   5. Recurse into python_bindings/ to generate the Boost.Python
#      wrappers.
#
# This replaces the ROS 1 path, which copied the same package into a
# CATKIN_WS and ran `catkin_make` on it.
# ─────────────────────────────────────────────────────────────────────

message(STATUS "[NRPROS] ROS 2 integration: preparing colcon workspace for nrp_ros_msgs")

set(ROS2_WS $ENV{ROS2_WS})
if ("${ROS2_WS}" STREQUAL "")
    set(ROS2_WS "$ENV{HOME}/ros2_ws")
    message(STATUS "[NRPROS] ROS2_WS not set; defaulting to ${ROS2_WS}")
else()
    message(STATUS "[NRPROS] Using ROS2_WS=${ROS2_WS}")
endif()

file(MAKE_DIRECTORY "${ROS2_WS}/src")

# Copy nrp_ros_msgs into the colcon workspace src/ tree. We use
# execute_process rather than file(COPY) so the source tree is always
# overwritten, mirroring the ROS 1 behaviour.
execute_process(
    COMMAND cp -rf "${NRP_CORE_MSGS_PATH}/nrp_ros_msgs" "${ROS2_WS}/src/"
    RESULT_VARIABLE _cp_result
)
if(NOT _cp_result EQUAL 0)
    message(FATAL_ERROR "[NRPROS] Failed to copy nrp_ros_msgs into ${ROS2_WS}/src (cp exit=${_cp_result})")
endif()

# colcon requires /opt/ros/<distro>/setup.bash in the shell env, so we
# go through bash -lc; the ROS 2 distro is discovered from ROS_DISTRO /
# an /opt/ros/*/setup.bash, whichever is available.
message(STATUS "[NRPROS] Running colcon build for nrp_ros_msgs in ${ROS2_WS}")
execute_process(
    WORKING_DIRECTORY ${ROS2_WS}
    COMMAND bash -c "source /opt/ros/\${ROS_DISTRO:-foxy}/setup.bash && colcon build --packages-select nrp_ros_msgs"
    RESULT_VARIABLE _colcon_result
)
if(NOT _colcon_result EQUAL 0)
    message(FATAL_ERROR "[NRPROS] colcon build of nrp_ros_msgs failed (exit=${_colcon_result})")
endif()

# Expose the install space for downstream find_package(nrp_ros_msgs).
set(ENV{CMAKE_PREFIX_PATH} "${ROS2_WS}/install/nrp_ros_msgs:$ENV{CMAKE_PREFIX_PATH}")
message(STATUS "[NRPROS] CMAKE_PREFIX_PATH now includes ${ROS2_WS}/install/nrp_ros_msgs")

# Re-install the colcon-built nrp_ros_msgs tree into the NRP deps prefix
# (EBR2-72). Without this step the typesupport .so files live in
# ${ROS2_WS}/install/nrp_ros_msgs/lib/ — a path that is NOT on the final
# image's LD_LIBRARY_PATH (the Dockerfile only COPYs ${NRP_INSTALL_DIR}
# and ${NRP_DEPS_INSTALL_DIR} from the builder stage to the final stage).
# At runtime event_loop.so dlopens libnrp_ros_msgs__rosidl_typesupport_cpp.so
# and fails with "cannot open shared object file: No such file or directory",
# breaking 14+ ctest cases (RosMsgConverter.*, ComputationalGraphPythonNodes.*,
# EventLoop.EVENT_LOOP / EventLoopPython / EventLoopEnginePython,
# EngineScript / ServerCallbacks / PyServerGrpcCallbacks). Installing the
# whole colcon tree into ${NRP_DEP_CMAKE_INSTALL_PREFIX} puts lib/ on the
# image's LD_LIBRARY_PATH and share/nrp_ros_msgs/cmake/ on a path that
# find_package(nrp_ros_msgs) can pick up post-install too.
install(
    DIRECTORY "${ROS2_WS}/install/nrp_ros_msgs/"
    DESTINATION "${NRP_DEP_CMAKE_INSTALL_PREFIX}"
    USE_SOURCE_PERMISSIONS
)

##########################################
## Build nrp ros python bindings (Boost.Python)
add_subdirectory(python_bindings)