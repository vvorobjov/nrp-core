cmake_minimum_required(VERSION 3.16)
project(NRPGazeboGrpcDeviceControllers VERSION ${NRP_VERSION} LANGUAGES CXX)

include(GNUInstallDirs)

# List library build files
set(LIB_SRC_FILES
		nrp_communication_controller.cpp
		gazebo_step_controller.cpp
		camera_device_controller.cpp
		joint_device_controller.cpp
		link_device_controller.cpp
)


##########################################
## Dependencies

find_package(Gazebo REQUIRED)

##########################################
## Header configuration

##########################################
# NRPGazeboGrpcDeviceControllers

add_library(NRPGazeboGrpcDeviceControllers SHARED ${LIB_SRC_FILES})
add_library(NRPGazeboGrpcDeviceControllers::NRPGazeboGrpcDeviceControllers ALIAS NRPGazeboGrpcDeviceControllers)
target_compile_options(NRPGazeboGrpcDeviceControllers PUBLIC $<$<OR:$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_ID:GNU>>:-Wall -Wextra>)

target_link_directories(NRPGazeboGrpcDeviceControllers PUBLIC ${GAZEBO_LIBRARY_DIRS})
set_target_properties(NRPGazeboGrpcDeviceControllers PROPERTIES PREFIX "")

target_include_directories(NRPGazeboGrpcDeviceControllers BEFORE
	PUBLIC 
	    $<INSTALL_INTERFACE:include>
		"$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>"
		"$<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/include>"
		${GAZEBO_INCLUDE_DIRS}

	PRIVATE
		${NRP_GAZEBO_GRPC_LIB}
)

target_link_libraries(NRPGazeboGrpcDeviceControllers
	PUBLIC
	    ${NRP_GEN_LIB_TARGET}
		${GAZEBO_LIBRARIES}
	PRIVATE
		${NRP_GAZEBO_GRPC_LIB}
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

set(INSTALL_CONFIGDIR "${CMAKE_INSTALL_LIBDIR}/cmake/NRPGazeboGrpcDeviceControllers")

# Install library files
install(TARGETS
		NRPGazeboGrpcDeviceControllers
	EXPORT
		NRPGazeboGrpcDeviceControllersTargets
	LIBRARY DESTINATION ${NRP_PLUGIN_INSTALL_DIR}
	ARCHIVE DESTINATION ${NRP_PLUGIN_INSTALL_DIR}
	RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

# Install export target
install(EXPORT NRPGazeboGrpcDeviceControllersTargets
	DESTINATION
		${INSTALL_CONFIGDIR}
	FILE
		NRPGazeboGrpcDeviceControllersTargets.cmake
	NAMESPACE
		"NRPGazeboGrpcDeviceControllers::"
)

# create cmake version and config files
#include(CMakePackageConfigHelpers)
#write_basic_package_version_file(
#	"${CMAKE_CURRENT_BINARY_DIR}/NRPGazeboGrpcDeviceControllersTargetsVersion.cmake"
#	VERSION ${PROJECT_VERSION}
#	COMPATIBILITY AnyNewerVersion
#)
#
#configure_package_config_file("${CMAKE_CURRENT_LIST_DIR}/cmake/ProjectConfig.cmake.in"
#	"${CMAKE_CURRENT_BINARY_DIR}/NRPGazeboGrpcDeviceControllersTargetsConfig.cmake"
#	INSTALL_DESTINATION ${INSTALL_CONFIGDIR}
#)
#
## Install cmake version and config files
#install(
#	FILES
#		"${CMAKE_CURRENT_BINARY_DIR}/NRPGazeboGrpcDeviceControllersTargetsConfig.cmake"
#		"${CMAKE_CURRENT_BINARY_DIR}/NRPGazeboGrpcDeviceControllersTargetsVersion.cmake"
#	DESTINATION ${INSTALL_CONFIGDIR}
#)
