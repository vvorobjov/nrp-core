#!/bin/bash

export NRP_DISPLAY="${NRP_DISPLAY:-100}"
export NRP_XPRA_PORT="${NRP_XPRA_PORT:-9876}"

LOCK_FILE="/tmp/.X${NRP_DISPLAY}-lock"
if [ -f "$LOCK_FILE" ]; then
    echo "$LOCK_FILE exists, trying to remove..."
    rm -f "$LOCK_FILE" && echo "Removed."
fi
set -x

# shellcheck disable=SC2001
CHILD_CMD=$(echo "$*" | sed 's|"|\\"|g')

xpra start --bind-tcp=0.0.0.0:"${NRP_XPRA_PORT}" --html=on --start-child="$CHILD_CMD" --exit-with-children --daemon=no ":${NRP_DISPLAY}"
