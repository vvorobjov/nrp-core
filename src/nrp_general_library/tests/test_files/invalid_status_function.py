from nrp_core import *
from nrp_core.data.nrp_json import SimulationStatus


@StatusFunction()
def transceiver_function():
    status = SimulationStatus()
    sdfsldldl
    status["test_int"] = 456
    status["test_flags"] = [True, False]
    return status

# EOF
