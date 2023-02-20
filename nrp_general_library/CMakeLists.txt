set(PROJECT_NAME "NRPGeneralLibrary")
set(HEADER_DIRECTORY "nrp_general_library")

set(NAMESPACE_NAME "${PROJECT_NAME}")

set(LIBRARY_NAME "${PROJECT_NAME}")
set(PYTHON_MODULE_NAME "nrp_core")
set(EXECUTABLE_NAME "${PROJECT_NAME}Main")
set(TEST_NAME "${PROJECT_NAME}Tests")

set(LIB_EXPORT_NAME "${LIBRARY_NAME}Targets")
set(LIB_CONFIG_NAME "${LIBRARY_NAME}Config")
set(LIB_VERSION_NAME "${LIB_CONFIG_NAME}Version")

cmake_minimum_required(VERSION 3.16)
project("${PROJECT_NAME}" VERSION ${NRP_VERSION})

include(GNUInstallDirs)

# List library build files
set(LIB_SRC_FILES
    nrp_general_library/datapack_interface/datapack_interface.cpp
    nrp_general_library/datapack_interface/datapack.cpp
    nrp_general_library/engine_interfaces/datapack_controller.cpp
    nrp_general_library/engine_interfaces/engine_client_interface.cpp
    nrp_general_library/engine_interfaces/engine_launcher_manager.cpp
    nrp_general_library/plugin_system/plugin.cpp
    nrp_general_library/plugin_system/plugin_manager.cpp
    nrp_general_library/process_launchers/launch_commands/basic_fork.cpp
    nrp_general_library/process_launchers/launch_commands/launch_command.cpp
    nrp_general_library/process_launchers/process_launcher.cpp
    nrp_general_library/process_launchers/process_launcher_basic.cpp
    nrp_general_library/process_launchers/process_launcher_manager.cpp
    nrp_general_library/transceiver_function/from_engine_datapack.cpp
    nrp_general_library/transceiver_function/status_function.cpp
    nrp_general_library/transceiver_function/transceiver_function.cpp
    nrp_general_library/transceiver_function/transceiver_datapack_interface.cpp
    nrp_general_library/transceiver_function/function_manager.cpp
    nrp_general_library/utils/file_finder.cpp
    nrp_general_library/utils/fixed_string.cpp
    nrp_general_library/utils/json_converter.cpp
    nrp_general_library/utils/json_schema_utils.cpp
    nrp_general_library/utils/nrp_exceptions.cpp
    nrp_general_library/utils/nrp_logger.cpp
    nrp_general_library/utils/ptr_templates.cpp
    nrp_general_library/utils/python_error_handler.cpp
    nrp_general_library/utils/python_interpreter_state.cpp
    nrp_general_library/utils/restclient_setup.cpp
    nrp_general_library/utils/time_utils.cpp
    nrp_general_library/utils/wchar_t_converter.cpp
    nrp_general_library/utils/zip_container.cpp
    nrp_general_library/utils/pipe_communication.cpp
    nrp_general_library/process_launchers/launch_commands/docker_launcher.cpp
)

# List of python module build files
set(PYTHON_MODULE_SRC_FILES
    nrp_general_library/python/python_module.cpp
)

# List executable build files
set(EXEC_SRC_FILES
)

# List testing build files
set(TEST_SRC_FILES
    tests/datapack_interface.cpp
    tests/datapack.cpp
    tests/engine_launcher_manager.cpp
    tests/plugin_manager.cpp
    tests/test_process_launcher_basic.cpp
    tests/test_function_manager.cpp
)

##########################################
## Dependencies

set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
include(FetchContent)

# cxxopts: Start Parameter Processing
find_package(cxxopts 2.2 QUIET)
if(NOT ${cxxopts_FOUND})
    message("Please wait. Downloading cxxopts...")
    ExternalProject_Add(cxxopts
        GIT_REPOSITORY https://github.com/jarro2783/cxxopts.git
        GIT_TAG v2.2.1
        INSTALL_DIR ${NRP_DEP_CMAKE_INSTALL_PREFIX}
        CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=${NRP_DEP_CMAKE_INSTALL_PREFIX})

    set(CXXOPTS_BUILD_EXAMPLES OFF)
    set(CXXOPTS_BUILD_TESTS OFF)

endif()

# restclient-cpp: REST client library
find_package(restclient-cpp 0.5.2 EXACT QUIET PATHS ${NRP_DEP_CMAKE_INSTALL_PREFIX} NO_DEFAULT_PATH)
if(NOT ${restclient-cpp_FOUND})
    message("Please wait. Downloading restclient-cpp...")

    ExternalProject_Add(restclient-cpp
            GIT_REPOSITORY https://github.com/mrtazz/restclient-cpp.git
            GIT_TAG 0.5.2
            INSTALL_DIR ${NRP_DEP_CMAKE_INSTALL_PREFIX}
            CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=${NRP_DEP_CMAKE_INSTALL_PREFIX} -Dwith-mpi=ON -Dwith-python=ON)

endif()

# pistache: REST server library
find_package(PkgConfig)
pkg_check_modules(Pistache REQUIRED IMPORTED_TARGET libpistache)

# nlohmann_json: JSON library
find_package(nlohmann_json REQUIRED)

# nlohmann_json schema validator
find_package(nlohmann_json_schema_validator 2.1.0 EXACT QUIET PATHS ${NRP_DEP_CMAKE_INSTALL_PREFIX} NO_DEFAULT_PATH)
if(NOT ${nlohmann_json_schema_validator_FOUND})
    message("Please wait. Downloading nlohmann_json_schema_validator...")
    ExternalProject_Add(nlohmann_json_schema_validator
            GIT_REPOSITORY https://github.com/pboettch/json-schema-validator.git
            GIT_TAG 2.1.0
            INSTALL_DIR ${NRP_DEP_CMAKE_INSTALL_PREFIX}
            CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=${NRP_DEP_CMAKE_INSTALL_PREFIX} -DBUILD_SHARED_LIBS=ON)


    set(BUILD_TESTS OFF)

endif()

## Log library
find_package(spdlog 1.11.0 EXACT QUIET PATHS ${NRP_DEP_CMAKE_INSTALL_PREFIX} NO_DEFAULT_PATH)
if(NOT ${spdlog_FOUND})
    message("Please wait. Downloading spdlog...")
    ExternalProject_Add(spdlog
            GIT_REPOSITORY https://github.com/gabime/spdlog.git
            GIT_TAG v1.11.0
            INSTALL_DIR ${NRP_DEP_CMAKE_INSTALL_PREFIX}
            CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=${NRP_DEP_CMAKE_INSTALL_PREFIX} -DSPDLOG_BUILD_SHARED=ON)



    set(SPDLOG_BUILD_SHARED ON)
    set(SPDLOG_INSTALL ON)

endif()

# libzip: ZIP Manipulation
find_package(PkgConfig REQUIRED)
pkg_check_modules(LibZip REQUIRED IMPORTED_TARGET libzip)


##########################################
## Header configuration

set(NRP_CLEAR_ENV "NRP_CLEAR_ENV")
set(NRP_CONFIG_DIR "config_schemas")
set(NRP_GEN_LIB_TARGET "${NAMESPACE_NAME}::${LIBRARY_NAME}" CACHE INTERNAL "NRP General Library target")
set(NRP_PLUGIN_INSTALL_DIR "${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_LIBDIR}" CACHE INTERNAL "NRP Plugin Installation Directory")

# General Header defines
configure_file("nrp_general_library/config/cmake_constants.h.in" "${CMAKE_CURRENT_BINARY_DIR}/include/${HEADER_DIRECTORY}/config/cmake_constants.h" @ONLY)

# Python Module defines
configure_file("nrp_general_library/python/__init__.py.in" "${CMAKE_CURRENT_BINARY_DIR}/src/__init__.py" @ONLY)

# Process startup script
configure_file("cmake/set_env_exec.sh.in" "${CMAKE_CURRENT_BINARY_DIR}/set_env_exec.sh" @ONLY)

# add to nrp env vars
list(APPEND NRP_ENV_VARS "PATH=${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_BINDIR}:\${PATH}")
list(APPEND NRP_ENV_VARS "LD_LIBRARY_PATH=${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_LIBDIR}:\${LD_LIBRARY_PATH}")
list(APPEND NRP_ENV_VARS "PATH=${NRP_DEP_CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_BINDIR}:\${PATH}")
list(APPEND NRP_ENV_VARS "LD_LIBRARY_PATH=${NRP_DEP_CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_LIBDIR}:\${LD_LIBRARY_PATH}")
set(NRP_ENV_VARS ${NRP_ENV_VARS} PARENT_SCOPE)


##########################################
# NRPGeneralLibrary
add_library("${LIBRARY_NAME}" SHARED ${LIB_SRC_FILES})
add_library(${NAMESPACE_NAME}::${LIBRARY_NAME} ALIAS ${LIBRARY_NAME})
target_compile_options(${LIBRARY_NAME} PUBLIC $<$<OR:$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_ID:GNU>>:${NRP_COMMON_COMPILATION_FLAGS}>)

add_library(lib_restclient STATIC IMPORTED )
set_property(TARGET lib_restclient PROPERTY
        IMPORTED_LOCATION "${NRP_DEP_CMAKE_INSTALL_PREFIX}/lib/librestclient-cpp.so")

add_library(lib_nlohmann_json_schema_validator STATIC IMPORTED )
set_property(TARGET lib_nlohmann_json_schema_validator PROPERTY
        IMPORTED_LOCATION "${NRP_DEP_CMAKE_INSTALL_PREFIX}/lib/libnlohmann_json_schema_validator.so")

add_library(lib_spdlog STATIC IMPORTED )
set_property(TARGET lib_spdlog PROPERTY
        IMPORTED_LOCATION "${NRP_DEP_CMAKE_INSTALL_PREFIX}/lib/libspdlog.so")

if(NOT ${cxxopts_FOUND})
    add_dependencies(${LIBRARY_NAME} cxxopts)
endif()

if(NOT ${restclient-cpp_FOUND})
    add_dependencies(${LIBRARY_NAME} restclient-cpp)
endif()

if(NOT ${nlohmann_json_schema_validator_FOUND})
    add_dependencies(${LIBRARY_NAME} nlohmann_json_schema_validator)
endif()

if(NOT ${spdlog_FOUND})
    add_dependencies(${LIBRARY_NAME} spdlog)
endif()

target_link_options(${LIBRARY_NAME} PUBLIC ${NRP_COMMON_LD_FLAGS})
target_link_libraries(${LIBRARY_NAME}
    PUBLIC
        ${BOOST_PYTHON}
        Boost::filesystem
        nlohmann_json::nlohmann_json
        PkgConfig::Pistache
        PkgConfig::LibZip
        Python3::Module
        Python3::Python
        lib_nlohmann_json_schema_validator
        lib_restclient
        lib_spdlog
        rt

    PRIVATE
)

target_include_directories(${LIBRARY_NAME} BEFORE
    PUBLIC
        "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>"
        "$<INSTALL_INTERFACE:${NRP_DEP_CMAKE_INSTALL_PREFIX}/include>"
        "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>"
        "$<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/include>"
        "$<BUILD_INTERFACE:${NRP_DEP_CMAKE_INSTALL_PREFIX}/include>"


    PRIVATE
)


##########################################
## NRPGeneralPythonModule
set(NRP_PYTHON_MODULE_NAME "${PYTHON_MODULE_NAME}" CACHE INTERNAL "NRP Python module name")

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
    target_link_libraries(${EXECUTABLE_NAME} ${LIBRARY_NAME})
endif()


##########################################
## Append Doxygen files
add_doxygen_source("${CMAKE_CURRENT_SOURCE_DIR}" ${EXEC_SRC_FILES} ${LIB_SRC_FILES} ${PYTHON_MODULE_SRC_FILES} "${HEADER_DIRECTORY}")


##########################################
## Tests
if(${ENABLE_TESTING} AND NOT "${TEST_SRC_FILES}" STREQUAL "")
    # Python Module for testing
    set(TEST_PYTHON_MODULE_NAME "NRPGeneralPythonTestModule")

    add_library(${TEST_PYTHON_MODULE_NAME} SHARED "tests/test_transceiver_function_manager.cpp")
    set_target_properties(${TEST_PYTHON_MODULE_NAME} PROPERTIES PREFIX "")
    target_link_libraries(${TEST_PYTHON_MODULE_NAME} ${NAMESPACE_NAME}::${LIBRARY_NAME})

    # Create testing process executable (used for testing ProcessLaunchers)
    set(TEST_NRP_PROCESS_EXEC "TestNRPProcessExec")
    add_executable(${TEST_NRP_PROCESS_EXEC} tests/test_process_exec.cpp)
    target_link_options(${TEST_NRP_PROCESS_EXEC} PUBLIC ${NRP_COMMON_LD_FLAGS})
    target_link_libraries(${TEST_NRP_PROCESS_EXEC}
        PUBLIC
        ${NAMESPACE_NAME}::${LIBRARY_NAME})

    # Create testing plugin library (used for testing PluginManager)
    set(TEST_NRP_PLUGIN "TestNRPPluginManager")
    add_library(${TEST_NRP_PLUGIN} SHARED tests/test_plugin.cpp)
    set_target_properties(${TEST_NRP_PLUGIN} PROPERTIES PREFIX "")
    target_link_options(${TEST_NRP_PLUGIN} PUBLIC ${NRP_COMMON_LD_FLAGS})
    target_link_libraries(${TEST_NRP_PLUGIN}
        PUBLIC
        ${NAMESPACE_NAME}::${LIBRARY_NAME})

    # Create testing env files
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
        GTest::GTest
        GTest::Main
        ${TEST_PYTHON_MODULE_NAME})

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

# Install configuration schemas
install(DIRECTORY "${CMAKE_SOURCE_DIR}/${NRP_CONFIG_DIR}"
        DESTINATION
        ${CMAKE_INSTALL_LIBDIR}
        FILES_MATCHING
        PATTERN "*.json"
        )

# Install python module
if(TARGET ${PYTHON_MODULE_NAME})
    install(TARGETS ${PYTHON_MODULE_NAME}
        DESTINATION "${PYTHON_INSTALL_DIR_REL}/${PYTHON_MODULE_NAME}")

    install(FILES "${CMAKE_CURRENT_BINARY_DIR}/src/__init__.py"
        DESTINATION "${PYTHON_INSTALL_DIR_REL}/${PYTHON_MODULE_NAME}")
endif()
install(FILES "${HEADER_DIRECTORY}/python/docker_handle.py"
    DESTINATION "${PYTHON_INSTALL_DIR_REL}/${PYTHON_MODULE_NAME}")
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

# Install env set script
install(PROGRAMS "${CMAKE_CURRENT_BINARY_DIR}/set_env_exec.sh"
    DESTINATION ${CMAKE_INSTALL_SBINDIR})
