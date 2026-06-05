#!/usr/bin/env bash
#
# examples/foraging_husky/check_functionality.sh
#
# Post-run functional gate for the foraging_husky compose example.
# `nrp-core exited with code 0` only proves the orchestration shut
# down cleanly — it does NOT prove the SNN drove the husky. This
# script parses the FORAGING_DIAG lines emitted by the four TFs
# (camera_to_sensory, obstacle_to_avoidance, drive_update,
# motors_to_husky) and asserts each functional checkpoint.
#
# Usage:
#   bash check_functionality.sh <compose_log_path>
#
# Exit status:
#   0  every mandatory check passed.
#   1  at least one mandatory check failed.
#
# Mandatory checks (the run is RED if any fail):
#   * camera_alive:    at least one FORAGING_DIAG cam line with any
#                      channel rate > 0 (the husky's camera is seeing
#                      coloured pixels — food / rest / obstacle).
#   * brain_alive:     at least one FORAGING_DIAG motor line with
#                      v_l > 0 OR v_r > 0 (NEST brain produced non-
#                      zero motor activity).
#   * husky_alive:     at least one FORAGING_DIAG motor line with
#                      any wheel velocity != 0 (the wheel-control
#                      pathway delivered a real command to Gazebo).
#   * ate_anything:    at least one FORAGING_DIAG ate line. Reaching
#                      a food cylinder and producing the SATED kick
#                      is the defining behaviour of the experiment.
#   * snn_arbitration: hungry_v > sated_v at some point AND
#                      sated_v > hungry_v at another. The drive
#                      populations only anti-correlate if NEST's
#                      mutual-inhibition arbitration is actually
#                      running — TFs can't fake this dynamics, so
#                      it's the cleanest functional proof that the
#                      SNN is doing real work.
#   * snn_spikes_nonzero: each of the five tracked populations
#                      (pop_hungry, pop_sated, motor_l, motor_r,
#                      brake) produced > 0 spikes during the run.
#                      The check also prints per-population totals —
#                      that report IS the spike analysis (no
#                      separate post-processing). Guards against
#                      silent runs where a tuning regression leaves
#                      a population at zero firing all sim long.
#
# Tracked in EBR2-32.

set -euo pipefail

if [[ $# -ne 1 ]]; then
    echo "Usage: $(basename "$0") <compose_log_path>" >&2
    exit 2
fi

LOG="$1"
if [[ ! -f "$LOG" ]]; then
    echo "[check_functionality] log file not found: $LOG" >&2
    exit 2
fi

# ---------------------------------------------------------------------------
# Helpers
# ---------------------------------------------------------------------------

pass=0
fail=0
warn=0

report() {
    local status="$1" name="$2" msg="$3"
    case "$status" in
        PASS) printf '  [PASS] %-15s %s\n' "$name" "$msg"; ((pass+=1)) ;;
        FAIL) printf '  [FAIL] %-15s %s\n' "$name" "$msg"; ((fail+=1)) ;;
        WARN) printf '  [WARN] %-15s %s\n' "$name" "$msg"; ((warn+=1)) ;;
    esac
}

# ---------------------------------------------------------------------------
# Mandatory check: camera sees coloured pixels
# ---------------------------------------------------------------------------
# At least one FORAGING_DIAG cam line with any rate > 0. Use awk to scan
# the green_l / green_r / blue_l / blue_r / grey_l / grey_r columns.

cam_lines=$(grep -aE "FORAGING_DIAG cam " "$LOG" 2>/dev/null | wc -l)
cam_with_signal=$(grep -aE "FORAGING_DIAG cam " "$LOG" 2>/dev/null \
    | awk '{
        for (i = 1; i <= NF; i++) {
            split($i, kv, "=")
            k = kv[1]
            v = kv[2] + 0.0
            if (k == "green_l" && v > 0) { print; next }
            if (k == "green_r" && v > 0) { print; next }
            if (k == "blue_l"  && v > 0) { print; next }
            if (k == "blue_r"  && v > 0) { print; next }
            if (k == "grey_l"  && v > 0) { print; next }
            if (k == "grey_r"  && v > 0) { print; next }
        }
    }' | wc -l)

if [[ "$cam_lines" -eq 0 ]]; then
    report FAIL camera_alive "no FORAGING_DIAG cam lines found (TF didn't run?)"
elif [[ "$cam_with_signal" -eq 0 ]]; then
    report FAIL camera_alive \
        "$cam_lines cam lines, none with any colour rate > 0 (husky sees only background)"
else
    report PASS camera_alive \
        "$cam_with_signal of $cam_lines cam ticks had non-zero channel rates"
fi

# ---------------------------------------------------------------------------
# Mandatory check: brain produced motor activity
# ---------------------------------------------------------------------------

motor_lines=$(grep -aE "FORAGING_DIAG motor " "$LOG" 2>/dev/null | wc -l)
motor_with_voltage=$(grep -aE "FORAGING_DIAG motor " "$LOG" 2>/dev/null \
    | awk '{
        v_l = 0.0; v_r = 0.0
        for (i = 1; i <= NF; i++) {
            split($i, kv, "=")
            if (kv[1] == "v_l") v_l = kv[2] + 0.0
            if (kv[1] == "v_r") v_r = kv[2] + 0.0
        }
        if (v_l > 0.0 || v_r > 0.0) print
    }' | wc -l)

if [[ "$motor_lines" -eq 0 ]]; then
    report FAIL brain_alive "no FORAGING_DIAG motor lines found"
elif [[ "$motor_with_voltage" -eq 0 ]]; then
    report FAIL brain_alive \
        "$motor_lines motor ticks all reported v_l=0 and v_r=0 (brain didn't fire)"
else
    report PASS brain_alive \
        "$motor_with_voltage of $motor_lines motor ticks had non-zero leaky-integrator V_m"
fi

# ---------------------------------------------------------------------------
# Mandatory check: husky received wheel commands
# ---------------------------------------------------------------------------

wheels_nonzero=$(grep -aE "FORAGING_DIAG motor " "$LOG" 2>/dev/null \
    | awk '{
        for (i = 1; i <= NF; i++) {
            if ($i ~ /^wheels=/) {
                gsub(/wheels=\(/, "", $i); gsub(/\)/, "", $i)
                split($i, w, ",")
                for (j = 1; j <= 4; j++) {
                    if (w[j] + 0.0 != 0.0) { print; next }
                }
            }
        }
    }' | wc -l)

if [[ "$wheels_nonzero" -eq 0 ]]; then
    report FAIL husky_alive \
        "every motor tick produced wheels=(0,0,0,0) (husky never moved)"
else
    report PASS husky_alive \
        "$wheels_nonzero motor ticks delivered non-zero wheel velocities"
fi

# ---------------------------------------------------------------------------
# Mandatory check: SNN mutual-inhibition arbitration is running
# ---------------------------------------------------------------------------
# The drive arbitration is implemented as POP_HUNGRY ↔ POP_SATED mutual
# inhibition in NEST. If NEST is genuinely doing the work, the two
# leaky-integrator readouts (hungry_v / sated_v) anti-correlate: one
# rises while the other falls. We assert there is at least one snn
# diag line where hungry_v > sated_v AND at least one where the order
# flips. That cannot be faked by a Python TF — it requires the NEST
# kernel to be running the iaf_psc_alpha + Poisson + DC + mutual
# inhibition machinery.

snn_lines=$(grep -acE "FORAGING_DIAG snn " "$LOG" 2>/dev/null || true)
hungry_wins=$(grep -aE "FORAGING_DIAG snn " "$LOG" 2>/dev/null \
    | awk '{
        h = 0.0; s = 0.0
        for (i = 1; i <= NF; i++) {
            split($i, kv, "=")
            if (kv[1] == "hungry_v") h = kv[2] + 0.0
            if (kv[1] == "sated_v")  s = kv[2] + 0.0
        }
        if (h > s) print
    }' | wc -l)
sated_wins=$(grep -aE "FORAGING_DIAG snn " "$LOG" 2>/dev/null \
    | awk '{
        h = 0.0; s = 0.0
        for (i = 1; i <= NF; i++) {
            split($i, kv, "=")
            if (kv[1] == "hungry_v") h = kv[2] + 0.0
            if (kv[1] == "sated_v")  s = kv[2] + 0.0
        }
        if (s > h) print
    }' | wc -l)

if [[ "$snn_lines" -eq 0 ]]; then
    report FAIL snn_arbitration "no FORAGING_DIAG snn lines found"
elif [[ "$hungry_wins" -eq 0 ]]; then
    report FAIL snn_arbitration \
        "hungry_v was never > sated_v in any of $snn_lines snn ticks (drive arbitration not running)"
elif [[ "$sated_wins" -eq 0 ]]; then
    report FAIL snn_arbitration \
        "sated_v was never > hungry_v in any of $snn_lines snn ticks (eat-kick / mutual-inhibition flip not happening)"
else
    report PASS snn_arbitration \
        "$hungry_wins HUNGRY-wins ticks + $sated_wins SATED-wins ticks (mutual-inhibition flip confirmed)"
fi

# ---------------------------------------------------------------------------
# Mandatory check: every NEST population produced spikes
# ---------------------------------------------------------------------------
# Voltages on the LeakyIntegrator readouts (snn_arbitration check)
# prove the populations *integrate* spikes, but if a recorder were
# disconnected from its source population the leaky cell would still
# read 0 quietly. Pulling raw spike counts straight off
# nest.spike_recorder and summing them by population gives an
# unambiguous "did this population fire at all" signal. The print
# block below is also the run's spike analysis report — totals + an
# implied average firing rate.

# Compute per-population spike totals by summing the per-bucket
# FORAGING_DIAG spikes deltas.
spike_totals=$(grep -aE "FORAGING_DIAG spikes " "$LOG" 2>/dev/null \
    | awk '{
        for (i = 1; i <= NF; i++) {
            split($i, kv, "=")
            k = kv[1]; v = kv[2] + 0
            if (k == "pop_hungry" || k == "pop_sated" \
             || k == "motor_l"    || k == "motor_r" \
             || k == "brake") {
                tot[k] += v
            }
        }
    } END {
        for (k in tot) printf "%s=%d\n", k, tot[k]
    }')

echo
echo "  --- SNN spike analysis ---"
if [[ -z "$spike_totals" ]]; then
    report FAIL snn_spikes_nonzero "no FORAGING_DIAG spikes lines found"
else
    # Print the totals report so the run output IS the analysis (no
    # separate post-processing step needed). Sort by population name
    # for stable output.
    while IFS= read -r line; do
        printf '  %s\n' "$line"
    done < <(echo "$spike_totals" | sort)
    # Assert every named population has > 0 spikes. Zero is the
    # failure mode the user explicitly called out — "It should not
    # be zeros".
    silent_pops=$(echo "$spike_totals" | awk -F= '$2 == 0 {print $1}')
    if [[ -n "$silent_pops" ]]; then
        report FAIL snn_spikes_nonzero \
            "silent populations during the run: $(echo "$silent_pops" | tr '\n' ' ')"
    else
        report PASS snn_spikes_nonzero \
            "every population produced spikes during the run"
    fi
fi

# ---------------------------------------------------------------------------
# Mandatory check: did the husky eat anything?
# ---------------------------------------------------------------------------

eat_events=$(grep -acE "FORAGING_DIAG ate " "$LOG" 2>/dev/null || true)
if [[ "$eat_events" -eq 0 ]]; then
    report FAIL ate_anything \
        "no eat events fired during this run (husky didn't reach a food cylinder)"
else
    report PASS ate_anything \
        "$eat_events eat events fired"
fi

# ---------------------------------------------------------------------------
# Summary
# ---------------------------------------------------------------------------

printf '\n[check_functionality] %d pass, %d fail, %d warn\n' "$pass" "$fail" "$warn"

if [[ "$fail" -gt 0 ]]; then
    echo "[check_functionality] foraging_husky run is RED — robot did not perform the task."
    exit 1
fi

echo "[check_functionality] foraging_husky run is GREEN — robot performed the task."
exit 0
