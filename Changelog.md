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
    
    
    
    
    