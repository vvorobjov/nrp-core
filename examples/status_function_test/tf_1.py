from nrp_core import *
from nrp_core.data.nrp_json import *


@EngineDataPack(keyword='datapack_engine1', id=DataPackIdentifier('datapack_engine1', 'python_1'))
@EngineDataPack(keyword='datapack_engine2', id=DataPackIdentifier('datapack_engine2', 'python_2'))
@TransceiverFunction("python_2")
def transceiver_function(datapack_engine1, datapack_engine2):
    return []
