FROM ubuntu:20.04

RUN apt-get update && apt-get install -y
RUN apt-get upgrade -y
RUN apt-get install -y python3
# ---------------------------------------------
# For NRP
RUN apt-get update && apt-get install -y
# Install dependencies
RUN apt-get install -y software-properties-common
RUN add-apt-repository ppa:pistache+team/unstable

RUN apt update
RUN apt install -y git pkg-config cmake libpistache-dev g++-10 libboost-python-dev libboost-filesystem-dev libboost-numpy-dev libcurl4-openssl-dev nlohmann-json3-dev libzip-dev cython3 python3-numpy libgrpc++-dev protobuf-compiler-grpc libprotobuf-dev doxygen libgsl-dev python3-pil python3-pip

RUN pip3 install grpcio-tools Flask --force

# For NRP-CORE
WORKDIR /root

ARG FILE_PATH
RUN mkdir -p /root/nrp-core
ADD . /root/nrp-core
COPY ${FILE_PATH}/build.sh /root

ARG NRP_INSTALL_PATH=/root/.local/nrp
RUN mkdir -p $NRP_INSTALL_PATH

RUN /bin/bash -c "source build.sh"
ARG NRP_PATH=$NRP_INSTALL_PATH
RUN /bin/bash -c "echo 'export PYTHONPATH='$NRP_PATH'/lib/python3.8/site-packages:\$PYTHONPATH' >> /root/.bashrc"
RUN /bin/bash -c "echo 'export LD_LIBRARY_PATH='$NRP_PATH'/lib:\$LD_LIBRARY_PATH' >> /root/.bashrc"
RUN /bin/bash -c "echo 'export PATH='$NRP_PATH'/bin:\$PATH' >> /root/.bashrc"

WORKDIR /root
