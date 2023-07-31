set(PROJECT_NAME "NRPEventLoop")
set(HEADER_DIRECTORY "nrp_event_loop")

set(NAMESPACE_NAME "${PROJECT_NAME}")

set(LIBRARY_NAME "${PROJECT_NAME}")
set(PYTHON_MODULE_NAME "event_loop")
set(EXECUTABLE_NAME "${PROJECT_NAME}Exec")
set(TEST_NAME "${PROJECT_NAME}Tests")

set(LIB_EXPORT_NAME "${LIBRARY_NAME}Targets")
set(LIB_CONFIG_NAME "${LIBRARY_NAME}Config")
set(LIB_VERSION_NAME "${LIB_CONFIG_NAME}Version")

cmake_minimum_required(VERSION 3.16)
project("${PROJECT_NAME}" VERSION ${NRP_VERSION})

include(GNUInstallDirs)
include(ExternalProject)

# List library build files
set(LIB_SRC_FILES
        nrp_event_loop/computational_graph/computational_node.cpp
        nrp_event_loop/computational_graph/port.cpp
        nrp_event_loop/computational_graph/computational_graph.cpp

        nrp_event_loop/computational_graph/input_port.cpp
        nrp_event_loop/computational_graph/output_port.cpp

        nrp_event_loop/computational_graph/functional_node.cpp
        nrp_event_loop/computational_graph/input_node.cpp
        nrp_event_loop/computational_graph/output_node.cpp

        nrp_event_loop/computational_graph/computational_graph_manager.cpp
        nrp_event_loop/fn_factory/functional_node_factory.cpp
        nrp_event_loop/fn_factory/functional_node_factory_manager.cpp

        nrp_event_loop/event_loop/event_loop_interface.cpp

        nrp_event_loop/python/functional_node.cpp
        nrp_event_loop/python/input_edge.cpp
        nrp_event_loop/python/output_edge.cpp

        nrp_event_loop/nodes/dummy/input_dummy.cpp
        nrp_event_loop/nodes/dummy/output_dummy.cpp
        nrp_event_loop/nodes/engine/input_node.cpp
        nrp_event_loop/nodes/engine/output_node.cpp

        nrp_event_loop/nodes/time/input_time.cpp

        nrp_event_loop/utils/graph_utils.cpp
        nrp_event_loop/utils/data_conversion.cpp
)

set(ROS_SRC_FILES
        nrp_event_loop/nodes/ros/input_node.cpp
        nrp_event_loop/nodes/ros/output_node.cpp
        )

set(MQTT_SRC_FILES
        nrp_event_loop/nodes/mqtt/input_node.cpp
        nrp_event_loop/nodes/mqtt/output_node.cpp
        )

set(SPINNAKER_SRC_FILES
        nrp_event_loop/nodes/spinnaker/input_node.cpp
        nrp_event_loop/nodes/spinnaker/output_node.cpp
        nrp_event_loop/nodes/spinnaker/spinnaker_proxy.cpp
        )

# List of python module build files
set(PYTHON_MODULE_SRC_FILES
        python/python_module.cpp
)

# List executable build files
set(EXEC_SRC_FILES
)

# List testing build files
set(TEST_SRC_FILES
        tests/test_files/helper_classes.cpp
        tests/test_ports.cpp
        tests/test_nodes.cpp
        tests/test_computational_graph.cpp
        tests/test_python_nodes.cpp
)

set(ROS_TEST_SRC_FILES
        tests/test_python_ros_nodes.cpp
        )

set(MQTT_TEST_SRC_FILES
        tests/test_event_loop_engine.cpp
        )


##########################################
## Dependencies

if(ENABLE_ROS)
    find_package(catkin REQUIRED COMPONENTS roscpp nrp_ros_msgs ${NRP_ROS_MSGS_PACKAGES})
endif()

##########################################
## Header configuration

configure_file("nrp_event_loop/config/cmake_constants.h.in" "${CMAKE_CURRENT_BINARY_DIR}/include/${HEADER_DIRECTORY}/config/cmake_constants.h" @ONLY)

# Configures ros_edge_factory.h
if(ENABLE_ROS)
    file(READ "${CMAKE_BINARY_DIR}/nrp_ros_msg_headers.txt" NRP_ROS_MSG_HEADERS)
    set(ROS_MSG_TYPES_INCLUDE "")
    foreach(MSG ${NRP_ROS_MSG_HEADERS})
        string(APPEND ROS_MSG_TYPES_INCLUDE "#include \"${MSG}\"\n")
    endforeach()

    file(READ "${CMAKE_BINARY_DIR}/nrp_ros_msg_types.txt" NRP_ROS_MSG_TYPES)
    set(ROS_MSG_TYPES_CHECK "")
    foreach(MSG ${NRP_ROS_MSG_TYPES})
        string(APPEND ROS_MSG_TYPES_CHECK "if (rosType == \"${MSG}\")\n        {\n            if(_isInput)\n                return pySetupInput<${MSG}>(obj);\n            else\n                return pySetupOutput<${MSG}>(obj);\n        }\n        else ")
    endforeach()

    configure_file("nrp_event_loop/nodes/ros/ros_edge_factory.h.in" "${CMAKE_CURRENT_BINARY_DIR}/include/${HEADER_DIRECTORY}/nodes/ros/ros_edge_factory.h" @ONLY)
endif()

# Configures mqtt_edge_factory.h
if(ENABLE_MQTT)
    file(READ "${CMAKE_BINARY_DIR}/nrp_proto_msg_headers.txt" PROTO_MSG_TYPES_INCLUDE)
    file(READ "${CMAKE_BINARY_DIR}/nrp_proto_msg_types.txt" NRP_PROTO_MSG_TYPES)
    string(REPLACE "," ";" NRP_PROTO_MSG_TYPES ${NRP_PROTO_MSG_TYPES})
    set(PROTO_MSG_TYPES_CHECK "")
    foreach(MSG ${NRP_PROTO_MSG_TYPES})
        string(REPLACE "::" "" PYTHON_MSG "${MSG}")
        string(APPEND PROTO_MSG_TYPES_CHECK "if (_className == \"${PYTHON_MSG}\")\n        {\n            if(_isInput)\n                return pySetupInput<${MSG}>(obj);\n            else\n                return pySetupOutput<${MSG}>(obj);\n        }\n        else ")
    endforeach()

    configure_file("nrp_event_loop/nodes/mqtt/mqtt_edge_factory.h.in" "${CMAKE_CURRENT_BINARY_DIR}/include/${HEADER_DIRECTORY}/nodes/mqtt/mqtt_edge_factory.h" @ONLY)
endif()

# Python Module defines
configure_file("${CMAKE_CURRENT_SOURCE_DIR}/python/__init__.py.in" "${CMAKE_CURRENT_BINARY_DIR}/src/__init__.py" @ONLY)

##########################################
# Library
add_library("${LIBRARY_NAME}" SHARED ${LIB_SRC_FILES})
add_library(${NAMESPACE_NAME}::${LIBRARY_NAME} ALIAS ${LIBRARY_NAME})
target_compile_options(${LIBRARY_NAME} PUBLIC $<$<OR:$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_ID:GNU>>:-Wall -Wextra>)
target_compile_options(${LIBRARY_NAME} PUBLIC $<$<CXX_COMPILER_ID:GNU>:-fconcepts>)

set(NRP_EVENT_LOOP_TARGET "${NAMESPACE_NAME}::${LIBRARY_NAME}" CACHE INTERNAL "NRP Event Loop Library target")

set_target_properties(${LIBRARY_NAME} PROPERTIES PREFIX "")

target_link_libraries(${LIBRARY_NAME}
    PUBLIC
        ${NRP_GEN_LIB_TARGET}
        ${Boost_LIBRARIES}
        ${catkin_LIBRARIES}


    PRIVATE
)

target_include_directories(${LIBRARY_NAME} BEFORE
    PUBLIC
        "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>"
        "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>"
        "$<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/include>"

        ${Boost_INCLUDE_DIRS}
        ${catkin_INCLUDE_DIRS}

    PRIVATE
)

# ROS nodes target
if(ENABLE_ROS)
    set(ROS_LIBRARY_NAME "${LIBRARY_NAME}ROSNodes")
    add_library(${ROS_LIBRARY_NAME} SHARED ${ROS_SRC_FILES})
    add_library(${NAMESPACE_NAME}::${ROS_LIBRARY_NAME} ALIAS ${ROS_LIBRARY_NAME})
    target_compile_options(${ROS_LIBRARY_NAME} PRIVATE $<$<OR:$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_ID:GNU>>:-Wall -Wextra>)

    target_include_directories(${ROS_LIBRARY_NAME} BEFORE
            PUBLIC
            "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>"
            "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>"
            "$<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/include>"

            ${catkin_INCLUDE_DIRS}
            ${Boost_INCLUDE_DIRS}
            )

    target_link_libraries(${ROS_LIBRARY_NAME}
            PUBLIC
            ${NRP_EVENT_LOOP_TARGET}
            ${NRP_GEN_LIB_TARGET}
            ${NRP_ROS_PROXY_TARGET}
            ${catkin_LIBRARIES}
            ${Boost_LIBRARIES}
            )
endif()

if(ENABLE_MQTT)
    # MQTT nodes target
    set(MQTT_LIBRARY_NAME "${LIBRARY_NAME}MQTTNodes")
    add_library(${MQTT_LIBRARY_NAME} SHARED ${MQTT_SRC_FILES})
    add_library(${NAMESPACE_NAME}::${MQTT_LIBRARY_NAME} ALIAS ${MQTT_LIBRARY_NAME})
    target_compile_options(${MQTT_LIBRARY_NAME} PRIVATE $<$<OR:$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_ID:GNU>>:-Wall -Wextra>)

    target_include_directories(${MQTT_LIBRARY_NAME} BEFORE
            PUBLIC
            "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>"
            "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>"
            "$<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/include>"
            )

    target_link_libraries(${MQTT_LIBRARY_NAME}
            PUBLIC
            ${NRP_EVENT_LOOP_TARGET}
            ${NRP_GEN_LIB_TARGET}
            ${NRP_MQTT_PROXY_TARGET}
            ${NRP_PROTO_LIB_TARGET}
            NRPProtobuf::ProtoDump
            NRPProtobuf::ProtoGazebo
            NRPProtobuf::ProtoEngineTest
            NRPProtobuf::ProtoNrpGenericProto
            NRPProtobuf::ProtoWrappers
            NRPProtobuf::ProtoEdlutData
            )

    # Event Loop Engine target
    set(EVENT_LOOP_ENGINE_TARGET "${LIBRARY_NAME}Engine")
    add_library(${EVENT_LOOP_ENGINE_TARGET} SHARED
            nrp_event_loop/event_loop/event_loop_engine.cpp
            nrp_event_loop/event_loop/event_loop_engine_opts_parser.cpp)

    add_library(${NAMESPACE_NAME}::${EVENT_LOOP_ENGINE_TARGET} ALIAS ${EVENT_LOOP_ENGINE_TARGET})
    target_compile_options(${EVENT_LOOP_ENGINE_TARGET} PRIVATE $<$<OR:$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_ID:GNU>>:-Wall -Wextra>)

    set(NRP_ELE_TARGET "${NAMESPACE_NAME}::${EVENT_LOOP_ENGINE_TARGET}" CACHE INTERNAL "NRP Event Loop Engine Library target")

    target_include_directories(${EVENT_LOOP_ENGINE_TARGET} BEFORE
            PUBLIC
            "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>"
            "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>"
            "$<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/include>"
            )

    target_link_libraries(${EVENT_LOOP_ENGINE_TARGET}
            PUBLIC
            ${NRP_EVENT_LOOP_TARGET}
            NRPGRPCEngineProtocol::NRPGRPCEngineProtocol
            ${NRP_GEN_LIB_TARGET}
            ${NRP_MQTT_PROXY_TARGET}
            NRPProtobuf::ProtoDump
            NRPProtobuf::ProtoGazebo
            NRPProtobuf::ProtoEngineTest
            NRPProtobuf::ProtoNrpGenericProto
            NRPProtobuf::ProtoWrappers
            NRPProtobuf::ProtoEdlutData
            )

endif()

# SPINNAKER nodes target
if(ENABLE_SPINNAKER)
    set(SPINNAKER_LIBRARY_NAME "${LIBRARY_NAME}SPINNAKERNodes")
    add_library(${SPINNAKER_LIBRARY_NAME} SHARED ${SPINNAKER_SRC_FILES})
    add_library(${NAMESPACE_NAME}::${SPINNAKER_LIBRARY_NAME} ALIAS ${SPINNAKER_LIBRARY_NAME})
    target_compile_options(${SPINNAKER_LIBRARY_NAME} PRIVATE $<$<OR:$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_ID:GNU>>:-Wall -Wextra>)

    FetchContent_Declare(SpyNNakerVisualiser
            GIT_REPOSITORY https://github.com/SpiNNakerManchester/Visualiser)
    FetchContent_MakeAvailable(SpyNNakerVisualiser)
    add_custom_target(
            sPyNNakerExternalDevices ALL
            COMMAND $(MAKE) clean && $(MAKE) CPPFLAGS=-fPIC
            WORKING_DIRECTORY ${spynnakervisualiser_SOURCE_DIR}/spynnaker_external_device_lib)
    include_directories(${spynnakervisualiser_SOURCE_DIR}/spynnaker_external_device_lib)
    add_dependencies(${SPINNAKER_LIBRARY_NAME} sPyNNakerExternalDevices)

    target_include_directories(${SPINNAKER_LIBRARY_NAME} BEFORE
            PUBLIC
            "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>"
            "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>"
            "$<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/include>"

            ${Boost_INCLUDE_DIRS}
            )

    target_link_libraries(${SPINNAKER_LIBRARY_NAME}
            PUBLIC
            ${NRP_EVENT_LOOP_TARGET}
            ${NRP_GEN_LIB_TARGET}
            ${Boost_LIBRARIES}
            ${spynnakervisualiser_SOURCE_DIR}/spynnaker_external_device_lib/libspynnaker_external_device_lib.a
            )
endif()

##########################################
## Python Module
if(NOT ${PYTHON_MODULE_SRC_FILES} STREQUAL "")
    add_library(${PYTHON_MODULE_NAME} SHARED ${PYTHON_MODULE_SRC_FILES})
    add_library(${NAMESPACE_NAME}::${PYTHON_MODULE_NAME} ALIAS ${PYTHON_MODULE_NAME})
    target_compile_options(${PYTHON_MODULE_NAME} PRIVATE $<$<OR:$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_ID:GNU>>:-Wall -Wextra>)
    set_target_properties(${PYTHON_MODULE_NAME} PROPERTIES PREFIX "")

    target_include_directories(${PYTHON_MODULE_NAME}
        PUBLIC
    )

    target_link_libraries(${PYTHON_MODULE_NAME}
        PUBLIC
            ${NRP_EVENT_LOOP_TARGET}
    )

    if(ENABLE_ROS)
        target_link_libraries(${PYTHON_MODULE_NAME}
                PUBLIC
                ${NAMESPACE_NAME}::${ROS_LIBRARY_NAME}
                )
    endif()

    if(ENABLE_MQTT)
        target_link_libraries(${PYTHON_MODULE_NAME}
                PUBLIC
                ${NAMESPACE_NAME}::${MQTT_LIBRARY_NAME}
                )
    endif()

    if(ENABLE_SPINNAKER)
        target_link_libraries(${PYTHON_MODULE_NAME}
                PUBLIC
                ${NAMESPACE_NAME}::${SPINNAKER_LIBRARY_NAME}
                )
    endif()
endif()


##########################################
## generateFNFactoryModule
add_custom_command(
        OUTPUT generateFNFactoryModule
        COMMAND g++ -I/usr/lib/llvm-10/include/ -L/usr/lib/llvm-10/lib ${CMAKE_CURRENT_SOURCE_DIR}/fn_factory_module/generate_fn_factory_module.cpp -x c++ -std=c++17 -lclang -o generateFNFactoryModule)

# Forces the generation of generateFNFactoryModule
add_custom_target(
        GENERATE_FN_FACTORY_MODULE
        COMMAND bash -c "echo \"Building generateFNFactoryModule tool\""
        DEPENDS generateFNFactoryModule)
add_dependencies("${LIBRARY_NAME}" GENERATE_FN_FACTORY_MODULE)

##########################################
## Append Doxygen files
add_doxygen_source("${CMAKE_CURRENT_SOURCE_DIR}" ${EXEC_SRC_FILES} ${LIB_SRC_FILES} ${PYTHON_MODULE_SRC_FILES} ${HEADER_DIRECTORY})

##########################################
## Tests
if(${ENABLE_TESTING} AND NOT "${TEST_SRC_FILES}" STREQUAL "")

    # Compile fn factory module tests
    add_test(
            NAME TEST_FN_FACTORY_MODULE
            COMMAND build_fn_factory_module.sh test_files/fn_factory_module/test_fn_cpp_node.h -DSOURCE_FILENAME=test_files/fn_factory_module/test_fn_cpp_node.cpp
            WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/tests")

    add_test(
            NAME TEST_FN_FACTORY_MODULE_UNNAMED_PARAM_ERROR
            COMMAND build_fn_factory_module.sh unnamed_param_error.h
            WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/tests/test_files/fn_factory_module")

    set_tests_properties(TEST_FN_FACTORY_MODULE_UNNAMED_PARAM_ERROR PROPERTIES WILL_FAIL TRUE)

    add_test(
            NAME TEST_FN_FACTORY_MODULE_WRONG_RETURN_ERROR
            COMMAND build_fn_factory_module.sh wrong_return_error.h
            WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/tests/test_files/fn_factory_module")

    set_tests_properties(TEST_FN_FACTORY_MODULE_WRONG_RETURN_ERROR PROPERTIES WILL_FAIL TRUE)

    add_test(
            NAME TEST_FN_FACTORY_MODULE_WRONG_PARAM_ERROR
            COMMAND build_fn_factory_module.sh wrong_param_error.h
            WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/tests/test_files/fn_factory_module")

    set_tests_properties(TEST_FN_FACTORY_MODULE_WRONG_PARAM_ERROR PROPERTIES WILL_FAIL TRUE)

    add_test(
            NAME TEST_FN_FACTORY_MODULE_WRONG_PARAM_ERROR_2
            COMMAND build_fn_factory_module.sh wrong_param_error_2.h
            WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/tests/test_files/fn_factory_module")

    set_tests_properties(TEST_FN_FACTORY_MODULE_WRONG_PARAM_ERROR_2 PROPERTIES WILL_FAIL TRUE)

    # Create testing executable
    enable_testing()
    add_executable(${TEST_NAME} ${TEST_SRC_FILES})
    target_link_libraries(${TEST_NAME}
        PUBLIC
        ${NRP_EVENT_LOOP_TARGET}
        GTest::GTest
        GTest::Main)

    gtest_add_tests(
        TARGET ${TEST_NAME}
        WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/tests"
        EXTRA_ARGS -VV)

    set_tests_properties(ComputationalNodes.FN_FACTORY_MANAGER PROPERTIES DEPENDS TEST_FN_FACTORY_MODULE)

    # Add python Event Loop test
    add_test(NAME EventLoopPython COMMAND py.test --junitxml "${CMAKE_BINARY_DIR}/xml/EventLoopPython.xml" test_event_loop.py WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/tests)

    if(ENABLE_ROS AND NOT "${ROS_TEST_SRC_FILES}" STREQUAL "")
        enable_testing()
        set(ROS_TEST_NAME "${TEST_NAME}ROS")
        add_executable(${ROS_TEST_NAME} ${ROS_TEST_SRC_FILES})
        target_link_libraries(${ROS_TEST_NAME}
                PUBLIC
                ${NRP_EVENT_LOOP_TARGET}
                ${NAMESPACE_NAME}::${ROS_LIBRARY_NAME}
                GTest::GTest
                GTest::Main)

        gtest_discover_tests(${ROS_TEST_NAME}
                WORKING_DIRECTORY "${PROJECT_SOURCE_DIR}/tests"
                EXTRA_ARGS -VV)
    endif()

    if(ENABLE_MQTT AND NOT "${MQTT_TEST_SRC_FILES}" STREQUAL "")
        enable_testing()
        set(MQTT_TEST_NAME "${TEST_NAME}MQTT")
        add_executable(${MQTT_TEST_NAME} ${MQTT_TEST_SRC_FILES})
        target_link_libraries(${MQTT_TEST_NAME}
                PUBLIC
                ${NRP_EVENT_LOOP_TARGET}
                ${EVENT_LOOP_ENGINE_TARGET}
                NRPProtobuf::ProtoEngineTest
                GTest::GTest
                GTest::Main)

        gtest_discover_tests(${MQTT_TEST_NAME}
                WORKING_DIRECTORY "${PROJECT_SOURCE_DIR}/tests"
                EXTRA_ARGS -VV)

        add_test(NAME EventLoopEnginePython COMMAND py.test --junitxml "${CMAKE_BINARY_DIR}/xml/EventLoopEnginePython.xml" test_event_loop_engine.py WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/tests)
    endif()

endif()

# Add tests based on running example experiments
if(ENABLE_EXAMPLES_TESTING)
    set(EXAMPLES_DIR "${CMAKE_CURRENT_SOURCE_DIR}/tests/examples")
    file(GLOB EXAMPLES RELATIVE ${EXAMPLES_DIR} ${EXAMPLES_DIR}/*)
    foreach(EXAMPLE ${EXAMPLES})
        set(CMD build_fn_factory_module.sh test.h)

        if("${EXAMPLE}" STREQUAL "ros_nodes_std_bool")
            list(APPEND CMD -DNRP_ROS_MSGS_PACKAGES=std_msgs)
        endif()

        if("${EXAMPLE}" STREQUAL "engine_nodes_protobuf_custom")
            list(APPEND CMD -DNRP_PROTO_MSGS_PACKAGES=MyPackage)
            add_test(
                    NAME "${EXAMPLE}_BuildProto"
                    COMMAND nrp_compile_protobuf.py
                    WORKING_DIRECTORY "${EXAMPLES_DIR}/${EXAMPLE}")
        endif()

        add_test(
                NAME "${EXAMPLE}_BuildFNModule"
                COMMAND ${CMD}
                WORKING_DIRECTORY "${EXAMPLES_DIR}/${EXAMPLE}")

        # Disable test if ROS is not installed
        if("${EXAMPLE}" STREQUAL "ros_nodes_std_bool")
            set_tests_properties("${EXAMPLE}_BuildFNModule" PROPERTIES DISABLED $<NOT:$<BOOL:${ENABLE_ROS}>>)
        endif()

        if("${EXAMPLE}" STREQUAL "engine_nodes_protobuf_custom")
            set_tests_properties("${EXAMPLE}_BuildFNModule" PROPERTIES DEPENDS "${EXAMPLE}_BuildProto")
        endif()

        # TODO: 'ros_nodes_std_bool' needs roscore, 'mqtt_nodes_str_protobuf' mqtt broker, also doesn't have a timeout
        # In 'fn_to_fn_edges' config file is not named 'simulation_config.json'
        if(NOT "${EXAMPLE}" STREQUAL "ros_nodes_std_bool" AND NOT "${EXAMPLE}" STREQUAL "mqtt_nodes_str_protobuf" AND NOT "${EXAMPLE}" STREQUAL "fn_to_fn_edges")
            # NOTE: not very useful, because NRPCoreSim always ends without an exception
            add_test(
                    NAME "${EXAMPLE}_Test"
                    COMMAND NRPCoreSim -c simulation_config.json
                    WORKING_DIRECTORY "${EXAMPLES_DIR}/${EXAMPLE}")

            set_tests_properties("${EXAMPLE}_Test" PROPERTIES DEPENDS "${EXAMPLE}_BuildFNModule")
        elseif("${EXAMPLE}" STREQUAL "fn_to_fn_edges")
            add_test(
                NAME "${EXAMPLE}_cpp2cpp_config_Test"
                COMMAND NRPCoreSim -c cpp2cpp_config.json
                WORKING_DIRECTORY "${EXAMPLES_DIR}/${EXAMPLE}")

            set_tests_properties("${EXAMPLE}_cpp2cpp_config_Test" PROPERTIES DEPENDS "${EXAMPLE}_BuildFNModule")

            add_test(
                NAME "${EXAMPLE}_py2cpp_config_Test"
                COMMAND NRPCoreSim -c py2cpp_config.json
                WORKING_DIRECTORY "${EXAMPLES_DIR}/${EXAMPLE}")

            set_tests_properties("${EXAMPLE}_py2cpp_config_Test" PROPERTIES DEPENDS "${EXAMPLE}_BuildFNModule")

            add_test(
                NAME "${EXAMPLE}_cpp2py_config_Test"
                COMMAND NRPCoreSim -c cpp2py_config.json
                WORKING_DIRECTORY "${EXAMPLES_DIR}/${EXAMPLE}")

            set_tests_properties("${EXAMPLE}_cpp2py_config_Test" PROPERTIES DEPENDS "${EXAMPLE}_BuildFNModule")

            add_test(
                NAME "${EXAMPLE}_py2py_config_Test"
                COMMAND NRPCoreSim -c py2py_config.json
                WORKING_DIRECTORY "${EXAMPLES_DIR}/${EXAMPLE}")

            set_tests_properties("${EXAMPLE}_py2py_config_Test" PROPERTIES DEPENDS "${EXAMPLE}_BuildFNModule")

        endif()
    endforeach()
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

# Install python module
if(TARGET ${PYTHON_MODULE_NAME})
    install(TARGETS ${PYTHON_MODULE_NAME}
            DESTINATION "${PYTHON_INSTALL_DIR_REL}/${NRP_PYTHON_MODULE_NAME}/${PYTHON_MODULE_NAME}")

    install(FILES
            "${CMAKE_CURRENT_BINARY_DIR}/src/__init__.py"
            "${CMAKE_CURRENT_SOURCE_DIR}/python/event_loop_interface.py"
            "${CMAKE_CURRENT_SOURCE_DIR}/python/event_loop_engine.py"
            DESTINATION "${PYTHON_INSTALL_DIR_REL}/${NRP_PYTHON_MODULE_NAME}/${PYTHON_MODULE_NAME}")
endif()

if(ENABLE_ROS)
    if(TARGET ${ROS_LIBRARY_NAME})
        install(TARGETS ${ROS_LIBRARY_NAME}
                DESTINATION "${NRP_PLUGIN_INSTALL_DIR}")
    endif()
endif()

if(ENABLE_MQTT)
    if(TARGET ${MQTT_LIBRARY_NAME})
        install(TARGETS ${MQTT_LIBRARY_NAME}
                DESTINATION "${NRP_PLUGIN_INSTALL_DIR}")
    endif()

    if(TARGET ${EVENT_LOOP_ENGINE_TARGET})
        install(TARGETS ${EVENT_LOOP_ENGINE_TARGET}
                DESTINATION "${NRP_PLUGIN_INSTALL_DIR}")
    endif()
endif()

if(ENABLE_SPINNAKER)
    if(TARGET ${SPINNAKER_LIBRARY_NAME})
        install(TARGETS ${SPINNAKER_LIBRARY_NAME}
                DESTINATION "${NRP_PLUGIN_INSTALL_DIR}")
    endif()
endif()

# Configure and Install fn_factory_module
configure_file(fn_factory_module/build_fn_factory_module.sh.in "${CMAKE_CURRENT_BINARY_DIR}/build_fn_factory_module.sh" @ONLY)

install(FILES "${CMAKE_CURRENT_BINARY_DIR}/generateFNFactoryModule" "${CMAKE_CURRENT_BINARY_DIR}/build_fn_factory_module.sh"
        PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE
        DESTINATION ${CMAKE_INSTALL_BINDIR})

install(
        FILES
        fn_factory_module/CMakeLists.txt
        fn_factory_module/fn_factory_module.cpp.template
        DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/${HEADER_DIRECTORY}/cmake/fn_factory_module"
)

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
