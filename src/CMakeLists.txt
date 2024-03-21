set(PROJECT_NAME "NRPCoreSrc")

set(NAMESPACE_NAME "${PROJECT_NAME}")

project("${PROJECT_NAME}" VERSION ${NRP_VERSION})


##########################################
## General NRP library
## Includes components to create new datapacks and engines
add_subdirectory(nrp_general_library)


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
    message(STATUS "Building with Spinnaker support")
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

set(NRP_ENV_VARS ${NRP_ENV_VARS} PARENT_SCOPE)


# Install nrp-run.bash, meant mainly to run nrp-core related commands with a correct environment
file(COPY
        "${CMAKE_CURRENT_SOURCE_DIR}/nrp-run.bash"
        DESTINATION
        "${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_BINDIR}"
        FILE_PERMISSIONS
        OWNER_READ OWNER_WRITE OWNER_EXECUTE GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE
        )

# Install xvfb-run-nrp. A customized xvfb-run that handle signals correctly (so to be launched by BasicFork).
file(COPY
        "${CMAKE_CURRENT_SOURCE_DIR}/nrp_scripts/xvfb-run-nrp"
        DESTINATION
        "${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_BINDIR}"
        FILE_PERMISSIONS
        OWNER_READ OWNER_WRITE OWNER_EXECUTE GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE
        )
