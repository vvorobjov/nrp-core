This README file contains information on how to get nrp-core installed in your system. Information on how to get started with nrp-core, architecture details and much more can be found at the nrp-core [online documentation](hbpneurorobotics.bitbucket.io)

**WARNING:** nrp-core is in alpha release state, use it at your own risk. Also notice that nrp-core has only been tested on Ubuntu 20.04 at the moment and this OS and version are assumed in instructions below. Installation in other environments might be possible but has not been tested yet.

## Dependency Installation

```
# Pistache REST Server
sudo add-apt-repository ppa:pistache+team/unstable
    
# Gazebo
sudo sh -c 'echo "deb http://packages.osrfoundation.org/gazebo/ubuntu-stable `lsb_release -cs` main" > /etc/apt/sources.list.d/gazebo-stable.list'
wget https://packages.osrfoundation.org/gazebo.key -O - | sudo apt-key add -
    
sudo apt update
sudo apt install git cmake libpistache-dev g++-10 libboost-python-dev libboost-filesystem-dev libboost-numpy-dev libcurl4-openssl-dev nlohmann-json3-dev libzip-dev cython3 python3-numpy libgrpc++-dev protobuf-compiler-grpc libprotobuf-dev doxygen libgsl-dev libopencv-dev python3-opencv python3-pil

# required by gazebo engine
sudo apt install libgazebo11-dev gazebo11 gazebo11-plugin-base

# required by nest-server (which is built and installed along with nrp-core)
sudo apt install python3-flask python3-flask-cors python3-restrictedpython uwsgi-core uwsgi-plugin-python3 

# required by nrp-server, which uses gRPC python bindings and mpi
pip install grpcio-tools pytest docopt mpi4py
    
# Fix deprecated type in OGRE (std::allocator<void>::const_pointer has been deprecated with glibc-10). Until the upstream libs are updated, use this workaround. It changes nothing, the types are the same
sudo sed -i "s/typename std::allocator<void>::const_pointer/const void*/g" /usr/include/OGRE/OgreMemorySTLAllocator.h
```

## Installation

```
git clone https://bitbucket.org/hbpneurorobotics/nrp-core.git
cd nrp-core
mkdir build
cd build
export CC=/usr/bin/gcc-10; export CXX=/usr/bin/g++-10
cmake .. -DCMAKE_INSTALL_PREFIX=/home/${USER}/.local/nrp
mkdir -p /home/${USER}/.local/nrp
# the installation process might take some time, as it downloads and compiles Nest as well. Also, Ubuntu has an outdated version of nlohman_json. CMake will download a newer version, which takes time as well
make
make install
# just in case of wanting to build the documentation. Documentation can then be found in a new doxygen folder
make doxygen 
```

## Running an experiment

 * Set environment:
 
 ```
export NRP=/home/${USER}/.local/nrp
export PYTHONPATH=${NRP}/lib/python3.8/site-packages:$PYTHONPATH
export LD_LIBRARY_PATH=${NRP}/lib:$LD_LIBRARY_PATH
export PATH=$PATH:${NRP}/bin
. /usr/share/gazebo-11/setup.sh
```
 * Start the simulation:
	`NRPSimulation -c <SIMULATION_CONFIG_FILE> -p <comma separated list of engine plugins>`

## Basic Information

 - The project is divided into multiple libraries, separated by folders:
	 - nrp_general_library: Main Library. Contains classes and methods to interface with Python, Engines, and Transceiver-Functions
	 - nrp_engine_protocols: Engine interfaces implementing server/client communication for different communication protocols
	 - nrp_nest_engines: Nest Engine
	 - nrp_gazebo_engines: Gazebo Engine
	 - nrp_python_json_engine: Python JSON Engine
	 - nrp_simulation: Contains the SimulationLoop and -Manager. Creates the NRPSimulation executable
 - Each of these folders also contains a 'tests' folder with basic integration testing capabilities. To run the tests, look for generated executables inside the build folder. Before running the tests, setup the environment as described above in **Running an experiment**
 - All libraries generate a python module. This can be used to interface with the devices from the TFs. After installation, they will be located inside `~/.local/nrp/lib/python3.8/site-packages`

## Examples

 - Examples are located in the examples subfolder:
	 - To run them, first set the environment as described in **Running an experiment**. Then:

			cd examples/<EXAMPLE_NAME>
			NRPSimulation -c <SIMULATION_CONFIG>
			
	 - If gazebo is running in the experiment, you can use `gzclient` to visualize the gazebo simulation

