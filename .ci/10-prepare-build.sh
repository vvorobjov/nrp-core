#!/usr/bin/env bash

# Get the root directory of the repo

repo_root=$(git rev-parse --show-toplevel)

# Create a build directory in the root directory

mkdir -p "$repo_root"/build
cd "$repo_root"/build || exit 1;

if [ -z "$NRP_INSTALL_DIR" ]; then
    echo "NRP_INSTALL_DIR is unset"
    exit 1
fi

# Set up the environment

source "$HOME"/.bashrc

# Install required python dependencies

python3 -m pip install --user grpcio-tools
python3 -m pip install --user pytest
python3 -m pip install --user flask==2.0

# Run cmake

cmake .. -DCMAKE_INSTALL_PREFIX="$NRP_INSTALL_DIR" -DCOVERAGE:BOOL=ON

# EOF
