cmake_minimum_required(VERSION 3.16)
project(NRPGazeboGrpcPlugins VERSION ${NRP_VERSION} LANGUAGES CXX)

include(GNUInstallDirs)

find_package(Gazebo REQUIRED)

find_package(PkgConfig)
pkg_check_modules(Pistache REQUIRED IMPORTED_TARGET libpistache)

find_package(nlohmann_json REQUIRED)

if(NOT DEFINED NRP_GAZEBO_COMMUNICATION_PLUGIN)
    set(NRP_GAZEBO_COMMUNICATION_PLUGIN NRPGazeboGrpcCommunicationPlugin)
endif()

########################################
## Gazebo Communication Plugin
add_library(${NRP_GAZEBO_COMMUNICATION_PLUGIN} SHARED "nrp_communication_controller/nrp_communication_plugin.cpp")
add_library("NRPGazeboGrpcPlugins::${NRP_GAZEBO_COMMUNICATION_PLUGIN}" ALIAS ${NRP_GAZEBO_COMMUNICATION_PLUGIN})
target_compile_options(${NRP_GAZEBO_COMMUNICATION_PLUGIN} PRIVATE $<$<OR:$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_ID:GNU>>:${NRP_COMMON_COMPILATION_FLAGS}>)

target_include_directories(${NRP_GAZEBO_COMMUNICATION_PLUGIN}
    PUBLIC
        $<INSTALL_INTERFACE:include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
        $<BUILD_INTERFACE:${GAZEBO_INCLUDE_DIRS}>

    PRIVATE
)

target_link_libraries(${NRP_GAZEBO_COMMUNICATION_PLUGIN}
    PUBLIC
        ${NRP_GAZEBO_LIB_TARGETS}

    PRIVATE
        ${NRP_GEN_LIB_TARGET}
        ${GAZEBO_LIBRARIES}
)

########################################
## Gazebo Async Communication Plugin
if(ENABLE_MQTT)
    add_library(NRPGazeboAsyncPlugin SHARED "nrp_communication_controller/nrp_async_plugin.cpp")
    add_library("NRPGazeboGrpcPlugins::NRPGazeboAsyncPlugin" ALIAS NRPGazeboAsyncPlugin)
    target_compile_options(NRPGazeboAsyncPlugin PRIVATE $<$<OR:$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_ID:GNU>>:${NRP_COMMON_COMPILATION_FLAGS}>)

    target_include_directories(NRPGazeboAsyncPlugin
            PUBLIC
            $<INSTALL_INTERFACE:include>
            $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
            $<BUILD_INTERFACE:${GAZEBO_INCLUDE_DIRS}>

            PRIVATE
            )

    target_link_libraries(NRPGazeboAsyncPlugin
            PUBLIC
            ${NRP_GAZEBO_LIB_TARGETS}

            PRIVATE
            ${NRP_GEN_LIB_TARGET}
            ${NRP_ELE_TARGET}
            ${GAZEBO_LIBRARIES}
            )
endif()

########################################
## Gazebo World Plugin
add_library(NRPGazeboGrpcWorldPlugin SHARED "nrp_world_plugin/nrp_world_plugin.cpp")
add_library(NRPGazeboGrpcPlugins::NRPGazeboGrpcWorldPlugin ALIAS NRPGazeboGrpcWorldPlugin)
target_compile_options(NRPGazeboGrpcWorldPlugin PRIVATE $<$<OR:$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_ID:GNU>>:${NRP_COMMON_COMPILATION_FLAGS}>)
target_include_directories(NRPGazeboGrpcWorldPlugin
    PUBLIC
        $<INSTALL_INTERFACE:include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
        $<BUILD_INTERFACE:${GAZEBO_INCLUDE_DIRS}>

    PRIVATE
        ${NRP_GAZEBO_LIB_TARGETS}
)

target_link_libraries(NRPGazeboGrpcWorldPlugin
    PUBLIC
        ${NRP_GAZEBO_LIB_TARGETS}

    PRIVATE
        ${NRP_GEN_LIB_TARGET}
        ${GAZEBO_LIBRARIES}
)


########################################
## Gazebo Joint Controller Plugin
add_library(NRPGazeboGrpcJointControllerPlugin SHARED "nrp_joint_controller/nrp_joint_controller.cpp")
add_library(NRPGazeboGrpcPlugins::NRPGazeboGrpcJointControllerPlugin ALIAS NRPGazeboGrpcJointControllerPlugin)
target_compile_options(NRPGazeboGrpcJointControllerPlugin PRIVATE $<$<OR:$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_ID:GNU>>:${NRP_COMMON_COMPILATION_FLAGS}>)

target_include_directories(NRPGazeboGrpcJointControllerPlugin
    PUBLIC
        $<INSTALL_INTERFACE:include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
        $<BUILD_INTERFACE:${GAZEBO_INCLUDE_DIRS}>
)

target_link_libraries(NRPGazeboGrpcJointControllerPlugin
    PUBLIC
        ${NRP_GAZEBO_LIB_TARGETS}

    PRIVATE
        ${NRP_GEN_LIB_TARGET}
        ${GAZEBO_LIBRARIES}
)


########################################
## Gazebo Link Controller Plugin
add_library(NRPGazeboGrpcLinkControllerPlugin SHARED "nrp_link_controller_plugin/nrp_link_controller_plugin.cpp")
add_library(NRPGazeboGrpcPlugins::NRPGazeboGrpcLinkControllerPlugin ALIAS NRPGazeboGrpcLinkControllerPlugin)
target_compile_options(NRPGazeboGrpcLinkControllerPlugin PRIVATE $<$<OR:$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_ID:GNU>>:${NRP_COMMON_COMPILATION_FLAGS}>)
add_library(NRPGazeboGrpcPlugins::NRPGazeboGrpcLinkControllerPlugin ALIAS NRPGazeboGrpcLinkControllerPlugin)

target_include_directories(NRPGazeboGrpcLinkControllerPlugin
    PUBLIC
        $<INSTALL_INTERFACE:include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
        $<BUILD_INTERFACE:${GAZEBO_INCLUDE_DIRS}>
)

target_link_libraries(NRPGazeboGrpcLinkControllerPlugin
    PUBLIC
        nlohmann_json::nlohmann_json
        ${NRP_GAZEBO_LIB_TARGETS}

    PRIVATE
        ${NRP_GEN_LIB_TARGET}
        ${GAZEBO_LIBRARIES}
)

########################################
## Gazebo Model Controller Plugin
add_library(NRPGazeboGrpcModelControllerPlugin SHARED "nrp_model_controller_plugin/nrp_model_controller_plugin.cpp")
add_library(NRPGazeboGrpcPlugins::NRPGazeboGrpcModelControllerPlugin ALIAS NRPGazeboGrpcModelControllerPlugin)
target_compile_options(NRPGazeboGrpcModelControllerPlugin PRIVATE $<$<OR:$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_ID:GNU>>:${NRP_COMMON_COMPILATION_FLAGS}>)
add_library(NRPGazeboGrpcPlugins::NRPGazeboGrpcModelControllerPlugin ALIAS NRPGazeboGrpcModelControllerPlugin)

target_include_directories(NRPGazeboGrpcModelControllerPlugin
        PUBLIC
        $<INSTALL_INTERFACE:include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
        $<BUILD_INTERFACE:${GAZEBO_INCLUDE_DIRS}>
        )

target_link_libraries(NRPGazeboGrpcModelControllerPlugin
        PUBLIC
        nlohmann_json::nlohmann_json
        ${NRP_GAZEBO_LIB_TARGETS}

        PRIVATE
        ${NRP_GEN_LIB_TARGET}
        ${GAZEBO_LIBRARIES}
        )


########################################
## Gazebo Camera Controller Plugin
add_library(NRPGazeboGrpcCameraControllerPlugin SHARED "nrp_camera_controller_plugin/nrp_camera_controller_plugin.cpp")
add_library(NRPGazeboGrpcPlugins::NRPGazeboGrpcCameraControllerPlugin ALIAS NRPGazeboGrpcCameraControllerPlugin)
target_compile_options(NRPGazeboGrpcCameraControllerPlugin PRIVATE $<$<OR:$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_ID:GNU>>:${NRP_COMMON_COMPILATION_FLAGS}>)

target_include_directories(NRPGazeboGrpcCameraControllerPlugin
    PUBLIC
        $<INSTALL_INTERFACE:include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
        $<BUILD_INTERFACE:${GAZEBO_INCLUDE_DIRS}>

    PRIVATE
        ${NRP_GAZEBO_LIB_TARGETS}
)

target_link_libraries(NRPGazeboGrpcCameraControllerPlugin
    PUBLIC
        ${NRP_GAZEBO_LIB_TARGETS}

    PRIVATE
        ${NRP_GEN_LIB_TARGET}
        ${GAZEBO_LIBRARIES}
        CameraPlugin
)


##########################################
## Append Doxygen files
add_doxygen_source("${CMAKE_CURRENT_SOURCE_DIR}" "")


########################################
## Configure targets
set(PLUGIN_TARGETS ${NRP_GAZEBO_COMMUNICATION_PLUGIN} NRPGazeboGrpcWorldPlugin
    NRPGazeboGrpcJointControllerPlugin NRPGazeboGrpcLinkControllerPlugin NRPGazeboGrpcModelControllerPlugin
    NRPGazeboGrpcCameraControllerPlugin)

if(ENABLE_MQTT)
    list(APPEND PLUGIN_TARGETS NRPGazeboAsyncPlugin)
endif()

set_target_properties(${PLUGIN_TARGETS}
    PROPERTIES PREFIX "")

list(JOIN PLUGIN_TARGETS "," PLUGIN_TARGETS_STRING)
set(NRP_GAZEBO_PLUGINS "${PLUGIN_TARGETS_STRING}"
    CACHE INTERNAL "List of compiled NRP Gazebo plugins")


########################################
## Installation
install(TARGETS ${PLUGIN_TARGETS}
    LIBRARY DESTINATION ${NRP_GAZEBO_PLUGINS_INSTALL_DIR}
    ARCHIVE DESTINATION ${NRP_GAZEBO_PLUGINS_INSTALL_DIR})
