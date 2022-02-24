set(PROJECT_NAME "nrp_ros_python_bindings")
set(PYTHON_MODULE_NAME "nrp_ros")

set(GEN_PYTHON_ROOT ${CMAKE_CURRENT_BINARY_DIR}/${PYTHON_MODULE_NAME})

cmake_minimum_required(VERSION 3.16)
project("${PROJECT_NAME}" VERSION ${NRP_VERSION})

include(GNUInstallDirs)

##########################################
## Dependencies
find_package(catkin REQUIRED COMPONENTS roscpp nrp_ros_msgs ${NRP_CATKIN_PACKAGES})

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
            ${catkin_INCLUDE_DIRS}
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
            ${catkin_LIBRARIES}
            rostime_boost_python
            )
endmacro()

# Top level macro generates boost python bindings as well as python conversion wrapper 
# for all messages in a given ros package `ROS_PACKAGE`
macro(gen_ros_package_python ROS_PACKAGE)
    set(GEN_DIR ${CMAKE_CURRENT_BINARY_DIR}/${ROS_PACKAGE}) #this is for generated cpp files.

    execute_process(COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/scripts/generate_pkg_bindings.py
            --package=${ROS_PACKAGE}
            --cpp_target_dir=${GEN_DIR}
            --py_target_dir=${GEN_PYTHON_ROOT}
            --current_package=${PROJECT_NAME}
            )

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
        file(APPEND "${CMAKE_BINARY_DIR}/nrp_ros_msg_types.txt" "${ROS_PACKAGE}::${msg_name};")
        file(APPEND "${CMAKE_BINARY_DIR}/nrp_ros_msg_headers.txt" "${ROS_PACKAGE}/${msg_name}.h;")
    endforeach()
endmacro()

##########################################
## Create init files in python module containing the generated python bindings
file(WRITE ${GEN_PYTHON_ROOT}/__init__.py "\n")

##########################################
## We need a special library for the ros time types since these are ROS builtins that don't have .msg files
add_definitions("-Wl,--no-undefined ")
add_library(rostime_boost_python src/rostime_exports.cpp)
set_target_properties(rostime_boost_python
  PROPERTIES
  LIBRARY_OUTPUT_DIRECTORY ${GEN_PYTHON_ROOT}
  PREFIX ""
)
target_link_libraries(rostime_boost_python
        ${NRP_GEN_LIB_TARGET}
        ${Boost_LIBRARIES}
        ${PYTHON_LIBRARIES}
        ${catkin_LIBRARIES}
        )

##########################################
file(WRITE "${CMAKE_BINARY_DIR}/nrp_ros_msg_types.txt" "")
file(WRITE "${CMAKE_BINARY_DIR}/nrp_ros_msg_headers.txt" "")

## Generate python bindings for nrp-core msg definitions
gen_ros_package_python(nrp_ros_msgs)
## Generate python bindings for additional external ros packages
foreach(PACK ${NRP_CATKIN_PACKAGES})
    gen_ros_package_python(${PACK})
endforeach()

#########################################
## Install python modules
install(DIRECTORY ${GEN_PYTHON_ROOT}
        DESTINATION "${PYTHON_INSTALL_DIR_REL}/${NRP_PYTHON_MODULE_NAME}/data")
