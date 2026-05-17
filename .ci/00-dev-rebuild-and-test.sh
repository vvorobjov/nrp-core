#!/usr/bin/env bash
#
# .ci/00-dev-rebuild-and-test.sh
#
# One-shot helper for the local dev loop in nrp-core:
#   1. Ensure the canonical devcontainer image
#      (nrp-local/nrp-nest-gazebo:local) exists. Rebuild it if
#      missing or when --rebuild-image is given.
#   2. Run cmake configure + make + make install inside the container,
#      against the mounted source tree.
#   3. Run the ctest suite.
#
# Only the jammy (Ubuntu 22.04 / Humble / Python 3.10) target is
# supported; EBR2-81 dropped the parallel focal chain.
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
CMAKE_CACHE_OVERRIDE=""

usage() {
    cat <<EOF
Usage: $(basename "$0") [options]

Options:
  --rebuild-image       Force rebuild of the nrp-nest-gazebo image even if present.
  --no-image            Skip the image existence check entirely (assume it exists).
  --test-filter REGEX   Pass -R REGEX to ctest (run only matching tests).
  --keep-build          Do not wipe build/ before configuring.
  --cmake-cache PATH    Override the cmake initial-cache preset. PATH must be
                        the in-container path (e.g.
                        /workspace/.ci/cmake_cache/vanilla.cmake). The default
                        is /workspace/.ci/cmake_cache/nest-gazebo.cmake.
  -h, --help            Show this message and exit.

Canonical image: nrp-local/nrp-nest-gazebo:local (Ubuntu 22.04 /
Humble / Python 3.10), built by docker-compose.yaml's nrp-nest-gazebo
service.
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
        --cmake-cache)
            [[ $# -ge 2 ]] || { echo "[$0] --cmake-cache requires an argument" >&2; exit 2; }
            CMAKE_CACHE_OVERRIDE="$2"
            shift
            ;;
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

IMAGE="nrp-local/nrp-nest-gazebo:local"
BUILD_SERVICE="nrp-nest-gazebo"
DEFAULT_CMAKE_CACHE="/workspace/.ci/cmake_cache/nest-gazebo.cmake"

# The canonical preset enables MQTT / NEST / Gazebo / ROS — i.e. it's
# what CLAUDE.md and reviewers expect ctest to exercise. Without this
# the in-container 11-prepare-build.sh falls back to vanilla.cmake and
# silently skips ~100 of the 185 tests (every MQTT / Gazebo / NEST /
# ROS gated test goes uncompiled), which masks real regressions.
# See EBR2-71.
CMAKE_CACHE_IN_CONTAINER="${CMAKE_CACHE_OVERRIDE:-$DEFAULT_CMAKE_CACHE}"

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
log "cmake initial-cache: $CMAKE_CACHE_IN_CONTAINER"

docker run --rm --net=host --privileged \
    -v "$REPO_ROOT:/workspace" -w /workspace \
    -e "NRP_TEST_FILTER=$TEST_FILTER" \
    -e "NRP_KEEP_BUILD=$KEEP_BUILD" \
    -e "CMAKE_CACHE_FILE=$CMAKE_CACHE_IN_CONTAINER" \
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
        # --timeout 120: cap each test at 2 min. Default ctest timeout
        # is 1500 s (25 min) — single hung test burns half an hour of
        # wall-clock for no info. Longest legitimately-passing test in
        # the canonical preset finishes in ~42 s, so 120 s leaves
        # ~3× headroom. Individual tests that genuinely need longer
        # can override in cmake via
        #   set_tests_properties(<name> PROPERTIES TIMEOUT <seconds>)
        # See EBR2-78.
        declare -a CTEST_ARGS=(--no-compress-output --test-output-size-failed 300000 --output-on-failure --timeout 120)
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
