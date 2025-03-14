# Download base image
ARG BASE_IMAGE=docker-registry.ebrains.eu/neurorobotics/nrp/nrp-core/nrp-vanilla-ubuntu20:latest
FROM ${BASE_IMAGE}

RUN pip install pandas pybullet
