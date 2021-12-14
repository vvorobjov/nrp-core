import random

from nrp_core import *
from nrp_core.data.nrp_json import *

@EngineDataPack(keyword='joint_angles', id=DataPackIdentifier('JointAngles', 'python'))
@TransceiverFunction("unity_grpc")
def transceiver_function(joint_angles):

    set_info_dev = SetInfo("SetInfo", "unity_grpc")

    set_info_dev.joint_angles[0] = joint_angles.data["joint_angles"][0]
    set_info_dev.joint_angles[1] = joint_angles.data["joint_angles"][1]
    set_info_dev.joint_angles[2] = joint_angles.data["joint_angles"][2]

    return [set_info_dev]

# EOF