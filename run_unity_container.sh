#!/bin/bash
docker run \
    --runtime=nvidia \
    -e DISPLAY \
    -v /tmp/.X11-unix:/tmp/.X11-unix:rw \
    --privileged \
    -it \
    --name nrp_game \
    nrp_core_unity_test