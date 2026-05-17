# Download base image
ARG BASE_IMAGE
FROM ${BASE_IMAGE}

# ROS 2 distribution. jammy ships Humble; this is the only supported
# Ubuntu target after EBR2-81 dropped focal.
ARG ROS_DISTRO=humble
ENV ROS_DISTRO=${ROS_DISTRO}

# ROS 2 apt repo
RUN sudo sh -c 'echo "deb http://packages.ros.org/ros2/ubuntu $(lsb_release -sc) main" > /etc/apt/sources.list.d/ros2-latest.list'
RUN sudo sh -c 'curl -s https://raw.githubusercontent.com/ros/rosdistro/master/ros.asc | apt-key add -'

# Add gazebo repository
RUN sudo sh -c 'echo "deb http://packages.osrfoundation.org/gazebo/ubuntu-stable `lsb_release -cs` main" > /etc/apt/sources.list.d/gazebo-stable.list'
RUN sudo sh -c 'wget https://packages.osrfoundation.org/gazebo.key -O - | apt-key add -'

# Install Gazebo 11 Classic + the ROS 2 stack we actually use.
# On jammy the Classic packages live in universe under `gazebo` /
# `gazebo-plugin-base` / `libgazebo-dev` (OSRF's jammy repo only ships
# the new Ignition packages, which we don't use).

COPY --chown=${NRP_USER}:${NRP_GROUP} .ci/dependencies/apt/requirements.gazebo.txt ${HOME}/.dependencies/apt/requirements.gazebo.txt
RUN sudo apt-get update && sudo apt-get -y install \
    gazebo gazebo-plugin-base libgazebo-dev \
    $(grep -vE "^\s*#" ${HOME}/.dependencies/apt/requirements.gazebo.txt | tr "\n" " ") \
    ros-${ROS_DISTRO}-ros-base \
    ros-${ROS_DISTRO}-rclcpp \
    ros-${ROS_DISTRO}-std-msgs \
    ros-${ROS_DISTRO}-geometry-msgs \
    ros-${ROS_DISTRO}-sensor-msgs \
    ros-${ROS_DISTRO}-builtin-interfaces \
    ros-${ROS_DISTRO}-ament-cmake \
    ros-${ROS_DISTRO}-ament-package \
    ros-${ROS_DISTRO}-rosidl-default-generators \
    ros-${ROS_DISTRO}-rosidl-default-runtime \
    python3-colcon-common-extensions

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
