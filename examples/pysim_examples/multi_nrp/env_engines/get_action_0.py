from nrp_core import *
from nrp_core.data.nrp_protobuf import *

@EngineDataPack(keyword='action_0', id=DataPackIdentifier('action_0', 'ppo_model'))
@TransceiverFunction("sim_engine_0")
def transceiver_function(action_0):
    rec_cmd = NrpGenericProtoArrayInt32DataPack("action_0", "sim_engine_0")
    rec_cmd.data.array[:] = action_0.data.array[:]
    return [rec_cmd]