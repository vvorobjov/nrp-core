from NRPPythonModule import *
from NRPNestJSONPythonModule import NestDevice

@FromEngineDevice(keyword='voltage', id=DeviceIdentifier('voltage', 'nest'))
@TransceiverFunction("nest")
def transceiver_function(voltage):
  print(voltage.data[0]['events'])
  noise_device = NestDevice("noise", "nest")
  noise_device.data = {"rate": 80000.0, "rate": 15000.0}

  return [ noise_device ]