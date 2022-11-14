# Download base image
ARG BASE_IMAGE
FROM ${BASE_IMAGE}


# Install nest dependencies

COPY --chown=${NRP_USER}:${NRP_GROUP} .ci/dependencies/apt/requirements.nest.txt ${HOME}/.dependencies/apt/requirements.nest.txt
RUN sudo apt-get update && sudo apt-get -y install $(grep -vE "^\s*#" ${HOME}/.dependencies/apt/requirements.nest.txt  | tr "\n" " ")

RUN pip install flask flask_cors RestrictedPython


# Install nest-simulator (to NRP_DEPS_INSTALL_DIR)
RUN git clone https://github.com/nest/nest-simulator.git \
    && cd nest-simulator \
    && git checkout v3.1 \
    && mkdir build && cd build \
    && cmake -DCMAKE_INSTALL_PREFIX:PATH=${NRP_DEPS_INSTALL_DIR} -Dwith-mpi=ON -Dwith-python=ON .. \
    && make -j4 && make install \
    && cd .. && rm -rf nest-simulator
ENV NEST_INSTALL_DIR ${NRP_DEPS_INSTALL_DIR}
ENV PYTHONPATH=$NRP_DEPS_INSTALL_DIR/lib/python3.8/site-packages:$PYTHONPATH

WORKDIR ${HOME}

# EOF
