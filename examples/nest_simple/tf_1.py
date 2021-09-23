from NRPPythonModule import *
from NRPJSONEngineProtocolPython import *

@FromEngineDataPack(keyword='voltage', id=DataPackIdentifier('voltage', 'nest'))
@TransceiverFunction("nest")
def transceiver_function(voltage):
  print(voltage.data[0]["events"])
  noise_datapack = JsonDataPack("noise", "nest")
  noise_datapack.data["rate"] = 15000.0

  return [ noise_datapack ]