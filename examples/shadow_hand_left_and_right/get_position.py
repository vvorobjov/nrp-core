from nrp_core import *
from nrp_core.data.nrp_protobuf import *
from nrp_core.data.nrp_json import *

import ranges


@EngineDataPack(keyword='rh_FFJ0', id=DataPackIdentifier('shadow_hand_right::rh_FFJ0', 'gazebo'))
@EngineDataPack(keyword='rh_FFJ1', id=DataPackIdentifier('shadow_hand_right::rh_FFJ1', 'gazebo'))
@EngineDataPack(keyword='rh_FFJ2', id=DataPackIdentifier('shadow_hand_right::rh_FFJ2', 'gazebo'))
@EngineDataPack(keyword='rh_FFJ3', id=DataPackIdentifier('shadow_hand_right::rh_FFJ3', 'gazebo'))
@EngineDataPack(keyword='lh_FFJ0', id=DataPackIdentifier('shadow_hand_left::lh_FFJ0', 'gazebo'))
@EngineDataPack(keyword='lh_FFJ1', id=DataPackIdentifier('shadow_hand_left::lh_FFJ1', 'gazebo'))
@EngineDataPack(keyword='lh_FFJ2', id=DataPackIdentifier('shadow_hand_left::lh_FFJ2', 'gazebo'))
@EngineDataPack(keyword='lh_FFJ3', id=DataPackIdentifier('shadow_hand_left::lh_FFJ3', 'gazebo'))
@TransceiverFunction("tvb")
def transceiver_function(rh_FFJ0, rh_FFJ1, rh_FFJ2, rh_FFJ3, lh_FFJ0, lh_FFJ1, lh_FFJ2, lh_FFJ3):
    left_index_finger  = JsonDataPack("left_index_finger_joints",  "tvb")
    right_index_finger = JsonDataPack("right_index_finger_joints", "tvb")

    left_index_finger.data["positions"] = [ranges.convert_to_tvb_range(lh_FFJ0.data.position),
                                           ranges.convert_to_tvb_range(lh_FFJ1.data.position),
                                           ranges.convert_to_tvb_range(lh_FFJ2.data.position),
                                           ranges.convert_to_tvb_range(lh_FFJ3.data.position)]

    # Velocity coming from the hand is currently not used
    # It's calculated in the TVB engine instead

    left_index_finger.data["velocities"] = [lh_FFJ0.data.velocity,
                                            lh_FFJ1.data.velocity,
                                            lh_FFJ2.data.velocity,
                                            lh_FFJ3.data.velocity]

    right_index_finger.data["positions"] = [ranges.convert_to_tvb_range(rh_FFJ0.data.position),
                                            ranges.convert_to_tvb_range(rh_FFJ1.data.position),
                                            ranges.convert_to_tvb_range(rh_FFJ2.data.position),
                                            ranges.convert_to_tvb_range(rh_FFJ3.data.position)]

    # Velocity coming from the hand is currently not used
    # It's calculated in the TVB engine instead

    right_index_finger.data["velocities"] = [rh_FFJ0.data.velocity,
                                             rh_FFJ1.data.velocity,
                                             rh_FFJ2.data.velocity,
                                             rh_FFJ3.data.velocity]

    return [ left_index_finger, right_index_finger ]

# EOF
