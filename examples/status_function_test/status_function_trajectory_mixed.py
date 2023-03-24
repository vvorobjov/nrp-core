from nrp_core import *
from nrp_core.data.nrp_protobuf import EngineTestTestPayloadRawData
from nrp_core.data.nrp_json import JsonRawData


@SimulationIteration(keyword="sim_iter")
@StatusFunction()
def status_function(sim_iter):
    done_flag = (sim_iter == 3)

    json_data = JsonRawData()
    json_data.data["int"] = sim_iter

    proto_data = EngineTestTestPayloadRawData()
    proto_data.data.integer = sim_iter

    return done_flag, [json_data, proto_data]

# EOF
