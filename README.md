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
pip install grpcio-tools pytest psutil
   
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
 export LD_LIBRARY_PATH="${NRP_INSTALL_DIR}"/lib:"${NRP_DEPS_INSTALL_DIR}"/lib:$LD_LIBRARY_PATH
 export PATH=$PATH:"${NRP_INSTALL_DIR}"/bin:"${NRP_DEPS_INSTALL_DIR}"/bin
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


## Docker-compose

For a convenient usage of the Docker images containing the dependencies and the executables of the NRP-core and the simulators, the file `docker-compose.yaml` can be used. There are the hierarchy of the images, which are built with different Dockerfiles providing the different layers. The Dockerfiles are

 -  `base.Dockerfile` provides contains the basic environment setup, like user/group definitions, directory creations and some basic utilities, like wget, git etc. Nothing related to NRP Core nor simulators is installed here. It’s also possible to specify the base image, e.g. regular Ubuntu or Ubuntu+NVidia/CUDA;
 -  `nrp-core.Dockerfile` is a multi-stage Dockerfile, providing the NRP-core specific dependencies and compiling the NRP-core itself;
 -  `<simulator>.Dockerfile` contain environments and executables needed to run different simulators. Generally there is one dockerfile per simulator, i.e. gazebo, opensim, nest, etc. All simulator Dockerfiles should be based on the NRP base image. It’s also possible to chain together multiple simulator images to create an image with multiple simulators (e.g. gazebo + nest).

The hierarchical structure allows to compile the Dockerfiles into a Docker image with almost any combination of the environment and simulators. The pattern for the naming is the following

 -  `base-<...>` is an image built from `base.Dockerfile` (derived from some standard image, i.e. Ubuntu 20.04);
 -  `<simulator(s)>-env` is an image with the environment and the executables of the specified simulators, which is derived from some base image or other `<...>-env` image;
 -  `nrp-<simulators>` is an image with the NRP-core installed in the environment with the specified simulators.

### Variables
which can be exported before calling docker-compose

 -  `NRP_DOCKER_REGISTRY` specifies the registry address (in a from "example.com/", with slash)
 -  `NRP_CORE_TAG` specifies the image tag (in the form ":tag", otherwise latest is used)

### Parameters

 -  `BASE_IMAGE` the base image that is used in the `FROM` directive in the Dockerfile (used for the images hierarchy)
 -  `CMAKE_CACHE_FILE` defines the file with CMake parameters

### Usage hints

 -  without specifying `NRP_DOCKER_REGISTRY`, the images are build with the name "nrp-core/image-name";
 -  as a `NRP_DOCKER_REGISTRY` one can specify the user name at DockerHub (to be able to pull/push there) or private Docker registry;
 -  if `NRP_CORE_TAG` is not defined, the images are built with `latest` tag;
 -  in order to build the chain of images (the desired and all in the dependency), run `docker-compose up --build <service-name>`;
 -  in order to push/pull to a specific registry, export the NRP_DOCKER_REGISTRY before running docker-compose

    ```bash
    export NRP_DOCKER_REGISTRY=mydockerhub/
    docker-compose pull gazebo-env
    docker-compose build nrp-gazebo
    docker-compose push nrp-gazebo
    ```
