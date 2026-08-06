#!/bin/bash
# Unified runner for the docker-compose example experiments.
#
# Two orthogonal axes:
#
#   experiment:  default = husky_braitenberg
#                --foraging = foraging_husky (EBR2-32, drive-state SNN)
#   display:     default = xvfb (headless)
#                --xpra = xpra (browser-accessible remote display on
#                ${NRP_XPRA_HOST_PORT:-9876})
#
# Combinations produce four possible compose files under examples/.
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
FORAGING=0

usage() {
    cat <<EOF
Usage: $(basename "$0") [--xpra] [--foraging] [-h|--help]

Runs a docker-compose example experiment. Two orthogonal axes:

  experiment:
    (default)    husky_braitenberg (one-shot Braitenberg-2b reflex).
    --foraging   foraging_husky (drive-state SNN with goal switching
                 and obstacle reflex — EBR2-32).

  display:
    (default)    xvfb (headless).
    --xpra       xpra HTML5 on \$NRP_XPRA_HOST_PORT (default 9876).

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
        --xpra)      XPRA=1 ;;
        --foraging)  FORAGING=1 ;;
        -h|--help)   usage; exit 0 ;;
        *)           echo "Unknown argument: $1" >&2; usage >&2; exit 1 ;;
    esac
    shift
done

# Compose-file dispatch: experiment × display.
if [[ $FORAGING -eq 1 ]]; then
    EXP_DIR="$EXAMPLES_PATH/foraging_husky/"
    if [[ $XPRA -eq 1 ]]; then
        COMPOSE_FILE="docker-compose-foraging-xpra-nest-gazebo.yaml"
    else
        COMPOSE_FILE="docker-compose-foraging-nest-gazebo.yaml"
    fi
else
    EXP_DIR="$EXAMPLES_PATH/husky_braitenberg/"
    if [[ $XPRA -eq 1 ]]; then
        COMPOSE_FILE="docker-compose-xpra-nest-gazebo.yaml"
    else
        COMPOSE_FILE="docker-compose-nest-gazebo.yaml"
    fi
fi

if [[ -f "$REPO_ROOT/.env" ]]; then
    # shellcheck disable=SC1091
    source "$REPO_ROOT/.env"
fi
export NRP_DOCKER_REGISTRY="${NRP_DOCKER_REGISTRY:-nrp-local}"
export NRP_CORE_TAG="${NRP_CORE_TAG:-local}"
export NRPCORE_EXPERIMENT_DIR="$EXP_DIR"

echo "[run_docker-compose_example] compose=$COMPOSE_FILE  exp_dir=$EXP_DIR  registry=$NRP_DOCKER_REGISTRY  tag=$NRP_CORE_TAG"

# Preflight: make sure the runtime images this compose file references
# exist. A fresh clone has none, and the documented build
# (build_nrp_core_image.sh nrp-nest-gazebo) produces the *-env chain plus
# nrp-nest-gazebo — NOT the images these compose files pull, which are
# nrp-vanilla (nrp-core-service) and nrp-gazebo / nrp-xpra-gazebo
# (gazebo-service). When the local registry is in use, build any that are
# missing so `docker compose up` doesn't fail; for a remote registry,
# assume they can be pulled and let compose fetch them.
ensure_image() {
    local service="$1"
    local image="${NRP_DOCKER_REGISTRY}/${service}:${NRP_CORE_TAG}"
    if docker image inspect "$image" >/dev/null 2>&1; then
        return 0
    fi
    if [[ "$NRP_DOCKER_REGISTRY" == "nrp-local" ]]; then
        echo "[run_docker-compose_example] $image missing — building via build_nrp_core_image.sh $service"
        ( cd "$REPO_ROOT" && bash build_nrp_core_image.sh "$service" )
    else
        echo "[run_docker-compose_example] $image not present locally — docker compose will pull it from $NRP_DOCKER_REGISTRY"
    fi
}

if [[ $XPRA -eq 1 ]]; then
    ensure_image "nrp-xpra-gazebo"
else
    ensure_image "nrp-gazebo"
fi
ensure_image "nrp-vanilla"

# For the foraging experiment we capture the compose log to a temp
# file and run a post-run functional check that asserts the SNN
# actually drove the husky (camera saw colour, brain produced motor
# output, wheels received non-zero commands). The husky_braitenberg
# experiment doesn't yet ship the same diagnostics, so the check is
# foraging-only — for husky we keep the simple "exec docker compose"
# path.
if [[ $FORAGING -eq 1 ]]; then
    LOG_FILE="$(mktemp -t nrp-foraging-XXXXXX.log)"
    echo "[run_docker-compose_example] capturing log to $LOG_FILE for post-run functional check"
    set +e
    docker compose -f "$EXAMPLES_PATH/$COMPOSE_FILE" \
        up --abort-on-container-exit --remove-orphans 2>&1 | tee "$LOG_FILE"
    compose_rc=${PIPESTATUS[0]}
    set -e
    echo
    echo "[run_docker-compose_example] --- functional check ---"
    # set+e while the check runs so a non-zero exit doesn't abort the
    # wrapper before check_rc is captured / messages are printed.
    set +e
    bash "$EXP_DIR/check_functionality.sh" "$LOG_FILE"
    check_rc=$?
    set -e
    if [[ "$compose_rc" -ne 0 ]]; then
        echo "[run_docker-compose_example] compose exited $compose_rc (non-zero)" >&2
        exit "$compose_rc"
    fi
    exit "$check_rc"
fi

exec docker compose -f "$EXAMPLES_PATH/$COMPOSE_FILE" \
    up --abort-on-container-exit --remove-orphans
