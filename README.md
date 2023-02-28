This README file contains information on how to get nrp-core installed in your system. Information on how to get started with nrp-core, architecture details, and much more can be found at the nrp-core [online documentation](hbpneurorobotics.bitbucket.io)

**WARNING:** nrp-core has only been tested on Ubuntu 20.04 at the moment and this OS and version are assumed in the instructions below. Installation in other environments might be possible but has not been tested yet.

 * Before starting the installation, define, please, the nrp-core installation directory:
 
 ```
export NRP_INSTALL_DIR="/home/${USER}/.local/nrp"
export NRP_DEPS_INSTALL_DIR="/home/${USER}/.local/nrp_deps"
 ```

## Dependency Installation

```bash
# Start of dependencies installation
# Pistache REST Server
sudo add-apt-repository ppa:pistache+team/unstable
    
# Gazebo repository
sudo sh -c 'echo "deb http://packages.osrfoundation.org/gazebo/ubuntu-stable `lsb_release -cs` main" > /etc/apt/sources.list.d/gazebo-stable.list'
wget https://packages.osrfoundation.org/gazebo.key -O - | sudo apt-key add -
    
sudo apt update
sudo apt install git cmake libpistache-dev libboost-python-dev libboost-filesystem-dev libboost-numpy-dev libcurl4-openssl-dev nlohmann-json3-dev libzip-dev cython3 python3-numpy libgrpc++-dev protobuf-compiler-grpc libprotobuf-dev doxygen libgsl-dev libopencv-dev python3-opencv python3-pil python3-pip libgmock-dev

# required by gazebo engine
sudo apt install libgazebo11-dev gazebo11 gazebo11-plugin-base

# Remove flask if it was installed to ensure it is installed from pip
sudo apt remove python3-flask python3-flask-cors
# required by Python engine
# If you are planning to use The Virtual Brain framework, you will most likely have to use flask version 1.1.4.
# By installing flask version 1.1.4 markupsafe library (included with flask) has to be downgraded to version 2.0.1 to run properly with gunicorn
# You can install that version with 
# pip install flask==1.1.4 gunicorn markupsafe==2.0.1
pip install flask gunicorn

# required by nest-server (which is built and installed along with nrp-core)
sudo apt install python3-restrictedpython uwsgi-core uwsgi-plugin-python3 
pip install flask_cors mpi4py docopt

# required by nrp-server, which uses gRPC python bindings
pip install grpcio-tools pytest psutil docker

# Required for using docker with ssh
pip install paramiko
   
# ROS

# Install ROS: follow the installation instructions: http://wiki.ros.org/noetic Installation/Ubuntu. To enable ros support in nrp on `ros-noetic-ros-base` is required.

#Tell nrp-core where your catkin workspace is located: export a variable CATKIN_WS pointing to an existing catkin workspace root folder. If the variable does not exist, a new catkin workspace will be created at `${HOME}/catkin_ws`.

# SpiNNaker
# Follow the instructions at: https://spinnakermanchester.github.io/development/gitinstall.html.
# Ensure that if using a virtualenv, this is active when running any SpiNNaker scripts.

# MQTT Paho library, required by datatransfer engine for streaming data over network
# More information on the project web site https://github.com/eclipse/paho.mqtt.cpp
# If you do not want to add network data streaming feature, you can skip this step.
# MQTT Paho C library
git clone https://github.com/eclipse/paho.mqtt.c.git
cd paho.mqtt.c
git checkout v1.3.8
cmake -Bbuild -H. -DPAHO_ENABLE_TESTING=OFF -DPAHO_BUILD_STATIC=OFF -DPAHO_BUILD_SHARED=ON -DPAHO_WITH_SSL=ON -DPAHO_HIGH_PERFORMANCE=ON -DCMAKE_INSTALL_PREFIX="${NRP_DEPS_INSTALL_DIR}"
cmake --build build/ --target install
sudo ldconfig && cd ..

# MQTT Paho CPP
git clone https://github.com/eclipse/paho.mqtt.cpp
cd paho.mqtt.cpp
git checkout v1.2.0
cmake -Bbuild -H. -DPAHO_BUILD_STATIC=OFF -DPAHO_BUILD_SHARED=ON -DCMAKE_INSTALL_PREFIX="${NRP_DEPS_INSTALL_DIR}" -DCMAKE_PREFIX_PATH="${NRP_DEPS_INSTALL_DIR}"
cmake --build build/ --target install
sudo ldconfig && cd ..

# End of dependencies installation
```

## Installation

```bash
# Start of installation
git clone https://bitbucket.org/hbpneurorobotics/nrp-core.git
cd nrp-core
mkdir build
cd build
# See the section "Common NRP-core CMake options" in the documentation for the additional ways to configure the project with CMake
cmake .. -DCMAKE_INSTALL_PREFIX="${NRP_INSTALL_DIR}" -DNRP_DEP_CMAKE_INSTALL_PREFIX="${NRP_DEPS_INSTALL_DIR}"
mkdir -p "${NRP_INSTALL_DIR}"
# the installation process might take some time, as it downloads and compiles Nest as well.
# If you haven't installed MQTT libraries, add ENABLE_MQTT=OFF definition to cmake (-DENABLE_MQTT=OFF).
make
make install
# just in case of wanting to build the documentation. Documentation can then be found in a new doxygen folder
make nrp_doxygen

# End of installation
```

## Running an experiment

 * Set environment:
 
 ```bash
 # Start of setting environment
 export NRP_INSTALL_DIR="/home/${USER}/.local/nrp" # The installation directory, which was given before
 export NRP_DEPS_INSTALL_DIR="/home/${USER}/.local/nrp_deps"
 export PYTHONPATH="${NRP_INSTALL_DIR}"/lib/python3.8/site-packages:"${NRP_DEPS_INSTALL_DIR}"/lib/python3.8/site-packages:$PYTHONPATH
 export LD_LIBRARY_PATH="${NRP_INSTALL_DIR}"/lib:"${NRP_DEPS_INSTALL_DIR}"/lib:${NRP_INSTALL_DIR}/lib/nrp_gazebo_plugins:$LD_LIBRARY_PATH
 export PATH=$PATH:"${NRP_INSTALL_DIR}"/bin:"${NRP_DEPS_INSTALL_DIR}"/bin
 export GAZEBO_PLUGIN_PATH=${NRP_INSTALL_DIR}/lib/nrp_gazebo_plugins:${GAZEBO_PLUGIN_PATH}
 . /usr/share/gazebo-11/setup.sh
 . /opt/ros/noetic/setup.bash
 . ${CATKIN_WS}/devel/setup.bash

 # End of setting environment
 ```


 * Start the simulation:

    `NRPCoreSim -c <SIMULATION_CONFIG_FILE>`

## Basic Information

 - The project is divided into multiple libraries, separated by folders:
     - nrp_general_library: Main Library. Contains classes and methods to interface with Python, Engines, and Transceiver-Functions
     - nrp_engine_protocols: Engine interfaces implementing server/client communication for different communication protocols
     - nrp_nest_engines: Nest Engine
     - nrp_gazebo_engines: Gazebo Engine
     - nrp_python_json_engine: Python JSON Engine
     - nrp_simulation: Contains the FTILoop and -Manager. Creates the NRPCoreSim executable
 - Each of these folders also contains a 'tests' folder with basic integration testing capabilities. To run the tests, look for generated executables inside the build folder. Before running the tests, setup the environment as described above in **Running an experiment**
 - All libraries generate a python module. This can be used to interface with the datapacks from the TFs. After installation, they will be located inside `${NRP_INSTALL_DIR}/lib/python3.8/site-packages`

## Examples

 - Examples are located in the examples subfolder:
     - To run them, first set the environment as described in **Running an experiment**. Then:

            cd examples/<EXAMPLE_NAME>
            NRPCoreSim -c <SIMULATION_CONFIG>
            
     - If gazebo is running in the experiment, you can use `gzclient` to visualize the gazebo simulation

## Acknowledgments

This repository is part of the Neurorobotics Platform software
Copyright (C) Human Brain Project
https://neurorobotics.ai

The Human Brain Project is a European Commission funded project
in the frame of the [Horizon2020 FET Flagship plan](http://ec.europa.eu/programmes/horizon2020/en/h2020-section/fet-flagships).

This work has received funding from the European Union’s Horizon 2020 Framework Programme for Research and Innovation under the Specific Grant Agreement No. 720270 (Human Brain Project SGA1), and the Specific Grant Agreement No. 785907 (Human Brain Project SGA2), and under the Specific Grant Agreement No. 945539 (Human Brain Project SGA3).
