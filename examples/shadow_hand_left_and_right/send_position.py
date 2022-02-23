from typing import List
from nrp_core import *
from nrp_core.data.nrp_protobuf import GazeboJointDataPack


shadow_hand_inital_joint_values={
    "shadow_hand_right" : ["rh_FFJ0", "rh_FFJ1", "rh_FFJ2", "rh_FFJ3"],
                           #{"rh_FFJ0":0,"rh_FFJ1":0,"rh_FFJ2":0,"rh_FFJ3":0,
                           #"rh_MFJ0":1.5707,"rh_MFJ1":1.5707,"rh_MFJ2":1.5707,"rh_MFJ3":0,
                           #"rh_RFJ0":1.5707,"rh_RFJ1":1.5707,"rh_RFJ2":1.5707,"rh_RFJ3":0,
                           #"rh_LFJ0":1.5707,"rh_LFJ1":1.5707,"rh_LFJ2":1.5707,"rh_LFJ3":0,"rh_LFJ4":0,
                           #"rh_THJ0":-1.5707,"rh_THJ1":-0.7,"rh_THJ2":0,"rh_THJ3":0,"rh_THJ4":-0.0053,
                           #"rh_WRJ0":0,"rh_WRJ1":0 },

    "shadow_hand_left" : ["lh_FFJ0", "lh_FFJ1", "lh_FFJ2", "lh_FFJ3"]
                           #{"lh_FFJ0":0,"lh_FFJ1":0,"lh_FFJ2":0,"lh_FFJ3":0,
                           #"lh_MFJ0":1.5707,"lh_MFJ1":1.5707,"lh_MFJ2":1.5707,"lh_MFJ3":0,
                           #"lh_RFJ0":1.5707,"lh_RFJ1":1.5707,"lh_RFJ2":1.5707,"lh_RFJ3":0,
                           #"lh_LFJ0":1.5707,"lh_LFJ1":1.5707,"lh_LFJ2":1.5707,"lh_LFJ3":0,"lh_LFJ4":0,
                           #"lh_THJ0":1.5707,"lh_THJ1":0.7,"lh_THJ2":0,"lh_THJ3":0,"lh_THJ4":-0.0053,
                           #"lh_WRJ0":0,"lh_WRJ1":0 }
}


def prepare_joint_datapacks(tvb_datapack, hand_model: str) -> List:
    joint_datapacks = []

    joints = shadow_hand_inital_joint_values[hand_model]

    for i in range(len(joints)):
        joint_datapack = GazeboJointDataPack(hand_model + "::" + joints[i], "gazebo")
        joint_datapack.data.position = tvb_datapack.data["positions"][i]
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
