cmake_minimum_required(VERSION 3.16)
project(GazeboEngines VERSION ${NRP_VERSION})

if(BUILD_GAZEBO_ENGINE_SERVER)
    set(NRP_GAZEBO_PLUGINS_INSTALL_DIR "${CMAKE_INSTALL_LIBDIR}/nrp_gazebo_plugins" CACHE PATH "Install location of gazebo plugins")

    set(NRP_GAZEBO_PLUGINS_INSTALL_DIR_ABSOLUTE ${NRP_GAZEBO_PLUGINS_INSTALL_DIR})
    if(NOT IS_ABSOLUTE ${NRP_GAZEBO_PLUGINS_INSTALL_DIR})
        set(NRP_GAZEBO_PLUGINS_INSTALL_DIR_ABSOLUTE ${CMAKE_INSTALL_PREFIX}/${NRP_GAZEBO_PLUGINS_INSTALL_DIR})
    endif()

    list(JOIN GAZEBO_LIBRARY_DIRS ":" DEFAULT_GAZEBO_LIB_DIRS)

    # add to nrp env vars
    list(APPEND NRP_ENV_VARS "GAZEBO_PLUGIN_PATH=${NRP_GAZEBO_PLUGINS_INSTALL_DIR_ABSOLUTE}:\${GAZEBO_PLUGIN_PATH}")
    list(APPEND NRP_ENV_VARS "LD_LIBRARY_PATH=${NRP_GAZEBO_PLUGINS_INSTALL_DIR_ABSOLUTE}:\${LD_LIBRARY_PATH}")
    set(NRP_ENV_VARS ${NRP_ENV_VARS} PARENT_SCOPE)
endif()

##########################################
## Gazebo Engines

add_subdirectory(nrp_gazebo_json_engine)
add_subdirectory(nrp_gazebo_grpc_engine)