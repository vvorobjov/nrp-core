#!/bin/bash
export NRP_DEPS_INSTALL_DIR="/home/${USER}/.local/nrp_deps"
_PYVER=$(python3 -c "import sys; print(f'{sys.version_info.major}.{sys.version_info.minor}')")
export PYTHONPATH="${NRP_INSTALL_DIR}/lib/python${_PYVER}/site-packages:${NRP_DEPS_INSTALL_DIR}/lib/python${_PYVER}/site-packages:${PYTHONPATH}"
unset _PYVER
export LD_LIBRARY_PATH="${NRP_INSTALL_DIR}"/lib:"${NRP_DEPS_INSTALL_DIR}"/lib:${NRP_INSTALL_DIR}/lib/nrp_gazebo_plugins:$LD_LIBRARY_PATH
export PATH=$PATH:"${NRP_INSTALL_DIR}"/bin:"${NRP_DEPS_INSTALL_DIR}"/bin
export GAZEBO_PLUGIN_PATH=${NRP_INSTALL_DIR}/lib/nrp_gazebo_plugins:${GAZEBO_PLUGIN_PATH}
source /usr/share/gazebo-11/setup.sh

# ROS 2 — source whichever distribution is installed (foxy on focal,
# humble on jammy). The loop breaks on the first match.
export ROS2_WS=${HOME}/ros2_ws
for _ros_distro in humble foxy; do
    [ -f /opt/ros/${_ros_distro}/setup.bash ] && source /opt/ros/${_ros_distro}/setup.bash && break
done
unset _ros_distro
[ -f ${ROS2_WS}/install/setup.bash ] && source ${ROS2_WS}/install/setup.bash

. "${HOME}/.bashrc"

exec "$@"
