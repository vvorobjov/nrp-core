from nrp_core import *
from nrp_core.data.nrp_protobuf import DumpStringRawData, EngineTestTestPayloadRawData

import json

@SimulationIteration(keyword="sim_iter")
@StatusFunction()
def status_function(sim_iter):
    done_flag = (sim_iter == 3)

    proto1 = DumpStringRawData()
    proto1.data.string_stream = json.dumps({"int": sim_iter})

    proto2 = EngineTestTestPayloadRawData()
    proto2.data.integer = sim_iter

    return done_flag, [proto1, proto2]

# EOF
