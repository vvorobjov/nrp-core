from nrp_core import *
from nrp_core.data.nrp_json import *


@EngineDataPack(keyword='joints', id=DataPackIdentifier('joints', 'server'))
@EngineDataPack(keyword='infos', id=DataPackIdentifier('infos', 'server'))
@TransceiverFunction("client")
def transceiver_function(infos, joints):
    rec_joints = JsonDataPack("joint_data", "client")
    rec_joints.data["shoulder"] = joints.data["shoulder"]
    rec_joints.data["elbow"] = joints.data["elbow"]

    return [rec_joints]
