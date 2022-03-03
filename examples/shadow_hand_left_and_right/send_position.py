from typing import List
from nrp_core import *
from nrp_core.data.nrp_protobuf import GazeboJointDataPack

import ranges


shadow_hand_joints = {
    "shadow_hand_right" : ["rh_FFJ0", "rh_FFJ1", "rh_FFJ2", "rh_FFJ3"],
    "shadow_hand_left"  : ["lh_FFJ0", "lh_FFJ1", "lh_FFJ2", "lh_FFJ3"]
}


def prepare_joint_datapacks(tvb_datapack, hand_model: str) -> List:
    joint_datapacks = []

    joints = shadow_hand_joints[hand_model]

    for i in range(len(joints)):
        joint_datapack = GazeboJointDataPack(hand_model + "::" + joints[i], "gazebo")
        joint_datapack.data.position = ranges.convert_to_joint_range(tvb_datapack.data["positions"][i])
        joint_datapacks.append(joint_datapack)

    return joint_datapacks


@EngineDataPack(keyword='left_index_finger_target',  id=DataPackIdentifier('left_index_finger_target', 'tvb'))
@EngineDataPack(keyword='right_index_finger_target', id=DataPackIdentifier('right_index_finger_target', 'tvb'))
@TransceiverFunction("gazebo")
def transceiver_function(left_index_finger_target, right_index_finger_target):

    if right_index_finger_target.isEmpty() or left_index_finger_target.isEmpty():
        return []

    return prepare_joint_datapacks(left_index_finger_target, "shadow_hand_left") + \
           prepare_joint_datapacks(right_index_finger_target, "shadow_hand_right")

# EOF
