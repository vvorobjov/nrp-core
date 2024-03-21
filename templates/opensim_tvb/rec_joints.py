from nrp_core import *
from nrp_core.data.nrp_json import *

import ranges

@EngineDataPack(keyword='joints', id=DataPackIdentifier('joints', 'opensim'))
@TransceiverFunction("tvb")
def transceiver_function(joints):
    rec_joints = JsonDataPack("joint_data", "tvb")
    rec_joints.data["shoulder"] = joints.data["shoulder"]
    rec_joints.data["elbow"] = ranges.opensim_displacement_to_tvb_voltage(joints.data["elbow"])

    return [rec_joints]
