# Download base image
ARG BASE_IMAGE
FROM ${BASE_IMAGE}

# Python version must match the base image's system python — 3.10 on
# jammy. Passed through from docker-compose.yaml.
ARG PYTHON_VERSION=3.10

# Install nest dependencies

COPY --chown=${NRP_USER}:${NRP_GROUP} .ci/dependencies/apt/requirements.nest.txt ${HOME}/.dependencies/apt/requirements.nest.txt
RUN sudo apt-get update && sudo apt-get -y install $(grep -vE "^\s*#" ${HOME}/.dependencies/apt/requirements.nest.txt  | tr "\n" " ")

RUN pip install flask flask_cors RestrictedPython

# NEST 3.10 requires Cython >= 3.0.0 to build PyNEST; jammy apt cython3 is
# 0.29.28 (too old), so install it from pip instead (EBR2-115).
RUN pip install 'cython>=3.0.0'


# Install nest-simulator (to NRP_DEPS_INSTALL_DIR).
# NEST is pinned to 3.10 (EBR2-115, mechanical bump from 3.9/EBR2-82).
# Keep this tag in lockstep with the compose-side nest-simulator image
# tag and with src/nrp_nest_engines/CMakeLists.txt — both land PyNEST
# bindings and the C++ runtime in the same ${NRP_DEPS_INSTALL_DIR}.
# NEST 3.10 needs Cython >= 3.0.0, installed via pip below (jammy apt
# cython3 is 0.29.28, too old); C++20 is already satisfied by jammy GCC 11.
RUN git clone https://github.com/nest/nest-simulator.git \
    && cd nest-simulator \
    && git checkout v3.10 \
    && mkdir build && cd build \
    && cmake -DCMAKE_INSTALL_PREFIX:PATH=${NRP_DEPS_INSTALL_DIR} -Dwith-mpi=ON -Dwith-python=ON .. \
    && make -j4 && make install \
    && cd .. && rm -rf nest-simulator
ENV NEST_INSTALL_DIR ${NRP_DEPS_INSTALL_DIR}
ENV PYTHONPATH=$NRP_DEPS_INSTALL_DIR/lib/python${PYTHON_VERSION}/site-packages:$PYTHONPATH

WORKDIR ${HOME}

# EOF
