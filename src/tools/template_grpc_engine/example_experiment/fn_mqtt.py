from nrp_core.data.nrp_protobuf import EngineTestTestPayload
from nrp_core.event_loop import *


iteration = 0

@MQTTSubscriber(keyword='mqtt_in', address="${engine_name_lowercase}_engine/test_datapack/get", type=EngineTestTestPayload)
@MQTTPublisher(keyword="mqtt_out", address="${engine_name_lowercase}_engine/test_datapack/set", type=EngineTestTestPayload)
@FunctionalNode(name="mqtt_proto", outputs=["mqtt_out"])
def transceiver_function_in(mqtt_in):
    if mqtt_in is not None:
        print('Message From Engine: {}'.format(mqtt_in.integer))

    global iteration
    out = EngineTestTestPayload()
    out.integer = iteration
    iteration += 1
    return [out]
