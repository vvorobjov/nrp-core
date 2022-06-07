# Download base image

ARG BASE_IMAGE
FROM ${BASE_IMAGE}

ENV NVIDIA_DRIVER_CAPABILITIES compute,graphics,utility

# Install Vulkan SDK

RUN wget -qO - http://packages.lunarg.com/lunarg-signing-key-pub.asc | sudo apt-key add -
RUN sudo wget -qO /etc/apt/sources.list.d/lunarg-vulkan-focal.list http://packages.lunarg.com/vulkan/lunarg-vulkan-focal.list
RUN sudo apt update && sudo apt install -y vulkan-sdk

COPY nvidia_icd.json /etc/vulkan/icd.d/nvidia_icd.json

WORKDIR ${HOME}

# EOF
