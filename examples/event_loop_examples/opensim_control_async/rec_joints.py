from nrp_core.event_loop import *
from nrp_core.data.nrp_json import JsonDataPack


@MQTTSubscriber(keyword='joints', address="server/get/joints", type=JsonDataPack)
@MQTTSubscriber(keyword='infos', address="server/get/infos", type=JsonDataPack)
@MQTTPublisher(keyword="datapack_out", address="client/set/joint_data", type=JsonDataPack)
@FunctionalNode(name="rec_joints", outputs=["datapack_out"])
def transceiver_function(infos, joints):
    rec_joints = JsonDataPack("joint_data", "client")
    rec_joints.data["shoulder"] = joints.data["shoulder"]
    rec_joints.data["elbow"] = joints.data["elbow"]

    return [rec_joints]
