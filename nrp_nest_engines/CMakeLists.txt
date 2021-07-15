cmake_minimum_required(VERSION 3.16)
project(NESTEngines VERSION ${NRP_VERSION})

##########################################
## NEST simulator
## Installs nest-simulator, used by nest engine servers as python modules (ie. not required at compilation time)
## Contains nest-server app

set(NRP_NEST_SERVER_EXECUTABLE "nest-server")

if(ENABLE_NEST STREQUAL FULL)
	if("${NEST_BUILD_DIR}" STREQUAL "")
		set(NEST_BUILD_DIR "${CMAKE_CURRENT_BINARY_DIR}/nest-simulator" CACHE INTERNAL "Nest build dir")
	endif()

	set(NRP_NEST_INSTALL_PREFIX ${CMAKE_CURRENT_BINARY_DIR}/nest-simulator/nest-simulator)
	if(NOT TARGET nest-simulator)
		ExternalProject_Add(nest-simulator
				GIT_REPOSITORY https://github.com/nest/nest-simulator.git
				GIT_TAG c354d429e60ddb35a61ad02e3a070477e5026fa8
				GIT_PROGRESS true

				PREFIX ${NEST_BUILD_DIR}
				INSTALL_DIR ${NRP_NEST_INSTALL_PREFIX}

				CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=${NRP_NEST_INSTALL_PREFIX} -Dwith-mpi=ON -Dwith-python=ON

				UPDATE_COMMAND ""
				)
	endif()

	# Workaround for uwsgi bug. Currently, the nest-server only work with uwsgi installed via pip. It fails
	# when using uwsgi installed with apt. This creates and installs a workaround installation.
	# Look at https://github.com/nest/nest-simulator/pull/1873 and https://github.com/nest/nest-simulator/issues/1872
	# for upstream status. Once the PR was merged, update the nest-simulator GIT-TAG here

	## Begin workaround
	add_custom_target(nest-server-uwsgi-bin ALL sh -c
			"sed 's/uwsgi --module nest\.server:app/ uwsgi --plugin python3 --module nest.server:app/' '${NEST_BUILD_DIR}/src/nest-simulator/extras/nest-server' > '${CMAKE_CURRENT_BINARY_DIR}/nest-server-uwsgi'"
			DEPENDS nest-simulator
			BYPRODUCTS "${CMAKE_CURRENT_BINARY_DIR}/nest-server-uwsgi"
			VERBATIM)
	set(NRP_NEST_SERVER_EXECUTABLE "nest-server-uwsgi")
	install(
			FILES "${CMAKE_CURRENT_BINARY_DIR}/nest-server-uwsgi"
			DESTINATION "${CMAKE_INSTALL_BINDIR}"
	)
	install(
		DIRECTORY ${NRP_NEST_INSTALL_PREFIX}/
		DESTINATION ${CMAKE_INSTALL_PREFIX}
	)
	## End workaround
endif()


##########################################
## Nest Engines

add_subdirectory(nrp_nest_json_engine)
add_subdirectory(nrp_nest_server_engine)