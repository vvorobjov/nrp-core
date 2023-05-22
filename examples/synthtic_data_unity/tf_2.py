import random

from nrp_core import *
from nrp_core.data.nrp_json import *
from nrp_core.data.nrp_protobuf import *

@EngineDataPack(keyword='input_frame', id=DataPackIdentifier("image", "unity"))
@TransceiverFunction("python_dumper")
def transceiver_function(input_frame):

    myObject = UnityProtoBImageDataPack("input_image", "python_dumper")
    myObject.data.image = input_frame.data.image
    return [myObject]

