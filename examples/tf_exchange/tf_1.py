from NRPPythonModule import *

@FromEngineDevice(keyword='device_python', id=DeviceIdentifier('device1', 'python_1'))
@TransceiverFunction("python_2")
def transceiver_function(device_python):
    rec_device1 = PythonDevice("rec_device2", "python_2")
    rec_device1.data = device_python.data

    return [ rec_device1 ]

