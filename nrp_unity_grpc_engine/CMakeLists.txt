set(PROJECT_NAME "NRPUnityGrpcEngine")
set(HEADER_DIRECTORY "nrp_unity_grpc_engine")

set(NAMESPACE_NAME "${PROJECT_NAME}")

set(LIBRARY_NAME "${PROJECT_NAME}")
set(PYTHON_MODULE_NAME "${PROJECT_NAME}Python")
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
	devices/info.cpp
	devices/camera.cpp
	nrp_unity_grpc_engine/config/unity_grpc_config.cpp
	nrp_unity_grpc_engine/nrp_client/unity_engine_grpc_nrp_client.cpp
)

# List of python module build files
set(PYTHON_MODULE_SRC_FILES
#	nrp_gazebo_grpc_engine/python/python_module.cpp
)

# List executable build files
set(EXEC_SRC_FILES
)

# List testing build files
set(TEST_SRC_FILES
)


##########################################
## Dependencies

##########################################
## Header configuration

# Gazebo defines
configure_file("nrp_unity_grpc_engine/config/cmake_constants.h.in" "${CMAKE_CURRENT_BINARY_DIR}/include/${HEADER_DIRECTORY}/config/cmake_constants.h" @ONLY)

# Python module dependencies
configure_file("nrp_unity_grpc_engine/python/__init__.py.in" "${CMAKE_CURRENT_BINARY_DIR}/src/__init__.py" @ONLY)

##########################################
# NRPUnityGrpcEngine
add_library("${LIBRARY_NAME}" SHARED ${LIB_SRC_FILES})
add_library(${NAMESPACE_NAME}::${LIBRARY_NAME} ALIAS ${LIBRARY_NAME})
target_compile_options(${LIBRARY_NAME} PUBLIC $<$<OR:$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_ID:GNU>>:-Wall -Wextra>)

set_target_properties(${LIBRARY_NAME} PROPERTIES PREFIX "")

target_include_directories(${LIBRARY_NAME} BEFORE
	PUBLIC 
		"$<INSTALL_INTERFACE:include>"
		"$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>"
		"$<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/include>"

	PRIVATE
)

target_link_libraries(${LIBRARY_NAME}
	PUBLIC
	    #NRPGazeboDevices::NRPGazeboDevices
		NRPGRPCEngineProtocol::NRPGRPCEngineProtocol
)


##########################################
## NRPUnityDevicePythonModule
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
		    ${NAMESPACE_NAME}::${LIBRARY_NAME}
	)
endif()


##########################################
## Executable
if(NOT "${EXEC_SRC_FILES}" STREQUAL "")
	add_executable("${EXECUTABLE_NAME}" ${EXEC_SRC_FILES})
	target_link_libraries("${EXECUTABLE_NAME}" "${LIBRARY_NAME}")
endif()


##########################################
## Append Doxygen files
add_doxygen_source("${CMAKE_CURRENT_SOURCE_DIR}" ${EXEC_SRC_FILES} ${LIB_SRC_FILES} ${PYTHON_MODULE_SRC_FILES} ${HEADER_DIRECTORY})


## This section needs Gazebo libraries installed


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
		DESTINATION "${PYTHON_INSTALL_DIR_REL}/${PYTHON_MODULE_NAME}")

	install(FILES "${CMAKE_CURRENT_BINARY_DIR}/src/__init__.py"
		DESTINATION "${PYTHON_INSTALL_DIR_REL}/${PYTHON_MODULE_NAME}")
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
