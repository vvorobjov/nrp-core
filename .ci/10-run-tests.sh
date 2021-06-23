#!/usr/bin/env bash

# Get the root directory of the repo

repo_root=$(git rev-parse --show-toplevel)

# Create a build directory in the root directory

cd "$repo_root"/build || exit 1;

# Run all tests. Save test results in build/xml directory (used by Jenkins).
# On headless machines we need to run with X virtual frame buffer.
# Otherwise some gazebo tests will fail

test_results_dir="${repo_root}/build/xml/"

mkdir -p "$test_results_dir"
export GTEST_OUTPUT="xml:$test_results_dir"

ctest_result=0
if [ -z "$DISPLAY" ]; then
    xvfb-run ctest
    ctest_result=$?
else
    ctest
    ctest_result=$?
fi

# ctest exit code is a bitmap. 0x8 means that some of the tests failed. We have to mask that bit.
# If this isn't done and there are any failing tests,
# Jenkins is going to treat the build as failed and will not process nor display the test results

if (((ctest_result & ~0x8) != 0)); then
    exit $ctest_result
fi

# EOF
