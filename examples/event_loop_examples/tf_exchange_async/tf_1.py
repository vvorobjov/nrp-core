from nrp_core.event_loop import *
from nrp_core.data.nrp_json import JsonDataPack


@MQTTSubscriber(keyword='datapack_python', address="python_1/get/datapack1", type=JsonDataPack)
@MQTTPublisher(keyword="datapack_out", address="python_2/set/rec_datapack2", type=JsonDataPack)
@FunctionalNode(name="transfer_fn", outputs=["datapack_out"])
def transceiver_function(datapack_python):
    rec_datapack1 = JsonDataPack("rec_datapack2", "python_2")
    for k in datapack_python.data.keys():
        rec_datapack1.data[k] = datapack_python.data[k]

    return [rec_datapack1]
