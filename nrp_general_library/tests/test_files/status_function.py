from nrp_core import *
from nrp_core.data.nrp_json import SimulationStatus


@EngineDataPack(keyword='datapack', id=DataPackIdentifier('tf_input', 'engine', 'type'))
@StatusFunction()
def transceiver_function(datapack):
    status = SimulationStatus()
    status["test_int"] = 456
    status["test_flags"] = [True, False]
    return status

# EOF
