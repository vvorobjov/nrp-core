#!/usr/bin/env bash

# Get the root directory of the repo

repo_root=$(git rev-parse --show-toplevel)

# Create a build directory in the root directory

cd "$repo_root"/build || exit 1;

make cppcheck-ci

# EOF
