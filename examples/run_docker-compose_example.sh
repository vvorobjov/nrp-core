#!/bin/bash
# Unified runner for the husky_braitenberg docker-compose example.
# Two display modes:
#
#   default      xvfb (headless)
#   --xpra       xpra (browser-accessible remote display on
#                ${NRP_XPRA_HOST_PORT:-9876})
#
# Only the jammy image set (Humble / Python 3.10) is supported;
# EBR2-81 dropped the parallel focal chain.
#
# NRP_DOCKER_REGISTRY and NRP_CORE_TAG are read from <repo>/.env (the
# same file build_nrp_core_image.sh sources). If .env is missing or
# does not set them, falls back to nrp-local / local — the locally
# built image set.

set -euo pipefail

EXAMPLES_PATH=$(cd "$(dirname "$0")" && pwd)
REPO_ROOT=$(cd "$EXAMPLES_PATH/.." && pwd)

XPRA=0

usage() {
    cat <<EOF
Usage: $(basename "$0") [--xpra] [-h|--help]

Runs the husky_braitenberg docker-compose example.

  --xpra       Use the xpra-enabled gazebo image; expose the xpra
               session on \$NRP_XPRA_HOST_PORT (default 9876).

Environment (read from $REPO_ROOT/.env, defaults shown):
  NRP_DOCKER_REGISTRY  nrp-local        (locally built image set)
  NRP_CORE_TAG         local
  NRP_XPRA_PORT        9876             (in-container xpra port)
  NRP_XPRA_HOST_PORT   9876             (host port published to xpra)
  NRP_DISPLAY          100              (xpra :NUM display)
EOF
}

while [[ $# -gt 0 ]]; do
    case "$1" in
        --xpra)     XPRA=1 ;;
        -h|--help)  usage; exit 0 ;;
        *)          echo "Unknown argument: $1" >&2; usage >&2; exit 1 ;;
    esac
    shift
done

if [[ $XPRA -eq 1 ]]; then
    COMPOSE_FILE="docker-compose-xpra-nest-gazebo.yaml"
else
    COMPOSE_FILE="docker-compose-nest-gazebo.yaml"
fi

if [[ -f "$REPO_ROOT/.env" ]]; then
    # shellcheck disable=SC1091
    source "$REPO_ROOT/.env"
fi
export NRP_DOCKER_REGISTRY="${NRP_DOCKER_REGISTRY:-nrp-local}"
export NRP_CORE_TAG="${NRP_CORE_TAG:-local}"
export NRPCORE_EXPERIMENT_DIR="$EXAMPLES_PATH/husky_braitenberg/"

echo "[run_docker-compose_example] compose=$COMPOSE_FILE  registry=$NRP_DOCKER_REGISTRY  tag=$NRP_CORE_TAG"

exec docker compose -f "$EXAMPLES_PATH/$COMPOSE_FILE" \
    up --abort-on-container-exit --remove-orphans
