#!/bin/bash

. "${HOME}/.bashrc"

export GAZEBO_ADDITIONAL_PARAMETERS="${GAZEBO_ADDITIONAL_PARAMETERS:-}"
export GAZEBO_PORT="${GAZEBO_PORT:-9000}"
export GAZEBO_NRP_PLUGINS="${GAZEBO_NRP_PLUGINS:-NRPGazeboGrpcCommunicationPlugin.so}"
export GAZEBO_WORLD_FILE="${GAZEBO_WORLD_FILE:-/experiment/empty.world}"
export GAZEBO_RNG_SEED="${GAZEBO_RNG_SEED:-0}"
export NRP_INSTALL_DIR="${NRP_INSTALL_DIR:-/home/nrpuser/.local/nrp}"

# If the display is going to be available, then run full gazebo
if [ -z "${NRP_DISPLAY}" ]; then
    GAZEBO_LAUNCH_COMMAND=gzserver
else
    GAZEBO_LAUNCH_COMMAND=gazebo
fi

# GAZEBO_ADDITIONAL_PARAMETERS without quotes 
# so that empty value or space-separated values were not counted as gzserver parameters
set -x
${GAZEBO_LAUNCH_COMMAND} --protobuf_plugins_path=${NRP_INSTALL_DIR}/lib --protobuf_plugins="[\"Gazebo\"]"  ${GAZEBO_ADDITIONAL_PARAMETERS} --engine=gazebo --serverurl=0.0.0.0:"${GAZEBO_PORT}" -s "${GAZEBO_NRP_PLUGINS}" --seed "${GAZEBO_RNG_SEED}" "${GAZEBO_WORLD_FILE}"
