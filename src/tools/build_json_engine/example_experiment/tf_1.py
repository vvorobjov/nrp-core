from nrp_core import *
from nrp_core.data.nrp_json import *

@EngineDataPack(keyword='input_datapack', id=DataPackIdentifier('test_datapack', 'build_engine'))
@TransceiverFunction("build_engine")
def transceiver_function(input_datapack):
    print("TF input data: " + str(input_datapack.data["test_int"]))

    output_datapack = JsonDataPack("test_datapack", "build_engine")
    output_datapack.data["test_string"] = "test"

    return [output_datapack]

