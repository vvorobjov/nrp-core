from nrp_core import *
from nrp_core.data.nrp_protobuf import *

@EngineDataPack(keyword='observation', id=DataPackIdentifier('observation', 'grpc_taxi'))
@EngineDataPack(keyword='reward',      id=DataPackIdentifier('reward',      'grpc_taxi'))
@EngineDataPack(keyword='is_done',     id=DataPackIdentifier('is_done',     'grpc_taxi'))
@StatusFunction()
def status_function(observation, reward, is_done):
    return True, [observation, reward, is_done]

# EOF
