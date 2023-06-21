set(PROJECT_NAME "NRPCorePyClient")
set(PYTHON_MODULE_NAME "nrp_client")

set(NAMESPACE_NAME "${PROJECT_NAME}")

cmake_minimum_required(VERSION 3.16)
project("${PROJECT_NAME}" VERSION ${NRP_VERSION})

include(GNUInstallDirs)

# List library build files
set(PROJECT_SRC_FILES
    nrp_client/__init__.py
    nrp_client/docker_handle.py
    nrp_client/nrp_server_docker_launchers.py
    nrp_client/nrp_server_launchers.py
)

# List library build files
set(PROJECT_ROOT_FILES
    setup.py
    pyproject.toml
)

find_package(
        Python3
        REQUIRED
        COMPONENTS Interpreter
)

# Module setuptools configuration file
configure_file("setup.cfg.in" "${CMAKE_CURRENT_BINARY_DIR}/${PYTHON_MODULE_NAME}/setup.cfg" @ONLY)

# Copy all files to the module directory tree
file(COPY ${PROJECT_SRC_FILES} DESTINATION "${CMAKE_CURRENT_BINARY_DIR}/${PYTHON_MODULE_NAME}/${PYTHON_MODULE_NAME}/")
file(COPY ${PROJECT_ROOT_FILES} DESTINATION "${CMAKE_CURRENT_BINARY_DIR}/${PYTHON_MODULE_NAME}/")

# Add Python installation targets
# nrp-py-client-dev-nodeps is run with make
add_custom_target(nrp-py-client-dev-nodeps ALL
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/${PYTHON_MODULE_NAME}
    COMMAND pip install . -U -t "${CMAKE_INSTALL_PREFIX}/${PYTHON_INSTALL_DIR_REL}/" --no-deps
    COMMENT "Installing NRP-Core Python client into NRP-Core installation folder"
    DEPENDS ${PROTO_PYTHON_FILES}
    VERBATIM
)

add_custom_target(nrp-py-client
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/${PYTHON_MODULE_NAME}
    COMMAND pip install . -U
    COMMENT "Installing NRP-Core Python client into Python site-packages"
    DEPENDS ${PROTO_PYTHON_FILES}
    VERBATIM
)

add_custom_target(nrp-py-client-docker
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/${PYTHON_MODULE_NAME}
    COMMAND pip install .[docker] -U
    COMMENT "Installing NRP-Core Python client with Docker support into Python site-packages"
    DEPENDS ${PROTO_PYTHON_FILES}
    VERBATIM
)

