#!/bin/bash
source ~/.bashrc
cd /home/${USER}
git clone https://bitbucket.org/hbpneurorobotics/nrp-core.git
cd nrp-core
git checkout event-loop-spinnaker
mkdir build
cd build
cmake .. -DCMAKE_INSTALL_PREFIX=/home/${USER}/.local/nrp -DENABLE_MQTT=OFF
mkdir -p /home/${USER}/.local/nrp
make
