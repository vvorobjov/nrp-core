from nrp_core import *
from nrp_core.data.nrp_json import JsonRawData

@SimulationTime("sim_time")
@SimulationIteration("sim_iter")
@StatusFunction()
def status_function(sim_time, sim_iter):
    """
    Status Function used for automated testing.
    It should return simulation time and simulation iteration number
    that are currently available in the function manager.
    """
    data = JsonRawData()
    data.data["sim_time"] = sim_time
    data.data["sim_iter"] = sim_iter

    return True, [data]

# EOF
