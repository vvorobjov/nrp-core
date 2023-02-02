from nrp_core import *
from nrp_core.data.nrp_protobuf import DumpStringRawData, EngineTestTestPayloadRawData

import json


@StatusFunction()
def status_function():
    iteration = getSimulationIteration()

    done_flag = (iteration == 3)

    proto1 = DumpStringRawData()
    proto1.data.string_stream = json.dumps({"int": iteration})

    proto2 = EngineTestTestPayloadRawData()
    proto2.data.integer = iteration

    return done_flag, [proto1, proto2]

# EOF
