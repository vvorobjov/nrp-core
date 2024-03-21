cmake_minimum_required(VERSION 3.16)
project(NRPGazeboJSONDataPackControllers VERSION ${NRP_VERSION} LANGUAGES CXX)

include(GNUInstallDirs)

# List library build files
set(LIB_SRC_FILES
        nrp_communication_controller.cpp
        gazebo_step_controller.cpp
        camera_datapack_controller.cpp
        joint_datapack_controller.cpp
        link_datapack_controller.cpp
)


##########################################
## Dependencies

find_package(Gazebo REQUIRED)

##########################################
## Header configuration

##########################################
# NRPGazeboJSONDataPackControllers

add_library(NRPGazeboJSONDataPackControllers SHARED ${LIB_SRC_FILES})
add_library(NRPGazeboJSONDataPackControllers::NRPGazeboJSONDataPackControllers ALIAS NRPGazeboJSONDataPackControllers)
target_compile_options(NRPGazeboJSONDataPackControllers PUBLIC $<$<OR:$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_ID:GNU>>:-Wall -Wextra>)

target_link_directories(NRPGazeboJSONDataPackControllers PUBLIC ${GAZEBO_LIBRARY_DIRS})
set_target_properties(NRPGazeboJSONDataPackControllers PROPERTIES PREFIX "")

target_include_directories(NRPGazeboJSONDataPackControllers BEFORE
    PUBLIC 
        $<INSTALL_INTERFACE:include>
        "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>"
        "$<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/include>"
        ${GAZEBO_INCLUDE_DIRS}

    PRIVATE
        ${NRP_GAZEBO_JSON_LIB}
)

target_link_libraries(NRPGazeboJSONDataPackControllers
    PUBLIC
        ${NRP_GEN_LIB_TARGET}
        ${GAZEBO_LIBRARIES}
        NRPProtobuf::ProtoGazebo
    PRIVATE
        ${NRP_GAZEBO_JSON_LIB}
)


##########################################
## PythonModule


##########################################
## Executable


##########################################
## Append Doxygen files
add_doxygen_source("${CMAKE_CURRENT_SOURCE_DIR}" ${LIB_SRC_FILES})


##########################################
## Tests


##########################################
## Installation

set(INSTALL_CONFIGDIR "${CMAKE_INSTALL_LIBDIR}/cmake/NRPGazeboJSONDataPackControllers")

# Install library files
install(TARGETS
        NRPGazeboJSONDataPackControllers
    EXPORT
        NRPGazeboJSONDataPackControllersTargets
    LIBRARY DESTINATION ${NRP_PLUGIN_INSTALL_DIR}
    ARCHIVE DESTINATION ${NRP_PLUGIN_INSTALL_DIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

# Install export target
install(EXPORT NRPGazeboJSONDataPackControllersTargets
    DESTINATION
        ${INSTALL_CONFIGDIR}
    FILE
        NRPGazeboJSONDataPackControllersTargets.cmake
    NAMESPACE
        "NRPGazeboJSONDataPackControllers::"
)

# create cmake version and config files
#include(CMakePackageConfigHelpers)
#write_basic_package_version_file(
#   "${CMAKE_CURRENT_BINARY_DIR}/NRPGazeboJSONDataPackControllersTargetsVersion.cmake"
#   VERSION ${PROJECT_VERSION}
#   COMPATIBILITY AnyNewerVersion
#)
#
#configure_package_config_file("${CMAKE_CURRENT_LIST_DIR}/cmake/ProjectConfig.cmake.in"
#   "${CMAKE_CURRENT_BINARY_DIR}/NRPGazeboJSONDataPackControllersTargetsConfig.cmake"
#   INSTALL_DESTINATION ${INSTALL_CONFIGDIR}
#)
#
## Install cmake version and config files
#install(
#   FILES
#       "${CMAKE_CURRENT_BINARY_DIR}/NRPGazeboJSONDataPackControllersTargetsConfig.cmake"
#       "${CMAKE_CURRENT_BINARY_DIR}/NRPGazeboJSONDataPackControllersTargetsVersion.cmake"
#   DESTINATION ${INSTALL_CONFIGDIR}
#)
