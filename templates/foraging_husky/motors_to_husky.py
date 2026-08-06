"""TransceiverFunction: motor leaky-integrator V_m → husky wheel velocities.

brain.py wires each motor population (MOTOR_L / MOTOR_R) into a
LeakyIntegratorAlpha cell registered as ``motor_l_voltage`` /
``motor_r_voltage``. The cell's V_m tracks the population's spike
rate; ``helpers.voltage_to_wheel_vels`` converts the two voltages
into a 4-tuple of husky wheel angular velocities using the
Braitenberg-2b convention.

Tracked in EBR2-32.
"""

from nrp_core import (
    EngineDataPack,
    TransceiverFunction,
    DataPackIdentifier,
)
from nrp_core.data.nrp_protobuf import GazeboJointDataPack

import params as P
from helpers import voltage_to_wheel_vels


# Module-level tick counter; see camera_to_sensory.py for the rationale.
_tick_count = [0]


@EngineDataPack(keyword='motor_l',
                id=DataPackIdentifier('motor_l_voltage', 'nest'))
@EngineDataPack(keyword='motor_r',
                id=DataPackIdentifier('motor_r_voltage', 'nest'))
@TransceiverFunction("gazebo")
def motors_to_husky(motor_l, motor_r):
    # bd.LeakyIntegratorAlpha registers a 1-cell datapack whose data
    # dict carries 'V_m' at index 0. The husky_braitenberg/mot_tf.py
    # uses the same access pattern (.data[0]['V_m']).
    l_v = motor_l.data[0]['V_m'] if not motor_l.isEmpty() else 0.0
    r_v = motor_r.data[0]['V_m'] if not motor_r.isEmpty() else 0.0

    bl, br, fl, fr = voltage_to_wheel_vels(l_v, r_v)

    back_left_j = GazeboJointDataPack("husky::back_left_joint", "gazebo")
    back_right_j = GazeboJointDataPack("husky::back_right_joint", "gazebo")
    front_left_j = GazeboJointDataPack("husky::front_left_joint", "gazebo")
    front_right_j = GazeboJointDataPack("husky::front_right_joint", "gazebo")

    back_left_j.data.velocity = bl
    back_right_j.data.velocity = br
    front_left_j.data.velocity = fl
    front_right_j.data.velocity = fr

    _tick_count[0] += 1
    if _tick_count[0] % P.DIAG_LOG_EVERY_N_TICKS == 0:
        print(
            f"FORAGING_DIAG motor tick={_tick_count[0]} "
            f"v_l={l_v:.3f} v_r={r_v:.3f} "
            f"wheels=({bl:.2f},{br:.2f},{fl:.2f},{fr:.2f})",
            flush=True,
        )

    return [back_left_j, back_right_j, front_left_j, front_right_j]
