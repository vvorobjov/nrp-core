set(PROJECT_NAME "CreateFNModule")
set(NAMESPACE_NAME "${PROJECT_NAME}")

cmake_minimum_required(VERSION 3.16)
project("${PROJECT_NAME}")

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fpic")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fpic")
set (CMAKE_CXX_STANDARD 17)

# Set header and source file path
if(NOT DEFINED HEADER_FILE_PATH)
    message(FATAL_ERROR "HEADER_FILE_PATH var not defined and it is required")
endif()

if(NOT EXISTS "${HEADER_FILE_PATH}" OR IS_DIRECTORY "${HEADER_FILE_PATH}")
    message(FATAL_ERROR "${HEADER_FILE_PATH} does not exists or is not a file")
endif()

if(DEFINED SOURCE_FILENAME)
    set(SOURCE_FILE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/../${SOURCE_FILENAME}")
    if(NOT EXISTS "${SOURCE_FILE_PATH}" OR IS_DIRECTORY "${SOURCE_FILE_PATH}")
        message(FATAL_ERROR "${SOURCE_FILE_PATH} does not exists or is not a file")
    endif()
endif()

if(NOT DEFINED FN_FACTORY_MODULE_INSTALL_DIR)
    message(FATAL_ERROR "FN_FACTORY_MODULE_INSTALL_DIR var not defined and it is required")
elseif(NOT EXISTS "${FN_FACTORY_MODULE_INSTALL_DIR}" OR NOT IS_DIRECTORY "${FN_FACTORY_MODULE_INSTALL_DIR}")
    message(FATAL_ERROR "${FN_FACTORY_MODULE_INSTALL_DIR} does not exists or is not a directory")
endif()

# Find dependencies
find_package(NRPEventLoop REQUIRED)

if(NOT ${NRP_ROS_MSGS_PACKAGES} STREQUAL "")
    find_package(catkin REQUIRED COMPONENTS roscpp ${NRP_ROS_MSGS_PACKAGES})
endif()

# clang include statement
set(GENERATE_FN_TOOL "generateFNFactoryModule")
set(FN_GEN_CMND ${GENERATE_FN_TOOL} ${HEADER_FILE_PATH} ${FN_FACTORY_MODULE_INSTALL_DIR} -x c++ -std=c++17 -I ${NRP_INSTALL_DIR}/include -I ${NRP_DEPS_INSTALL_DIR}/include)

foreach(DIR ${catkin_INCLUDE_DIRS})
    list(APPEND FN_GEN_CMND -I ${DIR})
endforeach() 

foreach(DIR ${Boost_INCLUDE_DIRS})
    list(APPEND FN_GEN_CMND -I ${DIR})
endforeach() 

foreach(DIR ${Python3_INCLUDE_DIRS})
    list(APPEND FN_GEN_CMND -I ${DIR})
endforeach() 

message(STATUS "COMMAND: ${FN_GEN_CMND}")

# Generate source file command
add_custom_command(
            OUTPUT fn_factory_module.cpp
            COMMAND ${FN_GEN_CMND}
            DEPENDS ${HEADER_FILE_PATH})

# FN Factory Module library target
add_library(FNFactoryModule SHARED fn_factory_module.cpp ${SOURCE_FILE_PATH})

target_link_libraries(FNFactoryModule
            PUBLIC
            ${NRP_EVENT_LOOP_TARGET}
            )

# Link to specified NRP compiled Protobuf libraries
foreach(PROTO_PACKAGE ${NRP_PROTO_MSGS_PACKAGES})
    set(PROTO_PACKAGE_LIB "lib_${PROTO_PACKAGE}")
    add_library(${PROTO_PACKAGE_LIB} STATIC IMPORTED )
    set_property(TARGET ${PROTO_PACKAGE_LIB} PROPERTY
            IMPORTED_LOCATION "${NRP_INSTALL_DIR}/lib/libProto${PROTO_PACKAGE}.so")

    target_link_libraries(FNFactoryModule
            PUBLIC
            ${PROTO_PACKAGE_LIB}
            )
endforeach()
