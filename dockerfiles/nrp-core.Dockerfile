# Download base image
ARG BASE_IMAGE
FROM ${BASE_IMAGE} AS nrp-core-env

## Environment image

# Install dependencies for testing

COPY --chown=${NRP_USER}:${NRP_GROUP} .ci/dependencies/apt/requirements.tests.txt ${HOME}/.dependencies/apt/requirements.tests.txt
RUN sudo apt-get update && sudo apt-get -y install $(grep -vE "^\s*#" ${HOME}/.dependencies/apt/requirements.tests.txt  | tr "\n" " ")

# Pistache REST Server

RUN sudo add-apt-repository ppa:pistache+team/unstable

# Install CLE dependencies

COPY --chown=${NRP_USER}:${NRP_GROUP} .ci/dependencies/apt/requirements.cle.txt ${HOME}/.dependencies/apt/requirements.cle.txt
RUN sudo apt-get update && sudo apt-get -y install $(grep -vE "^\s*#" ${HOME}/.dependencies/apt/requirements.cle.txt  | tr "\n" " ")

# If this image will be used for TVB integration, then flask==1.1.4 is needed and after markupsafe (included in flask) has to be downgraded to 2.0.1
# Pin pytest to the 7.x series:
#   - pytest < 7: rejected by setuptools' bundled typeguard pytest
#     plugin ("AssertionError" on type="string" addini).
#   - pytest >= 8: ROS 2 Foxy's launch_testing pytest hooks call
#     import_path(path, root=None) which became illegal in pytest 8.1
#     ("import_path() missing 1 required keyword-only argument:
#     'consider_namespace_packages'").
# pytest 7.x satisfies both. /home/nrpuser/.local/lib/python3.8/site-
# packages is already ahead of /usr/lib/python3/dist-packages on
# sys.path, so the pip-user pytest wins over the apt-provided 4.6.9.
RUN pip install "grpcio-tools>=1.62.2" "pytest>=7,<8" psutil flask gunicorn flask_cors mpi4py docopt docker "urllib3>=2.0" "paho-mqtt==1.6.1"

# Experiments — opencv-python is needed by examples/husky_braitenberg's
# cam_pf.py (and any other camera-data transceiver function). Without
# it the docker-compose example fails at "import cv2" before the
# simulation even starts. Pin numpy<2 so we don't drag in numpy 2.x
# (latest opencv-python wheel auto-upgrades to numpy 2.2 which is
# ABI-incompatible with the apt-installed python3-scipy that EDLUT's
# swig wrapper imports — "numpy.dtype size changed").
RUN pip install "numpy<2" opencv-python

# Install python_on_whales for the example of invoking nrp-core remotely with compose (See guides/remote_docker_compose.dox)
# Needed for tests
RUN pip install python-on-whales pyyaml
# Upgrade packaging alongside setuptools: modern setuptools (>= ~74)
# calls packaging.canonicalize_version(strip_trailing_zero=...), which
# was added in packaging 23.0. Jammy's apt-provided python3-packaging
# is 21.3 and shadows the pip-user one on some code paths, causing
# ament_cmake_python to fail colcon build with
#   TypeError: canonicalize_version() got an unexpected keyword
#   argument 'strip_trailing_zero'
RUN pip install -U setuptools pip packaging

# Install Documentation dependencies

COPY --chown=${NRP_USER}:${NRP_GROUP} .ci/dependencies/apt/requirements.docs.txt ${HOME}/.dependencies/apt/requirements.docs.txt
RUN sudo apt-get update && sudo apt-get -y install $(grep -vE "^\s*#" ${HOME}/.dependencies/apt/requirements.docs.txt  | tr "\n" " ")

# Add dependencies installation directory to PATH
ENV PATH=$PATH:$NRP_DEPS_INSTALL_DIR/bin:$HOME/.local/bin
ENV LD_LIBRARY_PATH=$NRP_DEPS_INSTALL_DIR/lib:$LD_LIBRARY_PATH

# Install MQTT (to NRP_DEPS_INSTALL_DIR)
RUN git clone https://github.com/eclipse/paho.mqtt.cpp \
    && cd paho.mqtt.cpp \
    && git checkout v1.4.0 \
    && git submodule init \
    && git submodule update \
    && cmake -Bbuild -H. -DPAHO_BUILD_STATIC=OFF -DPAHO_BUILD_SHARED=ON -DCMAKE_INSTALL_PREFIX="${NRP_DEPS_INSTALL_DIR}" -DCMAKE_PREFIX_PATH="${NRP_DEPS_INSTALL_DIR}" -DPAHO_WITH_MQTT_C=ON \
    && cmake --build build/ --target install \
    && sudo ldconfig && cd .. && rm -rf paho.mqtt.cpp

# Add nrp-core env vars to .bashrc
WORKDIR ${HOME}
RUN echo 'source $NRP_INSTALL_DIR/bin/.nrp_env' >> ${HOME}/.bashrc

## Build image
# Configure and install NRP in a intermediate sub-image
FROM nrp-core-env AS nrp-core-builder

# CMake configuration
ARG CMAKE_CACHE_FILE
ENV CMAKE_CACHE_FILE ${CMAKE_CACHE_FILE}

RUN mkdir -p ${HOME}/nrp-core-src
# TODO: copy source files more elegant, but without breaking the cache
# Copying the root (.) immediatly breaks the cache
COPY --chown=${NRP_USER}:${NRP_GROUP} src ${HOME}/nrp-core-src/src
COPY --chown=${NRP_USER}:${NRP_GROUP} docs ${HOME}/nrp-core-src/docs
COPY --chown=${NRP_USER}:${NRP_GROUP} examples ${HOME}/nrp-core-src/examples
COPY --chown=${NRP_USER}:${NRP_GROUP} cmake ${HOME}/nrp-core-src/cmake
COPY --chown=${NRP_USER}:${NRP_GROUP} CMakeLists.txt ${HOME}/nrp-core-src/CMakeLists.txt
COPY --chown=${NRP_USER}:${NRP_GROUP} .ci/cmake_cache ${HOME}/nrp-core-src/.ci/cmake_cache
COPY --chown=${NRP_USER}:${NRP_GROUP} .ci/11-prepare-build.sh ${HOME}/nrp-core-src/.ci/11-prepare-build.sh
COPY --chown=${NRP_USER}:${NRP_GROUP} .ci/20-build.sh ${HOME}/nrp-core-src/.ci/20-build.sh

RUN cd ${HOME}/nrp-core-src && ls -al && bash .ci/11-prepare-build.sh && bash .ci/20-build.sh


## NRP-core image
# Copy the installed nrp to the main image (the intermediate container with code will be unseen for production)
FROM nrp-core-env

# Python version must match the base image's system python — 3.10 on
# jammy (the only supported target after EBR2-81 dropped focal).
ARG PYTHON_VERSION=3.10

ARG NRP_TEMPLATES_DIR=/nrp-templates
ENV NRP_TEMPLATES_DIR ${NRP_TEMPLATES_DIR}

ENV PATH=$PATH:$NRP_INSTALL_DIR/bin:$HOME/.local/bin
ENV LD_LIBRARY_PATH=$NRP_INSTALL_DIR/lib:$LD_LIBRARY_PATH
ENV PYTHONPATH=$NRP_INSTALL_DIR/lib/python${PYTHON_VERSION}/site-packages:$PYTHONPATH

COPY --from=nrp-core-builder ${NRP_INSTALL_DIR} ${NRP_INSTALL_DIR}
COPY --from=nrp-core-builder ${NRP_DEPS_INSTALL_DIR} ${NRP_DEPS_INSTALL_DIR}
COPY --chown=${NRP_USER}:${NRP_GROUP} templates ${NRP_TEMPLATES_DIR}

# Define entrypoint
WORKDIR ${HOME}

COPY --chown=${NRP_USER}:${NRP_GROUP} src/nrp_scripts/nrp-core-entrypoint.bash /usr/nrp-core-entrypoint.bash
RUN chmod +x /usr/nrp-core-entrypoint.bash
ENTRYPOINT ["/usr/nrp-core-entrypoint.bash"]
CMD ["/bin/bash"]

# EOF
