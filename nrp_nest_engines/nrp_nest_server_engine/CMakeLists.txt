set(PROJECT_NAME "NRPNestServerEngine")
set(HEADER_DIRECTORY "nrp_nest_server_engine")

set(NAMESPACE_NAME "${PROJECT_NAME}")

set(LIBRARY_NAME "${PROJECT_NAME}")
set(PYTHON_MODULE_NAME "nest_server")
set(EXECUTABLE_NAME "NRPNestServerExecutable")
set(TEST_NAME "${PROJECT_NAME}Tests")

set(LIB_EXPORT_NAME "${LIBRARY_NAME}Targets")
set(LIB_CONFIG_NAME "${LIBRARY_NAME}Config")
set(LIB_VERSION_NAME "${LIB_CONFIG_NAME}Version")
set(ENV{NRP_ENGINE_LAUNCHERS} "${LIBRARY_NAME}.so;$ENV{NRP_ENGINE_LAUNCHERS}")

cmake_minimum_required(VERSION 3.16)
project("${PROJECT_NAME}" VERSION ${NRP_VERSION})

include(GNUInstallDirs)
include(ExternalProject)

# List library build files
set(LIB_SRC_FILES
    nrp_nest_server_engine/nrp_client/nest_engine_server_nrp_client.cpp
    nrp_nest_server_engine/python/create_datapack_class.cpp
    nrp_nest_server_engine/config/nest_server_config.cpp
)

# List of python module build files
set(PYTHON_MODULE_SRC_FILES
    nrp_nest_server_engine/python/nrp_nest_python_module.cpp
)

# List executable build files
if(BUILD_NEST_ENGINE_SERVER)
    set(EXEC_SRC_FILES
    )
endif()

# List testing build files
if(BUILD_NEST_ENGINE_SERVER)
    set(TEST_SRC_FILES
    )
endif()


##########################################
## Dependencies

##########################################
## Header configuration

# General Nest Header defines
set(NEST_CMAKE_HEADER_CONFIG_FILE "nrp_nest_server_engine/config/cmake_constants.h.in")
configure_file(${NEST_CMAKE_HEADER_CONFIG_FILE} "${CMAKE_CURRENT_BINARY_DIR}/include/${HEADER_DIRECTORY}/config/cmake_constants.h" @ONLY)

# Python module dependencies
configure_file("nrp_nest_server_engine/python/__init__.py.in" "${CMAKE_CURRENT_BINARY_DIR}/src/__init__.py" @ONLY)

##########################################
# NRPNestDataPackInterface
add_library("${LIBRARY_NAME}" SHARED ${LIB_SRC_FILES})
add_library(${NAMESPACE_NAME}::${LIBRARY_NAME} ALIAS ${LIBRARY_NAME})
target_compile_options(${LIBRARY_NAME} PUBLIC $<$<OR:$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_ID:GNU>>:${NRP_COMMON_COMPILATION_FLAGS}>)
target_compile_options(${LIBRARY_NAME} PUBLIC $<$<CXX_COMPILER_ID:GNU>:-fconcepts>)

set_target_properties(${LIBRARY_NAME} PROPERTIES PREFIX "")

target_link_libraries(${LIBRARY_NAME}
    PUBLIC
        NRPJSONEngineProtocol::NRPJSONEngineProtocol

    PRIVATE
)

target_include_directories(${LIBRARY_NAME} BEFORE
    PUBLIC 
        "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>"
        "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>"
        "$<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/include>"

    PRIVATE
)


##########################################
## NRPNestPythonModule
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
## NRPNestExecutable
if(NOT "${EXEC_SRC_FILES}" STREQUAL "")
    add_executable(${EXECUTABLE_NAME} ${EXEC_SRC_FILES})
    target_link_libraries(${EXECUTABLE_NAME} ${LIBRARY_NAME})
endif()


##########################################
## Append Doxygen files
add_doxygen_source("${CMAKE_CURRENT_SOURCE_DIR}" ${EXEC_SRC_FILES} ${LIB_SRC_FILES} ${PYTHON_MODULE_SRC_FILES} ${HEADER_DIRECTORY})


##########################################
## Tests
if(BUILD_NEST_ENGINE_SERVER AND ${ENABLE_TESTING} AND NOT "${TEST_SRC_FILES}" STREQUAL "")

    # Create testing executable
    enable_testing()
    add_executable(${TEST_NAME} ${TEST_SRC_FILES})
    target_compile_options(${TEST_NAME} PUBLIC $<$<OR:$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_ID:GNU>>:${NRP_COMMON_COMPILATION_FLAGS}>)
    target_link_options(${TEST_NAME} PUBLIC ${NRP_COMMON_LD_FLAGS})
    target_link_libraries(${TEST_NAME}
        PUBLIC
        ${NAMESPACE_NAME}::${LIBRARY_NAME}
        GTest::GTest
        GTest::Main)

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

# Patch nest-server to use package manager's uwsgi plugin system
if(BUILD_NEST_ENGINE_SERVER)
    install(CODE "execute_process(COMMAND sed -i 's/uwsgi --module nest\.server:app/ uwsgi --plugin python --module nest.server:app/' nest-server
        WORKING_DIRECTORY '${NEST_CMAKE_INSTALL_BINDIR}')")
endif()

# Install python module
if(TARGET ${PYTHON_MODULE_NAME})
    install(TARGETS ${PYTHON_MODULE_NAME}
        DESTINATION "${PYTHON_INSTALL_DIR_REL}/${NRP_PYTHON_MODULE_NAME}/engines/${PYTHON_MODULE_NAME}")

    install(FILES "${CMAKE_CURRENT_BINARY_DIR}/src/__init__.py"
        DESTINATION "${PYTHON_INSTALL_DIR_REL}/${NRP_PYTHON_MODULE_NAME}/engines/${PYTHON_MODULE_NAME}")

    install(FILES "${CMAKE_CURRENT_SOURCE_DIR}/nrp_nest_server_engine/python/numpy_json_serializer.py"
        DESTINATION "${PYTHON_INSTALL_DIR_REL}/${NRP_PYTHON_MODULE_NAME}/engines/${PYTHON_MODULE_NAME}")

    install(FILES "${CMAKE_CURRENT_SOURCE_DIR}/nrp_nest_server_engine/python/brain_devices.py"
        DESTINATION "${PYTHON_INSTALL_DIR_REL}/${NRP_PYTHON_MODULE_NAME}/engines/${PYTHON_MODULE_NAME}")
endif()

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
