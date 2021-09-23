from NRPPythonModule import *
from NRPProtoPythonModule import *

@FromEngineDevice(keyword='input_device', id=DeviceIdentifier('test_device', '${engine_name_lowercase}_engine'))
@TransceiverFunction("${engine_name_lowercase}_engine")
def transceiver_function(input_device):
    print("TF input data: " + str(input_device.data.integer))

    output_device = TestPayload("test_device", "${engine_name_lowercase}_engine")
    output_device.data.str = "test"

    return [output_device]

