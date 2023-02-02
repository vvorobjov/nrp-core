from nrp_core import *
from nrp_core.data.nrp_protobuf import EngineTestTestPayloadRawData
from nrp_core.data.nrp_json import JsonRawData


@StatusFunction()
def status_function():
    iteration = getSimulationIteration()

    done_flag = (iteration == 3)

    json_data = JsonRawData()
    json_data.data["int"] = iteration

    proto_data = EngineTestTestPayloadRawData()
    proto_data.data.integer = iteration

    return done_flag, [json_data, proto_data]

# EOF
