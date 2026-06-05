cmake_minimum_required(VERSION 3.16)

project("NRP Core Test Examples")

include(CTest)

add_test(
    NAME Examples.HuskyBraitenbergJson
    COMMAND NRPCoreSim -c simulation_config.json -p "NRPNestJSONEngine.so,NRPGazeboJSONEngine.so" -o SimulationTimeout=2
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/husky_braitenberg_json)
set_tests_properties(Examples.HuskyBraitenbergJson 
    PROPERTIES 
        DISABLED $<OR:$<NOT:$<BOOL:${BUILD_NEST_ENGINE_SERVER}>>,$<NOT:$<BOOL:${BUILD_GAZEBO_ENGINE_SERVER}>>>
        TIMEOUT 150)

add_test(
    NAME Examples.HuskyBraitenbergGRPC
    COMMAND NRPCoreSim -c simulation_config.json -p "NRPNestJSONEngine.so,NRPGazeboGrpcEngine.so" -o SimulationTimeout=2
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/husky_braitenberg)
set_tests_properties(Examples.HuskyBraitenbergGRPC 
    PROPERTIES 
        DISABLED $<OR:$<NOT:$<BOOL:${BUILD_NEST_ENGINE_SERVER}>>,$<NOT:$<BOOL:${BUILD_GAZEBO_ENGINE_SERVER}>>>
        TIMEOUT 150)

add_test(
    NAME Examples.HuskyBraitenbergDataTransfer 
    COMMAND NRPCoreSim -c simulation_config_data_transfer.json -p "NRPNestJSONEngine.so,NRPGazeboGrpcEngine.so,NRPDataTransferGrpcEngine.so" -o SimulationTimeout=2
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/husky_braitenberg)
set_tests_properties(Examples.HuskyBraitenbergDataTransfer 
    PROPERTIES 
        DISABLED $<OR:$<NOT:$<BOOL:${BUILD_NEST_ENGINE_SERVER}>>,$<NOT:$<BOOL:${BUILD_GAZEBO_ENGINE_SERVER}>>>
        TIMEOUT 150)

add_test(
    NAME Examples.TFExchange
    COMMAND NRPCoreSim -c simulation_config.json -p "NRPPythonJSONEngine.so" -o SimulationTimeout=5
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/tf_exchange)
set_tests_properties(Examples.TFExchange 
    PROPERTIES 
        TIMEOUT 150)

add_test(
    NAME Examples.NestSimple
    COMMAND NRPCoreSim -c simulation_config.json -p "NRPNestJSONEngine.so" 
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/nest_simple)
set_tests_properties(Examples.NestSimple 
    PROPERTIES 
        DISABLED $<NOT:$<BOOL:${BUILD_NEST_ENGINE_SERVER}>>
        TIMEOUT 150)

add_test(
    NAME Examples.Opensim 
    COMMAND NRPCoreSim -c simulation_config.json -p "NRPPythonJSONEngine.so,NRPPySimEngine.so" -o EngineConfigs.1.Visualizer=false
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/pysim_examples/opensim_control)
set_tests_properties(Examples.Opensim 
    PROPERTIES 
        DISABLED $<NOT:$<BOOL:${ENABLE_OPENSIM}>>
        TIMEOUT 150)

add_test(
    NAME Examples.OpensimTVB
    COMMAND NRPCoreSim -c simulation_config.json -p "NRPPythonJSONEngine.so,NRPPySimEngine.so" -o EngineConfigs.0.Visualizer=false
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/opensim_tvb)
set_tests_properties(Examples.OpensimTVB 
    PROPERTIES 
        DISABLED $<OR:$<NOT:$<BOOL:${ENABLE_OPENSIM}>>,$<NOT:$<BOOL:${ENABLE_TVB}>>>
        TIMEOUT 150)

add_test(
        NAME Examples.HuskyBraitenbergCG
        COMMAND NRPCoreSim -c simulation_config.json -p "NRPNestJSONEngine.so,NRPGazeboGrpcEngine.so" -o SimulationTimeout=2
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/event_loop_examples/husky_braitenberg_cg)
set_tests_properties(Examples.HuskyBraitenbergCG
        PROPERTIES
        DISABLED $<OR:$<NOT:$<BOOL:${BUILD_NEST_ENGINE_SERVER}>>,$<NOT:$<BOOL:${BUILD_GAZEBO_ENGINE_SERVER}>>>
        TIMEOUT 150)

# EBR2-32: foraging_husky drive-state SNN experiment.
# Integration smoke runs at SimulationTimeout=2 like the other
# example smoke entries — long enough to confirm nrp-core can launch
# both engines and ship one tick around the loop, short enough not
# to dominate ctest wall-clock.
add_test(
    NAME Examples.ForagingHusky
    COMMAND NRPCoreSim -c simulation_config.json -p "NRPNestJSONEngine.so,NRPGazeboGrpcEngine.so" -o SimulationTimeout=2
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/foraging_husky)
set_tests_properties(Examples.ForagingHusky
    PROPERTIES
        DISABLED $<OR:$<NOT:$<BOOL:${BUILD_NEST_ENGINE_SERVER}>>,$<NOT:$<BOOL:${BUILD_GAZEBO_ENGINE_SERVER}>>>
        TIMEOUT 150)

# Pure-Python unit tests over the TF helpers in foraging_husky/. These
# do not require Gazebo or NEST, so they stay enabled even on builds
# where ENGINE_SERVER flags are OFF.
add_test(
    NAME Examples.ForagingHuskyTFUnit
    COMMAND python3 -m pytest -q tests/test_tfs.py
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/foraging_husky)
set_tests_properties(Examples.ForagingHuskyTFUnit
    PROPERTIES
        TIMEOUT 60)

add_custom_target(gcovr-examples
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
    COMMAND echo "gcovr is not available for the Tests with examples"
)
if(NOT TARGET gcovr)
    add_custom_target(gcovr)
endif()
add_dependencies(gcovr gcovr-examples)