#!/usr/bin/env bash

# Get the root directory of the script

SCRIPT_DIR=$( cd -- "$( dirname -- "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )
export ROOT_DIR=$( cd -- "$SCRIPT_DIR/.." &> /dev/null && pwd )

# Create a build directory in the root directory

cd "${SCRIPT_DIR}/.." || return
rm -rf build
mkdir -p build

if [ -z "$NRP_INSTALL_DIR" ]; then
    echo "NRP_INSTALL_DIR is unset"
    exit 1
fi

# Set up the environment

source "$HOME"/.bashrc

# ROS 2's setup.bash sets AMENT_PREFIX_PATH (honoured by ament_cmake
# Find*.cmake modules) but leaves CMAKE_PREFIX_PATH empty. Plain
# find_package calls (e.g. find_package(fastcdr) as needed by Humble's
# rclcpp transitive target) don't go through ament_cmake, so they
# need the ROS install prefix explicitly in CMAKE_PREFIX_PATH.
if [ -n "${AMENT_PREFIX_PATH:-}" ]; then
    export CMAKE_PREFIX_PATH="${CMAKE_PREFIX_PATH:+$CMAKE_PREFIX_PATH:}${AMENT_PREFIX_PATH}"
fi

# Check if NEST_INSTALL_DIR is set and the external nest-simulator can be used
[[ -z "$NEST_INSTALL_DIR" ]] && NEST_INSTALL_OPTION="" || NEST_INSTALL_OPTION="-DNEST_INSTALL_DIR=${NEST_INSTALL_DIR}"

# Check if CMAKE_CACHE_FILE is set
CMAKE_CACHE_FILE="${CMAKE_CACHE_FILE:-${ROOT_DIR}/.ci/cmake_cache/vanilla.cmake}"

# set NRP_DEPS_INSTALL_DIR to NRP_INSTALL_DIR if not defined
NRP_DEPS_INSTALL_DIR="${NRP_DEPS_INSTALL_DIR:-$NRP_INSTALL_DIR}"

# set NRP_CMAKE_PROJECT directory
NRP_CMAKE_PROJECT="${NRP_CMAKE_PROJECT:-${ROOT_DIR}}"

CMAKE_ARGS="${CMAKE_ARGS:--DCMAKE_BUILD_TYPE=Release}"

# Run cmake

cmake -C "$CMAKE_CACHE_FILE" ${CMAKE_ARGS} -DCMAKE_INSTALL_PREFIX="$NRP_INSTALL_DIR" -DNRP_DEP_CMAKE_INSTALL_PREFIX="${NRP_DEPS_INSTALL_DIR}" "${NEST_INSTALL_OPTION}" -Bbuild -S"$NRP_CMAKE_PROJECT"

# EOF
