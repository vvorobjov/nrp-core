#!/bin/bash

set -ex

if [ -z "$NRP_INSTALL_DIR" ]; then
    echo "NRP_INSTALL_DIR is unset"
    exit 1
fi

if [ -z "$NRP_DEPS_INSTALL_DIR" ]; then
    echo "NRP_DEPS_INSTALL_DIR is unset"
    exit 1
fi

pushd /tmp

# MQTT Paho CPP
git clone https://github.com/eclipse/paho.mqtt.cpp.git
pushd paho.mqtt.cpp
git checkout v1.4.0
git submodule init
git submodule update
cmake -Bbuild -H. -DPAHO_BUILD_STATIC=OFF -DPAHO_BUILD_SHARED=ON -DCMAKE_INSTALL_PREFIX="${NRP_DEPS_INSTALL_DIR}" -DCMAKE_PREFIX_PATH="${NRP_DEPS_INSTALL_DIR}"  -DPAHO_WITH_MQTT_C=ON
cmake --build build/ --target install
sudo ldconfig
popd
rm -rf paho.mqtt.cpp

popd
