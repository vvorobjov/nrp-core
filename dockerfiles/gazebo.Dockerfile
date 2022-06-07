# Download base image
ARG BASE_IMAGE
FROM ${BASE_IMAGE}


# Add gazebo repository

RUN sudo sh -c 'echo "deb http://packages.osrfoundation.org/gazebo/ubuntu-stable `lsb_release -cs` main" > /etc/apt/sources.list.d/gazebo-stable.list'
RUN sudo sh -c 'wget https://packages.osrfoundation.org/gazebo.key -O - | apt-key add -'

# Install gazebo libraries and dependencies

COPY --chown=${NRP_USER}:${NRP_GROUP} .ci/dependencies/apt/requirements.gazebo.txt ${HOME}/.dependencies/apt/requirements.gazebo.txt
RUN sudo apt-get update && sudo apt-get -y install $(grep -vE "^\s*#" ${HOME}/.dependencies/apt/requirements.gazebo.txt  | tr "\n" " ")

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

ENV GAZEBO_PLUGIN_PATH "${NRP_INSTALL_DIR}/lib/nrp_gazebo_plugins:/usr/lib/x86_64-linux-gnu/gazebo-11/plugins:$GAZEBO_PLUGIN_PATH"
ENV LD_LIBRARY_PATH "${NRP_INSTALL_DIR}/lib:/usr/lib/x86_64-linux-gnu:/usr/lib/x86_64-linux-gnu/gazebo-11/plugins:${NRP_INSTALL_DIR}/lib/nrp_gazebo_plugins:/usr/lib/x86_64-linux-gnu/gazebo-11/plugins:$LD_LIBRARY_PATH"
ENV GAZEBO_MODEL_PATH "/usr/share/gazebo-11/models:${GAZEBO_MODEL_PATH}"

WORKDIR ${HOME}

# EOF
