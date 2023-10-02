import random

from nrp_core import *
from nrp_core.data.nrp_json import *
from nrp_core.data.nrp_protobuf import *

@EngineDataPack(keyword='image', id=DataPackIdentifier("image", "unity"))
@EngineDataPack(keyword='cameraSensorStrings', id=DataPackIdentifier("cameraSensorStrings", "unity"))
@EngineDataPack(keyword='cameraSensorFloats', id=DataPackIdentifier("cameraSensorFloats", "unity"))
@EngineDataPack(keyword='cameraSensorInts', id=DataPackIdentifier("cameraSensorInts", "unity"))
@EngineDataPack(keyword='cameraSensorBools', id=DataPackIdentifier("cameraSensorBools", "unity"))
@TransceiverFunction("python_dumper")
def transceiver_function(image,cameraSensorStrings,cameraSensorFloats,cameraSensorInts,cameraSensorBools):
    # print(f"Relaying datapack with data: '{image.data.data}' to python_dumper")
    image.engine_name = "python_dumper"
    cameraSensorStrings.engine_name = "python_dumper"
    cameraSensorFloats.engine_name = "python_dumper"
    cameraSensorInts.engine_name = "python_dumper"
    cameraSensorBools.engine_name = "python_dumper"
    # myObject = NrpGenericProtoImageDataPack("input_image", "python_dumper")
    # # print(input_frame.data.data)
    # myObject.data.data = input_frame.data.data
    return [image,cameraSensorStrings,cameraSensorFloats,cameraSensorInts,cameraSensorBools]

