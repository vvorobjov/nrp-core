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
