import random

from nrp_core import *
from nrp_core.data.nrp_json import *
from nrp_core.data.nrp_protobuf import *

@EngineDataPack(keyword='image', id=DataPackIdentifier("image", "unity"))
@TransceiverFunction("python_dumper")
def transceiver_function(image):
    # print(f"Relaying datapack with data: '{image.data.data}' to python_dumper")
    image.engine_name = "python_dumper"

    # myObject = NrpGenericProtoImageDataPack("input_image", "python_dumper")
    # # print(input_frame.data.data)
    # myObject.data.data = input_frame.data.data
    return [image]

