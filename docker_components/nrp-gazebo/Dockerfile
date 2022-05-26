FROM ubuntu:20.04

RUN apt-get update && apt-get install -y
RUN apt-get upgrade -y
# ---------------------------------------------
# For NRP
# Install dependencies
RUN apt-get update && apt-get install -y && \
	apt-get install -y software-properties-common && \
	add-apt-repository ppa:pistache+team/unstable

RUN apt update
RUN apt install -y git wget pkg-config cmake libpistache-dev g++-10 libboost-python-dev libboost-filesystem-dev libboost-numpy-dev libcurl4-openssl-dev nlohmann-json3-dev libzip-dev cython3 libgrpc++-dev protobuf-compiler-grpc libprotobuf-dev doxygen libgsl-dev python3 python3-pil python3-pip python3-numpy

RUN pip3 install gunicorn grpcio-tools Flask --force

# Gazebo
RUN sh -c 'echo "deb http://packages.osrfoundation.org/gazebo/ubuntu-stable `lsb_release -cs` main" > /etc/apt/sources.list.d/gazebo-stable.list' && \
	wget https://packages.osrfoundation.org/gazebo.key -O - | apt-key add - && \
	apt update && \
	apt install -y libgazebo11-dev gazebo11 gazebo11-plugin-base

# For NRP-CORE
WORKDIR /root

ARG FILE_PATH
ARG NRP_INSTALL_PATH=/root/.local/nrp
ARG NRP_PATH=$NRP_INSTALL_PATH
ADD . /root/nrp-core
ADD ${FILE_PATH}/exp_models/models /root/.gazebo/models
ADD ${FILE_PATH}/error_clear_list.txt /root/.gazebo/models
WORKDIR /root/.gazebo/models
RUN /bin/bash -c "source create-symlinks.sh || echo 'Something missing'" && \
	rm $(cat error_clear_list.txt) && \
	rm error_clear_list.txt

WORKDIR /root
COPY ${FILE_PATH}/build.sh /root
RUN mkdir -p /root/nrp-core && \
	mkdir -p $NRP_INSTALL_PATH

RUN /bin/bash -c "source build.sh" && \
	/bin/bash -c "echo 'export PYTHONPATH='$NRP_PATH'/lib/python3.8/site-packages:\$PYTHONPATH' >> /root/.bashrc" && \
	/bin/bash -c "echo 'export LD_LIBRARY_PATH='$NRP_PATH'/lib:\$LD_LIBRARY_PATH' >> /root/.bashrc" && \
	/bin/bash -c "echo 'export PATH='$NRP_PATH'/bin:\$PATH' >> /root/.bashrc" && \
	/bin/bash -c "echo 'export GAZEBO_PLUGIN_PATH='$NRP_PATH'/lib/nrp_gazebo_plugins:/usr/lib/x86_64-linux-gnu/gazebo-11/plugins:\$GAZEBO_PLUGIN_PATH' >> /root/.bashrc"

WORKDIR /root
