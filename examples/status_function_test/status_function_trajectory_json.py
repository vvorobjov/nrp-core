from nrp_core import *
from nrp_core.data.nrp_json import JsonRawData


@SimulationIteration(keyword="sim_iter")
@StatusFunction()
def status_function(sim_iter):
    done_flag = (sim_iter == 3)

    ret = JsonRawData()
    ret.data["iteration"] = sim_iter

    return done_flag, [ret]

# EOF
