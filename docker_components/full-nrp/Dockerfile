FROM nrp_env:v0

#ARG NRP_USER=nrpuser
USER ${NRP_USER}
ENV USER ${NRP_USER}
WORKDIR ${HOME}

# For NRP-CORE
WORKDIR ${HOME}
ADD . ${HOME}/nrp-core

WORKDIR ${HOME}/nrp-core
RUN /bin/bash -c "source .ci/10-prepare-build.sh"
RUN /bin/bash -c "source .ci/20-build.sh"

WORKDIR ${HOME}
