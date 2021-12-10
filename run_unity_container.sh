#!/bin/bash
docker run \
    --runtime=nvidia \
    -it \
    -e DISPLAY \
    -v /tmp/.X11-unix:/tmp/.X11-unix:rw \
    -v $PWD:/home/nrpuser/nrp-core:rw \
    --privileged \
    -w /home/nrpuser \
    nrp_unity_changed \
    /bin/bash
    