from NRPPythonModule import *
from NRPJSONEngineProtocolPython import *

@FromEngineDevice(keyword='input_device', id=DeviceIdentifier('test_device', '${engine_name_lowercase}_engine'))
@TransceiverFunction("${engine_name_lowercase}_engine")
def transceiver_function(input_device):
    print("TF input data: " + str(input_device.data["test_int"]))

    output_device = JsonDevice("test_device", "${engine_name_lowercase}_engine")
    output_device.data["test_string"] = "test"

    return [output_device]

