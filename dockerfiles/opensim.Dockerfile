# This dockerfile is a modified version of the instructions that can be found at
# https://github.com/opensim-org/opensim-core#for-the-impatient-ubuntu

# Download base image

ARG BASE_IMAGE
FROM ${BASE_IMAGE}

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
RUN git clone https://github.com/swig/swig
WORKDIR ${HOME}/swig
RUN ./autogen.sh
RUN ./configure
RUN make -j4
RUN sudo make install

# Create opensim directories

RUN mkdir ${HOME}/opensim_dependencies_build
RUN mkdir ${HOME}/opensim_dependencies_install

RUN mkdir ${HOME}/opensim_build
RUN mkdir ${HOME}/opensim_install

# Clone opensim

# NOTE:
# Both opensim and its dependencies should be built in Release mode (CMAKE_BUILD_TYPE=Release)!
# Building with Debug Symbols makes the size of the resulting image unacceptable

WORKDIR ${HOME}
RUN git clone https://github.com/opensim-org/opensim-core.git

# Build some of the dependencies (simbody, spdlog...) as part of OpenSim 'superbuild'
# OPENSIM_WITH_CASADI=ON and OPENSIM_WITH_TROPTER=ON switches will trigger
# builds of certain necessary dependencies, like casadi, adolc, colpack, etc.

WORKDIR ${HOME}/opensim_dependencies_build
RUN cmake ../opensim-core/dependencies/ \
      -DCMAKE_INSTALL_PREFIX=${HOME}/opensim_dependencies_install \
      -DCMAKE_BUILD_TYPE=Release \
      -DOPENSIM_WITH_CASADI=ON \
      -DOPENSIM_WITH_TROPTER=ON
RUN make -j4

# Fixes "opensim/modeling/SmoothSphereHalfSpaceForce.java:49: error: unmappable character for encoding ASCII"

ENV JAVA_TOOL_OPTIONS=-Dfile.encoding=UTF8

# Build opensim

WORKDIR ${HOME}/opensim_build
RUN cmake ../opensim-core \
      -DCMAKE_INSTALL_PREFIX="~/opensim_install" \
      -DCMAKE_BUILD_TYPE=Release \
      -DOPENSIM_DEPENDENCIES_DIR="~/opensim_dependencies_install" \
      -DBUILD_PYTHON_WRAPPING=ON \
      -DBUILD_JAVA_WRAPPING=ON \
      -DWITH_BTK=ON
      
RUN make -j8
RUN make -j8 install

WORKDIR ${HOME}

# Export opensim python wrappers and packages

RUN echo 'export PYTHONPATH=$HOME/opensim_install/lib/python3.8/site-packages/:$PYTHONPATH' >> .bashrc

# Export opensim libraries
# Some of the dependecies (ipopt, adolc) arent installed with 'make install', we have to export them too

RUN echo 'export LD_LIBRARY_PATH=$HOME/opensim_dependencies_install/ipopt/lib/:$LD_LIBRARY_PATH' >> .bashrc
RUN echo 'export LD_LIBRARY_PATH=$HOME/opensim_dependencies_install/adol-c/lib64/:$LD_LIBRARY_PATH' >> .bashrc
RUN echo 'export LD_LIBRARY_PATH=$HOME/opensim_install/lib/:$LD_LIBRARY_PATH' >> .bashrc

# EOF
