from nrp_core import *
from nrp_core.data.nrp_json import *
from nrp_core.data.nrp_protobuf import *
import json

@EngineDataPack(keyword='datapack_python', id=DataPackIdentifier('datapack1', 'python_1'))
@TransceiverFunction("python_2")
def transceiver_function(datapack_python):
    rec_datapack2 = DumpStringDataPack("rec_datapack2", "python_2")
    rec_datapack2.data.string_stream = datapack_python.data.string_stream
    return [rec_datapack2]
