from nrp_core import *
from nrp_core.data.nrp_protobuf import DumpArrayFloatDataPack
from nrp_core.data.nrp_protobuf import DumpStringDataPack

import numpy as np

@EngineDataPack(keyword='actors_dump', id=DataPackIdentifier('actors', 'nest'))
@TransceiverFunction("datatransfer_engine")
def transceiver_function(actors_dump):

    # Streaming formatted string
    string_datapack = DumpStringDataPack("test_datapack1", "datatransfer_engine")
    string_datapack.data.string_stream = f"{actors_dump.data[0]['V_m']:.5f},{actors_dump.data[1]['V_m']:.5f}"

    rand_data = list(np.random.rand(3, 2).flat)

    # Streaming 2D array
    float_datapack2d = DumpArrayFloatDataPack("test_datapack2", "datatransfer_engine")
    float_datapack2d.data.float_stream.extend(rand_data)
    float_datapack2d.data.dims.extend([3, 2])

    # Streamingthe same array as 1D
    float_datapack1d = DumpArrayFloatDataPack("test_datapack3", "datatransfer_engine")
    float_datapack1d.data.float_stream.extend(rand_data)

    return [float_datapack1d, float_datapack2d, string_datapack]
