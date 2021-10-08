from nrp_core import *
from nrp_core.data.nrp_protobuf import *

@EngineDataPack(keyword='input_datapack', id=DataPackIdentifier('test_datapack', '${engine_name_lowercase}_engine'))
@TransceiverFunction("${engine_name_lowercase}_engine")
def transceiver_function(input_datapack):
    print("TF input data: " + str(input_datapack.data.integer))

    output_datapack = EngineTestTestPayloadDataPack("test_datapack", "${engine_name_lowercase}_engine")
    output_datapack.data.str = "test"

    return [output_datapack]

