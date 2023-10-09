from nrp_core.event_loop import *
from nrp_core.data.nrp_json import JsonDataPack


@MQTTSubscriber(keyword='action', address="client/get/action", type=JsonDataPack)
@MQTTPublisher(keyword="datapack_out", address="server/set/control_cmd", type=JsonDataPack)
@FunctionalNode(name="send_cmd", outputs=["datapack_out"])
def transceiver_function(action):
    rec_cmd = JsonDataPack("control_cmd", "server")
    rec_cmd.data["act_list"] = action.data["act_list"]
    rec_cmd.data["reset"] = action.data["reset"]

    return [rec_cmd]
