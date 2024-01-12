# Download base image
ARG BASE_IMAGE
FROM ${BASE_IMAGE}

# Install dependecies

COPY --chown=${NRP_USER}:${NRP_GROUP} .ci/dependencies/apt/requirements.tvb.txt ${HOME}/.dependencies/apt/requirements.tvb.txt
RUN sudo apt-get update && sudo apt-get -y install $(grep -vE "^\s*#" ${HOME}/.dependencies/apt/requirements.tvb.txt  | tr "\n" " ")

# numba (dependency of tvb) requires numpy < 1.22

RUN pip install numpy==1.21 scipy numba==0.55

# Install TVB data

WORKDIR ${HOME}
RUN git clone https://github.com/the-virtual-brain/tvb-data.git && cd tvb-data && git checkout 354684981b9b3db16e0a28cedb825c2595cae295
WORKDIR ${HOME}/tvb-data
RUN sudo python3 setup.py develop

# Install TVB root

WORKDIR ${HOME}
RUN git clone -b '2.6' --single-branch --depth 1 https://github.com/the-virtual-brain/tvb-root.git
WORKDIR ${HOME}/tvb-root/tvb_build
RUN sh install_full_tvb.sh

WORKDIR ${HOME}

# EOF
