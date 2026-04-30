#!/usr/bin/env bash
#
# .ci/00-dev-rebuild-and-test.sh
#
# One-shot helper for the local dev loop in nrp-core:
#   1. Ensure the canonical devcontainer image
#      (nrp-local/nrp-nest-gazebo-ubuntu20:local) exists. Rebuild it if
#      missing or when --rebuild-image is given.
#   2. Run cmake configure + make + make install inside the container,
#      against the mounted source tree.
#   3. Run the ctest suite.
#
# Intended as the single entry point while iterating locally. The
# numbered stages .ci/11-prepare-build.sh, .ci/20-build.sh,
# .ci/30-run-tests.sh stay callable individually for CI use.
#
# Exit behaviour: non-zero on any failure, including any failed test.
# The bit-0x8 mask inside .ci/30-run-tests.sh is a Jenkins xunit
# reporting workaround and is NOT applied here -- locally a failed test
# must make this script fail.
#

set -euo pipefail

# -----------------------------------------------------------------------------
# Argument parsing
# -----------------------------------------------------------------------------

REBUILD_IMAGE=0
NO_IMAGE=0
TEST_FILTER=""
KEEP_BUILD=0
UBUNTU_VERSION="20"

usage() {
    cat <<EOF
Usage: $(basename "$0") [options]

Options:
  --rebuild-image       Force rebuild of the nrp-nest-gazebo image even if present.
  --no-image            Skip the image existence check entirely (assume it exists).
  --test-filter REGEX   Pass -R REGEX to ctest (run only matching tests).
  --keep-build          Do not wipe build/ before configuring.
  --ubuntu22            Target the Ubuntu 22.04 image (nrp-nest-gazebo-ubuntu22)
                        instead of the default Ubuntu 20.04 one.
  -h, --help            Show this message and exit.

Default image: nrp-local/nrp-nest-gazebo-ubuntu20:local (canonical).
Ubuntu 22 image: nrp-local/nrp-nest-gazebo-ubuntu22:local.
Both come from docker-compose.yaml.
EOF
}

while [[ $# -gt 0 ]]; do
    case "$1" in
        --rebuild-image) REBUILD_IMAGE=1 ;;
        --no-image)      NO_IMAGE=1 ;;
        --test-filter)
            [[ $# -ge 2 ]] || { echo "[$0] --test-filter requires an argument" >&2; exit 2; }
            TEST_FILTER="$2"
            shift
            ;;
        --keep-build)    KEEP_BUILD=1 ;;
        --ubuntu22)      UBUNTU_VERSION="22" ;;
        -h|--help)       usage; exit 0 ;;
        *)
            echo "[$0] unknown argument: $1" >&2
            usage >&2
            exit 2
            ;;
    esac
    shift
done

# -----------------------------------------------------------------------------
# Locate the repo and ensure we're at its root
# -----------------------------------------------------------------------------

SCRIPT_DIR="$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" &>/dev/null && pwd)"
REPO_ROOT="$(cd -- "$SCRIPT_DIR/.." &>/dev/null && pwd)"
cd "$REPO_ROOT"

# Ubuntu 20 and Ubuntu 22 images and their docker-compose service
# names don't line up (ubuntu20 keeps the original, unqualified
# "nrp-nest-gazebo" for historical reasons).
if [[ "$UBUNTU_VERSION" == "22" ]]; then
    IMAGE="nrp-local/nrp-nest-gazebo-ubuntu22:local"
    BUILD_SERVICE="nrp-nest-gazebo-ubuntu22"
else
    IMAGE="nrp-local/nrp-nest-gazebo-ubuntu20:local"
    BUILD_SERVICE="nrp-nest-gazebo"
fi

# Pretty logger
log() { printf '\n[%s] %s\n' "$(basename "$0")" "$*"; }

# -----------------------------------------------------------------------------
# Step 1: ensure the devcontainer image
# -----------------------------------------------------------------------------

if [[ "$NO_IMAGE" -eq 0 ]]; then
    need_rebuild=0
    if [[ "$REBUILD_IMAGE" -eq 1 ]]; then
        log "--rebuild-image given; forcing rebuild of $IMAGE."
        need_rebuild=1
    elif ! docker image inspect "$IMAGE" >/dev/null 2>&1; then
        log "image $IMAGE not found; will build it."
        need_rebuild=1
    else
        log "image $IMAGE already present; skipping rebuild. (use --rebuild-image to force.)"
    fi

    if [[ "$need_rebuild" -eq 1 ]]; then
        # build_nrp_core_image.sh reads NRP_DOCKER_REGISTRY and NRP_CORE_TAG
        # from .env if present; otherwise from its own defaults. For the
        # devcontainer tag we want nrp-local/*:local, so write .env if
        # missing.
        if [[ ! -f .env ]]; then
            log ".env missing; writing one that matches .devcontainer/devcontainer.json."
            cat > .env <<'ENVEOF'
NRP_CORE_TAG=local
NRP_DOCKER_REGISTRY=nrp-local
ENVEOF
        fi
        NRP_DOCKER_REGISTRY=nrp-local NRP_CORE_TAG=local \
            ./build_nrp_core_image.sh "$BUILD_SERVICE"
    fi
fi

# -----------------------------------------------------------------------------
# Step 2 + 3: configure, build, install, test -- inside the container
# -----------------------------------------------------------------------------

# The container WORKDIR is set to /workspace, matching what Jenkins does.
# --net=host lets tests that bind to localhost ports reuse the host stack.
# --privileged is needed for some gazebo / xvfb plumbing (keeps parity
# with the Jenkinsfile 'agent' args).

log "running configure + build + install + ctest inside $IMAGE"

docker run --rm --net=host --privileged \
    -v "$REPO_ROOT:/workspace" -w /workspace \
    -e "NRP_TEST_FILTER=$TEST_FILTER" \
    -e "NRP_KEEP_BUILD=$KEEP_BUILD" \
    "$IMAGE" \
    bash -lc '
        set -euo pipefail

        if [[ "${NRP_KEEP_BUILD:-0}" -eq 1 ]] && [[ -f build/CMakeCache.txt ]]; then
            echo "[inside] --keep-build: reusing existing build/"
        else
            rm -rf build
        fi

        # Configure + compile + install. 20-build.sh already runs "make install",
        # which is critical: otherwise ctest would load the libs baked into
        # $NRP_INSTALL_DIR when the image was built, not the ones just
        # compiled (see EBR2-17 for the incident that motivated this).
        bash .ci/11-prepare-build.sh
        bash .ci/20-build.sh

        # Run ctest directly (not via .ci/30-run-tests.sh) so the 0x8
        # Jenkins-xunit mask does not swallow real failures locally.
        cd build
        declare -a CTEST_ARGS=(--no-compress-output --test-output-size-failed 300000 --output-on-failure)
        if [[ -n "${NRP_TEST_FILTER:-}" ]]; then
            CTEST_ARGS+=(-R "$NRP_TEST_FILTER")
        fi

        if [[ -z "${DISPLAY:-}" ]]; then
            xvfb-run -s "-screen 0 1280x1024x24 -ac -nolisten tcp -nolisten unix" \
                --auto-servernum -e /dev/stdout \
                ctest "${CTEST_ARGS[@]}"
        else
            ctest "${CTEST_ARGS[@]}"
        fi
    '

log "all stages completed successfully"
