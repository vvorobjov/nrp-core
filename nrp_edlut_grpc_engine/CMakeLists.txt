set(PROJECT_NAME "NRPEdlutGrpcEngine")
set(HEADER_DIRECTORY "edlut_grpc_engine")

set(NAMESPACE_NAME "${PROJECT_NAME}")

set(LIBRARY_NAME "${PROJECT_NAME}")
set(PYTHON_MODULE_NAME "edlut_grpc")
set(EXECUTABLE_NAME "NRPEdlutEngineExecutable")
set(EXECUTABLE_ASYNC_NAME "EdlutEngineAsync")
set(TEST_NAME "${PROJECT_NAME}Tests")

set(LIB_EXPORT_NAME "${LIBRARY_NAME}Targets")
set(LIB_CONFIG_NAME "${LIBRARY_NAME}Config")
set(LIB_VERSION_NAME "${LIB_CONFIG_NAME}Version")
set(ENV{NRP_ENGINE_LAUNCHERS} "${LIBRARY_NAME}.so;$ENV{NRP_ENGINE_LAUNCHERS}")

cmake_minimum_required(VERSION 3.16)
project("${PROJECT_NAME}" VERSION 1.0.0)

include(GNUInstallDirs)
include(ExternalProject)

# List library build files
set(LIB_SRC_FILES
    edlut_grpc_engine/engine_client/edlut_grpc_client.cpp
)
if(BUILD_EDLUT_ENGINE_SERVER)
    list(APPEND LIB_SRC_FILES
        edlut_grpc_engine/engine_server/edlut_grpc_server.cpp
        edlut_grpc_engine/engine_server/edlut_spikes_datapack_controller.cpp
        edlut_grpc_engine/engine_server/edlut_currents_datapack_controller.cpp)
endif()

# List of Python module build files
set(PYTHON_MODULE_SRC_FILES
)

# List executable build files
if(BUILD_EDLUT_ENGINE_SERVER)
    set(EXEC_SRC_FILES
    edlut_grpc_engine/server_executable/main.cpp
    )

    set(EXEC_ASYNC_SRC_FILES
            edlut_grpc_engine/server_executable/main_async.cpp
            )
endif()

# List testing build files
set(TEST_SRC_FILES
)


##########################################
## Dependencies
if(BUILD_EDLUT_ENGINE_SERVER)
    message("Configuring EDLUT...")
    #If EDLUT is already installed FetchContent does not download it again
    FetchContent_Declare(edlut_sim
            GIT_REPOSITORY https://github.com/EduardoRosLab/edlut.git
            GIT_TAG 74220e77600b7fe7c233199e08e1627769068682
            )
    FetchContent_GetProperties(edlut_sim)
    if(NOT edlut_sim_POPULATED)
        FetchContent_Populate(edlut_sim)
        set(BUILD_SHARED_LIBS ON CACHE INTERNAL "Build EDLUT library shared")
        set(with-python "3" CACHE STRING "Enable Python binding. Optional: set Python version (2 or 3). [default=ON]")
        if(EDLUT_WITH_CUDA)
            set(with-cuda ON CACHE INTERNAL "Build EDLUT with cuda")
        endif()
        add_subdirectory(${edlut_sim_SOURCE_DIR}/Kernel ${edlut_sim_BINARY_DIR})
    endif()

endif()

##########################################
## Header configuration

# General Header defines
set(NRP_EDLUT_EXECUTABLE ${EXECUTABLE_NAME})
configure_file("edlut_grpc_engine/config/cmake_constants.h.in" "${CMAKE_CURRENT_BINARY_DIR}/include/${HEADER_DIRECTORY}/config/cmake_constants.h" @ONLY)

# Python module dependencies
#configure_file("nrp_example_engine/python/__init__.py.in" "${CMAKE_CURRENT_BINARY_DIR}/src/__init__.py" @ONLY)

##########################################
## NRPEdlutEngineLibrary
add_library("${LIBRARY_NAME}" SHARED ${LIB_SRC_FILES})
add_library(${NAMESPACE_NAME}::${LIBRARY_NAME} ALIAS ${LIBRARY_NAME})
target_compile_options(${LIBRARY_NAME} PUBLIC $<$<OR:$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_ID:GNU>>:${NRP_COMMON_COMPILATION_FLAGS}>)
target_compile_options(${LIBRARY_NAME} PUBLIC $<$<CXX_COMPILER_ID:GNU>:-fconcepts>)

set_target_properties(${LIBRARY_NAME} PROPERTIES PREFIX "")

target_link_libraries(${LIBRARY_NAME}
    PUBLIC
        ${NRP_GEN_LIB_TARGET}
        NRPGRPCEngineProtocol::NRPGRPCEngineProtocol
        NRPProtobuf::ProtoEdlutData
        ${NRP_PROTO_BINDINGS_LIB_TARGET}

    PRIVATE
)


target_include_directories(${LIBRARY_NAME} BEFORE
    PUBLIC 
        "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>"
        "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>"
        "$<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/include>"

    PRIVATE
)

if(BUILD_EDLUT_ENGINE_SERVER)
    target_link_libraries(${LIBRARY_NAME} PUBLIC ${edlut_BINARY_DIR}/libedlut.so)
    target_include_directories(${LIBRARY_NAME} BEFORE PUBLIC "$<BUILD_INTERFACE:${edlut_sim_SOURCE_DIR}/Kernel/include>")
endif()


##########################################
## edlut_engine
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
## NRPEdlutServerExecutable
if(NOT "${EXEC_SRC_FILES}" STREQUAL "")
    add_executable(${EXECUTABLE_NAME} ${EXEC_SRC_FILES})
    target_link_libraries(${EXECUTABLE_NAME} ${LIBRARY_NAME})
endif()

if(NOT "${EXEC_ASYNC_SRC_FILES}" STREQUAL "" AND ENABLE_MQTT)
    add_executable(${EXECUTABLE_ASYNC_NAME} ${EXEC_ASYNC_SRC_FILES})
    target_link_libraries(${EXECUTABLE_ASYNC_NAME}
            ${LIBRARY_NAME}
            ${NRP_ELE_TARGET}
            )
endif()

##########################################
## Tests
if(${ENABLE_TESTING} AND NOT "${TEST_SRC_FILES}" STREQUAL "")
    # Create testing env files
    configure_file("${CMAKE_CURRENT_SOURCE_DIR}/tests/test_files/test_env.sh.in" "${CMAKE_CURRENT_BINARY_DIR}/test_env.sh" @ONLY)
    configure_file("${CMAKE_CURRENT_SOURCE_DIR}/tests/test_env_cmake.h.in" "${CMAKE_CURRENT_BINARY_DIR}/include/tests/test_env_cmake.h" @ONLY)

    # Create testing executable
    enable_testing()
    add_executable(${TEST_NAME} ${TEST_SRC_FILES})
    target_link_libraries(${TEST_NAME}
        PUBLIC
        ${NAMESPACE_NAME}::${LIBRARY_NAME}
        GTest::gtest
        GTest::gtest_main)

    gtest_discover_tests(${TEST_NAME}
        WORKING_DIRECTORY "${PROJECT_SOURCE_DIR}/tests"
        EXTRA_ARGS -VV)
endif()


##########################################
## Installation

set(INSTALL_CONFIGDIR "${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}")

# Install library files
install(TARGETS
        ${LIBRARY_NAME}
    EXPORT
        ${LIB_EXPORT_NAME}
    LIBRARY DESTINATION ${NRP_PLUGIN_INSTALL_DIR}
    ARCHIVE DESTINATION ${NRP_PLUGIN_INSTALL_DIR}
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

# Install Python module
if(TARGET ${PYTHON_MODULE_NAME})
    install(TARGETS ${PYTHON_MODULE_NAME}
        DESTINATION "${PYTHON_INSTALL_DIR_REL}/${NRP_PYTHON_MODULE_NAME}/engines/${PYTHON_MODULE_NAME}")

    install(FILES "${CMAKE_CURRENT_BINARY_DIR}/src/__init__.py"
        DESTINATION "${PYTHON_INSTALL_DIR_REL}/${NRP_PYTHON_MODULE_NAME}/engines/${PYTHON_MODULE_NAME}")
endif()

# Install executable files
if(TARGET ${EXECUTABLE_NAME})
    install(TARGETS ${EXECUTABLE_NAME}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})
endif()

if(TARGET ${EXECUTABLE_ASYNC_NAME})
    install(TARGETS ${EXECUTABLE_ASYNC_NAME}
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
