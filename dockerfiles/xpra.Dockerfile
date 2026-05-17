# Download base image
ARG BASE_IMAGE
FROM ${BASE_IMAGE}

RUN sudo apt-get update && \
    sudo apt-get install -y wget gnupg2 apt-transport-https lsb-release && \
    wget -O - https://xpra.org/gpg.asc | sudo apt-key add - && \
    sudo bash -c "echo \"deb https://xpra.org/ $(lsb_release -cs) main\" > /etc/apt/sources.list.d/xpra.list"

# jammy's xpra 6.x bundles the HTML5 client into the main xpra package
# so xpra-html5 is technically redundant on noble too, but listing it
# explicitly makes the dependency obvious to readers.
RUN sudo apt-get update && \
    sudo DEBIAN_FRONTEND=noninteractive apt-get install -y xpra xpra-html5 xvfb

RUN sudo apt-key list


RUN sudo mkdir -p /run/user/1000/xpra && sudo chown ${NRP_USER} /run/user/1000/xpra && \
    sudo mkdir -p /run/xpra && sudo chown ${NRP_USER} /run/xpra
RUN sudo bash -c 'echo "LANG=en_US.UTF-8" >> /etc/locale.conf'

COPY --chown=${NRP_USER}:${NRP_GROUP} src/xpraconf/xpra.conf ${HOME}/xpra/xpra.conf
COPY --chown=${NRP_USER}:${NRP_GROUP} src/nrp_scripts/xpra-entrypoint.sh /usr/xpra-entrypoint.sh
RUN chmod +x /usr/xpra-entrypoint.sh

ENV NRP_XPRA_PORT 9876
ENV NRP_DISPLAY "100"

ENTRYPOINT ["/usr/xpra-entrypoint.sh"]
CMD ["/bin/bash"]

# EOF
