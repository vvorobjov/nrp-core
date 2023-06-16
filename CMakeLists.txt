set(NRP_VERSION 1.3.1)

cmake_minimum_required(VERSION 3.16)
project("NRP Core" VERSION ${NRP_VERSION})

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fpic")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fpic")

set (CMAKE_CXX_STANDARD 17)

set (CMAKE_EXPORT_COMPILE_COMMANDS ON)
set (CMAKE_INTERPROCEDURAL_OPTIMIZATION OFF)
#string(APPEND CMAKE_SHARED_LINKER_FLAGS " -Wl,--no-undefined")

## NRP-core compilation options (NAME DESCRIPTION DEFAULT)
# See the section "common NRP-core CMake options" for the explanation about these parameters
option(ENABLE_ROS "Enable ROS support" ON)
option(ENABLE_MQTT "Enable MQTT support" ON)
option(ENABLE_SPINNAKER "Enable Spinnaker support" ON)

include(CMakeDependentOption)
# See the section "common NRP-core CMake options" for the explanation about ENABLE_SIMULATOR parameters
option(ENABLE_NEST "Enable nest-simulator support" ON)
cmake_dependent_option(BUILD_NEST_ENGINE_SERVER "Build the server part of the nest-simulator engines" ON "ENABLE_NEST" OFF)

option(ENABLE_GAZEBO "Enable Gazebo support" ON)
cmake_dependent_option(BUILD_GAZEBO_ENGINE_SERVER "Build the server part of the Gazebo engines" ON "ENABLE_GAZEBO" OFF)

option(ENABLE_EDLUT "Enable EDLUT support" OFF)
option(EDLUT_WITH_CUDA "Enable EDLUT with cuda support" OFF)
cmake_dependent_option(BUILD_EDLUT_ENGINE_SERVER "Build the server part of the EDLUT engine" ON "ENABLE_EDLUT" OFF)

option(COVERAGE "Enables code coverage" OFF)
option(BUILD_RST "Convert Doxygen documentation into the reStructuredText source files." OFF)
## end of: NRP-core compilation options

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

# Set to ON to compile time profile calls
OPTION(ENABLE_TIME_PROFILE "If ON, nrp-core will be compiled with time profiling tools" OFF) # Disabled by default
if(ENABLE_TIME_PROFILE)
    add_definitions(-DTIME_PROFILE)
endif()

include(GNUInstallDirs)
include(ExternalProject)
include(FetchContent)

cmake_policy(SET CMP0077 NEW)
set(CMAKE_POLICY_DEFAULT_CMP0077 NEW)

##########################################
## Unit testing library
option(ENABLE_TESTING "Build tests" ON)
include(CMakeDependentOption)
cmake_dependent_option(ENABLE_EXAMPLES_TESTING "Run Tests with examples" OFF "ENABLE_TESTING" OFF)
if(${ENABLE_TESTING})
    enable_testing()

    include(GoogleTest)
    include(CTest)

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
        endif()
    endif()

    # Add cmake file that allows to find gmock libraries
    list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
    find_package(GMock REQUIRED)

    # Add tests based on running example experiments
    if(ENABLE_EXAMPLES_TESTING)
        add_subdirectory(examples)
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

if(NOT DEFINED NRP_DEP_CMAKE_INSTALL_PREFIX)
    set(NRP_DEP_CMAKE_INSTALL_PREFIX ${CMAKE_INSTALL_PREFIX})
else()
    message(STATUS "Installing nrp-core dependencies at: ${NRP_DEP_CMAKE_INSTALL_PREFIX}")
endif()

##########################################
## Update git submodules
find_package(Git QUIET)
if(GIT_FOUND AND EXISTS "${PROJECT_SOURCE_DIR}/.git")
# Update submodules as needed
    option(GIT_SUBMODULE "Check submodules during build" ON)
    if(GIT_SUBMODULE)
        message(STATUS "Submodule update")
        execute_process(COMMAND ${GIT_EXECUTABLE} submodule update --init
                        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
                        RESULT_VARIABLE GIT_SUBMOD_RESULT)
        if(NOT GIT_SUBMOD_RESULT EQUAL "0")
            message(FATAL_ERROR "git submodule update --init --recursive failed with ${GIT_SUBMOD_RESULT}, please checkout submodules")
        endif()
    endif()
endif()


##########################################
## General NRP library
## Includes components to create new datapacks and engines
set(NRP_ENV_VARS "")
add_subdirectory(nrp_general_library)


##########################################
## Declare nrp-core-msgs path
if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/nrp-core-msgs")
    set(NRP_CORE_MSGS_PATH "${CMAKE_CURRENT_SOURCE_DIR}/nrp-core-msgs/")

    if(NOT EXISTS "${NRP_CORE_MSGS_PATH}/protobuf")
        message(FATAL_ERROR "The Proto definitions are not found in submodule nrp-core-msgs! Please update submodules and try again.")
    endif()
    set(NRP_CORE_MSGS_PROTO_PATH "${NRP_CORE_MSGS_PATH}/protobuf/")

    if(NOT EXISTS "${NRP_CORE_MSGS_PATH}/nrp_ros_msgs")
        message(FATAL_ERROR "ROS definitions are not found in submodule nrp-core-msgs! Please update submodules and try again.")
    endif()
else()
    message(FATAL_ERROR "The submodule nrp-core-msgs does not exist! GIT_SUBMODULE was turned off or failed. Please update submodules and try again.")
endif()


##########################################
## NRP Supported payload libraries
# Protobuf
add_subdirectory(nrp_protobuf)

# ROS
if(NOT DEFINED NRP_ROS_MSGS_PACKAGES)
    set(NRP_ROS_MSGS_PACKAGES "")
endif()

set(DEFAULT_NRP_ROS_MSGS_PACKAGES "std_msgs;geometry_msgs;sensor_msgs" CACHE INTERNAL "ROS Catkin packages containing msg definitions supported in nrp-core")
foreach(PACK ${DEFAULT_NRP_ROS_MSGS_PACKAGES})
    if(NOT PACK IN_LIST NRP_ROS_MSGS_PACKAGES)
        list(APPEND NRP_ROS_MSGS_PACKAGES ${PACK})
    endif()
endforeach()

message(STATUS "NRP_ROS_MSGS_PACKAGES: ${NRP_ROS_MSGS_PACKAGES}")

if(ENABLE_ROS)
    message(STATUS "Building ROS msgs Python bindings")
    add_definitions(-DROS_ON)
    add_subdirectory(nrp_ros)
    add_subdirectory(nrp_comm_proxies/nrp_ros_proxy)
else()
    message(STATUS "ROS msgs python bindings will not be built")
endif()

# Spinnaker
if(ENABLE_SPINNAKER)
    message(STATUS "Building Spinnaker support")
    add_definitions(-DSPINNAKER_ON)
else()
    message(STATUS "Spinnaker support will not be built")
endif()

#MQTT
if(ENABLE_MQTT)
    message(STATUS "Building with MQTT support")
    add_subdirectory(nrp_comm_proxies/nrp_mqtt_proxy)
    add_definitions(-DMQTT_ON)
else()
    message(STATUS "MQTT support is disabled")
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
## NRP Engines
set(ENV{NRP_ENGINE_LAUNCHERS} ""
        CACHE STRING "Default engines which will always be available in the NRP simulation")

## Gazebo targets
if(ENABLE_GAZEBO)
    message(STATUS "Building Gazebo engines")
    add_subdirectory(nrp_gazebo_engines)
else()
    message(STATUS "Gazebo engines will not be built")
endif()

## NEST Engines
if(ENABLE_NEST)
    message(STATUS "Building nest-simulator engines")
    add_subdirectory(nrp_nest_engines)
else()
    message(STATUS "nest-simulator engines will not be built")
endif()

## EDLUT Engine
if(ENABLE_EDLUT)
    message(STATUS "Building EDLUT engine")
    add_subdirectory(nrp_edlut_grpc_engine)
else()
    message(STATUS "EDLUT engine will not be built")
endif()

## Python Engine
add_subdirectory(nrp_python_json_engine)
add_subdirectory(nrp_python_grpc_engine)

## Unity gRPC engine
add_subdirectory(nrp_unity_grpc_engine)

add_subdirectory(nrp_pysim_engines)
## Data Transfer Engine
add_subdirectory(nrp_datatransfer_grpc_engine)

##########################################
## NRP Simulation
add_subdirectory(nrp_simulation)

##########################################
## NRP clients
add_subdirectory(nrp_clients/python)

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
add_custom_target(gcovr-unittest
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
    COMMAND gcovr -r ../ --exclude "_deps" --exclude ".*example_engine.*" --xml gcovr.xml --html gcovr.html
    COMMENT "Running gcovr"
    VERBATIM
)
if(NOT TARGET gcovr)
    add_custom_target(gcovr)
endif()
add_dependencies(gcovr gcovr-unittest)

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
if(BUILD_RST)
    set(DOXYGEN_GENERATE_XML YES)
    set(DOXYGEN_XML_OUTPUT xml)
    set(DOXYGEN_XML_PROGRAMLISTING NO)
    set(DOXYGEN_CASE_SENSE_NAMES NO)
    set(DOXYGEN_HIDE_UNDOC_RELATIONS YES)
    set(DOXYGEN_EXTRACT_ALL YES)
endif()
set(DOXYGEN_EXAMPLE_PATH
        ${DOXYGEN_EXAMPLE_PATH}
        "docs/example_engine"
        "docs/example_add_proto"
        "examples"
        "config_schemas"
        "nrp_gazebo_engines/nrp_gazebo_json_engine/nrp_gazebo_json_engine/config/gazebo_json_config.h"
        "${NRP_CORE_MSGS_PROTO_PATH}/nrp_proto_defs"
        "${NRP_CORE_MSGS_PROTO_PATH}/engine_proto_defs"
        "nrp_general_library/CMakeLists.txt"
        "CMakeLists.txt"
        "${NRP_CORE_MSGS_PATH}/nrp_ros_msgs/CMakeLists.txt"
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
        "docs/architecture_overview/functions.dox"
        "docs/architecture_overview/process_launcher.dox"
        "docs/architecture_overview/launch_command.dox"
        "docs/architecture_overview/simulation_loop.dox"
        "docs/architecture_overview/sync_model_details.dox"
        "docs/architecture_overview/transceiver_function.dox"
        "docs/architecture_overview/preprocessing_function.dox"
        "docs/architecture_overview/supported_workflows.dox"
        "docs/architecture_overview/status_function.dox"
        "docs/architecture_overview/lifecycle_components.dox"
        "docs/architecture_overview/experiment_lifecycle.dox"
        "docs/architecture_overview/main_elements.dox"

        "docs/experiment_configuration/experiment_configuration.dox"
        "docs/experiment_configuration/engine_schema.dox"
        "docs/experiment_configuration/transceiver_function_schema.dox"
        "docs/experiment_configuration/simulation_schema.dox"
        "docs/experiment_configuration/event_loop_schema.dox"
        "docs/experiment_configuration/json_schema.dox"
        "docs/experiment_configuration/process_launcher_schema.dox"
        "docs/experiment_configuration/nrp_connectors.dox"

        "docs/engines/nrp_engines.dox"
        "docs/engines/engine_comm.dox"
        "docs/engines/gazebo/gazebo_engine.dox"
        "docs/engines/gazebo/gazebo_configuration.dox"
        "docs/engines/gazebo/gazebo_datapacks.dox"
        "docs/engines/gazebo/gazebo_plugins.dox"
        "docs/engines/nest/nest_engine.dox"
        "docs/engines/nest/nest_json.dox"
        "docs/engines/nest/nest_server.dox"
        "docs/engines/python_json_engine.dox"
        "docs/engines/pysim/pysim_engine.dox"
        "docs/engines/tvb/tvb_engine.dox"
        "docs/engines/datatransfer/datatransfer_engine.dox"
        "docs/engines/python_grpc/python_grpc_engine.dox"
        "docs/engines/edlut/edlut_engine.dox"

        "docs/getting_started/getting_started.dox"
        "docs/getting_started/installation.dox"
        "docs/getting_started/running_example_exp.dox"
        "docs/getting_started/python_client.dox"
        "docs/getting_started/running_example_docker_exp.dox"
        "docs/getting_started/running_example_docker_compose.dox"

        "docs/guides/guides.dox"
        "docs/guides/general_developer_guide.dox"
        "docs/guides/helpful_info.dox"
        "docs/guides/add_proto_definition.dox"
        "docs/guides/add_custom_ros_msgs.dox"
        "docs/guides/engine_creation_scratch.dox"
        "docs/guides/engine_creation_template.dox"
        "docs/guides/python_engine_creation.dox"
        "docs/guides/troubleshooting.dox"
        "docs/guides/docker_compose.dox"
        "docs/guides/docker_daemon.dox"
        "docs/guides/remote_docker_compose.dox"

        "docs/event_loop/event_loop.dox"
        "docs/event_loop/computational_graph.dox"
        "docs/event_loop/python_graph.dox"
        "docs/event_loop/computational_graph_integration.dox"
        "docs/event_loop/fn_cpp_nodes.dox"
        "docs/event_loop/async_experiments.dox"

        COMMENT "Generating documentation")


if(BUILD_RST)
    if("${DOXYREST_DIR}" STREQUAL "")
        set(DOXYREST_DIR "${CMAKE_CURRENT_BINARY_DIR}/doxyrest" CACHE INTERNAL "Doxyrest build dir")
    endif()

    set(NRP_DOXYREST_INSTALL_PREFIX ${DOXYREST_DIR}/build-bin)
    if(NOT TARGET doxyrest)
    ExternalProject_Add(doxyrest
            GIT_REPOSITORY https://github.com/vovkos/doxyrest_b.git
            GIT_PROGRESS true

            PREFIX "${DOXYREST_DIR}"
            INSTALL_DIR ${NRP_DOXYREST_INSTALL_PREFIX}

            CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=${NRP_DOXYREST_INSTALL_PREFIX}

            UPDATE_COMMAND ""
            )
    endif()

    set(DOXYREST_OUTPUT_DIR "${DOXYREST_DIR}/output")
    set(DOXYREST_RST_DIR "${DOXYREST_DIR}/rst-output")
    set(DOXYREST_SHARE_DIR "${NRP_DOXYREST_INSTALL_PREFIX}/share/doxyrest")

    # Variables to be set: DOXYREST_RST_DIR, DOXYREST_SHARE_DIR
    configure_file("docs/sphinx/doxyrest-config.lua.in" "${DOXYREST_DIR}/doxyrest-config.lua" @ONLY)

    add_custom_target(
        nrp_doxygen_rst
        COMMAND "${NRP_DOXYREST_INSTALL_PREFIX}/bin/doxyrest"
        WORKING_DIRECTORY "${DOXYREST_DIR}"
        DEPENDS nrp_doxygen doxyrest
    )

    add_custom_command(TARGET nrp_doxygen_rst POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory "${CMAKE_SOURCE_DIR}/${DOXYGEN_IMAGE_PATH}" "${DOXYREST_RST_DIR}"
    )

    # format bash-style snippets in rST
    add_custom_command(TARGET nrp_doxygen_rst POST_BUILD
        COMMAND sed -i 's@ref-code-block:: cpp@ref-code-block:: bash@' "${DOXYREST_RST_DIR}/page_installation.rst"
    )

    # format pictures
    add_custom_command(TARGET nrp_doxygen_rst POST_BUILD
        COMMAND python3 "${CMAKE_CURRENT_SOURCE_DIR}/docs/sphinx/format-pictures-check-toc.py" "${DOXYREST_RST_DIR}"
    )

    # zip rst files
    add_custom_command(TARGET nrp_doxygen_rst POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E tar "cfv" "${CMAKE_CURRENT_BINARY_DIR}/nrp-core-rst.zip" --format=zip "*"            
        WORKING_DIRECTORY "${DOXYREST_RST_DIR}"
    )
else()
    message(STATUS "Skipping generating rST documentation output")
endif()

# Print message with necessary env variables to run NRPCoreSim, compile and install file exporting this variables
message(STATUS "MESSAGE: before executing NRPCoreSim be sure to add the following variables to your environment:")
file(WRITE "${CMAKE_BINARY_DIR}/.nrp_env" "# Exports environment variables necessary to run nrp-core related commands\n")
foreach(VAR ${NRP_ENV_VARS})
    file(APPEND "${CMAKE_BINARY_DIR}/.nrp_env" "export ${VAR}\n")
    message(STATUS "\t${VAR}")
endforeach()

file(COPY
        "${CMAKE_BINARY_DIR}/.nrp_env"
        DESTINATION
        "${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_BINDIR}"
        )

# Install nrp-run.bash, meant mainly to run nrp-core related commands with a correct environment
file(COPY
        "${CMAKE_SOURCE_DIR}/nrp-run.bash"
        DESTINATION
        "${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_BINDIR}"
        FILE_PERMISSIONS
        OWNER_READ OWNER_WRITE OWNER_EXECUTE GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE
        )

# Install xvfb-run-nrp. A customized xvfb-run that handle signals correctly (so to be launched by BasicFork).
file(COPY
        "${CMAKE_SOURCE_DIR}/.ci/xvfb-run-nrp"
        DESTINATION
        "${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_BINDIR}"
        FILE_PERMISSIONS
        OWNER_READ OWNER_WRITE OWNER_EXECUTE GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE
        )