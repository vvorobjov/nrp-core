#!/bin/bash
# Unified runner for the husky_braitenberg docker-compose example.
# Replaces the earlier pair (run_docker-compose_example.sh +
# run_docker-compose_example-xpra.sh) and covers all four supported
# combinations:
#
#   default                Ubuntu 20.04 (focal) + xvfb (headless)
#   --ubuntu22             Ubuntu 22.04 (jammy) + xvfb (headless)
#   --xpra                 Ubuntu 20.04 + xpra (remote display)
#   --ubuntu22 --xpra      Ubuntu 22.04 + xpra
#
# NRP_DOCKER_REGISTRY and NRP_CORE_TAG are read from <repo>/.env (the
# same file build_nrp_core_image.sh sources). If .env is missing or
# does not set them, falls back to nrp-local / local — the locally
# built image set. Variables already exported in the environment win
# over the file (the script intentionally only fills defaults via
# ${VAR:-...} after sourcing).

set -euo pipefail

EXAMPLES_PATH=$(cd "$(dirname "$0")" && pwd)
REPO_ROOT=$(cd "$EXAMPLES_PATH/.." && pwd)

UBUNTU=20
XPRA=0

usage() {
    cat <<EOF
Usage: $(basename "$0") [--ubuntu20|--ubuntu22] [--xpra] [-h|--help]

Runs the husky_braitenberg docker-compose example.

  --ubuntu20   Ubuntu 20.04 / Foxy / Python 3.8 image set (default).
  --ubuntu22   Ubuntu 22.04 / Humble / Python 3.10 image set.
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
        --ubuntu20) UBUNTU=20 ;;
        --ubuntu22) UBUNTU=22 ;;
        --xpra)     XPRA=1 ;;
        -h|--help)  usage; exit 0 ;;
        *)          echo "Unknown argument: $1" >&2; usage >&2; exit 1 ;;
    esac
    shift
done

if [[ $XPRA -eq 1 && $UBUNTU -eq 22 ]]; then
    COMPOSE_FILE="docker-compose-xpra-nest-gazebo-ubuntu22.yaml"
elif [[ $XPRA -eq 1 ]]; then
    COMPOSE_FILE="docker-compose-xpra-nest-gazebo.yaml"
elif [[ $UBUNTU -eq 22 ]]; then
    COMPOSE_FILE="docker-compose-nest-gazebo-ubuntu22.yaml"
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
