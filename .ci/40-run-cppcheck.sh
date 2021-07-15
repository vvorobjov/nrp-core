#!/usr/bin/env bash

# Get the root directory of the repo

repo_root=$(git rev-parse --show-toplevel)

cd "$repo_root"/build || exit 1;

# Set up the environment

source "$HOME"/.bashrc

make cppcheck-ci

# EOF
