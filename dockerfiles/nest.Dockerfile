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


# Install nest-simulator (to NRP_DEPS_INSTALL_DIR).
# NEST is bumped to 3.9 (EBR2-82) — the previous v3.1 pin disagreed with
# the compose-side nest-simulator:3.3 image and with the polimi
# experiment's v3.0 / v3.7 pins; 3.9 is the current stable release.
RUN git clone https://github.com/nest/nest-simulator.git \
    && cd nest-simulator \
    && git checkout v3.9 \
    && mkdir build && cd build \
    && cmake -DCMAKE_INSTALL_PREFIX:PATH=${NRP_DEPS_INSTALL_DIR} -Dwith-mpi=ON -Dwith-python=ON .. \
    && make -j4 && make install \
    && cd .. && rm -rf nest-simulator
ENV NEST_INSTALL_DIR ${NRP_DEPS_INSTALL_DIR}
ENV PYTHONPATH=$NRP_DEPS_INSTALL_DIR/lib/python${PYTHON_VERSION}/site-packages:$PYTHONPATH

WORKDIR ${HOME}

# EOF
