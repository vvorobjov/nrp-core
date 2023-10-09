#!/bin/bash
pid=0

# SIGTERM-handler
term_handler() {
  if [ $pid -ne 0 ]; then
    #
    pkill -SIGINT -e -P "$pid"
    wait "$pid"
  fi
  exit 143; # 128 + 15 -- SIGTERM
}

# setup handlers
# on callback, execute the specified handler
trap 'term_handler' SIGTERM


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

# run xpra
xpra start --bind-tcp=0.0.0.0:"${NRP_XPRA_PORT}" --html=on --start-child="$CHILD_CMD" --exit-with-children --resize-display=1920x1080 --pulseaudio=no --terminate-children=yes --daemon=no ":${NRP_DISPLAY}" &
pid="$!"
wait ${pid}
