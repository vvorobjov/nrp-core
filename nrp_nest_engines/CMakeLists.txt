cmake_minimum_required(VERSION 3.16)
project(NESTEngines VERSION ${NRP_VERSION})

##########################################
## NEST simulator
## Installs nest-simulator, used by nest engine servers as python modules (ie. not required at compilation time)
## Contains nest-server app

set(NRP_NEST_SERVER_EXECUTABLE "nest-server")
set(NRP_NEST_SERVER_MPI_EXECUTABLE "nest-server-mpi")

if(ENABLE_NEST STREQUAL FULL)
    # If NEST_INSTALL_DIR is not specified at running cmake, then install nest-simulator from source
    if(NOT DEFINED NEST_INSTALL_DIR)
        if("${NEST_BUILD_DIR}" STREQUAL "")
            set(NEST_BUILD_DIR "${CMAKE_CURRENT_BINARY_DIR}/nest-simulator" CACHE INTERNAL "Nest build dir")
        endif()

        set(NRP_NEST_CMAKE_INSTALL_PREFIX ${CMAKE_CURRENT_BINARY_DIR}/nest-simulator/nest-simulator)
        set(NEST_INSTALL_DIR ${CMAKE_INSTALL_PREFIX})
        if(NOT TARGET nest-simulator)
            ExternalProject_Add(nest-simulator
                    GIT_REPOSITORY https://github.com/nest/nest-simulator.git
                    GIT_TAG v3.1
                    GIT_PROGRESS true

                    PREFIX ${NEST_BUILD_DIR}
                    INSTALL_DIR ${NRP_NEST_CMAKE_INSTALL_PREFIX}

                    CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=${NRP_NEST_CMAKE_INSTALL_PREFIX} -Dwith-mpi=ON -Dwith-python=ON

                    UPDATE_COMMAND ""
                    )
        endif()

        install(
                DIRECTORY ${NRP_NEST_CMAKE_INSTALL_PREFIX}/
                DESTINATION ${NEST_INSTALL_DIR}
                USE_SOURCE_PERMISSIONS
        )
    else()
        if(NOT EXISTS ${NEST_INSTALL_DIR}/${CMAKE_INSTALL_BINDIR}/${NRP_NEST_SERVER_EXECUTABLE})
            message( FATAL_ERROR "Couldn't locate ${NEST_INSTALL_DIR}/${CMAKE_INSTALL_BINDIR}/${NRP_NEST_SERVER_EXECUTABLE}. Check that NEST_INSTALL_DIR is set correctly, or unset." )
        endif()
        if(NOT EXISTS ${NEST_INSTALL_DIR}/${CMAKE_INSTALL_BINDIR}/${NRP_NEST_SERVER_MPI_EXECUTABLE})
            message( FATAL_ERROR "Couldn't locate ${NEST_INSTALL_DIR}/${CMAKE_INSTALL_BINDIR}/${NRP_NEST_SERVER_MPI_EXECUTABLE}. Check that NEST_INSTALL_DIR is set correctly, or unset." )
        endif()
        message("Using preinstalled nest-simulator: ${NEST_INSTALL_DIR}")
    endif()
endif()


##########################################
## Nest Engines

add_subdirectory(nrp_nest_json_engine)
add_subdirectory(nrp_nest_server_engine)