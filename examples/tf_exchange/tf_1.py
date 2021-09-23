from NRPPythonModule import *
from NRPJSONEngineProtocolPython import *

@FromEngineDataPack(keyword='datapack_python', id=DataPackIdentifier('datapack1', 'python_1'))
@TransceiverFunction("python_2")
def transceiver_function(datapack_python):
    rec_datapack1 = JsonDataPack("rec_datapack2", "python_2")
    rec_datapack1.data["test"] = 1

    return [ rec_datapack1 ]

