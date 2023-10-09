# Download base image
ARG BASE_IMAGE
FROM ${BASE_IMAGE}

RUN sudo apt-get update && \
    sudo apt-get install -y wget gnupg2 apt-transport-https && \
    wget -O - https://xpra.org/gpg.asc | sudo apt-key add - && \
    sudo bash -c 'echo "deb https://xpra.org/ focal main" > /etc/apt/sources.list.d/xpra.list'

RUN sudo apt-get update && \
    sudo DEBIAN_FRONTEND=noninteractive apt-get install -y xpra xvfb

RUN sudo apt-key list


RUN sudo mkdir -p /run/user/1000/xpra && sudo chown ${NRP_USER} /run/user/1000/xpra && \
    sudo mkdir -p /run/xpra && sudo chown ${NRP_USER} /run/xpra
RUN sudo bash -c 'echo "LANG=en_US.UTF-8" >> /etc/locale.conf'

COPY --chown=${NRP_USER}:${NRP_GROUP} xpraconf/xpra.conf ${HOME}/xpra/xpra.conf
COPY --chown=${NRP_USER}:${NRP_GROUP} .ci/xpra-entrypoint.sh /usr/xpra-entrypoint.sh
RUN chmod +x /usr/xpra-entrypoint.sh

ENV NRP_XPRA_PORT 9876
ENV NRP_DISPLAY "100"

ENTRYPOINT ["/usr/xpra-entrypoint.sh"]
CMD ["/bin/bash"]

# EOF
