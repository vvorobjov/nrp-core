#!/usr/bin/env bash

# Get the root directory of the repo

repo_root=$(git rev-parse --show-toplevel)

# Create a build directory in the root directory

mkdir -p $repo_root/build
cd $repo_root/build

# Use gcc 10

export CC=/usr/bin/gcc-10
export CXX=/usr/bin/g++-10

# Run cmake and make

if [ -z ${NRP_INSTALL_DIR} ]; then 
    echo "NRP_INSTALL_DIR is unset"
    exit 1
fi

cmake .. -DCMAKE_INSTALL_PREFIX=$NRP_INSTALL_DIR
make -j
make install

# Run all tests. We assume that test executables are called *Tests
# On headless machines we need to run with X virtual frame buffer, otherwise some
# gazebo tests will fail

if [ -z ${DISPLAY} ]; then
    xvfb-run find . -name "*Tests" -exec {} --gtest_output=xml:$repo_root/build/xml/{}.xml \;
else
    find . -name "*Tests" -exec {} --gtest_output=xml:$repo_root/build/xml/{}.xml \;
fi

# EOF
