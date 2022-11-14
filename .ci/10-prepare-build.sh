#!/usr/bin/env bash

# Get the root directory of the repo

repo_root=$(git rev-parse --show-toplevel)

# Create a build directory in the root directory

rm -rf "$repo_root"/build
mkdir -p "$repo_root"/build
cd "$repo_root"/build || exit 1;

if [ -z "$NRP_INSTALL_DIR" ]; then
    echo "NRP_INSTALL_DIR is unset"
    exit 1
fi

# Set up the environment

source "$HOME"/.bashrc

# Check if NEST_INSTALL_DIR is set and the external nest-simulator can be used
[[ -z "$NEST_INSTALL_DIR" ]] && NEST_INSTALL_OPTION="" || NEST_INSTALL_OPTION="-DNEST_INSTALL_DIR=${NEST_INSTALL_DIR}"

# set NRP_DEPS_INSTALL_DIR to NRP_INSTALL_DIR if not defined
NRP_DEPS_INSTALL_DIR="${NRP_DEPS_INSTALL_DIR:-$NRP_INSTALL_DIR}"

# Run cmake

cmake .. -DCMAKE_INSTALL_PREFIX="$NRP_INSTALL_DIR" -DNRP_DEP_CMAKE_INSTALL_PREFIX="${NRP_DEPS_INSTALL_DIR}" "${NEST_INSTALL_OPTION}" -DCOVERAGE:BOOL=ON -DBUILD_RST=ON -DGIT_SUBMODULE=ON

# EOF
