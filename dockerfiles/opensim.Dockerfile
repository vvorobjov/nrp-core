# This dockerfile is a modified version of the instructions that can be found at
# https://github.com/opensim-org/opensim-core#for-the-impatient-ubuntu

# Download base image

ARG BASE_IMAGE
FROM ${BASE_IMAGE} AS opensim-env

# Install dependecies

COPY --chown=${NRP_USER}:${NRP_GROUP} .ci/dependencies/apt/requirements.opensim.txt ${HOME}/.dependencies/apt/requirements.opensim.txt
RUN sudo apt-get update && sudo apt-get -y install $(grep -vE "^\s*#" ${HOME}/.dependencies/apt/requirements.opensim.txt  | tr "\n" " ")

# We install numpy using pip, because it's easier to uninstall
# in case a specific version is needed by other simulators

RUN pip install numpy==1.21

# Compile and install (globally) the latest version of swig
# The version available through apt (4.0.1) is incompatible with the latest opensim

RUN sudo apt install -y libpcre2-dev bison byacc
WORKDIR ${HOME}
RUN git clone https://github.com/swig/swig \
    && cd swig \
    && git checkout v4.1.1 \
    && ./autogen.sh \
    && ./configure \
    && make -j -l$((`nproc`-2)) \
    && sudo make install \
    && cd .. && rm -rf swig


# Create opensim directories
ENV NRP_OPENSIM_INSTALL_DIR ${HOME}/opensim_install
ENV NRP_OPENSIM_DEP_INSTALL_DIR ${HOME}/opensim_dependencies_install

RUN mkdir ${NRP_OPENSIM_INSTALL_DIR}
RUN mkdir ${NRP_OPENSIM_DEP_INSTALL_DIR}

# Fixes "opensim/modeling/SmoothSphereHalfSpaceForce.java:49: error: unmappable character for encoding ASCII"
ENV JAVA_TOOL_OPTIONS=-Dfile.encoding=UTF8


# Intermediate image to build opensim
FROM opensim-env as opensim-build

ENV NRP_OPENSIM_GIT_DIR ${HOME}/opensim-core

# Clone opensim
# NOTE:
# Both opensim and its dependencies should be built in Release mode (CMAKE_BUILD_TYPE=Release)!
# Building with Debug Symbols makes the size of the resulting image unacceptable

# Build some of the dependencies (simbody, spdlog...) as part of OpenSim 'superbuild'
# OPENSIM_WITH_CASADI=ON and OPENSIM_WITH_TROPTER=ON switches will trigger
# builds of certain necessary dependencies, like casadi, adolc, colpack, etc.

RUN git clone https://github.com/opensim-org/opensim-core.git ${NRP_OPENSIM_GIT_DIR}
WORKDIR ${NRP_OPENSIM_GIT_DIR}
RUN git checkout 4.4
RUN mkdir opensim_dependencies_build 
RUN cd opensim_dependencies_build \
    && cmake ${HOME}/opensim-core/dependencies/ \
      -DCMAKE_INSTALL_PREFIX=${NRP_OPENSIM_DEP_INSTALL_DIR} \
      -DCMAKE_BUILD_TYPE=Release \
      -DOPENSIM_WITH_CASADI=ON \
      -DOPENSIM_WITH_TROPTER=ON \
    && make -j -l$((`nproc`-2))

# We are still in NRP_OPENSIM_GIT_DIR dir
RUN mkdir opensim_build
RUN cd opensim_build \
    && cmake ${HOME}/opensim-core \
      -DCMAKE_INSTALL_PREFIX="${NRP_OPENSIM_INSTALL_DIR}" \
      -DCMAKE_BUILD_TYPE=Release \
      -DOPENSIM_DEPENDENCIES_DIR="${NRP_OPENSIM_DEP_INSTALL_DIR}" \
      -DBUILD_PYTHON_WRAPPING=ON \
      -DBUILD_JAVA_WRAPPING=ON \
      -DWITH_BTK=ON \
    && make -j -l$((`nproc`-2)) && make -j install

# Start a new image that doesn't have the build artifacts
FROM opensim-env

# Copy only the installed objects
COPY --from=opensim-build ${NRP_OPENSIM_DEP_INSTALL_DIR} ${NRP_OPENSIM_DEP_INSTALL_DIR}
COPY --from=opensim-build ${NRP_OPENSIM_INSTALL_DIR} ${NRP_OPENSIM_INSTALL_DIR}

# Export opensim python wrappers and packages

RUN echo 'export PYTHONPATH=${NRP_OPENSIM_INSTALL_DIR}/lib/python3.8/site-packages/:"${PYTHONPATH}"' >> .bashrc

# Export opensim libraries
# Some of the dependecies (ipopt, adolc) arent installed with 'make install', we have to export them too

RUN echo 'export LD_LIBRARY_PATH=${NRP_OPENSIM_DEP_INSTALL_DIR}/ipopt/lib/:"${LD_LIBRARY_PATH}"' >> .bashrc
RUN echo 'export LD_LIBRARY_PATH=${NRP_OPENSIM_DEP_INSTALL_DIR}/adol-c/lib64/:"${LD_LIBRARY_PATH}"' >> .bashrc
RUN echo 'export LD_LIBRARY_PATH=${NRP_OPENSIM_INSTALL_DIR}/lib/:"${LD_LIBRARY_PATH}"' >> .bashrc

# EOF
