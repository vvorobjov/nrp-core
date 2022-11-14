#!/usr/bin/env bash

# Get the root directory of the script

SCRIPT_DIR=$( cd -- "$( dirname -- "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )

cd "$SCRIPT_DIR"/../build || exit 1;

# Set up the environment

source "$HOME"/.bashrc

make cppcheck-ci

# EOF
