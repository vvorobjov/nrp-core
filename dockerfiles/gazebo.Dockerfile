# Download base image
ARG BASE_IMAGE
FROM ${BASE_IMAGE}

# ROS

RUN sudo sh -c 'echo "deb http://packages.ros.org/ros/ubuntu $(lsb_release -sc) main" > /etc/apt/sources.list.d/ros-latest.list'
RUN sudo sh -c 'curl -s https://raw.githubusercontent.com/ros/rosdistro/master/ros.asc | apt-key add -'

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

# Setup gazebo
WORKDIR ${HOME}
RUN echo '. /usr/share/gazebo-11/setup.sh' >> ${HOME}/.bashrc

# Define entrypoint
COPY --chown=${NRP_USER}:${NRP_GROUP} src/nrp_scripts/xvfb-run-gazebo-runcmd.bash /usr/xvfb-run-gazebo-runcmd.bash
RUN chmod +x /usr/xvfb-run-gazebo-runcmd.bash
COPY --chown=${NRP_USER}:${NRP_GROUP} src/nrp_scripts/run-gazebo-runcmd.bash /usr/run-gazebo-runcmd.bash
RUN chmod +x /usr/run-gazebo-runcmd.bash

# EOF
