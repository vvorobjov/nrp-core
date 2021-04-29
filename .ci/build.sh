#!/usr/bin/env bash

# Get the root directory of the repo

repo_root=$(git rev-parse --show-toplevel)

# Create a build directory in the root directory

mkdir -p "$repo_root"/build
cd "$repo_root"/build || exit 1;

# Use gcc 10

export CC=/usr/bin/gcc-10
export CXX=/usr/bin/g++-10

# Run cmake and make

if [ -z "$NRP_INSTALL_DIR" ]; then
    echo "NRP_INSTALL_DIR is unset"
    exit 1
fi

cmake .. -DCMAKE_INSTALL_PREFIX="$NRP_INSTALL_DIR"

NPROC=$(nproc)

# By default, limit jobs by 70% of cores,
# or up to 100% if we have few cores:
#    if 70% is less than 4, then use 4 parallel jobs
#    if the $NPROC is less than 4, use -j${NPROC}

J_LIM=$(python3 -c "print(max(${NPROC} * 7 // 10, min(${NPROC}, 4)))")

# Limit job scheduling by 100% of CPU load
# if 1min or 5min CPU load is higher than 100%, wait with job scheduling

L_LIM="${NPROC}"

echo "Running make with -j${J_LIM} -l${L_LIM}"
make -j"${J_LIM}" -l"${L_LIM}"
make install

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
