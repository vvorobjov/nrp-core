from nrp_core import *
from nrp_core.data.nrp_protobuf import *

@EngineDataPack(keyword='observation_0', id=DataPackIdentifier('observation_0', 'sim_engine_0'))
@EngineDataPack(keyword='observation_1', id=DataPackIdentifier('observation_1', 'sim_engine_1'))
@TransceiverFunction("ppo_model")
def transceiver_function(observation_0, observation_1):
    observation_list = [observation_0, observation_1]

    return_list = []
    sim_num = len(observation_list)
    for i in range(sim_num):
        datapack_name = "observation_"+str(i)
        rec_datapack = DumpStringDataPack(datapack_name, "ppo_model")
        rec_datapack.data.string_stream =  observation_list[i].data.string_stream
        return_list.append(rec_datapack)

    return return_list
