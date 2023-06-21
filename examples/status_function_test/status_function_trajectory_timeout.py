from nrp_core import *
from nrp_core.data.nrp_json import JsonRawData


@SimulationIteration(keyword="sim_iter")
@StatusFunction()
def status_function(sim_iter):
    ret = JsonRawData()
    ret.data["iteration"] = sim_iter

    return False, [ret]

# EOF
