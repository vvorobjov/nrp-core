## NRPCore 1

## NRPCore 1.0.0 (2021-10-9)

1. First version of NRPCore, providing a framework, tools and a standalone application for executing supported simulators synchronously with exchange and transformation of data between the simulations via transceiver functions and datapacks. NRPCore provides simulator abstraction in the form of an Engine client/server interface for convinently integrating new simulators. NRPCore 1.0.0 was shipped with Engine implementations for Gazebo, NEST and arbitrary time-step simulators with a Python interface.

## NRPCore 1.1.0 (2021-12-24)

1. First implementation of the EventLoop in NRPCore, providing support for asynchronous communication between processes in NRPCore. First implementation of the ComputationalGraph as a generalization of Transceiver Functions with support for synchronous, asynchronous and mixed communication between Engines and other processes in NRPCore experiments. Implementation of Input and Output nodes in the ComputationalGraph for interacting with ROS nodes and NRPCore Engines
    * [Pull request #59](https://bitbucket.org/hbpneurorobotics/nrp-core/pull-requests/59)
    * [Pull request #62](https://bitbucket.org/hbpneurorobotics/nrp-core/pull-requests/62)
    
2. Implemented ROS msgs Python bindings, thus giving them support in Transceiver Functions and ComputationalGraph nodes
    * [Pull request #52](https://bitbucket.org/hbpneurorobotics/nrp-core/pull-requests/52)

3. gtest coverage
    * [Pull request #53](https://bitbucket.org/hbpneurorobotics/nrp-core/pull-requests/53)
    
4. Added reset hook to pythonjsonengine
    * [Pull request #55](https://bitbucket.org/hbpneurorobotics/nrp-core/pull-requests/55)

5. Improve ENABLE_SIMULATORS build configuration feature
    * [Pull request #56](https://bitbucket.org/hbpneurorobotics/nrp-core/pull-requests/56)
    
6. Fix typos in documentation
    * [Pull request #54](https://bitbucket.org/hbpneurorobotics/nrp-core/pull-requests/54)
    
7. Fix bug with xvfb support
    * [Pull request #60](https://bitbucket.org/hbpneurorobotics/nrp-core/pull-requests/60)
    
    
## NRPCore 1.2.0 (2022-05-13)

1. Enable setting node policies from decorators in Python FunctionalNode (ComputationalGraph) 
    * [Pull request #67](https://bitbucket.org/hbpneurorobotics/nrp-core/pull-requests/67)
    
2. Fix bug in mapping input ports to function arguments in Python FunctionalNode
    * [Pull request #69](https://bitbucket.org/hbpneurorobotics/nrp-core/pull-requests/69)

3. data logging and streaming engine
    * [Pull request #66](https://bitbucket.org/hbpneurorobotics/nrp-core/pull-requests/66)
    * [Pull request #70](https://bitbucket.org/hbpneurorobotics/nrp-core/pull-requests/70)
    * [Pull request #90](https://bitbucket.org/hbpneurorobotics/nrp-core/pull-requests/90)
    * [Pull request #93](https://bitbucket.org/hbpneurorobotics/nrp-core/pull-requests/93)

4. Give support for also launching processes which are not engines using ProcessLauncher and specifying these in experiment configuration files
    * [Pull request #75](https://bitbucket.org/hbpneurorobotics/nrp-core/pull-requests/75)
    
5. Spinnaker integration in the EventLoop/ComputationalGraph: implemented Spinnaker input and output nodes
    * [Pull request #74](https://bitbucket.org/hbpneurorobotics/nrp-core/pull-requests/74)
    
6. Remove the need for manually specifying which plugins will be loaded when executing NRPCoreSim
    * [Pull request #79](https://bitbucket.org/hbpneurorobotics/nrp-core/pull-requests/79)
    
7. New, Python-based implementation of the PythonJSONEngine
    * [Pull request #73](https://bitbucket.org/hbpneurorobotics/nrp-core/pull-requests/73)
    
8. Refactored NRPCore GRPC services
    * [Pull request #77](https://bitbucket.org/hbpneurorobotics/nrp-core/pull-requests/77)
    
9. Added missing tests for different components
    * [Pull request #61](https://bitbucket.org/hbpneurorobotics/nrp-core/pull-requests/61)
    * [Pull request #83](https://bitbucket.org/hbpneurorobotics/nrp-core/pull-requests/83)

10. Added support for generated sphinx documentation
    * [Pull request #76](https://bitbucket.org/hbpneurorobotics/nrp-core/pull-requests/76)
    * [Pull request #87](https://bitbucket.org/hbpneurorobotics/nrp-core/pull-requests/87)
    
11. MQTT integration in the EventLoop/ComputationalGraph: implemented MQTT input and output nodes
    * [Pull request #78](https://bitbucket.org/hbpneurorobotics/nrp-core/pull-requests/78)
    
12. Fix linking of gmock and gtest
    * [Pull request #88](https://bitbucket.org/hbpneurorobotics/nrp-core/pull-requests/88)
    * [Pull request #106](https://bitbucket.org/hbpneurorobotics/nrp-core/pull-requests/106)
    
13. Refactored examples
    * [Pull request #91](https://bitbucket.org/hbpneurorobotics/nrp-core/pull-requests/91)
    
14. NRPCoreSim CLI for overriding the configuration file parameters
    * [Pull request #92](https://bitbucket.org/hbpneurorobotics/nrp-core/pull-requests/92)
    
15. Rename rest routes in JSON registration server
    * [Pull request #82](https://bitbucket.org/hbpneurorobotics/nrp-core/pull-requests/82)

16. Using gunicore in PythonJSONEngine
    * [Pull request #101](https://bitbucket.org/hbpneurorobotics/nrp-core/pull-requests/101)
    
17. Refactoring of the integration of Python based simulators through Pysim Engine. Added support for OpenAI and Mujoco to the existing OpenSim
    * [Pull request #80](https://bitbucket.org/hbpneurorobotics/nrp-core/pull-requests/80)
    
18. Refactoring of the SimulationManager to properly and symmetrically integrate FTILoop and EventLoop. Added an explicit experiment lifecycle FSM to SimulationManager to make more transparent and logical the management of the possible states and transitions in the experiment lifecycle. Refactoring of NRPServer and NRPCore Python client to increase robustness and clarity when processing requests from client
    * [Pull request #99](https://bitbucket.org/hbpneurorobotics/nrp-core/pull-requests/99)

19. Added example of how to use TVB in NRPCore experiments
    * [Pull request #100](https://bitbucket.org/hbpneurorobotics/nrp-core/pull-requests/100)
    
20. Fix several bugs in NRPCore shutdown process
    * [Pull request #104](https://bitbucket.org/hbpneurorobotics/nrp-core/pull-requests/104)
    
    
## NRPCore 1.3.0 (2022-11-14)

1. Added support for launching NRPCore experiments and Engines in Docker containers. A new DockerLauncher has been implemented which allows to launch Engine servers in Docker containers. NRPCore Python client has been extended to allow to launch NRPCore experiments in Docker containers
    * [Pull request #71](https://bitbucket.org/hbpneurorobotics/nrp-core/pull-requests/71)
    * [Pull request #115](https://bitbucket.org/hbpneurorobotics/nrp-core/pull-requests/115)
    * [Pull request #113](https://bitbucket.org/hbpneurorobotics/nrp-core/pull-requests/113)
    * [Pull request #126](https://bitbucket.org/hbpneurorobotics/nrp-core/pull-requests/126)
    * [Pull request #71](https://bitbucket.org/hbpneurorobotics/nrp-core/pull-requests/71)

2. Introduced Modular Dockerfiles for NRPCore. A set of Dockerfiles were added which allows to compose NRPCore images in a modular way with different combinations of Engines and theirs respective dependencies
    * [Pull request #111](https://bitbucket.org/hbpneurorobotics/nrp-core/pull-requests/111)
    * [Pull request #116](https://bitbucket.org/hbpneurorobotics/nrp-core/pull-requests/116)
    * [Pull request #118](https://bitbucket.org/hbpneurorobotics/nrp-core/pull-requests/118)

3. Several improvements around the Datatransfer Engine
    * [Pull request #67](https://bitbucket.org/hbpneurorobotics/nrp-core/pull-requests/67)
    * [Pull request #131](https://bitbucket.org/hbpneurorobotics/nrp-core/pull-requests/131)
    * [Pull request #141](https://bitbucket.org/hbpneurorobotics/nrp-core/pull-requests/141)
    * [Pull request #129](https://bitbucket.org/hbpneurorobotics/nrp-core/pull-requests/129)

4. NRPCore msg definitions were moved to a separate repository and used as a submodule
    * [Pull request #110](https://bitbucket.org/hbpneurorobotics/nrp-core/pull-requests/110)

5. Added a generic, GRPC based Unity Engine client which allows to connect to Engine servers running within Unity 3D apps
    * [Pull request #112](https://bitbucket.org/hbpneurorobotics/nrp-core/pull-requests/112)

6. Added support for data exchange between the NRPCore Python client and Engines running in an experiment. This feature enables sending actions and retrieving observations at runtime, thus adding support for executing Reinforcement Learning algorithms with NRPCore using the Python client
    * [Pull request #123](https://bitbucket.org/hbpneurorobotics/nrp-core/pull-requests/123)
    * [Pull request #119](https://bitbucket.org/hbpneurorobotics/nrp-core/pull-requests/119)
    * [Pull request #127](https://bitbucket.org/hbpneurorobotics/nrp-core/pull-requests/127)
    * [Pull request #134](https://bitbucket.org/hbpneurorobotics/nrp-core/pull-requests/134)

7. Several fixes and improvements related to Engine servers
    * [Pull request #128](https://bitbucket.org/hbpneurorobotics/nrp-core/pull-requests/128)
    * [Pull request #130](https://bitbucket.org/hbpneurorobotics/nrp-core/pull-requests/130)
    * [Pull request #136](https://bitbucket.org/hbpneurorobotics/nrp-core/pull-requests/136)
    * [Pull request #135](https://bitbucket.org/hbpneurorobotics/nrp-core/pull-requests/135)

8. All example experiments are run as part of the NRPCore test suite
    * [Pull request #124](https://bitbucket.org/hbpneurorobotics/nrp-core/pull-requests/124)

9. Add support for declaring multiple datapacks from a single decorator in TFs
    * [Pull request #114](https://bitbucket.org/hbpneurorobotics/nrp-core/pull-requests/114)
    
    
## NRPCore 1.3.1 (2023-02-28)

1. [Pull request #167](https://bitbucket.org/hbpneurorobotics/nrp-core/pull-requests/167) Compatibility with the other NRP components for the v4.0.
    * Unmasked signals in engine servers launched by BasicFork. SIGTERM (shutdown command), SIGINT, SIGHUP (parent has died) are expected to be received so be sure they are unblocked. Increased forked processes polling time to 500ms.
    * The template experiments are added: examples/templates.
    * Renamed configuration attribute from "SimDescription" to "SimulationDescripion" in experiments.
    * The templates are added to the NRP-Core Docker images.

## NRPCore 1.4.0 (2023-06-16)

1. Bug fixes:
    * [Pull request #148](https://bitbucket.org/hbpneurorobotics/nrp-core/pull-requests/148)
    * [Pull request #150](https://bitbucket.org/hbpneurorobotics/nrp-core/pull-requests/150)
    * [Pull request #152](https://bitbucket.org/hbpneurorobotics/nrp-core/pull-requests/152)
    * [Pull request #153](https://bitbucket.org/hbpneurorobotics/nrp-core/pull-requests/153)
    * [Pull request #157](https://bitbucket.org/hbpneurorobotics/nrp-core/pull-requests/157)
    * [Pull request #158](https://bitbucket.org/hbpneurorobotics/nrp-core/pull-requests/158)
    * [Pull request #133](https://bitbucket.org/hbpneurorobotics/nrp-core/pull-requests/133)
    * [Pull request #173](https://bitbucket.org/hbpneurorobotics/nrp-core/pull-requests/173)
    * [Pull request #175](https://bitbucket.org/hbpneurorobotics/nrp-core/pull-requests/175)
    * [Pull request #188](https://bitbucket.org/hbpneurorobotics/nrp-core/pull-requests/188)
    * [Pull request #185](https://bitbucket.org/hbpneurorobotics/nrp-core/pull-requests/185)
    * [Pull request #186](https://bitbucket.org/hbpneurorobotics/nrp-core/pull-requests/186)
    * [Pull request #181](https://bitbucket.org/hbpneurorobotics/nrp-core/pull-requests/181)
    * [Pull request #191](https://bitbucket.org/hbpneurorobotics/nrp-core/pull-requests/191)

2. Completed support in NRP-Core for the execution of experiments under soft real-time constraints with the possibility of running Engines in real-time and with asynchronous communications using the EventLoop:
    * [Pull request #184](https://bitbucket.org/hbpneurorobotics/nrp-core/pull-requests/184)

3. Added support, guide and example on how to vectorize NRP-Core experiments using openai gym API:
    * [Pull request #182](https://bitbucket.org/hbpneurorobotics/nrp-core/pull-requests/182)
    * [Pull request #187](https://bitbucket.org/hbpneurorobotics/nrp-core/pull-requests/187)

4. Added support to distribute the execution of NRP-Core experiments using the Python client and docker compose
    * [Pull request #154](https://bitbucket.org/hbpneurorobotics/nrp-core/pull-requests/154)
    * [Pull request #189](https://bitbucket.org/hbpneurorobotics/nrp-core/pull-requests/189)

5. Implemented a Python gRPC Engine, similar in structure to the already existing Python JSON Engine but it exchanges Protobuf messages over gRPC, thus improving considerably the Engine performance:
    * [Pull request #132](https://bitbucket.org/hbpneurorobotics/nrp-core/pull-requests/132)

6. Added a command line tool to compile protobuf message files into c++ modules which can be dynamically loaded by Engines to extend the type of protobuf messages they can exchange without the need to recompile NRP-Core. It is particularly useful when used in combination with the Python GRPC Engine:
    * [Pull request #146](https://bitbucket.org/hbpneurorobotics/nrp-core/pull-requests/146)
    * [Pull request #159](https://bitbucket.org/hbpneurorobotics/nrp-core/pull-requests/159)
    * [Pull request #161](https://bitbucket.org/hbpneurorobotics/nrp-core/pull-requests/161)
    * [Pull request #180](https://bitbucket.org/hbpneurorobotics/nrp-core/pull-requests/180)

7. Removed SimulationTimestep parameter from the experiment configuration. This parameter is computed now as the minimum value in the EngineTimestep values of the engines included in the experiment:
    * [Pull request #176](https://bitbucket.org/hbpneurorobotics/nrp-core/pull-requests/176)

8. Added support for using precompiled C++ Functional Nodes in the Computational Graph, along with a tool to compile C++ modules from the node function definitions source code:
    * [Pull request #169](https://bitbucket.org/hbpneurorobotics/nrp-core/pull-requests/169)
    * [Pull request #160](https://bitbucket.org/hbpneurorobotics/nrp-core/pull-requests/160)

9. Added an Engine implementation for the EDLUT spiking neural network simulator (https://github.com/EduardoRosLab/edlut):
    * [Pull request #165](https://bitbucket.org/hbpneurorobotics/nrp-core/pull-requests/165)
    * [Pull request #183](https://bitbucket.org/hbpneurorobotics/nrp-core/pull-requests/183)

10. Added support to access the current simulation time and simulation iteration in TFs and the Computational Graph:
    * [Pull request #164](https://bitbucket.org/hbpneurorobotics/nrp-core/pull-requests/164)
    * [Pull request #177](https://bitbucket.org/hbpneurorobotics/nrp-core/pull-requests/177)

11. Multiple improvements in the Gazebo Engine: support for using separate SDF files containing additional models to be spawned in the simulation; included new example experiments; other bug fixes and improvemnts:
    * [Pull request #143](https://bitbucket.org/hbpneurorobotics/nrp-core/pull-requests/143)

13. Added a new execution mode in the Computational Graph in which Output nodes can have a period property expressing the frequency at which they should publish a message and only those Functional Nodes required to procude those messages are executed at each cycle:
    * [Pull request #137](https://bitbucket.org/hbpneurorobotics/nrp-core/pull-requests/137)

14. Added PyBullet to the collection of simulators that can be interfaced from the PySim Engine:
    * [Pull request #122](https://bitbucket.org/hbpneurorobotics/nrp-core/pull-requests/122)

15. Several improvements and bug fixes in EventLoop and Computational Graph:
    * [Pull request #162](https://bitbucket.org/hbpneurorobotics/nrp-core/pull-requests/162)

16. Improvement in the DataTransfer Engine logging format and added support for retained mqtt msgs:
    * [Pull request #144](https://bitbucket.org/hbpneurorobotics/nrp-core/pull-requests/144)
    * [Pull request #147](https://bitbucket.org/hbpneurorobotics/nrp-core/pull-requests/147)
    * [Pull request #190](https://bitbucket.org/hbpneurorobotics/nrp-core/pull-requests/190)

17. Introduced a new channel of communication between the NRPCore python client and Engines:
    * [Pull request #149](https://bitbucket.org/hbpneurorobotics/nrp-core/pull-requests/149)
    * [Pull request #155](https://bitbucket.org/hbpneurorobotics/nrp-core/pull-requests/155)

18. Integration of xpra for remote experiment visualization:
    * [Pull request #179](https://bitbucket.org/hbpneurorobotics/nrp-core/pull-requests/179)

19. Separate the NRP-Core Python client as an independent Python module which can be installed and used standalone, without installing the rest of NRP-Core:
    * [Pull request #172](https://bitbucket.org/hbpneurorobotics/nrp-core/pull-requests/172)

## NRPCore 1.4.1 (2023-09-29)

1. Bug fixes:
    * [Pull request #193](https://bitbucket.org/hbpneurorobotics/nrp-core/pull-requests/193)
    * [Pull request #195](https://bitbucket.org/hbpneurorobotics/nrp-core/pull-requests/195)
    * [Pull request #194](https://bitbucket.org/hbpneurorobotics/nrp-core/pull-requests/194)
    * [Pull request #196](https://bitbucket.org/hbpneurorobotics/nrp-core/pull-requests/196)
    * [Pull request #197](https://bitbucket.org/hbpneurorobotics/nrp-core/pull-requests/197)
    * [Pull request #198](https://bitbucket.org/hbpneurorobotics/nrp-core/pull-requests/198)
    * [Pull request #199](https://bitbucket.org/hbpneurorobotics/nrp-core/pull-requests/199)
    * [Pull request #208](https://bitbucket.org/hbpneurorobotics/nrp-core/pull-requests/208)
    * [Pull request #204](https://bitbucket.org/hbpneurorobotics/nrp-core/pull-requests/204)

2. Added a script for building the full chain of images:
    * [Pull request #203](https://bitbucket.org/hbpneurorobotics/nrp-core/pull-requests/203)

3. Simulation configuration CLI interface enhancements:
    * [Pull request #205](https://bitbucket.org/hbpneurorobotics/nrp-core/pull-requests/205)

4. Added support for connecting precompiled C++ functional nodes in the computational graph using the decorator FromFunctionalNode. Before only Python Functional Nodes could be connected:
    * [Pull request #209](https://bitbucket.org/hbpneurorobotics/nrp-core/pull-requests/209)

5. Added support for in-engine distributed learning. Implemented a new Pysim Engine leveraging StableBaselines for the forementioned purpose along with an example on how to use it:
    * [Pull request #170](https://bitbucket.org/hbpneurorobotics/nrp-core/pull-requests/170)

6. A few improvements regarding the MQTT topics formatting used by the DataTransfer Engine to publish datapacks:
    * [Pull request #200](https://bitbucket.org/hbpneurorobotics/nrp-core/pull-requests/200)
    * [Pull request #202](https://bitbucket.org/hbpneurorobotics/nrp-core/pull-requests/202)
    * [Pull request #206](https://bitbucket.org/hbpneurorobotics/nrp-core/pull-requests/206)

7. Added section about recording of Gazebo simulations to docs:
    * [Pull request #163](https://bitbucket.org/hbpneurorobotics/nrp-core/pull-requests/163)

8. Several improvements on the NEST server Engine:
    * [Pull request #201](https://bitbucket.org/hbpneurorobotics/nrp-core/pull-requests/201)
    * [Pull request #207](https://bitbucket.org/hbpneurorobotics/nrp-core/pull-requests/207)
    * [Pull request #210](https://bitbucket.org/hbpneurorobotics/nrp-core/pull-requests/210)

