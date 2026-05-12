set(PROJECT_NAME "nrp_ros_python_bindings")
set(HEADER_DIRECTORY "python_bindings")

set(NAMESPACE_NAME "${PROJECT_NAME}")

set(LIBRARY_NAME "${PROJECT_NAME}")
set(PYTHON_MODULE_NAME "nrp_ros")
set(EXECUTABLE_NAME "${PROJECT_NAME}Main")
set(TEST_NAME "${PROJECT_NAME}Tests")

set(LIB_EXPORT_NAME "${LIBRARY_NAME}Targets")
set(LIB_CONFIG_NAME "${LIBRARY_NAME}Config")
set(LIB_VERSION_NAME "${LIB_CONFIG_NAME}Version")

set(GEN_PYTHON_ROOT ${CMAKE_CURRENT_BINARY_DIR}/${PYTHON_MODULE_NAME})

cmake_minimum_required(VERSION 3.16)
project("${PROJECT_NAME}" VERSION ${NRP_VERSION})

include(GNUInstallDirs)


# List testing build files
set(TEST_SRC_FILES
        tests/rosmsg_converter.cpp
        )
##########################################
## Dependencies (ROS 2)

message(STATUS "[nrp_ros_python_bindings] Configuring ROS 2 Boost.Python bindings (ament_cmake + rclcpp + rosidl packages)")

find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
# Humble's rclcpp cmake export needs fastcdr as a target.
find_package(fastcdr QUIET)
find_package(builtin_interfaces REQUIRED)
find_package(nrp_ros_msgs REQUIRED)
foreach(PACK ${NRP_ROS_MSGS_PACKAGES})
    message(STATUS "[nrp_ros_python_bindings] Locating extra ROS 2 msg package: ${PACK}")
    find_package(${PACK} REQUIRED)
endforeach()

find_package(Threads REQUIRED)
find_package(Boost COMPONENTS
  python
  thread
  system
  REQUIRED
  )

find_package(PythonLibs REQUIRED)

##########################################
## Macro definitions which perform the python bindings generation

# This macro adds boost python bindings target
macro(add_python_target ROS_PACKAGE)
    include_directories(
            ${PYTHON_INCLUDE_PATH}
            ${Boost_INCLUDE_DIRS}
    )

    add_library(${ROS_PACKAGE} SHARED ${ARGN})

    set_target_properties(${ROS_PACKAGE}
            PROPERTIES
            OUTPUT_ROS_PACKAGE ${ROS_PACKAGE}
            LIBRARY_OUTPUT_DIRECTORY ${GEN_PYTHON_ROOT}
            PREFIX ""
            )

    target_link_libraries(${ROS_PACKAGE}
            ${NRP_GEN_LIB_TARGET}
            ${Boost_LIBRARIES}
            ${PYTHON_LIBRARIES}
            rostime_boost_python
            )

    # Pull ROS 2 include dirs and link libs from ament
    ament_target_dependencies(${ROS_PACKAGE} rclcpp builtin_interfaces nrp_ros_msgs ${NRP_ROS_MSGS_PACKAGES})
endmacro()

# Top level macro generates boost python bindings as well as python conversion wrapper
# for all messages in a given ROS 2 package `ROS_PACKAGE`
#
# PACKAGE_SRC_PATH is the filesystem path to the package's source tree
# (containing msg/*.msg). If empty, the generator script falls back to
# ament_index_python.packages.get_package_share_directory(ROS_PACKAGE).
macro(gen_ros_package_python ROS_PACKAGE PACKAGE_SRC_PATH)
    set(GEN_DIR ${CMAKE_CURRENT_BINARY_DIR}/${ROS_PACKAGE}) #this is for generated cpp files.

    message(STATUS "[nrp_ros_python_bindings] Generating ROS 2 bindings for ${ROS_PACKAGE} (src=${PACKAGE_SRC_PATH})")

    execute_process(COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/scripts/generate_pkg_bindings.py
            --package=${ROS_PACKAGE}
            --package_path=${PACKAGE_SRC_PATH}
            --cpp_target_dir=${GEN_DIR}
            --py_target_dir=${GEN_PYTHON_ROOT}
            --current_package=${PROJECT_NAME}
            RESULT_VARIABLE _gen_result)
    if(NOT _gen_result EQUAL 0)
        message(FATAL_ERROR "generate_pkg_bindings.py failed for ${ROS_PACKAGE} (exit=${_gen_result})")
    endif()

    file(GLOB ${ROS_PACKAGE}_srcs  ${GEN_DIR}/*.cpp)

    add_python_target(${ROS_PACKAGE}
            ${${ROS_PACKAGE}_srcs}
            )

    list(POP_FRONT ${ROS_PACKAGE}_srcs)
    foreach(ROS_MSG ${${ROS_PACKAGE}_srcs})
        string(FIND "${ROS_MSG}" "_" pos REVERSE)
        string(FIND "${ROS_MSG}" "." end REVERSE)
        math(EXPR msg_pos "${pos}+1")
        math(EXPR msg_end "${end}-${msg_pos}")
        string(SUBSTRING "${ROS_MSG}" "${msg_pos}" "${msg_end}" msg_name)
        # ROS 2 headers use snake_case: "MyMsg" -> "my_msg.hpp".
        # Apply the two standard camel-to-snake regexes, then lowercase.
        string(REGEX REPLACE "(.)([A-Z][a-z]+)" "\\1_\\2" msg_name_sc "${msg_name}")
        string(REGEX REPLACE "([a-z0-9])([A-Z])" "\\1_\\2" msg_name_sc "${msg_name_sc}")
        string(TOLOWER "${msg_name_sc}" msg_name_sc)
        # ROS 2 type keys: "<pkg>::msg::<Msg>"; headers "<pkg>/msg/<snake>.hpp"
        file(APPEND "${CMAKE_BINARY_DIR}/nrp_ros_msg_types.txt" "${ROS_PACKAGE}::msg::${msg_name};")
        file(APPEND "${CMAKE_BINARY_DIR}/nrp_ros_msg_headers.txt" "${ROS_PACKAGE}/msg/${msg_name_sc}.hpp;")
    endforeach()
endmacro()

##########################################
## Create init files in python module containing the generated python bindings
file(WRITE ${GEN_PYTHON_ROOT}/__init__.py "\n")

##########################################
## Python bindings for the ROS 2 builtin_interfaces/Time and /Duration
## (these have no .msg file in-tree; they ship with rclcpp).
add_definitions("-Wl,--no-undefined ")
# SHARED not the default STATIC: this target is both a Python extension
# (`PREFIX ""` + `LIBRARY_OUTPUT_DIRECTORY ${GEN_PYTHON_ROOT}` below
# only make sense for a loadable .so) AND a library that the per-
# package boost::python bindings link against (see the
# `target_link_libraries(${ROS_PACKAGE} ... rostime_boost_python ...)`
# in gen_ros_package_python a few lines above). MODULE would satisfy
# Python's import but cmake refuses `target_link_libraries(... <module>)`,
# so SHARED is the only fit.
#
# The previous bare
#   add_library(rostime_boost_python src/rostime_exports.cpp)
# produced rostime_boost_python.a, the `install(DIRECTORY)` rule below
# installed nothing for this module, and
# `import nrp_core.data.nrp_ros.rostime_boost_python` raised
# ModuleNotFoundError. That broke the RosMsgConverter tests (#70-72)
# because their SetUpTestSuite imports test_functions.py which in turn
# imports rostime_boost_python at module load. See EBR2-77.
add_library(rostime_boost_python SHARED src/rostime_exports.cpp)
set_target_properties(rostime_boost_python
  PROPERTIES
  LIBRARY_OUTPUT_DIRECTORY ${GEN_PYTHON_ROOT}
  PREFIX ""
)
target_link_libraries(rostime_boost_python
        ${NRP_GEN_LIB_TARGET}
        ${Boost_LIBRARIES}
        ${PYTHON_LIBRARIES}
        )
ament_target_dependencies(rostime_boost_python rclcpp builtin_interfaces)

##########################################
file(WRITE "${CMAKE_BINARY_DIR}/nrp_ros_msg_types.txt" "")
file(WRITE "${CMAKE_BINARY_DIR}/nrp_ros_msg_headers.txt" "")

## Generate python bindings for nrp-core msg definitions (in-tree submodule)
gen_ros_package_python(nrp_ros_msgs "${NRP_CORE_MSGS_PATH}/nrp_ros_msgs")
## Generate python bindings for additional external ros packages (installed via ROS 2 apt)
foreach(PACK ${NRP_ROS_MSGS_PACKAGES})
    gen_ros_package_python(${PACK} "")
endforeach()

#########################################
## Install python modules
install(DIRECTORY ${GEN_PYTHON_ROOT}
        DESTINATION "${PYTHON_INSTALL_DIR_REL}/${NRP_PYTHON_MODULE_NAME}/data")



## Header configuration

configure_file("python_bindings/config/cmake_constants.h.in" "${CMAKE_CURRENT_BINARY_DIR}/include/${HEADER_DIRECTORY}/config/cmake_constants.h" @ONLY)
#configure_file("python/__init__.py.in" "${CMAKE_CURRENT_BINARY_DIR}/src/__init__.py" @ONLY)




##########################################
## Tests
if(${ENABLE_TESTING} AND NOT "${TEST_SRC_FILES}" STREQUAL "")
    # Create testing executable
    enable_testing()
    add_executable(${TEST_NAME} ${TEST_SRC_FILES})
    target_compile_options(${TEST_NAME} PUBLIC $<$<OR:$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_ID:GNU>>:${NRP_COMMON_COMPILATION_FLAGS}>)
    target_link_options(${TEST_NAME} PUBLIC ${NRP_COMMON_LD_FLAGS})
    # ament_target_dependencies uses the plain signature of
    # target_link_libraries; must match here (no PUBLIC/PRIVATE keyword).
    target_link_libraries(${TEST_NAME}
            ${NRP_GEN_LIB_TARGET}
            GTest::GTest
            GTest::Main)
    ament_target_dependencies(${TEST_NAME} rclcpp builtin_interfaces nrp_ros_msgs ${NRP_ROS_MSGS_PACKAGES})

    gtest_discover_tests(${TEST_NAME}
            WORKING_DIRECTORY "${PROJECT_SOURCE_DIR}/tests"
            EXTRA_ARGS -VV)

    target_include_directories(${TEST_NAME} BEFORE
            PUBLIC
            "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>"
            "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>"
            "$<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/include>"

            PRIVATE
            )
endif()




##########################################
## Installation

set(INSTALL_CONFIGDIR "${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}")

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

