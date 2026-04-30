#!/bin/bash
export NRP_DEPS_INSTALL_DIR="/home/${USER}/.local/nrp_deps"
export PYTHONPATH="${NRP_INSTALL_DIR}"/lib/python3.8/site-packages:"${NRP_DEPS_INSTALL_DIR}"/lib/python3.8/site-packages:$PYTHONPATH
export LD_LIBRARY_PATH="${NRP_INSTALL_DIR}"/lib:"${NRP_DEPS_INSTALL_DIR}"/lib:${NRP_INSTALL_DIR}/lib/nrp_gazebo_plugins:$LD_LIBRARY_PATH
export PATH=$PATH:"${NRP_INSTALL_DIR}"/bin:"${NRP_DEPS_INSTALL_DIR}"/bin
export GAZEBO_PLUGIN_PATH=${NRP_INSTALL_DIR}/lib/nrp_gazebo_plugins:${GAZEBO_PLUGIN_PATH}
source /usr/share/gazebo-11/setup.sh

# ROS 2 Foxy (replaces ROS 1 Noetic + catkin_ws)
export ROS2_WS=${HOME}/ros2_ws
[ -f /opt/ros/foxy/setup.bash ] && source /opt/ros/foxy/setup.bash
[ -f ${ROS2_WS}/install/setup.bash ] && source ${ROS2_WS}/install/setup.bash

. "${HOME}/.bashrc"

exec "$@"
