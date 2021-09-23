from NRPPythonModule import *
from NRPProtoPythonModule import *

@FromEngineDataPack(keyword='input_datapack', id=DataPackIdentifier('test_datapack', '${engine_name_lowercase}_engine'))
@TransceiverFunction("${engine_name_lowercase}_engine")
def transceiver_function(input_datapack):
    print("TF input data: " + str(input_datapack.data.integer))

    output_datapack = TestPayload("test_datapack", "${engine_name_lowercase}_engine")
    output_datapack.data.str = "test"

    return [output_datapack]

