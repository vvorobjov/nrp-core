"""Pure-Python helpers for the foraging_husky TFs.

Everything in this module is free of ``nrp_core`` imports so the unit
tests under ``tests/test_tfs.py`` can exercise the arithmetic without
spinning up Gazebo or NEST. The TF entry-point files
(``camera_to_sensory.py``, ``drive_update.py``,
``obstacle_to_avoidance.py``, ``motors_to_husky.py``) import from this
module and add only the decorator + datapack-glue logic on top.

Tracked in EBR2-32.
"""

import math
import random

import numpy as np
import cv2

import params as P


# ---------------------------------------------------------------------------
# Camera frame -> per-channel left/right Poisson rates
# (used by camera_to_sensory.camera_to_sensory)
# ---------------------------------------------------------------------------

def image_to_channel_rates(rgb_image):
    """Compute per-channel left/right Poisson rates from an RGB image.

    Parameters
    ----------
    rgb_image : numpy.ndarray or None
        H × W × 3 uint8 array in RGB order. Empty / None tolerated
        (returns all-zero rates — the camera datapack is empty for
        ~1 sim-step at startup).

    Returns
    -------
    dict
        Six keys: ``green_left``, ``green_right``, ``blue_left``,
        ``blue_right``, ``grey_left``, ``grey_right``. Values are
        Poisson rates in Hz, normalised to fraction-of-half-image
        of the channel's HSV mask, scaled by the per-channel peak.
    """
    out = {
        "green_left": 0.0, "green_right": 0.0,
        "blue_left": 0.0,  "blue_right": 0.0,
        "grey_left": 0.0,  "grey_right": 0.0,
    }
    if rgb_image is None or rgb_image.size == 0:
        return out

    hsv = cv2.cvtColor(rgb_image, cv2.COLOR_RGB2HSV)
    half = hsv.shape[1] // 2
    half_pixels = hsv.shape[0] * half
    if half_pixels == 0:
        return out

    channels = [
        ("green", P.HSV_GREEN_LOWER, P.HSV_GREEN_UPPER, P.GOAL_POISSON_PEAK),
        ("blue",  P.HSV_BLUE_LOWER,  P.HSV_BLUE_UPPER,  P.GOAL_POISSON_PEAK),
        ("grey",  P.HSV_GREY_LOWER,  P.HSV_GREY_UPPER,  P.OBSTACLE_POISSON_PEAK),
    ]
    for name, lower, upper, peak_hz in channels:
        mask = cv2.inRange(hsv, np.array(lower), np.array(upper))
        left = int(cv2.countNonZero(mask[:, :half])) / half_pixels
        right = int(cv2.countNonZero(mask[:, half:])) / half_pixels
        out[f"{name}_left"] = float(left) * peak_hz
        out[f"{name}_right"] = float(right) * peak_hz
    return out


# ---------------------------------------------------------------------------
# Obstacle alarm (used by obstacle_to_avoidance.obstacle_to_avoidance)
# ---------------------------------------------------------------------------

# Threshold (in normalised Hz units, fraction of OBSTACLE_POISSON_PEAK)
# above which the obstacle channel is treated as an alarm and bumped to
# its peak rate. 0.25 means "if more than 25% of either half-frame is
# grey, treat as an obstacle".
GREY_ALARM_THRESHOLD = 0.25


def apply_obstacle_alarm(rates_dict):
    """Bump both grey channels to peak when either side exceeds threshold.

    Parameters
    ----------
    rates_dict : dict
        Output of ``image_to_channel_rates()`` (six per-channel Hz values).

    Returns
    -------
    dict
        New dict with the same keys; ``grey_left`` / ``grey_right`` may
        have been bumped to ``OBSTACLE_POISSON_PEAK`` if either was
        above the alarm threshold.
    """
    out = dict(rates_dict)
    threshold_hz = GREY_ALARM_THRESHOLD * P.OBSTACLE_POISSON_PEAK
    if (out.get("grey_left", 0.0) > threshold_hz
            or out.get("grey_right", 0.0) > threshold_hz):
        out["grey_left"] = max(out.get("grey_left", 0.0),
                               P.OBSTACLE_POISSON_PEAK)
        out["grey_right"] = max(out.get("grey_right", 0.0),
                                P.OBSTACLE_POISSON_PEAK)
    return out


# ---------------------------------------------------------------------------
# Eat-event distance check + random respawn pose
# (used by drive_update.drive_update)
# ---------------------------------------------------------------------------

def check_eat_event(husky_xy, food_poses, eat_distance):
    """Return the index of the closest food within ``eat_distance``, or None."""
    closest_idx = None
    closest_dist = float('inf')
    for i, (fx, fy) in enumerate(food_poses):
        d = math.hypot(fx - husky_xy[0], fy - husky_xy[1])
        if d < eat_distance and d < closest_dist:
            closest_idx = i
            closest_dist = d
    return closest_idx


def random_respawn_pose(bounds_xy, rng=None):
    """Uniform random (x, y) inside ((xmin,xmax),(ymin,ymax))."""
    r = rng if rng is not None else random
    (xmin, xmax), (ymin, ymax) = bounds_xy
    return r.uniform(xmin, xmax), r.uniform(ymin, ymax)


# ---------------------------------------------------------------------------
# Motor population spike counts -> husky wheel velocities
# (used by motors_to_husky.motors_to_husky)
# ---------------------------------------------------------------------------

def voltage_to_wheel_vels(motor_l_voltage, motor_r_voltage):
    """Convert motor-readout leaky-integrator voltages to four wheel velocities.

    The brain.py wires each motor population (MOTOR_L / MOTOR_R) into
    a LeakyIntegratorAlpha cell; the cell's V_m tracks the population
    spike rate. We normalise to [0, 1] via VOLTAGE_SCALE so the
    Braitenberg-2b arithmetic below maps cleanly onto the husky's
    velocity limits.

    Differential drive: forward = min of two sides (Braitenberg 2b
    convention); rotation = right - left, scaled.

    Parameters
    ----------
    motor_l_voltage, motor_r_voltage : float
        V_m from the matching LeakyIntegratorAlpha cell (mV).

    Returns
    -------
    tuple(float, float, float, float)
        (back_left, back_right, front_left, front_right) wheel
        angular velocities in rad/s.
    """
    norm_l = max(0.0, min(1.0, motor_l_voltage / VOLTAGE_SCALE))
    norm_r = max(0.0, min(1.0, motor_r_voltage / VOLTAGE_SCALE))

    forward_vel = P.MAX_FORWARD_VEL * min(norm_l, norm_r)
    rot_vel = P.MAX_ROT_VEL * (norm_r - norm_l)

    back_left = forward_vel - rot_vel
    back_right = forward_vel + rot_vel
    front_left = forward_vel - rot_vel
    front_right = forward_vel + rot_vel
    return back_left, back_right, front_left, front_right


# Normalisation constant for the leaky-integrator V_m readout. The
# LeakyIntegratorAlpha cell in brain.py uses E_L=0 and unbounded V_th,
# so V_m grows monotonically with cumulative motor-population spike
# activity. Under the EBR2-32 follow-up tuning (tonic motor bias +
# camera-dominant goal pathway) the cell rests around 0.01-0.05 mV;
# saturating the norm at 0.03 mV maps a typical V_m to wheel speed
# while leaving enough range for the MOTOR_L vs MOTOR_R differential
# to register. Combined with MAX_FORWARD_VEL=8 below this puts linear
# ground speed at roughly 1 m/s, slow enough for the eat-distance
# check (0.6 m radius, NEST-cadence TF) to actually fire as the
# husky passes through.
VOLTAGE_SCALE = 0.03
