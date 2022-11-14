#!/bin/bash

. "${HOME}/.bashrc"

export GAZEBO_ADDITIONAL_PARAMETERS="${GAZEBO_ADDITIONAL_PARAMETERS:-}"
export GAZEBO_PORT="${GAZEBO_PORT:-9000}"
export GAZEBO_NRP_PLUGINS="${GAZEBO_NRP_PLUGINS:-NRPGazeboGrpcCommunicationPlugin.so}"
export GAZEBO_WORLD_FILE="${GAZEBO_WORLD_FILE:-/experiment/empty.world}"
export GAZEBO_RNG_SEED="${GAZEBO_RNG_SEED:-0}"

echo GAZEBO_ADDITIONAL_PARAMETERS "$GAZEBO_ADDITIONAL_PARAMETERS"
echo GAZEBO_PORT "$GAZEBO_PORT"
echo GAZEBO_NRP_PLUGINS "$GAZEBO_NRP_PLUGINS"
echo GAZEBO_WORLD_FILE "$GAZEBO_WORLD_FILE"
echo GAZEBO_RNG_SEED "$GAZEBO_RNG_SEED"

# GAZEBO_ADDITIONAL_PARAMETERS without quotes 
# so that empty value or space-separated values were not counted as gzserver parameters
xvfb-run --auto-servernum gzserver ${GAZEBO_ADDITIONAL_PARAMETERS} --engine=gazebo --serverurl=0.0.0.0:"${GAZEBO_PORT}" -s "${GAZEBO_NRP_PLUGINS}" --seed "${GAZEBO_RNG_SEED}" "${GAZEBO_WORLD_FILE}"
