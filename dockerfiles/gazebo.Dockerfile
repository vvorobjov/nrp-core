# Download base image
ARG BASE_IMAGE
FROM ${BASE_IMAGE}

# ROS 2 distribution is parameterised so the same Dockerfile builds
# ubuntu20/focal/foxy and ubuntu22/jammy/humble. Default matches the
# canonical ubuntu20 image.
ARG ROS_DISTRO=foxy
ENV ROS_DISTRO=${ROS_DISTRO}

# ROS 2 apt repo (works for both focal and jammy — lsb_release picks
# the right codename).
RUN sudo sh -c 'echo "deb http://packages.ros.org/ros2/ubuntu $(lsb_release -sc) main" > /etc/apt/sources.list.d/ros2-latest.list'
RUN sudo sh -c 'curl -s https://raw.githubusercontent.com/ros/rosdistro/master/ros.asc | apt-key add -'

# Add gazebo repository

RUN sudo sh -c 'echo "deb http://packages.osrfoundation.org/gazebo/ubuntu-stable `lsb_release -cs` main" > /etc/apt/sources.list.d/gazebo-stable.list'
RUN sudo sh -c 'wget https://packages.osrfoundation.org/gazebo.key -O - | apt-key add -'

# Install Gazebo 11 Classic + the ROS 2 stack we actually use.
# Gazebo Classic package names differ between focal and jammy:
#   - focal: OSRF ubuntu-stable repo ships gazebo11 / gazebo11-plugin-base /
#     libgazebo11-dev. (Jammy OSRF only has the new Ignition packages,
#     no gazebo11.)
#   - jammy: universe ships the same Gazebo 11 under gazebo /
#     gazebo-plugin-base / libgazebo11-dev.
# Detect the codename at build time and pick the right set. ROS 2
# packages and colcon are installed in one apt transaction so that
# dependency resolution runs once.

COPY --chown=${NRP_USER}:${NRP_GROUP} .ci/dependencies/apt/requirements.gazebo.txt ${HOME}/.dependencies/apt/requirements.gazebo.txt
RUN CODENAME=$(lsb_release -cs) && \
    case "$CODENAME" in \
      focal) GAZEBO_PKGS="gazebo11 gazebo11-plugin-base libgazebo11-dev" ;; \
      jammy) GAZEBO_PKGS="gazebo gazebo-plugin-base libgazebo-dev" ;; \
      *)     echo "Unsupported Ubuntu codename: $CODENAME"; exit 1 ;; \
    esac && \
    sudo apt-get update && sudo apt-get -y install $GAZEBO_PKGS \
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
