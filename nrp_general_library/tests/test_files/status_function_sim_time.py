from nrp_core import *
from nrp_core.data.nrp_json import JsonRawData

@StatusFunction()
def status_function():
    """
    Status Function used for automated testing.
    It should return simulation time and simulation iteration number
    that are currently available in the function manager.
    """
    data = JsonRawData()
    data.data["sim_time"] = getSimulationTime()
    data.data["sim_iter"] = getSimulationIteration()

    return True, [data]

# EOF
