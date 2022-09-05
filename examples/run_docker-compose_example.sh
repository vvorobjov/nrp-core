#!/bin/bash

EXAMPLES_PATH=$(dirname "$0")                   # relative
EXAMPLES_PATH=$(cd "$EXAMPLES_PATH" && pwd)     # absolutized and normalized
if [[ -z "$EXAMPLES_PATH" ]] ; then
  # error; for some reason, the path is not accessible
  # to the script (e.g. permissions re-evaled after suid)
  exit 1  # fail
fi

export NRPCORE_EXPERIMENT_DIR="$EXAMPLES_PATH/husky_braitenberg/"
# export NRP_DOCKER_REGISTRY
export NRP_CORE_TAG=":latest"

docker-compose -f "$EXAMPLES_PATH/docker-compose-nest-gazebo.yaml" up --abort-on-container-exit --remove-orphans
