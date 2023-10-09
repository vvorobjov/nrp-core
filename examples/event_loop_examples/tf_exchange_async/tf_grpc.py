from nrp_core.event_loop import *
from nrp_core.data.nrp_protobuf import *


@MQTTSubscriber(keyword='datapack_python', address="python_1/get/datapack1", type=DumpStringDataPack)
@MQTTPublisher(keyword="datapack_out", address="python_2/set/rec_datapack2", type=DumpStringDataPack)
@FunctionalNode(name="transfer_fn", outputs=["datapack_out"])
def transceiver_function(datapack_python):
    rec_datapack2 = DumpStringDataPack("rec_datapack2", "python_2")
    rec_datapack2.data.string_stream = datapack_python.data.string_stream
    return [rec_datapack2]
