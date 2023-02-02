set(PROJECT_NAME "NRPCoreSim")
set(HEADER_DIRECTORY "nrp_simulation")

set(NAMESPACE_NAME "${PROJECT_NAME}")

set(LIBRARY_NAME "NRPCoreSimLib")
set(PYTHON_MODULE_NAME "Py${PROJECT_NAME}")
set(EXECUTABLE_NAME "NRPCoreSim")
set(TEST_NAME "${PROJECT_NAME}Tests")

set(LIB_EXPORT_NAME "${LIBRARY_NAME}Targets")
set(LIB_CONFIG_NAME "${LIBRARY_NAME}Config")
set(LIB_VERSION_NAME "${LIB_CONFIG_NAME}Version")

cmake_minimum_required(VERSION 3.16)
project("${PROJECT_NAME}" VERSION ${NRP_VERSION})

include(GNUInstallDirs)

# List library build files
set(LIB_SRC_FILES
    nrp_simulation/simulation/nrp_core_server.cpp
    nrp_simulation/simulation/fti_loop.cpp
    nrp_simulation/simulation/simulation_manager.cpp
    nrp_simulation/simulation/simulation_manager_fti.cpp
    nrp_simulation/simulation/simulation_manager_event_loop.cpp
    nrp_simulation/simulation/simulation_parameters.cpp
    nrp_simulation/datapack_handle/datapack_handle.cpp
    nrp_simulation/datapack_handle/tf_manager_handle.cpp
    nrp_simulation/datapack_handle/simulation_data_manager.cpp
    nrp_simulation/datapack_handle/computational_graph_handle.cpp
)

# List executable build files
set(EXEC_SRC_FILES
    nrp_simulation_executable/main.cpp
)

# List testing build files
set(TEST_SRC_FILES
    tests/fti_loop.cpp
    tests/simulation_parameters.cpp
    tests/simulation_manager.cpp
    tests/fti_simulation_manager.cpp
    tests/test_simulation_data_manager.cpp
)


##########################################
## Dependencies

# pistache: REST server library
find_package(PkgConfig)
pkg_check_modules(Pistache REQUIRED IMPORTED_TARGET libpistache)

pkg_check_modules(PROTOBUF REQUIRED protobuf)
pkg_check_modules(GRPC REQUIRED grpc++)

if(NOT ENABLE_ROS STREQUAL OFF)
    find_package(catkin REQUIRED COMPONENTS roscpp)
endif()


##########################################
## Header configuration

# Default engines
set(NRP_ENGINE_LAUNCHERS_LIST "$ENV{NRP_ENGINE_LAUNCHERS}")
list(JOIN NRP_ENGINE_LAUNCHERS_LIST "\", \"" NRP_ENGINE_LAUNCHERS_STR)

# General Header defines
configure_file("nrp_simulation/config/cmake_conf.h.in" "${CMAKE_CURRENT_BINARY_DIR}/include/${HEADER_DIRECTORY}/config/cmake_conf.h" @ONLY)

##########################################
# NRPCoreSim
add_library(${LIBRARY_NAME} SHARED ${LIB_SRC_FILES})
add_library(${NAMESPACE_NAME}::${LIBRARY_NAME} ALIAS ${LIBRARY_NAME})
target_compile_options(${LIBRARY_NAME} PUBLIC $<$<OR:$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_ID:GNU>>:${NRP_COMMON_COMPILATION_FLAGS}>)

target_link_libraries(${LIBRARY_NAME}
    PUBLIC
    ${NRP_GEN_LIB_TARGET}
    ${NRP_PROTO_LIB_TARGET}
    ${PROTOBUF_LIBRARIES}
    ${GRPC_LIBRARIES}
    ProtoDump
    NRPJSONEngineProtocol::NRPJSONEngineProtocol
    ${NRP_ELE_LIB_TARGET}
    ${NRP_ROS_PROXY_TARGET}
    ${NRP_MQTT_PROXY_TARGET}
    ${catkin_LIBRARIES}
)

target_include_directories(${LIBRARY_NAME} BEFORE
    PUBLIC 
        "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>"
        "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>"
        "$<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/include>"

        ${catkin_INCLUDE_DIRS}

    PRIVATE
)


##########################################
## NRPServerPythonModule
if(NOT ${PYTHON_MODULE_SRC_FILES} STREQUAL "")
    add_library(${PYTHON_MODULE_NAME} SHARED ${PYTHON_MODULE_SRC_FILES})
    add_library(${NAMESPACE_NAME}::${PYTHON_MODULE_NAME} ALIAS ${PYTHON_MODULE_NAME})
    target_compile_options(${PYTHON_MODULE_NAME} PRIVATE $<$<OR:$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_ID:GNU>>:${NRP_COMMON_COMPILATION_FLAGS}>)
    set_target_properties(${PYTHON_MODULE_NAME} PROPERTIES PREFIX "")

    target_include_directories(${PYTHON_MODULE_NAME}
        PUBLIC
    )

     target_link_libraries(${PYTHON_MODULE_NAME}
        PUBLIC
            ${NAMESPACE_NAME}::${LIBRARY_NAME}
    )
endif()


##########################################
## Executable
if(NOT "${EXEC_SRC_FILES}" STREQUAL "")
    add_executable(${EXECUTABLE_NAME} ${EXEC_SRC_FILES})
    target_link_libraries(${EXECUTABLE_NAME}
        ${NAMESPACE_NAME}::${LIBRARY_NAME}
    )
endif()


##########################################
## Append Doxygen files
add_doxygen_source("${CMAKE_CURRENT_SOURCE_DIR}" ${EXEC_SRC_FILES} ${LIB_SRC_FILES} ${PYTHON_MODULE_SRC_FILES} "${HEADER_DIRECTORY}")


##########################################
## Tests
if(BUILD_GAZEBO_ENGINE_SERVER AND BUILD_NEST_ENGINE_SERVER AND ${ENABLE_TESTING} AND NOT "${TEST_SRC_FILES}" STREQUAL "")
    find_package(Gazebo REQUIRED)

    set(GAZEBO_WORLD_FILE "${CMAKE_CURRENT_SOURCE_DIR}/tests/test_files/empty_world.sdf")
    set(TEST_SIM_CONFIG_FILE "${CMAKE_CURRENT_BINARY_DIR}/tests/test_files/simulation_config.json")

    # Create testing env files
    configure_file("${CMAKE_CURRENT_SOURCE_DIR}/tests/test_files/simulation_config.json.in" "${TEST_SIM_CONFIG_FILE}" @ONLY)
    configure_file("${CMAKE_CURRENT_SOURCE_DIR}/tests/test_files/test_env.sh.in" "${CMAKE_CURRENT_BINARY_DIR}/test_env.sh" @ONLY)
    configure_file("${CMAKE_CURRENT_SOURCE_DIR}/tests/test_env_cmake.h.in" "${CMAKE_CURRENT_BINARY_DIR}/include/tests/test_env_cmake.h" @ONLY)

    # Create testing executable
    enable_testing()
    add_executable(${TEST_NAME} ${TEST_SRC_FILES})
    target_compile_options(${TEST_NAME} PUBLIC $<$<OR:$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_ID:GNU>>:${NRP_COMMON_COMPILATION_FLAGS}>)
    target_link_options(${TEST_NAME} PUBLIC ${NRP_COMMON_LD_FLAGS})
    target_link_libraries(${TEST_NAME}
        PUBLIC
        ${NAMESPACE_NAME}::${LIBRARY_NAME}
        NRPNestJSONEngine::NRPNestJSONEngine
        NRPGazeboGrpcEngine::NRPGazeboGrpcEngine
        GTest::Main
        GTest::GTest
        gmock)

    gtest_discover_tests(${TEST_NAME}
        WORKING_DIRECTORY "${PROJECT_SOURCE_DIR}/tests"
        EXTRA_ARGS -VV)
endif()

# Add tests based on running example experiments
if(ENABLE_EXAMPLES_TESTING)
    add_test(
        NAME Examples.LoopSyncTF
        COMMAND NRPCoreSim -c simulation_config.json -p "NRPNestJSONEngine.so,NRPPythonJSONEngine.so" 
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/tests/test_experiments/loop_sync_test_tf)
    set_tests_properties(Examples.LoopSyncTF 
        PROPERTIES 
            DISABLED $<NOT:$<BOOL:${BUILD_NEST_ENGINE_SERVER}>>)
    add_test(
        NAME Examples.LoopSyncCG
        COMMAND NRPCoreSim -c simulation_config.json -p "NRPNestJSONEngine.so,NRPPythonJSONEngine.so" 
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/tests/test_experiments/loop_sync_test_cg)
    set_tests_properties(Examples.LoopSyncCG 
        PROPERTIES 
            DISABLED $<NOT:$<BOOL:${BUILD_NEST_ENGINE_SERVER}>>)
    add_test(
        NAME Examples.EventLoopSync 
        COMMAND NRPCoreSim -c simulation_config.json -p "NRPNestJSONEngine.so,NRPPythonJSONEngine.so" 
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/tests/test_experiments/loop_sync_test_eloop)
    set_tests_properties(Examples.EventLoopSync 
        PROPERTIES 
            DISABLED $<NOT:$<BOOL:${BUILD_NEST_ENGINE_SERVER}>>)
endif()

# Add python tests for NRP Server
find_package(Python3 COMPONENTS Interpreter)
if(Python3_Interpreter_FOUND)
    add_test(NAME NrpServer COMMAND py.test server.py "${CMAKE_SOURCE_DIR}/examples/status_function_test" --junitxml "${CMAKE_BINARY_DIR}/xml/NrpServer.xml" WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/tests)
    set_property(TEST NrpServer PROPERTY ENVIRONMENT
        "PYTHONPATH=${PYTHONPATH}:${CMAKE_INSTALL_PREFIX}/${PYTHON_INSTALL_DIR_REL}/${PYTHON_MODULE_NAME}:${CMAKE_INSTALL_PREFIX}/${PYTHON_INSTALL_DIR_REL}")
endif()

##########################################
## Installation

set(INSTALL_CONFIGDIR "${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}")

# Install library files
install(TARGETS
        ${LIBRARY_NAME}
    EXPORT
        ${LIB_EXPORT_NAME}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}

    PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${HEADER_DIRECTORY}
)

# Install export target
install(EXPORT ${LIB_EXPORT_NAME}
    DESTINATION
        ${INSTALL_CONFIGDIR}
    FILE
        "${LIB_EXPORT_NAME}.cmake"
    NAMESPACE
        "${NAMESPACE_NAME}::"
)

# Install headers
install(DIRECTORY "${HEADER_DIRECTORY}" "${CMAKE_CURRENT_BINARY_DIR}/include/${HEADER_DIRECTORY}"
    DESTINATION
        ${CMAKE_INSTALL_INCLUDEDIR}
    FILES_MATCHING
        PATTERN "*.h"
        PATTERN "*.hpp"
)

# Install python module
if(TARGET ${PYTHON_MODULE_NAME})
    install(TARGETS ${PYTHON_MODULE_NAME}
        DESTINATION "${PYTHON_INSTALL_DIR_REL}/${NRP_PYTHON_MODULE_NAME}")

    install(FILES "${CMAKE_CURRENT_BINARY_DIR}/src/__init__.py"
        DESTINATION "${PYTHON_INSTALL_DIR_REL}/${NRP_PYTHON_MODULE_NAME}")
endif()

# Install python module with the NRP client
install(FILES
        "${CMAKE_CURRENT_SOURCE_DIR}/nrp_simulation/python/client.py"
        "${CMAKE_CURRENT_SOURCE_DIR}/nrp_simulation/python/nrp_server_launchers.py"
    DESTINATION "${PYTHON_INSTALL_DIR_REL}/${NRP_PYTHON_MODULE_NAME}")

# Install executable files
if(TARGET ${EXECUTABLE_NAME})
    install(TARGETS ${EXECUTABLE_NAME}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})
endif()

# create cmake version and config files
include(CMakePackageConfigHelpers)
write_basic_package_version_file(
    "${CMAKE_CURRENT_BINARY_DIR}/${LIB_VERSION_NAME}.cmake"
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY AnyNewerVersion
)

configure_package_config_file("${CMAKE_CURRENT_LIST_DIR}/cmake/ProjectConfig.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/${LIB_CONFIG_NAME}.cmake"
    INSTALL_DESTINATION ${INSTALL_CONFIGDIR}
)

# Install cmake version and config files
install(
    FILES
        "${CMAKE_CURRENT_BINARY_DIR}/${LIB_CONFIG_NAME}.cmake"
        "${CMAKE_CURRENT_BINARY_DIR}/${LIB_VERSION_NAME}.cmake"
    DESTINATION ${INSTALL_CONFIGDIR}
)
