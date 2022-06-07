#!/usr/bin/env bash

# Get the root directory of the script

SCRIPT_DIR=$( cd -- "$( dirname -- "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )

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

# Check if NEST_INSTALL_DIR is set and the external nest-simulator can be used

[[ -z "$CMAKE_CACHE_FILE" ]] && CMAKE_CACHE_FILE="${repo_root}/.ci/cmake_cache/vanilla.cmake"

# Run cmake

cmake -C "$CMAKE_CACHE_FILE" -DCMAKE_INSTALL_PREFIX="$NRP_INSTALL_DIR" "${NEST_INSTALL_OPTION}" -Bbuild

# EOF
