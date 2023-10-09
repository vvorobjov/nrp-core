# Download base image
ARG BASE_IMAGE
FROM ${BASE_IMAGE}

# Make bash default shell

SHELL ["/bin/bash", "-c"]

# Setup default Docker arguments

ARG NRP_USER=nrpuser
ARG NRP_GROUP=nrpgroup
ARG UID=1000
ARG GID=1000
ARG HOME_PARENT_FOLDER=/home

# Set environment

ENV HOME ${HOME_PARENT_FOLDER}/${NRP_USER}
ENV HOME_PARENT_FOLDER ${HOME_PARENT_FOLDER}
ENV NRP_INSTALL_DIR ${HOME}/.local/nrp
ENV NRP_DEPS_INSTALL_DIR ${HOME}/.local/nrp_deps

ENV NRP_USER ${NRP_USER}
ENV NRP_GROUP ${NRP_GROUP}

# Disable Prompt During Packages Installation

ENV DEBIAN_FRONTEND "noninteractive"

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
    ${NRP_DEPS_INSTALL_DIR} \
    && chown -R ${NRP_USER}:${NRP_GROUP} $HOME

# Copy .bashrc for the NRP_USER

COPY --chown=${NRP_USER}:${NRP_GROUP} .ci/bashrc $HOME/.bashrc

# Install basic dependencies

COPY --chown=${NRP_USER}:${NRP_GROUP} .ci/dependencies/apt/requirements.basic.txt ${HOME}/.dependencies/apt/requirements.basic.txt
RUN apt-get update && apt-get -y install $(grep -vE "^\s*#" ${HOME}/.dependencies/apt/requirements.basic.txt  | tr "\n" " ")

# Switch to NRP user

USER ${NRP_USER}
ENV USER ${NRP_USER}
ENV NRP_USER ${NRP_USER}
ENV NRP_USER ${NRP_USER}
ENV NRP_UID ${UID}
WORKDIR ${HOME}

# EOF
