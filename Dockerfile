# Download base image ubuntu 20.04

FROM ubuntu:20.04

ARG NRP_USER=nrpuser
ARG NRP_GROUP=nrpgroup
ARG UID=1000
ARG GID=1000
ARG HOME_PARENT_FOLDER=/home

# Set environment

ENV HOME ${HOME_PARENT_FOLDER}/${NRP_USER}
ENV HOME_PARENT_FOLDER ${HOME_PARENT_FOLDER}
ENV NRP_INSTALL_DIR ${HOME}/.local

# Disable Prompt During Packages Installation

ARG DEBIAN_FRONTEND=noninteractive

# INSTALL sudo

RUN apt-get update -y && apt-get install -y sudo

# Set NRP_USER user

RUN mkdir -p ${HOME_PARENT_FOLDER} \
    && groupadd --gid ${GID} ${NRP_GROUP} \
    && useradd --home-dir ${HOME} --create-home --uid ${UID} --gid ${GID} --groups ${NRP_GROUP} -ms /bin/bash ${NRP_USER} \
    && echo "${NRP_USER} ALL=(ALL) NOPASSWD: ALL" >> /etc/sudoers

# Set NRP_USER directories

RUN mkdir -p \
    ${NRP_INSTALL_DIR} \
    && chown -R ${NRP_USER}:${NRP_GROUP} $HOME

# Copy .bashrc for the NRP_USER

COPY --chown=${NRP_USER}:${NRP_GROUP} .ci/bashrc $HOME/.bashrc

# Copy requirements files

COPY --chown=${NRP_USER}:${NRP_GROUP} .ci/dependencies ${HOME}/.dependencies

# Install basic dependencies

RUN apt-get update && apt-get -y install $(grep -vE "^\s*#" ${HOME}/.dependencies/apt/requirements.basic.txt  | tr "\n" " ")

# Install dependencies for testing

RUN apt-get update && apt-get -y install $(grep -vE "^\s*#" ${HOME}/.dependencies/apt/requirements.tests.txt  | tr "\n" " ")

# Pistache REST Server

RUN add-apt-repository ppa:pistache+team/unstable

# Gazebo

RUN sh -c 'echo "deb http://packages.osrfoundation.org/gazebo/ubuntu-stable `lsb_release -cs` main" > /etc/apt/sources.list.d/gazebo-stable.list'
RUN wget https://packages.osrfoundation.org/gazebo.key -O - | apt-key add -

# ROS

RUN sh -c 'echo "deb http://packages.ros.org/ros/ubuntu $(lsb_release -sc) main" > /etc/apt/sources.list.d/ros-latest.list'
RUN curl -s https://raw.githubusercontent.com/ros/rosdistro/master/ros.asc | apt-key add -

# Install CLE dependencies

RUN apt-get update && apt-get -y install $(grep -vE "^\s*#" ${HOME}/.dependencies/apt/requirements.cle.txt  | tr "\n" " ")

# If this image will be used for TVB integration, then flask==1.1.4 is needed and after markupsafe (included in flask) has to be downgraded to 2.0.1
RUN pip install grpcio-tools pytest flask gunicorn

# Install Documentation dependencies

RUN apt-get update && apt-get -y install $(grep -vE "^\s*#" ${HOME}/.dependencies/apt/requirements.docs.txt  | tr "\n" " ")

# Switch to NRP user

USER ${NRP_USER}
ENV USER ${NRP_USER}
WORKDIR ${HOME}

# MQTT
RUN git clone https://github.com/eclipse/paho.mqtt.c.git \
    && cd paho.mqtt.c \
    && git checkout v1.3.8 \
    && cmake -Bbuild -H. -DPAHO_ENABLE_TESTING=OFF -DPAHO_BUILD_STATIC=OFF -DPAHO_BUILD_SHARED=ON -DPAHO_WITH_SSL=ON -DPAHO_HIGH_PERFORMANCE=ON -DCMAKE_INSTALL_PREFIX="${NRP_INSTALL_DIR}"\
    && cmake --build build/ --target install \
    && sudo ldconfig && cd .. && rm -rf paho.mqtt.c

RUN git clone https://github.com/eclipse/paho.mqtt.cpp \
    && cd paho.mqtt.cpp \
    && git checkout v1.2.0 \
    && cmake -Bbuild -H. -DPAHO_BUILD_STATIC=OFF -DPAHO_BUILD_SHARED=ON -DCMAKE_INSTALL_PREFIX="${NRP_INSTALL_DIR}" -DCMAKE_PREFIX_PATH="${NRP_INSTALL_DIR}"\
    && cmake --build build/ --target install \
    && sudo ldconfig && cd .. && rm -rf paho.mqtt.cpp

# Install Gazebo Models. TODO/WARNING: extra building time and container size!!!

RUN mkdir ${HOME}/nrp \
    && cd ${HOME}/nrp \
    && git clone https://@bitbucket.org/hbpneurorobotics/models.git \
    && git clone https://@bitbucket.org/hbpneurorobotics/gzweb.git \
    && export HBP=/home/${USER}/.local/nrp \
    && mkdir -p ${HBP}/gzweb/http/client/assets \
    && mkdir -p ${HOME}/.gazebo/models \
    && cd models \
    && ./create-symlinks.sh

# EOF
