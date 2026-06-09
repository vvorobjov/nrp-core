# Download base image

ARG BASE_IMAGE
FROM ${BASE_IMAGE}

ENV NVIDIA_DRIVER_CAPABILITIES compute,graphics,utility

# Install Vulkan SDK

RUN wget -qO - http://packages.lunarg.com/lunarg-signing-key-pub.asc | sudo apt-key add -
# jammy is the only supported target after EBR2-81 dropped focal; pull the
# LunarG Vulkan SDK from the matching jammy (22.04) repo.
RUN sudo wget -qO /etc/apt/sources.list.d/lunarg-vulkan-jammy.list http://packages.lunarg.com/vulkan/lunarg-vulkan-jammy.list
RUN sudo apt update && sudo apt install -y vulkan-sdk

COPY src/nvidia_icd.json /etc/vulkan/icd.d/nvidia_icd.json

WORKDIR ${HOME}

# EOF
