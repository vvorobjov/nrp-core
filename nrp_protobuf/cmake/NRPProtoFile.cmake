set(NRP_PROTO_OPS_LIB_PREFIX "NRPProto")
set(NRP_PROTO_OPS_LIB_SUFIX "Ops")

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
                 --proto_path ${PROTO_FILES_PATH}
            "${hw_proto}"
            DEPENDS "${hw_proto}")

    set(${PROTO_SRC_FILES_VAR} ${${PROTO_SRC_FILES_VAR}} ${proto_srcs} PARENT_SCOPE)

endfunction()

# list proto files to process
file(GLOB ENGINE_PROTO_FILES "${PROTO_FILES_PATH}/*.proto")
message(STATUS "Compiling .proto files: ${ENGINE_PROTO_FILES}")
# set output variables of the functions
set(PROTO_PYTHON_MODULES "")
set(PROTO_INCLUDE_STRS "")
set(PROTO_MSG_TYPES_ALL "")

foreach(PROTO_FILE ${ENGINE_PROTO_FILES})
    # generate cpp code
    set(PROTO_SRC_FILE "")
    generate_proto_cpp("${PROTO_FILE}" PROTO_SRC_FILE)

    get_filename_component(hw_proto_name "${PROTO_FILE}" NAME_WE)
    set(PROTO_INCLUDE_STR "#include \"${HEADER_DIRECTORY}/${hw_proto_name}.pb.h\"\n")

    # store required name variables
    set(PACKAGE_NAME "")
    set(PROTO_BINDING_DEFS "")
    set(PROTO_MSG_TYPES "")

    # Read and process proto file
    file(STRINGS "${PROTO_FILE}" PROTO_FILE)
    foreach(LINE IN LISTS PROTO_FILE)
        # Find package name
        if(LINE MATCHES "package")
            string(FIND "${LINE}" " " pos REVERSE)
            string(FIND "${LINE}" ";" end REVERSE)
            math(EXPR msg_pos "${pos}+1")
            math(EXPR msg_end "${end}-${msg_pos}")
            string(SUBSTRING "${LINE}" "${msg_pos}" "${msg_end}" msg_name)
            set(PACKAGE_NAME "${msg_name}")
        endif()
        # Find message name
        if(LINE MATCHES "message")
            string(FIND "${LINE}" " " pos REVERSE)
            math(EXPR msg_pos "${pos}+1")
            string(SUBSTRING "${LINE}" "${msg_pos}" -1 msg_name)

            # Add msg name to associated variables
            string(APPEND PROTO_BINDING_DEFS "    proto_python_bindings<${PACKAGE_NAME}::${msg_name}>::create();\n")
            string(APPEND PROTO_BINDING_DEFS "    DataPack<${PACKAGE_NAME}::${msg_name}>::create_python(\"${PACKAGE_NAME}${msg_name}DataPack\");\n\n")
            string(APPEND PROTO_MSG_TYPES ",${PACKAGE_NAME}::${msg_name}")
        endif()
    endforeach()

    string(SUBSTRING ${PROTO_MSG_TYPES} 1 -1 PROTO_MSG_TYPES)
    string(TOLOWER ${PACKAGE_NAME} PACKAGE_NAME_LOW)
    set(PROTO_PYTHON_MODULE_NAME ${PACKAGE_NAME_LOW})
    list(APPEND PROTO_PYTHON_MODULES "${PROTO_PYTHON_MODULE_NAME}")
    list(APPEND PROTO_MSG_TYPES_ALL ${PROTO_MSG_TYPES})
    string(APPEND PROTO_INCLUDE_STRS ${PROTO_INCLUDE_STR})

    # Configure files
    configure_file("${NRP_PROTO_CMAKE_DIR}/python_module.cpp.in" "${CMAKE_CURRENT_BINARY_DIR}/src/${PACKAGE_NAME}/python_module.cpp" @ONLY)
    configure_file("${NRP_PROTO_CMAKE_DIR}/protobuf_ops_module.cpp.in" "${CMAKE_CURRENT_BINARY_DIR}/src/${PACKAGE_NAME}/protobuf_ops.cpp" @ONLY)

    ## define targets

    # Protobuf library
    set(PROTO_LIB_NAME "Proto${PACKAGE_NAME}")
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

    # Protobuf Operations library
    set(PROTO_OPS_LIB_NAME "${NRP_PROTO_OPS_LIB_PREFIX}${PACKAGE_NAME}${NRP_PROTO_OPS_LIB_SUFIX}")
    add_library("${PROTO_OPS_LIB_NAME}" SHARED "${CMAKE_CURRENT_BINARY_DIR}/src/${PACKAGE_NAME}/protobuf_ops.cpp")
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

            PRIVATE
            )

    # Python bindings library
    add_library(${PROTO_PYTHON_MODULE_NAME} SHARED "${CMAKE_CURRENT_BINARY_DIR}/src/${PACKAGE_NAME}/python_module.cpp")
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

endforeach()
