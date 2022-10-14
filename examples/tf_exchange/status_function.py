from nrp_core import *
from nrp_core.data.nrp_json import SimulationStatus, JsonDataPack

counter = 0

@EngineDataPack(keyword='datapack_engine', id=DataPackIdentifier('datapack1', 'python_1'))
@StatusFunction()
def transceiver_function(datapack_engine, client_data):
    global counter
    status = SimulationStatus()

    counter_mod = counter % 3
    if counter_mod == 0:
        status["integers"] = [0, 1, 2]
    elif counter_mod == 1:
        status["flags"] = [True, False]
    else:
        # Return an empty status
        pass

    counter += 1

    datapack = JsonDataPack("actions", "python_1")
    datapack.data["test"] = 5
    return status, [datapack]

# EOF
