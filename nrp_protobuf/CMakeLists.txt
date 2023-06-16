set(PROJECT_NAME "NRPProtobuf")
set(HEADER_DIRECTORY "nrp_protobuf")

set(NAMESPACE_NAME "${PROJECT_NAME}")

set(LIBRARY_NAME "${PROJECT_NAME}")
set(PYTHON_MODULE_NAME "nrp_protobuf")
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
    nrp_protobuf/proto_python_bindings/proto_field_ops.cpp
    nrp_protobuf/proto_python_bindings/repeated_field_proxy.cpp
    nrp_protobuf/proto_python_bindings/proto_python_bindings.cpp
)

# List of python module build files
set(PYTHON_MODULE_SRC_FILES
)
# List of python module configuration files
set(PYTHON_MODULE_ROOT_FILES
    python_module/setup.py
    python_module/pyproject.toml
)
set(PYTHON_MODULE_CMAKE_ROOT_DIR
    ${CMAKE_CURRENT_BINARY_DIR}/py-${PYTHON_MODULE_NAME}
)
set(PYTHON_MODULE_CMAKE_SRC_DIR
    ${PYTHON_MODULE_CMAKE_ROOT_DIR}/${PYTHON_MODULE_NAME}
)

# List executable build files
set(EXEC_SRC_FILES
)

# List testing build files
set(TEST_SRC_FILES
)


##########################################
## Dependencies

find_package(PkgConfig REQUIRED)

find_package(
        Python3
        REQUIRED
        COMPONENTS Interpreter
)

# Protobuf: google's Protocol Buffers library
pkg_check_modules(PROTOBUF REQUIRED protobuf)
message(STATUS "Using protobuf ${PROTOBUF_VERSION}")

# gRPC: google's Remote Procedure Calls library
pkg_check_modules(GRPC REQUIRED grpc++)
message(STATUS "Using gRPC ${GRPC_VERSION}")

# Protobuf compiler
find_program(PROTOC protoc)
message(STATUS "Using protoc ${PROTOC}")

# gRPC cpp plugin
find_program(GRPC_CPP_PLUGIN grpc_cpp_plugin)
message(STATUS "Using gRPC cpp plugin ${GRPC_CPP_PLUGIN}")

##########################################
## Functions for automatizing protobuf code generation and compilation tasks
# Generates C++ protobuf code from file PROTO_FILE_NAME. Generated files are appended to var PROTO_SRC_FILES_VAR
function(GENERATE_PROTO_CPP PROTO_FILE_NAME PROTO_SRC_FILES_VAR)

    # List of protobuf proto files
    get_filename_component(hw_proto "${PROTO_FILE_NAME}" ABSOLUTE)
    get_filename_component(hw_proto_name "${hw_proto}" NAME_WE)

    # Generated sources
    set(proto_srcs "${CMAKE_CURRENT_BINARY_DIR}/include/${HEADER_DIRECTORY}/${hw_proto_name}.pb.cc")
    set(proto_hdrs "${CMAKE_CURRENT_BINARY_DIR}/include/${HEADER_DIRECTORY}/${hw_proto_name}.pb.h")

    # Generate
    add_custom_command(
        OUTPUT "${proto_srcs}" "${proto_hdrs}"
        COMMAND "mkdir" ARGS -p "${CMAKE_CURRENT_BINARY_DIR}/include/${HEADER_DIRECTORY}"
        COMMAND ${PROTOC}
        ARGS --cpp_out "${CMAKE_CURRENT_BINARY_DIR}/include/${HEADER_DIRECTORY}"
            --proto_path "${NRP_CORE_MSGS_PROTO_PATH}/nrp_proto_defs"
            --proto_path "${NRP_CORE_MSGS_PROTO_PATH}/engine_proto_defs"
            "${hw_proto}"
        DEPENDS "${hw_proto}")

    set(${PROTO_SRC_FILES_VAR} ${${PROTO_SRC_FILES_VAR}} ${proto_srcs} PARENT_SCOPE)

endfunction()

# Generates C++ grpc code from file PROTO_FILE_NAME. Generated files are appended to var PROTO_SRC_FILES_VAR
function(GENERATE_GRPC_CPP PROTO_FILE_NAME PROTO_SRC_FILES_VAR)

    # List of protobuf proto files
    get_filename_component(hw_proto "${PROTO_FILE_NAME}" ABSOLUTE)
    get_filename_component(hw_proto_name "${hw_proto}" NAME_WE)

    # Generated sources
    set(grpc_srcs "${CMAKE_CURRENT_BINARY_DIR}/include/${HEADER_DIRECTORY}/${hw_proto_name}.grpc.pb.cc")
    set(grpc_hdrs "${CMAKE_CURRENT_BINARY_DIR}/include/${HEADER_DIRECTORY}/${hw_proto_name}.grpc.pb.h")

    # Generate
    add_custom_command(
            OUTPUT "${grpc_srcs}" "${grpc_hdrs}"
            COMMAND "mkdir" ARGS -p "${CMAKE_CURRENT_BINARY_DIR}/include/${HEADER_DIRECTORY}"
            COMMAND ${PROTOC}
            ARGS --grpc_out "${CMAKE_CURRENT_BINARY_DIR}/include/${HEADER_DIRECTORY}"
            --proto_path "${NRP_CORE_MSGS_PROTO_PATH}/nrp_proto_defs"
            --proto_path "${NRP_CORE_MSGS_PROTO_PATH}/engine_proto_defs"
            --plugin=protoc-gen-grpc="${GRPC_CPP_PLUGIN}"
            "${hw_proto}"
            DEPENDS "${hw_proto}")

    set(${PROTO_SRC_FILES_VAR} ${${PROTO_SRC_FILES_VAR}} ${grpc_srcs} PARENT_SCOPE)

endfunction()

# Generates Python proto code from file PROTO_FILE_NAME. Generated files are appended to var PROTO_PYTHON_FILES_VAR
function(GENERATE_PROTO_PYTHON PROTO_FILE_NAME PROTO_PYTHON_FILES_VAR)
    # List of protobuf proto files
    get_filename_component(hw_proto "${PROTO_FILE_NAME}" ABSOLUTE)
    get_filename_component(hw_proto_name "${hw_proto}" NAME_WE)

    # Generated sources
    set(proto_python "${PYTHON_MODULE_CMAKE_SRC_DIR}/${hw_proto_name}_pb2.py")

    # Generate
    add_custom_command(
            OUTPUT "${proto_python}"
            COMMAND "mkdir" ARGS -p "${PYTHON_MODULE_CMAKE_SRC_DIR}"
            COMMAND python3
            ARGS -m grpc_tools.protoc
            --proto_path "${NRP_CORE_MSGS_PROTO_PATH}/nrp_proto_defs"
            --python_out "${PYTHON_MODULE_CMAKE_SRC_DIR}/"
            "${hw_proto}"
            COMMAND sed -i 's@^import .*_pb2 as@from . \\0@' ${proto_python}
            COMMENT "Generating Python proto file ${proto_python}"
            DEPENDS "${hw_proto}")

    set(${PROTO_PYTHON_FILES_VAR} ${${PROTO_PYTHON_FILES_VAR}} ${proto_python} PARENT_SCOPE)

endfunction()

# Generates Python grpc code from file PROTO_FILE_NAME. Generated files are appended to var PROTO_PYTHON_FILES_VAR
function(GENERATE_GRPC_PYTHON PROTO_FILE_NAME PROTO_PYTHON_FILES_VAR)
    # List of protobuf proto files
    get_filename_component(hw_proto "${PROTO_FILE_NAME}" ABSOLUTE)
    get_filename_component(hw_proto_name "${hw_proto}" NAME_WE)

    # Generated sources
    set(proto_python "${PYTHON_MODULE_CMAKE_SRC_DIR}/${hw_proto_name}_pb2_grpc.py")

    # Generate
    add_custom_command(
            OUTPUT "${proto_python}"
            COMMAND "mkdir" ARGS -p "${PYTHON_MODULE_CMAKE_SRC_DIR}/"
            COMMAND python3
            ARGS -m grpc_tools.protoc
            --proto_path "${NRP_CORE_MSGS_PROTO_PATH}/nrp_proto_defs"
            --grpc_python_out "${PYTHON_MODULE_CMAKE_SRC_DIR}/"
            "${hw_proto}"
            COMMAND sed -i 's@^import .*_pb2 as@from . \\0@' ${proto_python}
            COMMENT "Generating Python gRPC file ${proto_python}"
            DEPENDS "${hw_proto}" )

    set(${PROTO_PYTHON_FILES_VAR} ${${PROTO_PYTHON_FILES_VAR}} ${proto_python} PARENT_SCOPE)

endfunction()

##########################################
## Protobuf code generation

# Proto generation
set(PROTO_SRC_FILES "")
set(PROTO_PYTHON_FILES_SRC "")

generate_proto_cpp("${NRP_CORE_MSGS_PROTO_PATH}/nrp_proto_defs/engine_grpc.proto" PROTO_SRC_FILES)
generate_grpc_cpp("${NRP_CORE_MSGS_PROTO_PATH}/nrp_proto_defs/engine_grpc.proto" PROTO_SRC_FILES)
generate_proto_python("${NRP_CORE_MSGS_PROTO_PATH}/nrp_proto_defs/engine_grpc.proto" PROTO_PYTHON_FILES_SRC)
generate_grpc_python("${NRP_CORE_MSGS_PROTO_PATH}/nrp_proto_defs/engine_grpc.proto" PROTO_PYTHON_FILES_SRC)

generate_proto_cpp("${NRP_CORE_MSGS_PROTO_PATH}/nrp_proto_defs/nrp_server.proto" PROTO_SRC_FILES)
generate_grpc_cpp("${NRP_CORE_MSGS_PROTO_PATH}/nrp_proto_defs/nrp_server.proto" PROTO_SRC_FILES)
generate_proto_python("${NRP_CORE_MSGS_PROTO_PATH}/nrp_proto_defs/nrp_server.proto" PROTO_PYTHON_FILES_SRC)
generate_grpc_python("${NRP_CORE_MSGS_PROTO_PATH}/nrp_proto_defs/nrp_server.proto" PROTO_PYTHON_FILES_SRC)


####################################
## Python protobuf module
# Module setuptools configuration file
configure_file("python_module/setup.cfg.in" "${PYTHON_MODULE_CMAKE_ROOT_DIR}/setup.cfg" @ONLY)
file(COPY ${PYTHON_MODULE_ROOT_FILES} DESTINATION "${PYTHON_MODULE_CMAKE_ROOT_DIR}/")

# add_subdirectory(python_module)

# Forces the generation of proto and grpc python code
add_custom_target(
        FORCE_PROTO_PYTHON
        COMMAND bash -c "echo \"Generating proto python files\""
        DEPENDS ${PROTO_PYTHON_FILES_SRC})

##########################################
# NRPProtobuf
add_library("${LIBRARY_NAME}" SHARED ${PROTO_SRC_FILES} nrp_protobuf/proto_ops/proto_ops_manager.cpp)
add_library(${NAMESPACE_NAME}::${LIBRARY_NAME} ALIAS ${LIBRARY_NAME})
target_compile_options(${LIBRARY_NAME} PUBLIC $<$<OR:$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_ID:GNU>>:${NRP_COMMON_COMPILATION_FLAGS}>)

set(NRP_PROTO_LIB_TARGET "${NAMESPACE_NAME}::${LIBRARY_NAME}" CACHE INTERNAL "NRP Protobuf Library target")

target_include_directories(${LIBRARY_NAME} BEFORE
    PUBLIC
        "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>"
        "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>"
        "$<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/include>"

    PRIVATE
)

target_link_libraries(${LIBRARY_NAME}
    PUBLIC
        ${NRP_GEN_LIB_TARGET}
        ${PROTOBUF_LIBRARIES}
        ${GRPC_LIBRARIES}

    PRIVATE
)

# NRPProtoBindings
set(BINDINGS_LIB_NAME "NRPProtoBindings")
add_library("${BINDINGS_LIB_NAME}" SHARED ${LIB_SRC_FILES})
add_library(${NAMESPACE_NAME}::${BINDINGS_LIB_NAME} ALIAS ${BINDINGS_LIB_NAME})
target_compile_options(${BINDINGS_LIB_NAME} PUBLIC $<$<OR:$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_ID:GNU>>:${NRP_COMMON_COMPILATION_FLAGS}>)

set(NRP_PROTO_BINDINGS_LIB_TARGET "${NAMESPACE_NAME}::${BINDINGS_LIB_NAME}" CACHE INTERNAL "NRP Protobuf Python bindings target")

target_include_directories(${BINDINGS_LIB_NAME} BEFORE
        PUBLIC
        "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>"
        "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>"
        "$<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/include>"

        PRIVATE
        )

target_link_libraries(${BINDINGS_LIB_NAME}
        PUBLIC
        ${NRP_GEN_LIB_TARGET}
        ${PROTOBUF_LIBRARIES}

        PRIVATE
        )

# Forces the generation of proto and grpc python code
add_dependencies("${BINDINGS_LIB_NAME}" FORCE_PROTO_PYTHON)

##########################################
## Executable
if(NOT "${EXEC_SRC_FILES}" STREQUAL "")
    add_executable(${EXECUTABLE_NAME} ${EXEC_SRC_FILES})
    target_link_libraries(${EXECUTABLE_NAME} ${LIBRARY_NAME})
endif()


##########################################
## Append Doxygen files
add_doxygen_source("${CMAKE_CURRENT_SOURCE_DIR}" ${EXEC_SRC_FILES} ${LIB_SRC_FILES} "${HEADER_DIRECTORY}")


##########################################
## Tests
if(${ENABLE_TESTING} AND NOT "${TEST_SRC_FILES}" STREQUAL "")
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
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}

    PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${HEADER_DIRECTORY}
)

install(EXPORT ${LIB_EXPORT_NAME}
        DESTINATION
        ${INSTALL_CONFIGDIR}
        FILE
        "${LIB_EXPORT_NAME}.cmake"
        NAMESPACE
        "${NAMESPACE_NAME}::"
        )

install(TARGETS
        ${BINDINGS_LIB_NAME}
        EXPORT
        ${BINDINGS_LIB_NAME}Targets
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}

        PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${HEADER_DIRECTORY}
        )

install(EXPORT ${BINDINGS_LIB_NAME}Targets
        DESTINATION
        ${INSTALL_CONFIGDIR}
        FILE
        "${BINDINGS_LIB_NAME}Targets.cmake"
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

##########################################

# Process all Engine proto files distributed with nrp-core
# NRP_PROTO_PYTHON_CMAKE_DATA_DIR -Python proto (.py + .so) cmake build directory
set(NRP_PROTO_PYTHON_CMAKE_DATA_DIR "${PYTHON_MODULE_CMAKE_SRC_DIR}")
set(NRP_PROTO_PYTHON_INSTALL_DIR ${CMAKE_INSTALL_PREFIX}/${PYTHON_INSTALL_DIR_REL}/nrp_core/data/nrp_protobuf)
set(NRP_PROTO_PYTHON_FILES_TARGETS "")
set(PROTO_FILES_PATH "${NRP_CORE_MSGS_PROTO_PATH}/engine_proto_defs")
set(NRP_PROTO_CMAKE_DIR "${CMAKE_CURRENT_LIST_DIR}/cmake")
include(cmake/NRPProtoFile.cmake)

##########################################
## cmake configuration
configure_file("nrp_protobuf/config/cmake_constants.h.in" "${CMAKE_CURRENT_BINARY_DIR}/include/${HEADER_DIRECTORY}/config/cmake_constants.h" @ONLY)
configure_file("cmake/nrp_compile_protobuf/nrp_compile_protobuf.py.in" "${CMAKE_CURRENT_BINARY_DIR}/nrp_compile_protobuf.py" @ONLY)
configure_file("cmake/nrp_compile_protobuf/NRPProtoCMakeLists.txt.in" "${CMAKE_CURRENT_BINARY_DIR}/cmake/NRPProtoCMakeLists.txt" @ONLY)

# Configure and install nrp_protobuf __init__.py
set(PROTO_PYTHON_IMPORTS "")
foreach(PROTO_PYTHON_MODULE ${PROTO_PYTHON_MODULES})
    string(APPEND PROTO_PYTHON_IMPORTS "from .${PROTO_PYTHON_MODULE} import *\n")
endforeach()

configure_file("python_module/__init__.py.in" "${CMAKE_CURRENT_BINARY_DIR}/src/__init__.py" @ONLY)

install(FILES "${CMAKE_CURRENT_BINARY_DIR}/src/__init__.py"
        DESTINATION ${NRP_PROTO_PYTHON_INSTALL_DIR})

install(FILES "${CMAKE_CURRENT_BINARY_DIR}/nrp_compile_protobuf.py"
        PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE
        DESTINATION ${CMAKE_INSTALL_BINDIR})

# Generate a comma-separated list of strings of names of all user-defined proto files

set(PROTO_MODULES_LIST, "")
foreach(PROTO_FILE ${PROTO_PYTHON_FILES})
    get_filename_component(PROTO_FILE_BASENAME ${PROTO_FILE} NAME_WE)
    string(APPEND PROTO_MODULES_LIST "\"nrp_protobuf.${PROTO_FILE_BASENAME}\",\n")
endforeach()

# Store the list generated above in a python module

configure_file("python_module/proto_modules.py.in" "${PYTHON_MODULE_CMAKE_SRC_DIR}/proto_modules.py" @ONLY)
configure_file("cmake/proto_libraries.h.in" "${CMAKE_CURRENT_BINARY_DIR}/include/${HEADER_DIRECTORY}/proto_libraries.h" @ONLY)

install(FILES "${CMAKE_CURRENT_BINARY_DIR}/include/${HEADER_DIRECTORY}/proto_libraries.h" DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${HEADER_DIRECTORY})

# Install nrp protobuf cmake module and dependencies
install(
        FILES
        "${CMAKE_CURRENT_LIST_DIR}/cmake/NRPProtoFile.cmake"
        "${CMAKE_CURRENT_LIST_DIR}/cmake/python_module.cpp.in"
        "${CMAKE_CURRENT_LIST_DIR}/cmake/protobuf_ops_module.cpp.in"
        "${CMAKE_CURRENT_BINARY_DIR}/cmake/NRPProtoCMakeLists.txt"
        DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/${HEADER_DIRECTORY}/cmake"
)

# Write files with protobuf types info used by other libraries
file(WRITE "${CMAKE_BINARY_DIR}/nrp_proto_msg_headers.txt" "${PROTO_INCLUDE_STRS}")
file(WRITE "${CMAKE_BINARY_DIR}/nrp_proto_msg_types.txt" "")
foreach(PROTO_MSG_TYPES ${PROTO_MSG_TYPES_ALL})
    file(APPEND "${CMAKE_BINARY_DIR}/nrp_proto_msg_types.txt" ${PROTO_MSG_TYPES})
    file(APPEND "${CMAKE_BINARY_DIR}/nrp_proto_msg_types.txt" ",")
endforeach()


# Python module installation


# Add Python installation targets
# nrp-py-protobuf-dev-nodeps is run with make
add_custom_target(nrp-py-protobuf-dev-nodeps ALL
    WORKING_DIRECTORY ${PYTHON_MODULE_CMAKE_ROOT_DIR}
    COMMAND pip install . -U -t "${CMAKE_INSTALL_PREFIX}/${PYTHON_INSTALL_DIR_REL}/" --no-deps
    COMMENT "Installing NRP-Core Python protobuf into NRP-Core installation folder"
    DEPENDS ${PROTO_PYTHON_FILES_SRC} ${NRP_PROTO_PYTHON_FILES_TARGETS}
    VERBATIM
)

add_custom_target(nrp-py-protobuf
    WORKING_DIRECTORY ${PYTHON_MODULE_CMAKE_ROOT_DIR}
    COMMAND pip install . -U
    COMMENT "Installing NRP-Core Python protobuf into Python site-packages"
    DEPENDS ${PROTO_PYTHON_FILES_SRC} ${NRP_PROTO_PYTHON_FILES_TARGETS}
    VERBATIM
)
