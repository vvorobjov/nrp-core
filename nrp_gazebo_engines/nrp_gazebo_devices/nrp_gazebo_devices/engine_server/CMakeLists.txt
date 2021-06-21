cmake_minimum_required(VERSION 3.16)
project(NRPGazeboDeviceControllers VERSION ${NRP_VERSION} LANGUAGES CXX)

include(GNUInstallDirs)

# List library build files
set(LIB_SRC_FILES
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
# NRPGazeboDeviceControllers

add_library(NRPGazeboDeviceControllers SHARED ${LIB_SRC_FILES})
add_library(NRPGazeboDeviceControllers::NRPGazeboDeviceControllers ALIAS NRPGazeboDeviceControllers)
target_compile_options(NRPGazeboDeviceControllers PUBLIC $<$<OR:$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_ID:GNU>>:${NRP_COMMON_COMPILATION_FLAGS}>)

target_link_directories(NRPGazeboDeviceControllers PUBLIC ${GAZEBO_LIBRARY_DIRS})
set_target_properties(NRPGazeboDeviceControllers PROPERTIES PREFIX "")

target_include_directories(NRPGazeboDeviceControllers BEFORE
	PUBLIC 
	    $<INSTALL_INTERFACE:include>
		"$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>"
		"$<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/include>"
		${GAZEBO_INCLUDE_DIRS}

	PRIVATE
		${NRP_GAZEBO_DEVICES_LIB_TARGET}
)

target_link_libraries(NRPGazeboDeviceControllers
	PUBLIC
	    ${NRP_GEN_LIB_TARGET}
		${GAZEBO_LIBRARIES}
		${NRP_GAZEBO_DEVICES_LIB_TARGET}
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

set(INSTALL_CONFIGDIR "${CMAKE_INSTALL_LIBDIR}/cmake/NRPGazeboDeviceControllers")

# Install library files
install(TARGETS
		NRPGazeboDeviceControllers
	EXPORT
		NRPGazeboDeviceControllersTargets
	LIBRARY DESTINATION ${NRP_PLUGIN_INSTALL_DIR}
	ARCHIVE DESTINATION ${NRP_PLUGIN_INSTALL_DIR}
	RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

# Install export target
install(EXPORT NRPGazeboDeviceControllersTargets
	DESTINATION
		${INSTALL_CONFIGDIR}
	FILE
		NRPGazeboDeviceControllersTargets.cmake
	NAMESPACE
		"NRPGazeboDeviceControllers::"
)

# create cmake version and config files
include(CMakePackageConfigHelpers)
write_basic_package_version_file(
	"${CMAKE_CURRENT_BINARY_DIR}/NRPGazeboDeviceControllersTargetsVersion.cmake"
	VERSION ${PROJECT_VERSION}
	COMPATIBILITY AnyNewerVersion
)

configure_package_config_file("${CMAKE_CURRENT_LIST_DIR}/cmake/ProjectConfig.cmake.in"
	"${CMAKE_CURRENT_BINARY_DIR}/NRPGazeboDeviceControllersTargetsConfig.cmake"
	INSTALL_DESTINATION ${INSTALL_CONFIGDIR}
)

# Install cmake version and config files
install(
	FILES
		"${CMAKE_CURRENT_BINARY_DIR}/NRPGazeboDeviceControllersTargetsConfig.cmake"
		"${CMAKE_CURRENT_BINARY_DIR}/NRPGazeboDeviceControllersTargetsVersion.cmake"
	DESTINATION ${INSTALL_CONFIGDIR}
)
