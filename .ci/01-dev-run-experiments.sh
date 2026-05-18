#!/usr/bin/env bash
#
# .ci/01-dev-run-experiments.sh
#
# Ultimate-test runner: walks every supported experiment under
# examples/<subset>/simulation_config*.json and launches each one
# end-to-end inside the canonical jammy container. Reports a
# PASS/FAIL/SKIP summary; exits 0 iff every non-skipped experiment
# completed cleanly (NRPCoreSim exit code 0) within its per-test
# wall-clock cap.
#
# This is the "gold standard" gate on top of the minimum gate
# documented in CLAUDE.md ("the two rules that always win" — one
# ctest run plus the two husky_braitenberg compose runs). The
# enumeration here was assembled from EBR2-83 and captures the
# experiments that have historically caught regressions ctest
# missed.
#
# Only the jammy (Ubuntu 22.04 / Humble / Python 3.10) image is
# supported; EBR2-81 dropped the parallel focal chain and there
# is no --ubuntu20 flag here by design.
#
# Tracked in EBR2-83.

set -euo pipefail

# -----------------------------------------------------------------------------
# Argument parsing
# -----------------------------------------------------------------------------

REBUILD_IMAGE=0
NO_IMAGE=0
KEEP_BUILD=0
FILTER=""
CMAKE_CACHE_OVERRIDE=""

usage() {
    cat <<EOF
Usage: $(basename "$0") [options]

Drives every supported NRPCoreSim experiment under examples/ inside
the canonical jammy container (nrp-local/nrp-nest-gazebo:local) and
reports a PASS/FAIL/SKIP summary.

Options:
  --rebuild-image       Force rebuild of the nrp-nest-gazebo image even if present.
  --no-image            Skip the image existence check (assume it exists).
  --filter REGEX        Only run experiments whose config path matches REGEX.
  --keep-build          Accepted for API parity with .ci/00-dev-rebuild-and-test.sh;
                        this script does not (re)build nrp-core itself, so the
                        flag is a no-op here.
  --cmake-cache PATH    Accepted for API parity with .ci/00-dev-rebuild-and-test.sh;
                        no-op here (this script runs prebuilt binaries from the
                        image, not a fresh cmake build).
  -h, --help            Show this message and exit.

Image:
  nrp-local/nrp-nest-gazebo:local  (Ubuntu 22.04 / Humble / Python 3.10)

Side processes the script may start on the host:
  * eclipse-mosquitto    when an experiment needs MQTT (on localhost:1883).
                         Started lazily and torn down on exit.

Skips (logged, not silent):
  * SpiNNaker variants — require physical SpiNNaker hardware.
  * opensim_control_async — needs an external OpenSim simulator publishing
    over MQTT; not bundled.

Exit status:
  0   every non-skipped experiment passed.
  >0  at least one experiment failed (or arg parsing failed).
EOF
}

while [[ $# -gt 0 ]]; do
    case "$1" in
        --rebuild-image) REBUILD_IMAGE=1 ;;
        --no-image)      NO_IMAGE=1 ;;
        --keep-build)    KEEP_BUILD=1 ;;
        --filter)
            [[ $# -ge 2 ]] || { echo "[$0] --filter requires an argument" >&2; exit 2; }
            FILTER="$2"
            shift
            ;;
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

# Surface API-parity flags that this script does not honour, so the
# user does not assume they took effect.
if [[ "$KEEP_BUILD" -eq 1 ]]; then
    echo "[$(basename "$0")] note: --keep-build is a no-op here (no in-script build)." >&2
fi
if [[ -n "$CMAKE_CACHE_OVERRIDE" ]]; then
    echo "[$(basename "$0")] note: --cmake-cache is a no-op here (no in-script build)." >&2
fi

# -----------------------------------------------------------------------------
# Locate the repo
# -----------------------------------------------------------------------------

SCRIPT_DIR="$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" &>/dev/null && pwd)"
REPO_ROOT="$(cd -- "$SCRIPT_DIR/.." &>/dev/null && pwd)"
cd "$REPO_ROOT"

IMAGE="nrp-local/nrp-nest-gazebo:local"
BUILD_SERVICE="nrp-nest-gazebo"

log() { printf '\n[%s] %s\n' "$(basename "$0")" "$*"; }

# -----------------------------------------------------------------------------
# Step 1: ensure the canonical image
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
# Step 2: experiment enumeration
# -----------------------------------------------------------------------------
#
# Each entry: "<rel/path/to/simulation_config*.json>|<flags>"
# Flags (comma-separated, all optional):
#   SKIP=<reason>          Experiment is documented-skipped (e.g. needs hardware
#                          we don't provide locally). Logged in the summary.
#   NEEDS_MQTT             Spin up an eclipse-mosquitto broker on the host
#                          before running this experiment.
#   NEEDS_DOCKER_SOCKET    Mount /var/run/docker.sock so NRPCoreSim's
#                          docker_launcher can spawn sibling engine containers.
#
# The list mirrors the EBR2-83 spec and totals 24 entries (20 active,
# 4 documented skips: 3× SpiNNaker + 1× external-OpenSim).

EXPERIMENTS=(
    "examples/event_loop_examples/cpp_nodes_simple/simulation_config.json|"
    "examples/event_loop_examples/husky_braitenberg_async/simulation_config.json|NEEDS_MQTT"
    "examples/event_loop_examples/husky_braitenberg_cg/simulation_config.json|"
    "examples/event_loop_examples/husky_braitenberg_ros/simulation_config.json|"
    "examples/event_loop_examples/husky_braitenberg_spinnaker/simulation_config.json|SKIP=SpiNNaker hardware required"
    "examples/event_loop_examples/husky_braitenberg_spinnaker_ros/simulation_config.json|SKIP=SpiNNaker hardware required"
    "examples/event_loop_examples/mqtt_simple/simulation_config.json|NEEDS_MQTT"
    "examples/event_loop_examples/opensim_control_async/simulation_config.json|SKIP=External OpenSim simulator required (MQTT data publisher not bundled)"
    "examples/event_loop_examples/spinnaker_simple/simulation_config.json|SKIP=SpiNNaker hardware required"
    "examples/event_loop_examples/tf_exchange_async/simulation_config_json.json|NEEDS_MQTT"
    "examples/event_loop_examples/tf_exchange_async/simulation_config_proto.json|NEEDS_MQTT"
    "examples/husky_braitenberg_multi_robot/simulation_config.json|"
    "examples/status_function_test/simulation_config.json|"
    "examples/status_function_test/simulation_config_done_flag.json|"
    "examples/status_function_test/simulation_config_loopback.json|"
    "examples/status_function_test/simulation_config_trajectory_json.json|"
    "examples/status_function_test/simulation_config_trajectory_mixed.json|"
    "examples/status_function_test/simulation_config_trajectory_proto.json|"
    "examples/status_function_test/simulation_config_trajectory_timeout.json|"
    "examples/nrp_vectorization/simulation_config.json|"
    "examples/baseball_icub/simulation_config.json|"
    "examples/generic_proto_test/simulation_config.json|"
    "examples/nest_simple/simulation_config.json|"
    "examples/nest_simple/simulation_config_docker.json|NEEDS_DOCKER_SOCKET"
)

# Sanity-check the manifest at script start so a typo'd path is flagged
# before we spend time spinning up containers.
for _entry in "${EXPERIMENTS[@]}"; do
    _cfg="${_entry%%|*}"
    if [[ ! -f "$REPO_ROOT/$_cfg" ]]; then
        echo "[$(basename "$0")] manifest error: $_cfg does not exist." >&2
        exit 3
    fi
done
unset _entry _cfg

log "manifest validated: ${#EXPERIMENTS[@]} entries."

# -----------------------------------------------------------------------------
# Step 3: side-process management (MQTT broker)
# -----------------------------------------------------------------------------
#
# A small subset of experiments (mqtt_simple, husky_braitenberg_async,
# tf_exchange_async × 2) needs a reachable MQTT broker on localhost:1883.
# The canonical compose example brings up eclipse-mosquitto as part of
# the husky_braitenberg topology; here we start a single broker container
# on the host network and tear it down at script exit. We use a fixed
# name so re-runs don't accidentally leak brokers.

MQTT_BROKER_NAME="nrp-ult-mosquitto"
MQTT_STARTED=0
MQTT_CONF_HOST=""

start_mqtt_broker() {
    if [[ "$MQTT_STARTED" -eq 1 ]]; then
        return
    fi
    if docker ps --format '{{.Names}}' | grep -qx "$MQTT_BROKER_NAME"; then
        log "MQTT broker '$MQTT_BROKER_NAME' is already running; reusing it."
        MQTT_STARTED=1
        return
    fi
    # eclipse-mosquitto 2.x defaults to allow_anonymous=false and only
    # listens on 127.0.0.1, which breaks the experiments. Write a minimal
    # permissive config and mount it. This mirrors what
    # examples/husky_braitenberg/mosquitto.conf exposes for the canonical
    # compose example.
    MQTT_CONF_HOST="$(mktemp -t nrp-ult-mqtt-XXXXXX.conf)"
    cat > "$MQTT_CONF_HOST" <<'EOF'
listener 1883
protocol mqtt
allow_anonymous true
connection_messages true
log_type all
EOF
    log "starting MQTT broker '$MQTT_BROKER_NAME' (eclipse-mosquitto, host net, anonymous OK)."
    docker run -d --rm --name "$MQTT_BROKER_NAME" --net=host \
        -v "$MQTT_CONF_HOST:/mosquitto/config/mosquitto.conf:ro" \
        eclipse-mosquitto >/dev/null
    MQTT_STARTED=1
    # Give the broker a beat to bind 1883 before we let the first client
    # connect. Tests sometimes fail fast on a connection refused if we
    # don't wait at all.
    sleep 1
}

stop_mqtt_broker() {
    if [[ "$MQTT_STARTED" -eq 1 ]]; then
        log "stopping MQTT broker '$MQTT_BROKER_NAME'."
        docker stop "$MQTT_BROKER_NAME" >/dev/null 2>&1 || true
        MQTT_STARTED=0
    fi
    if [[ -n "$MQTT_CONF_HOST" ]] && [[ -f "$MQTT_CONF_HOST" ]]; then
        rm -f "$MQTT_CONF_HOST"
    fi
}

# -----------------------------------------------------------------------------
# Docker-launcher image alias
# -----------------------------------------------------------------------------
#
# examples/nest_simple/simulation_config_docker.json hardcodes the
# registry-pushed engine image name
#   nrp-core/nrp-nest-gazebo:latest
# which is not present on a dev machine that only has the locally-
# built canonical image (nrp-local/nrp-nest-gazebo:local). Rather than
# rewriting the experiment config (out of scope here) or skipping the
# experiment (we'd miss the docker_launcher code path entirely), we
# tag-alias the canonical local image to the expected name for the
# duration of this script run. `docker tag` only adds another name
# reference to the same underlying image — no mutation of image
# content, just a pointer — and we remove the alias on exit so the
# host tag namespace is unchanged after.

DOCKER_ALIAS_TAG="nrp-core/nrp-nest-gazebo:latest"
DOCKER_ALIAS_ADDED=0

ensure_docker_alias() {
    if [[ "$DOCKER_ALIAS_ADDED" -eq 1 ]]; then return; fi
    if docker image inspect "$DOCKER_ALIAS_TAG" >/dev/null 2>&1; then
        log "image $DOCKER_ALIAS_TAG already present; using as-is."
        return
    fi
    log "tag-aliasing $IMAGE -> $DOCKER_ALIAS_TAG (removed on script exit)."
    docker tag "$IMAGE" "$DOCKER_ALIAS_TAG"
    DOCKER_ALIAS_ADDED=1
}

remove_docker_alias() {
    if [[ "$DOCKER_ALIAS_ADDED" -eq 1 ]]; then
        log "removing docker-launcher tag alias $DOCKER_ALIAS_TAG."
        docker rmi "$DOCKER_ALIAS_TAG" >/dev/null 2>&1 || true
        DOCKER_ALIAS_ADDED=0
    fi
}

cleanup() {
    stop_mqtt_broker
    remove_docker_alias
}
trap cleanup EXIT

# -----------------------------------------------------------------------------
# Step 4: per-experiment runner
# -----------------------------------------------------------------------------

# Per-test wall-clock cap. Baseball-iCub loads a full Gazebo iCub and a
# NEST brain at startup; it is the slowest of the supported experiments.
# 180 s leaves comfortable headroom over its observed ~30-40 s init +
# 10 s SimulationTimeout. The cap exists so a hung experiment does not
# burn the whole gate (it's the same pattern as ctest's --timeout 120
# in .ci/00-dev-rebuild-and-test.sh).
PER_TEST_TIMEOUT=180

# All per-experiment stdout/stderr is captured under a single tmpdir
# so we can tail only the failed ones in the summary.
LOG_DIR="$(mktemp -d -t nrp-ult-XXXXXX)"
log "per-experiment logs: $LOG_DIR"

PASS=()
FAIL=()
SKIP=()

# Each entry's flag string is parsed once into local booleans + a
# skip reason. Flags can co-exist (e.g. NEEDS_MQTT,NEEDS_DOCKER_SOCKET)
# even if no current experiment uses both.
run_one() {
    local config="$1" flagstr="$2"

    local skip_reason=""
    local needs_mqtt=0
    local needs_docker_socket=0

    IFS=',' read -ra flags <<< "$flagstr"
    for f in "${flags[@]}"; do
        case "$f" in
            SKIP=*)             skip_reason="${f#SKIP=}" ;;
            NEEDS_MQTT)         needs_mqtt=1 ;;
            NEEDS_DOCKER_SOCKET) needs_docker_socket=1 ;;
            "")                 ;;
            *) echo "[$(basename "$0")] unknown flag '$f' on $config" >&2; exit 4 ;;
        esac
    done

    if [[ -n "$skip_reason" ]]; then
        printf '[SKIP] %s — %s\n' "$config" "$skip_reason"
        SKIP+=("$config|$skip_reason")
        return
    fi

    if [[ "$needs_mqtt" -eq 1 ]]; then
        start_mqtt_broker
    fi
    if [[ "$needs_docker_socket" -eq 1 ]]; then
        ensure_docker_alias
    fi

    local exp_dir="$REPO_ROOT/$(dirname "$config")"
    local config_basename
    config_basename="$(basename "$config")"
    local logfile="$LOG_DIR/$(echo "$config" | tr '/' '_').log"

    # docker run arguments. --net=host lets the in-container NRPCoreSim
    # talk to the host-side mosquitto on localhost:1883 without extra
    # plumbing. --privileged matches what .ci/00-dev-rebuild-and-test.sh
    # passes (some gazebo/xvfb plumbing wants it).
    local docker_args=( --rm --net=host --privileged )
    if [[ "$needs_docker_socket" -eq 1 ]]; then
        # NRPCoreSim's docker_launcher needs to call out to the host
        # daemon to start sibling engine containers (e.g. nest-simulator).
        # The image's nrpuser (uid 1000) is not in the host's docker
        # group by default, so the socket mount alone yields Permission
        # denied; --group-add with the host socket's GID makes the
        # in-container nrpuser a member of that group at runtime.
        local host_docker_gid
        host_docker_gid=$(stat -c '%g' /var/run/docker.sock 2>/dev/null || true)
        if [[ -n "$host_docker_gid" ]]; then
            docker_args+=( -v /var/run/docker.sock:/var/run/docker.sock \
                           --group-add "$host_docker_gid" )
        else
            docker_args+=( -v /var/run/docker.sock:/var/run/docker.sock )
        fi
    fi
    docker_args+=( -v "$exp_dir":/experiment -w /experiment "$IMAGE" )

    printf '[RUN ] %s ... ' "$config"
    local t_start t_end elapsed rc
    t_start=$(date +%s)
    set +e
    timeout "$PER_TEST_TIMEOUT" docker run "${docker_args[@]}" \
        bash -lc "NRPCoreSim -c '$config_basename' --floglevel info -o SimulationTimeout=10" \
        >"$logfile" 2>&1
    rc=$?
    set -e
    t_end=$(date +%s)
    elapsed=$((t_end - t_start))

    if [[ "$rc" -eq 0 ]]; then
        printf 'PASS (%ds)\n' "$elapsed"
        PASS+=("$config|${elapsed}s")
    else
        if [[ "$rc" -eq 124 ]]; then
            printf 'FAIL (timeout after %ds)\n' "$PER_TEST_TIMEOUT"
            FAIL+=("$config|timeout after ${PER_TEST_TIMEOUT}s")
        else
            printf 'FAIL (exit=%d, %ds)\n' "$rc" "$elapsed"
            FAIL+=("$config|exit=$rc, ${elapsed}s")
        fi
        printf '  --- last 30 lines of %s ---\n' "$logfile"
        tail -n 30 "$logfile" | sed 's/^/  /'
        printf '  --- end ---\n'
    fi
}

for entry in "${EXPERIMENTS[@]}"; do
    cfg="${entry%%|*}"
    flagstr="${entry#*|}"
    if [[ -n "$FILTER" ]] && ! [[ "$cfg" =~ $FILTER ]]; then
        continue
    fi
    run_one "$cfg" "$flagstr"
done

# -----------------------------------------------------------------------------
# Step 5: summary reporter
# -----------------------------------------------------------------------------

print_section() {
    local title="$1"
    shift
    printf '%s (%d):\n' "$title" "$#"
    if [[ "$#" -eq 0 ]]; then
        printf '  (none)\n'
        return
    fi
    for entry in "$@"; do
        local cfg="${entry%%|*}"
        local meta="${entry#*|}"
        if [[ "$cfg" == "$meta" ]]; then
            printf '  - %s\n' "$cfg"
        else
            printf '  - %s  [%s]\n' "$cfg" "$meta"
        fi
    done
}

printf '\n'
printf '%s\n' "============================================================"
printf '  Ultimate-test summary (image: %s)\n' "$IMAGE"
printf '%s\n' "============================================================"
print_section "PASS"  "${PASS[@]}"
print_section "FAIL"  "${FAIL[@]}"
print_section "SKIP"  "${SKIP[@]}"
printf '%s\n' "============================================================"
printf 'Totals: %d pass, %d fail, %d skipped (of %d enumerated)\n' \
    "${#PASS[@]}" "${#FAIL[@]}" "${#SKIP[@]}" "${#EXPERIMENTS[@]}"
printf 'Per-experiment logs kept under: %s\n' "$LOG_DIR"
printf '%s\n' "============================================================"

if [[ "${#FAIL[@]}" -eq 0 ]]; then
    log "ultimate-test gate GREEN."
    exit 0
else
    log "ultimate-test gate RED — ${#FAIL[@]} experiment(s) failed."
    exit 1
fi
