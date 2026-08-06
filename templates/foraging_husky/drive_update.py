"""TransceiverFunction: husky proximity to food → POP_SATED kick + respawn.

Reads:
  - ``husky::base_link`` (GazeboLinkDataPack)  — husky position
  - ``food_0`` .. ``food_3`` (GazeboModelDataPack)

Writes:
  - ``eat_kick_dc`` (NEST DC generator) — non-zero amplitude during
    the EAT_KICK_DURATION_TICKS window after a consumption event,
    zero outside that window. The held window lets POP_SATED actually
    beat POP_HUNGRY through the mutual inhibition long enough that
    the blue-seeking phase is visible — a single-tick kick would
    barely show up in the population dynamics.
  - ``food_i`` (GazeboModelDataPack) — eaten food respawns at a random
    point inside RESPAWN_BOUNDS_XY.
  - ``food_*`` + ``rest_*`` (GazeboModelDataPack, all) — on tick 1
    only, every food / rest model is teleported to a fresh random
    position. The SDF default poses become bootstrap placeholders;
    the actual run layout is fresh each invocation.

The pure logic (distance check + respawn coords) lives in
``helpers.check_eat_event`` and ``helpers.random_respawn_pose`` so the
unit tests under ``tests/test_tfs.py`` can exercise it without nrp_core.

FOOD_COUNT and REST_COUNT live in params.py but the EngineDataPack
decorator stack is static (nrp_core's decorator framework binds
keyword names at decoration time). If those constants change, bump
the foraging_world.sdf model count and the decorator block below in
lockstep.

Tracked in EBR2-32.
"""

import math

from nrp_core import (
    EngineDataPack,
    TransceiverFunction,
    DataPackIdentifier,
)
from nrp_core.data.nrp_json import JsonDataPack
from nrp_core.data.nrp_protobuf import GazeboModelDataPack

import params as P
from helpers import check_eat_event, random_respawn_pose


# Module-level state; see camera_to_sensory.py for the closure-vs-
# module-level rationale.
_tick_count = [0]
# Ticks remaining in the current eat-kick hold window. Set to
# P.EAT_KICK_DURATION_TICKS on each eat event; decremented per tick;
# while > 0, eat_kick_dc amplitude is held at P.EAT_KICK_AMPLITUDE.
_eat_kick_remaining = [0]
# True once the tick-1 random-spawn has run successfully. Used in
# preference to `_tick_count[0] == 1` because the first few TF
# invocations happen during NRP-core engine bringup, where stdout
# routing is not yet wired to the compose log capture — a tick-1
# init's print() lines can be silently lost. Latching on a flag means
# the init runs deterministically once, no matter which sim tick the
# stdout pipe actually starts capturing on.
_initialized = [False]


def _pose_xy_or_far(datapack):
    """Extract (x, y) from a Link/Model datapack, or return a far placeholder.

    Gazebo's protobuf Link/Model messages expose `position` as a
    RepeatedScalarField of length 3 — [x, y, z] — not a vec3 with
    .x/.y/.z. Indexing into the list is the safe access pattern.
    """
    if datapack is None or datapack.isEmpty():
        return (1e9, 1e9)
    p = datapack.data.position
    if len(p) < 2:
        return (1e9, 1e9)
    return (p[0], p[1])


def _random_pose_away_from_husky(husky_xy, bounds, z, min_distance=1.2):
    """Random (x, y, z) inside bounds, at least min_distance from husky.

    Used both for the tick-1 initial spawn (so no target lands on top
    of the husky's start pose) and for respawning eaten food. Caps at
    8 rejection-sampling attempts; falls back to a deterministic
    "outside the husky" pose if all 8 land too close (extreme arena
    crowding, unlikely with REST_COUNT + FOOD_COUNT = 7 and bounds
    7×7 m).
    """
    for _ in range(8):
        x, y = random_respawn_pose(bounds)
        if math.hypot(x - husky_xy[0], y - husky_xy[1]) >= min_distance:
            return x, y, z
    # Fallback: deterministic offset on +x.
    return husky_xy[0] + min_distance + 0.3, husky_xy[1], z


def _v_m_or_zero(datapack):
    """Read V_m from a LeakyIntegratorAlpha datapack, default 0.0."""
    if datapack is None or datapack.isEmpty():
        return 0.0
    try:
        return float(datapack.data[0]['V_m'])
    except (KeyError, IndexError, TypeError):
        return 0.0


# Running totals of spike-recorder n_events, one entry per spike
# recorder datapack. Subtracting the running total from the current
# reading gives the per-tick delta — the number of spikes that fired
# in this TF tick window. We accumulate the deltas inside the diag
# emitter so the FORAGING_DIAG spikes line carries the spike count
# over the last DIAG_LOG_EVERY_N_TICKS ticks rather than the
# running cumulative total.
_last_n_events = {
    'pop_hungry': 0, 'pop_sated': 0,
    'motor_l': 0, 'motor_r': 0,
    'brake': 0,
}
# Same per-bucket accumulation as _last_n_events but for the diag
# window (gets zeroed every DIAG_LOG_EVERY_N_TICKS).
_bucket_spikes = {
    'pop_hungry': 0, 'pop_sated': 0,
    'motor_l': 0, 'motor_r': 0,
    'brake': 0,
}


def _spike_delta(datapack, key):
    """Compute spikes since last call from a spike_recorder datapack.

    NEST's spike_recorder accumulates n_events from t=0. The first
    call after recorder creation returns 0 (no previous total). Each
    subsequent call returns the delta since the previous read; we
    track the previous total per-key in _last_n_events. If the
    datapack is empty or the field isn't there, returns 0.
    """
    if datapack is None or datapack.isEmpty():
        return 0
    try:
        # nest_json exposes spike_recorder.n_events as a scalar in the
        # JsonDataPack data dict at the recorder node's status.
        total = int(datapack.data['n_events'])
    except (KeyError, TypeError, IndexError):
        # Some NEST/nrp-core versions return data as a list of node
        # statuses; pick the first one.
        try:
            total = int(datapack.data[0]['n_events'])
        except (KeyError, TypeError, IndexError):
            return 0
    delta = total - _last_n_events[key]
    if delta < 0:  # paranoia — recorder reset would look like a negative
        delta = total
    _last_n_events[key] = total
    return delta


@EngineDataPack(keyword='husky_link',
                id=DataPackIdentifier('husky::base_link', 'gazebo'))
@EngineDataPack(keyword='food_0',
                id=DataPackIdentifier('food_0', 'gazebo'))
@EngineDataPack(keyword='food_1',
                id=DataPackIdentifier('food_1', 'gazebo'))
@EngineDataPack(keyword='food_2',
                id=DataPackIdentifier('food_2', 'gazebo'))
@EngineDataPack(keyword='food_3',
                id=DataPackIdentifier('food_3', 'gazebo'))
@EngineDataPack(keyword='rest_0',
                id=DataPackIdentifier('rest_0', 'gazebo'))
@EngineDataPack(keyword='rest_1',
                id=DataPackIdentifier('rest_1', 'gazebo'))
@EngineDataPack(keyword='rest_2',
                id=DataPackIdentifier('rest_2', 'gazebo'))
@EngineDataPack(keyword='pop_hungry_v',
                id=DataPackIdentifier('pop_hungry_voltage', 'nest'))
@EngineDataPack(keyword='pop_sated_v',
                id=DataPackIdentifier('pop_sated_voltage', 'nest'))
@EngineDataPack(keyword='motor_l_v',
                id=DataPackIdentifier('motor_l_voltage', 'nest'))
@EngineDataPack(keyword='motor_r_v',
                id=DataPackIdentifier('motor_r_voltage', 'nest'))
@EngineDataPack(keyword='brake_v',
                id=DataPackIdentifier('brake_voltage', 'nest'))
@EngineDataPack(keyword='sr_pop_hungry',
                id=DataPackIdentifier('pop_hungry_spikes', 'nest'))
@EngineDataPack(keyword='sr_pop_sated',
                id=DataPackIdentifier('pop_sated_spikes', 'nest'))
@EngineDataPack(keyword='sr_motor_l',
                id=DataPackIdentifier('motor_l_spikes', 'nest'))
@EngineDataPack(keyword='sr_motor_r',
                id=DataPackIdentifier('motor_r_spikes', 'nest'))
@EngineDataPack(keyword='sr_brake',
                id=DataPackIdentifier('brake_spikes', 'nest'))
@TransceiverFunction("nest")
def drive_update(husky_link, food_0, food_1, food_2, food_3,
                 rest_0, rest_1, rest_2,
                 pop_hungry_v, pop_sated_v, motor_l_v, motor_r_v,
                 brake_v,
                 sr_pop_hungry, sr_pop_sated, sr_motor_l, sr_motor_r,
                 sr_brake):
    _tick_count[0] += 1

    husky_xy = _pose_xy_or_far(husky_link)
    food_poses = [
        _pose_xy_or_far(food_0),
        _pose_xy_or_far(food_1),
        _pose_xy_or_far(food_2),
        _pose_xy_or_far(food_3),
    ]
    rest_poses = [
        _pose_xy_or_far(rest_0),
        _pose_xy_or_far(rest_1),
        _pose_xy_or_far(rest_2),
    ]

    # Closest rest distance — used to drive the brake pathway in brain.py
    # via near_blue_poisson. When the husky is < REST_DISTANCE_M of any
    # rest tile AND POP_SATED is winning, the brake population fires and
    # inhibits both motor populations → husky parks at the rest zone
    # until SATED decays back to HUNGRY.
    min_rest_dist = min(
        ((rx - husky_xy[0]) ** 2 + (ry - husky_xy[1]) ** 2) ** 0.5
        for rx, ry in rest_poses
    )
    near_blue = min_rest_dist < P.REST_DISTANCE_M

    out = []

    # First TF tick: randomise every food + rest model so the arena
    # layout is not pre-coded — each invocation gets a fresh set of
    # target positions. The husky's search behaviour has to actually
    # find them; whatever the SDF declared is just a bootstrap
    # placeholder. _initialized latches so this only runs once, no
    # matter how many TF calls happen before the first stdout-captured
    # tick.
    if not _initialized[0]:
        _initialized[0] = True
        for i in range(P.FOOD_COUNT):
            x, y, z = _random_pose_away_from_husky(
                husky_xy, P.RESPAWN_BOUNDS_XY, 0.25)
            tp = GazeboModelDataPack(f"food_{i}", "gazebo")
            tp.data.position[:] = [x, y, z]
            out.append(tp)
            print(
                f"FORAGING_DIAG init food_{i} spawn=({x:.2f},{y:.2f})",
                flush=True,
            )
        for i in range(P.REST_COUNT):
            x, y, z = _random_pose_away_from_husky(
                husky_xy, P.RESPAWN_BOUNDS_XY, 0.05)
            tp = GazeboModelDataPack(f"rest_{i}", "gazebo")
            tp.data.position[:] = [x, y, z]
            out.append(tp)
            print(
                f"FORAGING_DIAG init rest_{i} spawn=({x:.2f},{y:.2f})",
                flush=True,
            )

    # Eat events fire only when the husky is HUNGRY. While the
    # SATED hold window is still open, the husky physically drives
    # over food without consuming it — that's what lets the SATED
    # window actually decay (HUNGRY → eat → SATED hold → eventual
    # decay → HUNGRY again → eat next food → cycle). Without this
    # gate every food the husky encounters refreshes the window
    # and the state stays permanently SATED for the rest of the
    # run, defeating the "observe a full cycle in one minute"
    # acceptance criterion.
    if _eat_kick_remaining[0] == 0:
        eaten = check_eat_event(husky_xy, food_poses, P.EAT_DISTANCE_M)
    else:
        eaten = None

    if eaten is not None:
        # New eat event — start the SATED hold window. If a previous
        # window was still active, reset it (a fresh kick replaces the
        # tail of the previous).
        _eat_kick_remaining[0] = P.EAT_KICK_DURATION_TICKS
        x, y, _ = _random_pose_away_from_husky(
            husky_xy, P.RESPAWN_BOUNDS_XY, 0.25)
        respawn = GazeboModelDataPack(f"food_{eaten}", "gazebo")
        respawn.data.position[:] = [x, y, 0.25]
        out.append(respawn)
        # Always log eat events (no throttling) — they're rare and
        # functionally critical. check_functionality.sh treats at
        # least one of these as a strong-pass signal.
        print(
            f"FORAGING_DIAG ate food_{eaten} husky_xy=({husky_xy[0]:.2f},{husky_xy[1]:.2f}) "
            f"respawn=({x:.2f},{y:.2f})",
            flush=True,
        )

    # Decide eat_kick amplitude based on whether the hold window is
    # still open. The TF runs every tick, so eat_kick_dc gets its
    # amplitude updated each iteration; the hold counter is what
    # carries the SATED state across ticks.
    sated = _eat_kick_remaining[0] > 0
    if sated:
        amp = P.EAT_KICK_AMPLITUDE
        _eat_kick_remaining[0] -= 1
    else:
        amp = 0.0
    eat_kick = JsonDataPack("eat_kick_dc", "nest")
    eat_kick.data["amplitude"] = amp
    out.append(eat_kick)
    # eat_kick_dc's amplitude doubles as the drive-state indicator —
    # obstacle_to_avoidance.py reads it back and zeros the gated
    # camera channel accordingly. Re-using the existing registered
    # datapack avoids the cross-engine free-form-datapack problem
    # (a gazebo-side JsonDataPack written from a "nest"-target TF
    # fails with "DataPack drive_state is not registered in engine
    # gazebo").

    # near_blue_poisson rate — feeds the brake coincidence detector
    # in brain.py. Goes to NEAR_BLUE_POISSON_RATE when the husky is
    # within REST_DISTANCE_M of any blue rest tile, 0 otherwise.
    # The brake fires only when this AND POP_SATED both exceed
    # threshold, so the brake pathway is a true SNN-side AND gate.
    nb = JsonDataPack("near_blue_poisson", "nest")
    nb.data["rate"] = P.NEAR_BLUE_POISSON_RATE if near_blue else 0.0
    out.append(nb)

    # Accumulate spike deltas per tick; emit the totals every diag
    # interval so a single log line summarises the spike activity
    # over a known window rather than per-tick.
    _bucket_spikes['pop_hungry'] += _spike_delta(sr_pop_hungry, 'pop_hungry')
    _bucket_spikes['pop_sated'] += _spike_delta(sr_pop_sated, 'pop_sated')
    _bucket_spikes['motor_l'] += _spike_delta(sr_motor_l, 'motor_l')
    _bucket_spikes['motor_r'] += _spike_delta(sr_motor_r, 'motor_r')
    _bucket_spikes['brake'] += _spike_delta(sr_brake, 'brake')

    if _tick_count[0] % P.DIAG_LOG_EVERY_N_TICKS == 0:
        f0_x, f0_y = food_poses[0]
        d0 = ((f0_x - husky_xy[0]) ** 2 + (f0_y - husky_xy[1]) ** 2) ** 0.5
        state = "SATED" if _eat_kick_remaining[0] > 0 else "HUNGRY"
        print(
            f"FORAGING_DIAG drive tick={_tick_count[0]} state={state} "
            f"husky_xy=({husky_xy[0]:.2f},{husky_xy[1]:.2f}) "
            f"food_0=({f0_x:.2f},{f0_y:.2f}) dist0={d0:.2f} "
            f"min_rest={min_rest_dist:.2f} near_blue={int(near_blue)} "
            f"eat_amp={amp:.0f} sated_ticks_left={_eat_kick_remaining[0]}",
            flush=True,
        )
        # SNN observer line — proves the drive arbitration is actually
        # running in NEST. hungry_v and sated_v are V_m on
        # bd.LeakyIntegratorAlpha cells reading the respective drive
        # populations; mutual inhibition + eat-kick DC + tonic Poisson
        # produce anti-correlated time series. check_functionality.sh
        # asserts at least one tick where hungry_v > sated_v AND at
        # least one tick where sated_v > hungry_v.
        hv = _v_m_or_zero(pop_hungry_v)
        sv = _v_m_or_zero(pop_sated_v)
        mlv = _v_m_or_zero(motor_l_v)
        mrv = _v_m_or_zero(motor_r_v)
        bv = _v_m_or_zero(brake_v)
        print(
            f"FORAGING_DIAG snn tick={_tick_count[0]} "
            f"hungry_v={hv:.4f} sated_v={sv:.4f} brake_v={bv:.4f} "
            f"motor_l_v={mlv:.4f} motor_r_v={mrv:.4f}",
            flush=True,
        )
        # Per-window spike totals — count of spikes from each
        # population during the last DIAG_LOG_EVERY_N_TICKS TF calls.
        # Voltages are derivative; raw spike counts are the
        # ground-truth evidence that the SNN is producing activity.
        # check_functionality.sh sums these across the run and
        # asserts each population fires > 0 spikes total — guards
        # against silent runs where a tuning regression leaves a
        # population at zero firing the whole sim.
        print(
            f"FORAGING_DIAG spikes tick={_tick_count[0]} "
            f"pop_hungry={_bucket_spikes['pop_hungry']} "
            f"pop_sated={_bucket_spikes['pop_sated']} "
            f"motor_l={_bucket_spikes['motor_l']} "
            f"motor_r={_bucket_spikes['motor_r']} "
            f"brake={_bucket_spikes['brake']}",
            flush=True,
        )
        for k in _bucket_spikes:
            _bucket_spikes[k] = 0

    return out
