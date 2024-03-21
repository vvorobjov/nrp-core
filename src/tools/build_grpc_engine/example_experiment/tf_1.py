from nrp_core import *
from nrp_core.data.nrp_protobuf import *

@EngineDataPack(keyword='input_datapack', id=DataPackIdentifier('test_datapack', 'build_engine'))
@TransceiverFunction("build_engine")
def transceiver_function(input_datapack):
    print("TF input data: " + str(input_datapack.data.integer))

    output_datapack = EngineTestTestPayloadDataPack("test_datapack", "build_engine")
    output_datapack.data.str = "test"

    return [output_datapack]

