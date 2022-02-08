set(NRP_VERSION 1.1.0)

cmake_minimum_required(VERSION 3.16)
project("NRP Core" VERSION ${NRP_VERSION})

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fpic")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fpic")

set (CMAKE_CXX_STANDARD 17)

set (CMAKE_EXPORT_COMPILE_COMMANDS ON)
set (CMAKE_INTERPROCEDURAL_OPTIMIZATION OFF)
#string(APPEND CMAKE_SHARED_LINKER_FLAGS " -Wl,--no-undefined")

set(COVERAGE OFF CACHE BOOL "Enables code coverage")

if(${COVERAGE} STREQUAL ON)
    set(GCOV_CXX_FLAGS -coverage)
    set(GCOV_LD_FLAGS -coverage)
else()
    set(GCOV_CXX_FLAGS "")
    set(GCOV_LD_FLAGS "")
endif()

set(NRP_COMMON_COMPILATION_FLAGS -Wall -Wextra -Werror ${GCOV_CXX_FLAGS})
set(NRP_COMMON_LD_FLAGS ${GCOV_LD_FLAGS})

# Uncomment to compile a production version of code (without trace calls)
# add_definitions(-DPRODUCTION_RELEASE)

# Uncomment to compile time profile calls
# add_definitions(-DTIME_PROFILE)

include(GNUInstallDirs)
include(ExternalProject)
include(FetchContent)

cmake_policy(SET CMP0077 NEW)
set(CMAKE_POLICY_DEFAULT_CMP0077 NEW)

##########################################
## Unit testing library
option(ENABLE_TESTING "Build tests" ON)
if(${ENABLE_TESTING})
    enable_testing()

    include(GoogleTest)

    find_package(GTest 1.10 QUIET)
    if(NOT ${GTest_FOUND})
        message("Please wait. Downloading GTest...")
        FetchContent_Declare(GTest
            GIT_REPOSITORY https://github.com/google/googletest.git
            GIT_TAG v1.10.x)

        FetchContent_GetProperties(GTest)
        if(NOT GTest_POPULATED)
            FetchContent_Populate(GTest)

            set(INSTALL_GTEST OFF)
            add_subdirectory(${gtest_SOURCE_DIR} ${gtest_BINARY_DIR})

            add_library(GTest::GTest ALIAS gtest)
            add_library(GTest::Main  ALIAS gtest_main)
        endif()
    endif()
endif()


##########################################
## Python 3 libraries
find_package(Python3 REQUIRED COMPONENTS Interpreter Development)

##########################################
## Boost Python libraries
## find_package(Boost) changes behavior after Boost version 1.67, so adapt component names accordingly
add_compile_definitions(BOOST_ASIO_DISABLE_CONCEPTS)
find_package(Boost REQUIRED)
if(Boost_VERSION VERSION_GREATER_EQUAL 1.67)
    set(BOOST_PYTHON_COMPONENT "python${Python3_VERSION_MAJOR}${Python3_VERSION_MINOR}" "numpy${Python3_VERSION_MAJOR}${Python3_VERSION_MINOR}")
else()
    set(BOOST_PYTHON_COMPONENT "python${Python3_VERSION_MAJOR}" "numpy${Python3_VERSION_MAJOR}")
endif()
find_package(Boost REQUIRED COMPONENTS ${BOOST_PYTHON_COMPONENT} filesystem)
set(BOOST_PYTHON ${BOOST_PYTHON_COMPONENT})
list(TRANSFORM BOOST_PYTHON PREPEND "Boost::")

set(PYTHON_INSTALL_DIR_REL "${CMAKE_INSTALL_LIBDIR}/python${Python3_VERSION_MAJOR}.${Python3_VERSION_MINOR}/site-packages" CACHE INTERNAL "Relative install location for python libraries")




##########################################
## Log library
find_package(spdlog 1.8.1 QUIET)
if(NOT ${spdlog_FOUND})
    message("Please wait. Downloading spdlog...")
    FetchContent_Declare(spdlog
        GIT_REPOSITORY https://github.com/gabime/spdlog.git
        GIT_TAG v1.8.1)

    FetchContent_GetProperties(spdlog)
    if(NOT spdlog_POPULATED)
        FetchContent_Populate(spdlog)

        set(SPDLOG_BUILD_SHARED ON)
        set(SPDLOG_INSTALL ON)

        add_subdirectory(${spdlog_SOURCE_DIR} ${spdlog_BINARY_DIR})
    endif()
endif()


##########################################
## Doxygen
find_package(Doxygen REQUIRED)
set_property(GLOBAL PROPERTY DOXYGEN_SOURCE_LIST)
function(add_doxygen_source directory)
    set(NUM_SOURCES ${ARGC}-1)
    list(SUBLIST ARGV 1 ${NUM_SOURCES} ADD_SOURCES)
    get_property(tmp GLOBAL PROPERTY DOXYGEN_SOURCE_LIST)
    foreach(arg ${ADD_SOURCES})
        set(tmp ${tmp} "${directory}/${arg}")
    endforeach()
    set_property(GLOBAL PROPERTY DOXYGEN_SOURCE_LIST "${tmp}")
endfunction(add_doxygen_source)


##########################################
## General NRP library
## Includes components to create new datapacks and engines
add_subdirectory(nrp_general_library)

##########################################
## NRP Supported payload libraries
# Protobuf
add_subdirectory(nrp_protobuf)

# ROS
set(NRP_CATKIN_PACKAGES "std_msgs;geometry_msgs;sensor_msgs" CACHE INTERNAL "Catkin packages containing msg definitions supported in nrp-core")
if(NOT DEFINED ENABLE_ROS)
    message(VERBOSE "ENABLE_ROS defaults to FULL")
    set(ENABLE_ROS FULL)
else()
    message(VERBOSE "ENABLE_ROS is set to ${ENABLE_ROS}")
endif()

if(NOT ENABLE_ROS STREQUAL OFF)
    message(STATUS "Building ROS msgs Python bindings")
    add_definitions(-DROS_ON)
    add_subdirectory(nrp_ros/nrp_ros_msgs)
    add_subdirectory(nrp_ros/nrp_ros_proxy)
else()
    message(STATUS "Ros msgs python bindings will not be built")
endif()

# Spinnaker
if(NOT DEFINED ENABLE_SPINNAKER)
    message(VERBOSE "ENABLE_SPINNAKER defaults to FULL")
    set(ENABLE_SPINNAKER FULL)
else()
    message(VERBOSE "ENABLE_SPINNAKER is set to ${ENABLE_SPINNAKER}")
endif()

if(ENABLE_SPINNAKER STREQUAL FULL)
    message(STATUS "Building Spinnaker support")
    add_definitions(-DSPINNAKER_ON)
else()
    message(STATUS "Spinnaker support will not be built")
endif()

#MQTT
if(NOT DEFINED ENABLE_MQTT)
    message(VERBOSE "ENABLE_MQTT defaults to FULL")
    set(ENABLE_MQTT FULL)
else()
    message(VERBOSE "ENABLE_MQTT is set to ${ENABLE_MQTT}")
endif()

if(NOT ENABLE_MQTT STREQUAL OFF)
    message(STATUS "Building with MQTT support")
    add_definitions(-DMQTT_ON)
endif()

##########################################
## NRP Event Loop Library
add_subdirectory(nrp_event_loop)

##########################################
## NRP Communication Protocols

## JSON Communication Protocol
add_subdirectory(nrp_engine_protocols/nrp_json_engine_protocol)

## GRPC Communication Protocol
add_subdirectory(nrp_engine_protocols/nrp_grpc_engine_protocol)

##########################################
## ENABLE_SIMULATOR Flags
## 
## These flags allow to disable the compilation of those parts of nrp-core that depends on or install a specific simulator (eg. gazebo, nest)
##
## The expected behavior for each of these flags is as follows:
## - the NRPCoreSim is always built regardless of any of the flags values.
## - if a flag is set to OFF:
##   - the related simulator won't be assumed to be installed in the system, ie. make won't fail if it isn't. Also it won't be installed in the compilation process if this possibility is available (as in the case of nest)
##   - The engines connected with this simulator won't be built (nor client nor server components)
##   - tests that would fail if the related simulator is not available won't be built
## - if the flag is set to CLIENT: Same as above but:
##   - the engine clients connected to this simulator will be built. This means that they should not depend on or link to any specific simulator
##   - the engine server side components might or might not be built, depending on if the related simulator is required at compilation time
## - if the flag is set to FULL the simulator is assumed to be installed or it will be installed from source if this option is available. All targets connected with this simulator will be built
##
## This flag system allows to configure the resulting nrp-core depending on which simulators are available on the system, both for avoiding potential dependency conflicts
## between simulators and enforcing modularity, opening the possibility of having specific engine servers running on a different machine or inside containers.

if(NOT DEFINED ENABLE_NEST)
    message(VERBOSE "ENABLE_NEST defaults to FULL")
    set(ENABLE_NEST FULL)
else()
    message(VERBOSE "ENABLE_NEST is set to ${ENABLE_NEST}")
endif()

if(NOT DEFINED ENABLE_GAZEBO)
    message(VERBOSE "ENABLE_GAZEBO defaults to FULL")
    set(ENABLE_GAZEBO FULL)
else()
    message(VERBOSE "ENABLE_GAZEBO is set to ${ENABLE_GAZEBO}")
endif()


##########################################
## NRP Engines

## Gazebo targets
if(NOT ENABLE_GAZEBO STREQUAL OFF)
    message(STATUS "Building Gazebo engines")
    add_subdirectory(nrp_gazebo_engines/nrp_gazebo_json_engine)
    add_subdirectory(nrp_gazebo_engines/nrp_gazebo_grpc_engine)
else()
    message(STATUS "Gazebo engines will not be built")
endif()

## NEST Engines
if(NOT ENABLE_NEST STREQUAL OFF)
    message(STATUS "Building NEST engines")
    add_subdirectory(nrp_nest_engines)
else()
    message(STATUS "NEST engines will not be built")
endif()

## Python Engine
add_subdirectory(nrp_python_json_engine)

add_subdirectory(nrp_pysim_engines)

## Data Transfer Engine
add_subdirectory("nrp_datatransfer_grpc_engine")

##########################################
## NRP Simulation
set(NRP_SIMULATION_DEFAULT_ENGINE_LAUNCHERS "NRPPythonJSONEngine.so"
    CACHE STRING "Default engines which will always be available in the NRP simulation")
add_subdirectory(nrp_simulation)

##########################################
## cppcheck

# Generates a file with exact compiler calls for all translation units, which is needed by cppcheck
# Searches for *.cpp, *.cc, *.hpp and *.h files in the _deps subdirectory of the build directory
# All found files will be excluded from cppcheck analysis
# Their absolute paths are saved in cppcheck_suppress.txt file, which will be used by cppcheck --suppressions
add_custom_target(cppcheck-base
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
    COMMAND mkdir -p cppcheck
    COMMAND ${CMAKE_COMMAND} .. -DCMAKE_EXPORT_COMPILE_COMMANDS=ON
    COMMAND find _deps/ -type f \( -iname \*.cpp -o -iname \*.cc -o -iname \*.hpp -o -iname \*.h \) -exec sh -c "printf \"*:${CMAKE_BINARY_DIR}/{}\\n\"" \; > cppcheck/cppcheck_suppress.txt
    COMMENT "Generating compile_commands.json and cppcheck/cppcheck_suppress.txt"
    VERBATIM
)

# Runs cppcheck on the project and saves the results in cppcheck_results.xml
add_custom_target(cppcheck-ci
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
    COMMAND cppcheck --quiet --verbose --enable=all --suppressions-list=cppcheck/cppcheck_suppress.txt --project=compile_commands.json --xml --xml-version=2 2> cppcheck/cppcheck_results.xml
    COMMENT "Running cppcheck and generating cppcheck/cppcheck_results.xml"
    DEPENDS cppcheck-base
    VERBATIM
)

# Runs cppcheck on the project and prints the results to stderr
add_custom_target(cppcheck
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
    COMMAND cppcheck --quiet --verbose --enable=all --suppressions-list=cppcheck/cppcheck_suppress.txt --project=compile_commands.json
    COMMENT "Running cppcheck"
    DEPENDS cppcheck-base
    VERBATIM
)

##########################################
## coverage

add_custom_target(gcovr
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
    COMMAND gcovr -r ../ --exclude "_deps" --exclude ".*example_engine.*" --xml gcovr.xml --html gcovr.html
    COMMENT "Running gcovr"
    VERBATIM
)

# Generate code from the template gRPC engine
execute_process(
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    COMMAND tools/create_new_engine.py --name Build --type grpc
    COMMAND tools/create_new_engine.py --name Build --type json
)

add_subdirectory("tools/build_grpc_engine")
add_subdirectory("tools/build_json_engine")

##########################################
## Doxygen

set(DOXYGEN_GENERATE_LATEX YES)
set(DOXYGEN_GENERATE_TREEVIEW YES)
set(DOXYGEN_DISABLE_INDEX YES)
set(DOXYGEN_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/doxygen")
set(DOXYGEN_IMAGE_PATH "docs/images")
set(DOXYGEN_EXAMPLE_PATH
        ${DOXYGEN_EXAMPLE_PATH}
        "docs/example_engine"
        "examples"
        "config_schemas"
        "nrp_gazebo_engines/nrp_gazebo_json_engine/nrp_gazebo_json_engine/config/gazebo_json_config.h"
        "nrp_protobuf/nrp_proto_defs"
        "nrp_protobuf/engine_proto_defs"
        "nrp_general_library/CMakeLists.txt"
        "CMakeLists.txt"
        "nrp_ros/nrp_ros_msgs/nrp_ros_msgs/CMakeLists.txt"
        "README.md")

## Example Engine. It is built to ensure the examples are not out of date in the documentation
add_subdirectory("docs/example_engine")

get_property(DOXYGEN_SOURCES GLOBAL PROPERTY DOXYGEN_SOURCE_LIST)
doxygen_add_docs(nrp_doxygen
        ${DOXYGEN_SOURCES}
        "docs/main_page.dox"

        # TODO: review, complete and enable this section. Include information about the variable system to enable or
        #  disable the compilation of engines
        # "docs/architecture_overview/cmake_component.dox"

        "docs/architecture_overview/architecture_overview.dox"
        "docs/architecture_overview/datapack.dox"
        "docs/architecture_overview/engine.dox"
        "docs/architecture_overview/nrp_simulation.dox"
        "docs/architecture_overview/plugin_system.dox"
        "docs/architecture_overview/process_launcher.dox"
        "docs/architecture_overview/simulation_loop.dox"
        "docs/architecture_overview/sync_model_details.dox"
        "docs/architecture_overview/transceiver_function.dox"
        "docs/architecture_overview/preprocessing_function.dox"
        "docs/architecture_overview/lifecycle_components.dox"

        "docs/experiment_configuration/experiment_configuration.dox"
        "docs/experiment_configuration/engine_schema.dox"
        "docs/experiment_configuration/transceiver_function_schema.dox"
        "docs/experiment_configuration/simulation_schema.dox"
        "docs/experiment_configuration/json_schema.dox"
        "docs/experiment_configuration/process_launcher_schema.dox"

        "docs/engines/nrp_engines.dox"
        "docs/engines/engine_comm.dox"
        "docs/engines/gazebo/gazebo_engine.dox"
        "docs/engines/gazebo/gazebo_datapacks.dox"
        "docs/engines/gazebo/gazebo_plugins.dox"
        "docs/engines/nest/nest_engine.dox"
        "docs/engines/nest/nest_json.dox"
        "docs/engines/nest/nest_server.dox"
        "docs/engines/python_json_engine.dox"
        "docs/engines/opensim/opensim_engine.dox"

        "docs/getting_started/getting_started.dox"
        "docs/getting_started/installation.dox"
        "docs/getting_started/running_example_exp.dox"

        "docs/guides/guides.dox"
        "docs/guides/general_developer_guide.dox"
        "docs/guides/helpful_info.dox"
        "docs/guides/add_proto_definition.dox"
        "docs/guides/add_custom_ros_msgs.dox"
        "docs/guides/engine_creation_scratch.dox"
        "docs/guides/engine_creation_template.dox"
        "docs/guides/python_engine_creation.dox"

        "docs/event_loop/event_loop.dox"
        "docs/event_loop/computational_graph.dox"
        "docs/event_loop/python_graph.dox"
        "docs/event_loop/computational_graph_integration.dox"

        COMMENT "Generating documentation")
