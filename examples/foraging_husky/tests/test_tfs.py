"""Pytest unit tests for the foraging_husky pure-Python TF helpers.

The TF modules themselves (camera_to_sensory, drive_update,
obstacle_to_avoidance, motors_to_husky) import nrp_core at module
load, which is not safe to do outside an NRPCoreSim process — pytest
collection would crash on the bare import. So those modules delegate
their arithmetic to ``helpers.py``, which is nrp_core-free, and this
test module imports from ``helpers`` directly.

EBR2-32 acceptance:
  * green-only-left image → higher right-motor rate
  * large grey area on left → right-turn bias dominates goal-seeking
  * eating-event distance threshold
  * hunger=1 with symmetric drive input → POP_HUNGRY rate higher than
    POP_SATED  (the drive arbitration is a NEST simulation; we don't
    re-simulate it here — we test the params-level invariants instead)
"""

import os
import random
import sys

import numpy as np
import pytest

# helpers.py and params.py live in the parent directory; make them
# importable regardless of where pytest is invoked from.
THIS_DIR = os.path.dirname(os.path.abspath(__file__))
EXAMPLE_DIR = os.path.dirname(THIS_DIR)
sys.path.insert(0, EXAMPLE_DIR)

import params as P  # noqa: E402
from helpers import (  # noqa: E402
    image_to_channel_rates,
    apply_obstacle_alarm,
    GREY_ALARM_THRESHOLD,
    check_eat_event,
    random_respawn_pose,
    voltage_to_wheel_vels,
    VOLTAGE_SCALE,
)


def _solid_rgb(width, height, rgb):
    img = np.zeros((height, width, 3), dtype=np.uint8)
    img[:, :, 0] = rgb[0]
    img[:, :, 1] = rgb[1]
    img[:, :, 2] = rgb[2]
    return img


# ---------------------------------------------------------------------------
# image_to_channel_rates
# ---------------------------------------------------------------------------

def test_image_to_channel_rates_empty_input_returns_zeros():
    expected = {
        "green_left": 0.0, "green_right": 0.0,
        "blue_left": 0.0, "blue_right": 0.0,
        "grey_left": 0.0, "grey_right": 0.0,
    }
    assert image_to_channel_rates(None) == expected
    assert image_to_channel_rates(np.zeros((0, 0, 3), dtype=np.uint8)) == expected


def test_green_only_on_left_yields_higher_right_motor_rate():
    """Green-on-left maps to cam_green_left_poisson → MOTOR_R in brain.py."""
    img = _solid_rgb(40, 20, (0, 0, 0))
    img[:, :20, 1] = 200  # left half: bright green
    rates = image_to_channel_rates(img)
    assert rates["green_left"] > rates["green_right"]
    assert rates["green_left"] > 0.5 * P.GOAL_POISSON_PEAK
    assert rates["green_right"] < 1e-6
    # Blue/grey channels stay quiet on a pure-green stimulus.
    assert rates["blue_left"] < 1e-6
    assert rates["grey_left"] < 1e-6


def test_blue_only_on_right_yields_higher_left_motor_rate():
    img = _solid_rgb(40, 20, (0, 0, 0))
    img[:, 20:, 2] = 200  # right half: bright blue
    rates = image_to_channel_rates(img)
    assert rates["blue_right"] > rates["blue_left"]
    assert rates["blue_right"] > 0.5 * P.GOAL_POISSON_PEAK


# ---------------------------------------------------------------------------
# apply_obstacle_alarm
# ---------------------------------------------------------------------------

def test_grey_on_left_above_threshold_triggers_alarm_on_both_sides():
    """Large grey area on left -> alarm raises both grey channels to peak."""
    img = _solid_rgb(40, 20, (120, 120, 120))  # uniform grey, both sides
    # Make the right half a non-grey colour so only the left half is
    # grey beyond the alarm threshold.
    img[:, 20:, :] = (10, 200, 10)
    rates = image_to_channel_rates(img)
    assert rates["grey_left"] > GREY_ALARM_THRESHOLD * P.OBSTACLE_POISSON_PEAK
    bumped = apply_obstacle_alarm(rates)
    assert bumped["grey_left"] >= P.OBSTACLE_POISSON_PEAK - 1e-6
    assert bumped["grey_right"] >= P.OBSTACLE_POISSON_PEAK - 1e-6


def test_apply_obstacle_alarm_no_op_below_threshold():
    sub_threshold = GREY_ALARM_THRESHOLD * P.OBSTACLE_POISSON_PEAK * 0.5
    rates = {"green_left": 100.0, "green_right": 0.0,
             "blue_left": 0.0, "blue_right": 0.0,
             "grey_left": sub_threshold, "grey_right": 0.0}
    bumped = apply_obstacle_alarm(rates)
    assert bumped == rates


# ---------------------------------------------------------------------------
# check_eat_event / random_respawn_pose
# ---------------------------------------------------------------------------

def test_eat_event_fires_when_within_threshold():
    husky_xy = (0.0, 0.0)
    food = [(P.EAT_DISTANCE_M - 0.01, 0.0), (5.0, 5.0)]
    assert check_eat_event(husky_xy, food, P.EAT_DISTANCE_M) == 0


def test_eat_event_does_not_fire_when_outside_threshold():
    husky_xy = (0.0, 0.0)
    food = [(P.EAT_DISTANCE_M + 0.01, 0.0), (5.0, 5.0)]
    assert check_eat_event(husky_xy, food, P.EAT_DISTANCE_M) is None


def test_eat_event_picks_closest_when_multiple_in_range():
    husky_xy = (0.0, 0.0)
    food = [(0.5, 0.0),  # 0.5 m
            (0.2, 0.0),  # 0.2 m — closest
            (0.4, 0.0)]
    assert check_eat_event(husky_xy, food, P.EAT_DISTANCE_M) == 1


def test_random_respawn_pose_within_bounds():
    bounds = ((-2.0, 3.0), (-1.0, 5.0))
    rng = random.Random(42)
    for _ in range(100):
        x, y = random_respawn_pose(bounds, rng=rng)
        assert -2.0 <= x <= 3.0
        assert -1.0 <= y <= 5.0


# ---------------------------------------------------------------------------
# voltage_to_wheel_vels
# ---------------------------------------------------------------------------

def test_symmetric_motor_input_yields_pure_forward_motion():
    bl, br, fl, fr = voltage_to_wheel_vels(VOLTAGE_SCALE, VOLTAGE_SCALE)
    assert bl == br == fl == fr == pytest.approx(P.MAX_FORWARD_VEL, rel=1e-6)


def test_motor_r_dominant_turns():
    """MOTOR_R > MOTOR_L: rot_vel > 0 → left wheels -rot, right wheels +rot
    → husky pivots in place. forward_vel = min(0, 1) * MAX_FORWARD_VEL = 0."""
    bl, br, fl, fr = voltage_to_wheel_vels(0.0, VOLTAGE_SCALE)
    assert bl == pytest.approx(-P.MAX_ROT_VEL, rel=1e-6)
    assert br == pytest.approx(P.MAX_ROT_VEL, rel=1e-6)
    assert fl == bl
    assert fr == br


def test_motor_zero_input_yields_zero_velocity():
    assert voltage_to_wheel_vels(0.0, 0.0) == (0.0, 0.0, 0.0, 0.0)


def test_motor_negative_voltage_clamped_to_zero():
    """The LeakyIntegratorAlpha cell has V_reset=0 and shouldn't go
    negative, but defensive clamping ensures bad input doesn't produce
    reversed wheel velocities."""
    assert voltage_to_wheel_vels(-10.0, -10.0) == (0.0, 0.0, 0.0, 0.0)


# ---------------------------------------------------------------------------
# params.py invariants
# ---------------------------------------------------------------------------

def test_drive_arbitration_invariants():
    """If a future params bump drove TONIC_HUNGER_RATE to zero, or
    EAT_KICK_AMPLITUDE below zero, the drive arbitration would be
    pathologically stuck. Keep the invariant explicit."""
    assert P.TONIC_HUNGER_RATE > 0.0
    assert P.DRIVE_INHIB_WEIGHT < 0.0
    assert P.EAT_KICK_AMPLITUDE > 0.0
