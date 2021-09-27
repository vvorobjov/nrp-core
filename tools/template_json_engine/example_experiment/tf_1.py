from nrp_core import *
from nrp_core.data.nrp_json import *

@FromEngineDataPack(keyword='input_datapack', id=DataPackIdentifier('test_datapack', '${engine_name_lowercase}_engine'))
@TransceiverFunction("${engine_name_lowercase}_engine")
def transceiver_function(input_datapack):
    print("TF input data: " + str(input_datapack.data["test_int"]))

    output_datapack = JsonDataPack("test_datapack", "${engine_name_lowercase}_engine")
    output_datapack.data["test_string"] = "test"

    return [output_datapack]

