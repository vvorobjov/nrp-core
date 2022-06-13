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
    proto_python_bindings/proto_field_ops.cpp
    proto_python_bindings/repeated_field_proxy.cpp
    proto_python_bindings/proto_python_bindings.cpp
)

# List of python module build files
set(PYTHON_MODULE_SRC_FILES
        ${CMAKE_CURRENT_BINARY_DIR}/python_module/python_module.cpp
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

# Parse a .proto file PROTO_FILE_PATH and adds code for generating python bindings for each message definition found
# The code is divided into include statements (appended to var PROTO_PYTHON_INCLUDE) and body statements (appended to
# var PROTO_PYTHON_BODY)
function(GENERATE_PROTO_BINDINGS PROTO_FILE_PATH PROTO_PYTHON_INCLUDE PROTO_PYTHON_BODY PROTO_TYPES_FILE)

    set(PROTO_BINDING_DEFS "")

    # Add include
    get_filename_component(hw_proto_name "${PROTO_FILE_PATH}" NAME_WE)
    set(${PROTO_PYTHON_INCLUDE} "${${PROTO_PYTHON_INCLUDE}}#include \"${HEADER_DIRECTORY}/${hw_proto_name}.pb.h\"\n" PARENT_SCOPE)

    # Read proto file
    file(STRINGS "${PROTO_FILE_PATH}" PROTO_FILE)

    # Find msg definitions and add python bindings for them
    foreach(LINE IN LISTS PROTO_FILE)
        if(LINE MATCHES "package")
            string(FIND "${LINE}" " " pos REVERSE)
            string(FIND "${LINE}" ";" end REVERSE)
            math(EXPR msg_pos "${pos}+1")
            math(EXPR msg_end "${end}-${msg_pos}")
            string(SUBSTRING "${LINE}" "${msg_pos}" "${msg_end}" msg_name)
            set(package_name "${msg_name}")
        endif()
        if(LINE MATCHES "message")
            string(FIND "${LINE}" " " pos REVERSE)
            math(EXPR msg_pos "${pos}+1")
            string(SUBSTRING "${LINE}" "${msg_pos}" -1 msg_name)
            string(APPEND PROTO_BINDING_DEFS "    proto_python_bindings<${package_name}::${msg_name}>::create();\n")
            string(APPEND PROTO_BINDING_DEFS "    DataPack<${package_name}::${msg_name}>::create_python(\"${package_name}${msg_name}DataPack\");\n\n")
            file(APPEND "${PROTO_TYPES_FILE}" "${package_name}::${msg_name};")
        endif()
    endforeach()

    # Set var
    set(${PROTO_PYTHON_BODY} "${${PROTO_PYTHON_BODY}}${PROTO_BINDING_DEFS}" PARENT_SCOPE)

endfunction()

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
    set(proto_python "${CMAKE_CURRENT_BINARY_DIR}/include/${HEADER_DIRECTORY}/${hw_proto_name}_pb2.py")

    # Generate
    add_custom_command(
            OUTPUT "${proto_python}"
            COMMAND "mkdir" ARGS -p "${CMAKE_CURRENT_BINARY_DIR}/include/${HEADER_DIRECTORY}"
            COMMAND python3
            ARGS -m grpc_tools.protoc
            --proto_path "${NRP_CORE_MSGS_PROTO_PATH}/nrp_proto_defs"
            --proto_path "${NRP_CORE_MSGS_PROTO_PATH}/engine_proto_defs"
            --python_out "${CMAKE_CURRENT_BINARY_DIR}/include/${HEADER_DIRECTORY}"
            "${hw_proto}"
            DEPENDS "${hw_proto}")

    set(${PROTO_PYTHON_FILES_VAR} ${${PROTO_PYTHON_FILES_VAR}} ${proto_python} PARENT_SCOPE)

endfunction()

# Generates Python grpc code from file PROTO_FILE_NAME. Generated files are appended to var PROTO_PYTHON_FILES_VAR
function(GENERATE_GRPC_PYTHON PROTO_FILE_NAME PROTO_PYTHON_FILES_VAR)
    # List of protobuf proto files
    get_filename_component(hw_proto "${PROTO_FILE_NAME}" ABSOLUTE)
    get_filename_component(hw_proto_name "${hw_proto}" NAME_WE)

    # Generated sources
    set(proto_python "${CMAKE_CURRENT_BINARY_DIR}/include/${HEADER_DIRECTORY}/${hw_proto_name}_pb2_grpc.py")

    # Generate
    add_custom_command(
            OUTPUT "${proto_python}"
            COMMAND "mkdir" ARGS -p "${CMAKE_CURRENT_BINARY_DIR}/include/${HEADER_DIRECTORY}"
            COMMAND python3
            ARGS -m grpc_tools.protoc
            --proto_path "${NRP_CORE_MSGS_PROTO_PATH}/nrp_proto_defs"
            --proto_path "${NRP_CORE_MSGS_PROTO_PATH}/engine_proto_defs"
            --grpc_python_out "${CMAKE_CURRENT_BINARY_DIR}/include/${HEADER_DIRECTORY}"
            "${hw_proto}"
            DEPENDS "${hw_proto}")

    set(${PROTO_PYTHON_FILES_VAR} ${${PROTO_PYTHON_FILES_VAR}} ${proto_python} PARENT_SCOPE)

endfunction()

##########################################
## Protobuf code generation

file(GLOB ENGINE_PROTO_FILES "${NRP_CORE_MSGS_PROTO_PATH}/engine_proto_defs/*.proto")

# Python bindings
set(PROTO_PYTHON_INCLUDE_STR "")
set(PROTO_PYTHON_BODY_STR "")
file(WRITE "${CMAKE_BINARY_DIR}/nrp_proto_msg_types.txt" "")

foreach(PROTO_FILE ${ENGINE_PROTO_FILES})
    generate_proto_bindings("${PROTO_FILE}" PROTO_PYTHON_INCLUDE_STR PROTO_PYTHON_BODY_STR "${CMAKE_BINARY_DIR}/nrp_proto_msg_types.txt")
endforeach()

file(WRITE "${CMAKE_BINARY_DIR}/nrp_proto_msg_headers.txt" "${PROTO_PYTHON_INCLUDE_STR}")

# Proto generation
set(PROTO_SRC_FILES "")
set(PROTO_PYTHON_FILES "")

foreach(PROTO_FILE ${ENGINE_PROTO_FILES})
    generate_proto_cpp("${PROTO_FILE}" PROTO_SRC_FILES)
endforeach()

generate_proto_cpp("${NRP_CORE_MSGS_PROTO_PATH}/nrp_proto_defs/engine_grpc.proto" PROTO_SRC_FILES)
generate_grpc_cpp("${NRP_CORE_MSGS_PROTO_PATH}/nrp_proto_defs/engine_grpc.proto" PROTO_SRC_FILES)
generate_proto_cpp("${NRP_CORE_MSGS_PROTO_PATH}/nrp_proto_defs/nrp_server.proto" PROTO_SRC_FILES)
generate_grpc_cpp("${NRP_CORE_MSGS_PROTO_PATH}/nrp_proto_defs/nrp_server.proto" PROTO_SRC_FILES)
generate_proto_python("${NRP_CORE_MSGS_PROTO_PATH}/nrp_proto_defs/nrp_server.proto" PROTO_PYTHON_FILES)
generate_grpc_python("${NRP_CORE_MSGS_PROTO_PATH}/nrp_proto_defs/nrp_server.proto" PROTO_PYTHON_FILES)

# Protobuf utils
set(DATAPACK_MSG_TYPES "")
set(DATAPACK_HEADERS "")

# Find all protobuf messages defined by users
foreach(PROTO_FILE ${ENGINE_PROTO_FILES})
    file(STRINGS ${PROTO_FILE} PROTO_FILE_LINES)
    foreach(LINE IN LISTS PROTO_FILE_LINES)

        # Find the package name
        if(LINE MATCHES "package")
            string(STRIP "${LINE}" LINE)
            string(FIND "${LINE}" " " start)
            string(SUBSTRING "${LINE}" ${start} -1 package_name)
            string(REPLACE ";" "::" package_name "${package_name}")
        endif()

        # Find message names
        if(LINE MATCHES "message")
            string(STRIP "${LINE}" LINE)
            string(FIND "${LINE}" " " end)
            string(SUBSTRING "${LINE}" ${end} -1 message_name)
            string(STRIP "${message_name}" message_name)

            # Append full message name (package name + message name) to the list
            string(APPEND DATAPACK_MSG_TYPES "${package_name}")
            string(APPEND DATAPACK_MSG_TYPES "${message_name}")
        endif()

    endforeach()
endforeach()

string(STRIP ${DATAPACK_MSG_TYPES} DATAPACK_MSG_TYPES)
string(REPLACE " " "," DATAPACK_MSG_TYPES ${DATAPACK_MSG_TYPES})

# Create include clauses for protobuf_utils.h
foreach(PROTO_FILE ${ENGINE_PROTO_FILES})
    get_filename_component(proto_basename ${PROTO_FILE} NAME_WLE)
    string(APPEND proto_basename ".pb.h\"\n")
    string(PREPEND proto_basename "\#include \"nrp_protobuf/")
    string(APPEND DATAPACK_HEADERS ${proto_basename})
endforeach()

# Forces the generation of proto and grpc python code
add_custom_target(
        FORCE_PROTO_PYTHON
        COMMAND bash -c "echo \"Generating proto python files\""
        DEPENDS ${PROTO_PYTHON_FILES})

##########################################
## Header configuration

# General Header defines
configure_file("config/cmake_constants.h.in" "${CMAKE_CURRENT_BINARY_DIR}/include/${HEADER_DIRECTORY}/config/cmake_constants.h" @ONLY)

# Python Module defines
configure_file("python_module/python_module.cpp.in" "${CMAKE_CURRENT_BINARY_DIR}/python_module/python_module.cpp" @ONLY)
configure_file("python_module/__init__.py.in" "${CMAKE_CURRENT_BINARY_DIR}/src/__init__.py" @ONLY)

# Protobuf utils
configure_file("utils/protobuf_utils.h.in" "${CMAKE_CURRENT_BINARY_DIR}/include/${HEADER_DIRECTORY}/protobuf_utils.h" @ONLY)

##########################################
# NRPProtobuf
add_library("${LIBRARY_NAME}" SHARED ${LIB_SRC_FILES} ${PROTO_SRC_FILES})
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


##########################################
## nrp_core.data.nrp_protobuf
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

    add_dependencies(${PYTHON_MODULE_NAME} FORCE_PROTO_PYTHON)
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
        DESTINATION "${PYTHON_INSTALL_DIR_REL}/${NRP_PYTHON_MODULE_NAME}/data/${PYTHON_MODULE_NAME}")

    install(FILES "${CMAKE_CURRENT_BINARY_DIR}/src/__init__.py"
        DESTINATION "${PYTHON_INSTALL_DIR_REL}/${NRP_PYTHON_MODULE_NAME}/data/${PYTHON_MODULE_NAME}")

    # Install automatically generated python modules with proto and gRPC
    if(PROTO_PYTHON_FILES)
        install(FILES ${PROTO_PYTHON_FILES}
                DESTINATION "${PYTHON_INSTALL_DIR_REL}")
    endif()
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
