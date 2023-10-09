from nrp_core import *
from nrp_core.data.nrp_protobuf import *

@EngineDataPack(keyword='action_1', id=DataPackIdentifier('action_1', 'ppo_model'))
@TransceiverFunction("sim_engine_1")
def transceiver_function(action_1):
    rec_cmd = NrpGenericProtoArrayInt32DataPack("action_1", "sim_engine_1")
    rec_cmd.data.array[:] = action_1.data.array[:]
    return [rec_cmd]
