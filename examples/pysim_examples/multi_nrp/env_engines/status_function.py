from nrp_core import *
from nrp_core.data.nrp_protobuf import *

@EngineDataPack(keyword='data_buffer', id=DataPackIdentifier('data_buffer', 'ppo_model'))
@StatusFunction()
def status_function(data_buffer):
    return True, [data_buffer]

# EOF
