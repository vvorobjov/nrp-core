set(NRP_PROTO_OPS_LIB_PREFIX "NRPProto")
set(NRP_PROTO_OPS_LIB_SUFIX "Ops")

# Generates C++ protobuf code from file PROTO_FILE_PATH. Generated file name is set to var PROTO_SRC_FILE_VAR
function(GENERATE_PROTO_CPP PROTO_FILE_PATH PROTO_SRC_FILE_VAR)

    # List of protobuf proto files
    get_filename_component(hw_proto_path "${PROTO_FILE_PATH}" ABSOLUTE)
    get_filename_component(hw_proto_dir "${hw_proto_path}" DIRECTORY)
    get_filename_component(hw_proto_name "${hw_proto_path}" NAME_WE)

    # Generated sources
    set(proto_srcs "${CMAKE_CURRENT_BINARY_DIR}/include/${HEADER_DIRECTORY}/${hw_proto_name}.pb.cc")
    set(proto_hdrs "${CMAKE_CURRENT_BINARY_DIR}/include/${HEADER_DIRECTORY}/${hw_proto_name}.pb.h")

    # Generate
    add_custom_command(
            OUTPUT "${proto_srcs}" "${proto_hdrs}"
            COMMAND "mkdir" ARGS -p "${CMAKE_CURRENT_BINARY_DIR}/include/${HEADER_DIRECTORY}"
            COMMAND ${PROTOC}
            ARGS --cpp_out "${CMAKE_CURRENT_BINARY_DIR}/include/${HEADER_DIRECTORY}"
                 --proto_path ${hw_proto_dir}
            "${hw_proto_path}"
            DEPENDS "${hw_proto_path}")

    set(${PROTO_SRC_FILE_VAR} ${proto_srcs} PARENT_SCOPE)

endfunction()

# Generates Python proto code from file PROTO_FILE_PATH. Generated file name is set to var PROTO_PYTHON_FILES_VAR
function(GENERATE_PROTO_PYTHON PROTO_FILE_PATH PROTO_PYTHON_FILE_VAR)

    # List of protobuf proto files
    get_filename_component(hw_proto_path "${PROTO_FILE_PATH}" ABSOLUTE)
    get_filename_component(hw_proto_dir "${hw_proto_path}" DIRECTORY)
    get_filename_component(hw_proto_name "${hw_proto_path}" NAME_WE)

    # Generated sources
    set(proto_python "${CMAKE_CURRENT_BINARY_DIR}/include/${HEADER_DIRECTORY}/${hw_proto_name}_pb2.py")

    # Generate
    add_custom_command(
            OUTPUT "${proto_python}"
            COMMAND "mkdir" ARGS -p "${CMAKE_CURRENT_BINARY_DIR}/include/${HEADER_DIRECTORY}"
            COMMAND python3
            ARGS -m grpc_tools.protoc
            --proto_path ${hw_proto_dir}
            --python_out "${CMAKE_CURRENT_BINARY_DIR}/include/${HEADER_DIRECTORY}"
            "${hw_proto_path}"
            DEPENDS "${hw_proto_path}")

    set(${PROTO_PYTHON_FILE_VAR} ${proto_python} PARENT_SCOPE)

endfunction()

# list proto files to process
file(GLOB ENGINE_PROTO_FILES "${PROTO_FILES_PATH}/*.proto")
message(STATUS "Compiling .proto files: ${ENGINE_PROTO_FILES}")
# set output variables of the functions
set(PROTO_PYTHON_MODULES "")
set(PROTO_INCLUDE_STRS "")
set(PROTO_MSG_TYPES_ALL "")
set(PROTO_PYTHON_FILES "")
set(PROTO_OPS_LIBS "")

foreach(PROTO_FILE ${ENGINE_PROTO_FILES})

    # store required name variables
    unset(PACKAGE_CPP_NAME)
    unset(PACKAGE_SHORT_NAME)
    unset(PACKAGE_FILE_NAME)
    set(PROTO_BINDING_DEFS "")
    set(PROTO_MSG_TYPES "")

    # Read and process proto file
    file(STRINGS "${PROTO_FILE}" PROTO_FILE_STR)
    foreach(LINE IN LISTS PROTO_FILE_STR)
        # Find package name
        if(LINE MATCHES "package")
            string(FIND "${LINE}" " " pos REVERSE)
            string(FIND "${LINE}" ";" end REVERSE)
            math(EXPR msg_pos "${pos}+1")
            math(EXPR msg_len "${end}-${msg_pos}")
            string(SUBSTRING "${LINE}" "${msg_pos}" "${msg_len}" msg_name)
            string(REPLACE "." "::" PACKAGE_CPP_NAME ${msg_name})
            string(REPLACE "." "" PACKAGE_SHORT_NAME ${msg_name})
            string(REPLACE "." "_" PACKAGE_FILE_NAME ${msg_name})
        endif()
        # Find message name
        if(LINE MATCHES "message")
            string(FIND "${LINE}" " " pos REVERSE)
            math(EXPR msg_pos "${pos}+1")
            string(SUBSTRING "${LINE}" "${msg_pos}" -1 msg_name)

            # Add msg name to associated variables
            string(APPEND PROTO_BINDING_DEFS "    proto_python_bindings<${PACKAGE_CPP_NAME}::${msg_name}>::create();\n")
            string(APPEND PROTO_BINDING_DEFS "    DataPack<${PACKAGE_CPP_NAME}::${msg_name}>::create_python(\"${PACKAGE_SHORT_NAME}${msg_name}DataPack\");\n\n")
            string(APPEND PROTO_BINDING_DEFS "    RawData<${PACKAGE_CPP_NAME}::${msg_name}>::create_python(\"${PACKAGE_SHORT_NAME}${msg_name}RawData\");\n\n")
            string(APPEND PROTO_MSG_TYPES ",${PACKAGE_CPP_NAME}::${msg_name}")
        endif()
    endforeach()

    # Check if the .proto file has a package descriptor
    if(NOT DEFINED PACKAGE_FILE_NAME)
        message(FATAL_ERROR "Protobuf file \"${PROTO_FILE}\" does not have a \"package\" descriptor. This is required to compile it in nrp-core.")
    endif()

    # Set variables
    string(SUBSTRING ${PROTO_MSG_TYPES} 1 -1 PROTO_MSG_TYPES)
    string(TOLOWER ${PACKAGE_SHORT_NAME} PACKAGE_NAME_LOW)
    string(TOLOWER ${PACKAGE_FILE_NAME} PACKAGE_FILE_NAME)
    set(PROTO_PYTHON_MODULE_NAME ${PACKAGE_NAME_LOW})
    list(APPEND PROTO_PYTHON_MODULES "${PROTO_PYTHON_MODULE_NAME}")
    list(APPEND PROTO_MSG_TYPES_ALL ${PROTO_MSG_TYPES})

    # copy .proto file and rename it using package name, this is to avoid possible conflicts in generated protobuf libraries
    set(TEMP_PROTO_DIR "${CMAKE_CURRENT_BINARY_DIR}/proto_srcs")
    get_filename_component(PROTO_FILE_NAME "${PROTO_FILE}" NAME)
    file(COPY ${PROTO_FILE} DESTINATION "${TEMP_PROTO_DIR}")
    file(RENAME "${TEMP_PROTO_DIR}/${PROTO_FILE_NAME}" "${TEMP_PROTO_DIR}/${PACKAGE_FILE_NAME}.proto")

    # generate cpp code
    set(PROTO_SRC_FILE "")
    generate_proto_cpp("${TEMP_PROTO_DIR}/${PACKAGE_FILE_NAME}.proto" PROTO_SRC_FILE)
    set(PROTO_INCLUDE_STR "#include \"${HEADER_DIRECTORY}/${PACKAGE_FILE_NAME}.pb.h\"\n")
    string(APPEND PROTO_INCLUDE_STRS ${PROTO_INCLUDE_STR})

    # generate python code
    set(PROTO_PYTHON_FILE "")
    generate_proto_python("${TEMP_PROTO_DIR}/${PACKAGE_FILE_NAME}.proto" PROTO_PYTHON_FILE)
    list(APPEND PROTO_PYTHON_FILES ${PROTO_PYTHON_FILE})

    # Configure files
    configure_file("${NRP_PROTO_CMAKE_DIR}/python_module.cpp.in" "${CMAKE_CURRENT_BINARY_DIR}/src/${PACKAGE_SHORT_NAME}/python_module.cpp" @ONLY)
    configure_file("${NRP_PROTO_CMAKE_DIR}/protobuf_ops_module.cpp.in" "${CMAKE_CURRENT_BINARY_DIR}/src/${PACKAGE_SHORT_NAME}/protobuf_ops.cpp" @ONLY)

    ## define targets

    # Protobuf library
    set(PROTO_LIB_NAME "Proto${PACKAGE_SHORT_NAME}")
    add_library("${PROTO_LIB_NAME}" SHARED ${PROTO_SRC_FILE})
    add_library(${NAMESPACE_NAME}::${PROTO_LIB_NAME} ALIAS ${PROTO_LIB_NAME})
    target_compile_options(${PROTO_LIB_NAME} PUBLIC $<$<OR:$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_ID:GNU>>:${NRP_COMMON_COMPILATION_FLAGS}>)
    target_link_options(${PROTO_LIB_NAME} PUBLIC ${NRP_COMMON_LD_FLAGS})
    target_include_directories(${PROTO_LIB_NAME} BEFORE
            PUBLIC
            "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>"
            "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>"
            "$<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/include>"

            PRIVATE
            )

    target_link_libraries(${PROTO_LIB_NAME}
            PUBLIC
            ${PROTOBUF_LIBRARIES}

            PRIVATE
            )

    # Forces the generation of proto python code
    add_custom_target(
            "FORCE_PROTO_PYTHON_${PACKAGE_SHORT_NAME}"
            COMMAND bash -c "echo \"Generating proto python files for package ${PACKAGE_SHORT_NAME}\""
            DEPENDS ${PROTO_PYTHON_FILE})
    add_dependencies("${PROTO_LIB_NAME}" "FORCE_PROTO_PYTHON_${PACKAGE_SHORT_NAME}")

    # Protobuf Operations library
    set(PROTO_OPS_LIB_NAME "${NRP_PROTO_OPS_LIB_PREFIX}${PACKAGE_SHORT_NAME}${NRP_PROTO_OPS_LIB_SUFIX}")
    list(APPEND PROTO_OPS_LIBS "lib${PROTO_OPS_LIB_NAME}.so")
    add_library("${PROTO_OPS_LIB_NAME}" SHARED "${CMAKE_CURRENT_BINARY_DIR}/src/${PACKAGE_SHORT_NAME}/protobuf_ops.cpp")
    add_library(${NAMESPACE_NAME}::${PROTO_OPS_LIB_NAME} ALIAS ${PROTO_OPS_LIB_NAME})
    target_compile_options(${PROTO_OPS_LIB_NAME} PUBLIC $<$<OR:$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_ID:GNU>>:${NRP_COMMON_COMPILATION_FLAGS}>)

    target_include_directories(${PROTO_OPS_LIB_NAME} BEFORE
            PUBLIC
            "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>"
            "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>"
            "$<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/include>"

            PRIVATE
            )

    target_link_libraries(${PROTO_OPS_LIB_NAME}
            PUBLIC
            ${NRP_GEN_LIB_TARGET}
            ${NAMESPACE_NAME}::${PROTO_LIB_NAME}
            ${NRP_PROTO_LIB_TARGET}
            PRIVATE
            )

    add_dependencies(${PROTO_OPS_LIB_NAME} ${NRP_PROTO_LIB_TARGET})

    # Python bindings library
    add_library(${PROTO_PYTHON_MODULE_NAME} SHARED "${CMAKE_CURRENT_BINARY_DIR}/src/${PACKAGE_SHORT_NAME}/python_module.cpp")
    add_library(${NAMESPACE_NAME}::${PROTO_PYTHON_MODULE_NAME} ALIAS ${PROTO_PYTHON_MODULE_NAME})
    target_compile_options(${PROTO_PYTHON_MODULE_NAME} PRIVATE $<$<OR:$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_ID:GNU>>:${NRP_COMMON_COMPILATION_FLAGS}>)
    set_target_properties(${PROTO_PYTHON_MODULE_NAME} PROPERTIES PREFIX "")

    target_include_directories(${PROTO_PYTHON_MODULE_NAME}
            PUBLIC
            )

    target_link_libraries(${PROTO_PYTHON_MODULE_NAME}
            PUBLIC
            ${NAMESPACE_NAME}::${PROTO_LIB_NAME}
            ${NRP_PROTO_BINDINGS_LIB_TARGET}
            )

    ## Install targets

    # Protobuf library
    install(TARGETS
            ${PROTO_LIB_NAME}
            EXPORT
            ${PROTO_LIB_NAME}Targets
            LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
            ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}

            PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${HEADER_DIRECTORY}
            )

    install(EXPORT ${PROTO_LIB_NAME}Targets
            DESTINATION
            ${INSTALL_CONFIGDIR}
            FILE
            "${PROTO_LIB_NAME}Targets.cmake"
            NAMESPACE
            "${NAMESPACE_NAME}::"
            )

    # Protobuf Operations library
    install(TARGETS ${PROTO_OPS_LIB_NAME}
            DESTINATION ${CMAKE_INSTALL_LIBDIR})

    # Python module
    install(TARGETS ${PROTO_PYTHON_MODULE_NAME}
            DESTINATION ${NRP_PROTO_PYTHON_INSTALL_DIR})

    # Python proto file
    install(FILES ${PROTO_PYTHON_FILE}
            DESTINATION "${NRP_PROTO_PYTHON_INSTALL_DIR}")

endforeach()

# Generate a comma-separated list of strings of names of all user-defined proto files

set(PROTO_MODULES_LIST, "")
foreach(PROTO_FILE ${PROTO_PYTHON_FILES})
    get_filename_component(PROTO_FILE_BASENAME ${PROTO_FILE} NAME_WE)
    string(APPEND PROTO_MODULES_LIST "\"nrp_core.data.nrp_protobuf.${PROTO_FILE_BASENAME}\",\n")
endforeach()

# Store the list generated above in a python module

configure_file("python/proto_modules.py.in" "${CMAKE_CURRENT_BINARY_DIR}/include/${HEADER_DIRECTORY}/proto_modules.py" @ONLY)
configure_file("cmake/proto_libraries.h.in" "${CMAKE_CURRENT_BINARY_DIR}/include/${HEADER_DIRECTORY}/proto_libraries.h" @ONLY)

install(FILES "${CMAKE_CURRENT_BINARY_DIR}/include/${HEADER_DIRECTORY}/proto_modules.py" DESTINATION "${PYTHON_INSTALL_DIR_REL}")
install(FILES "${CMAKE_CURRENT_BINARY_DIR}/include/${HEADER_DIRECTORY}/proto_libraries.h" DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${HEADER_DIRECTORY})
