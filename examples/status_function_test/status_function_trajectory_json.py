from nrp_core import *
from nrp_core.data.nrp_json import JsonRawData


@StatusFunction()
def status_function():
    iteration = getSimulationIteration()

    done_flag = (iteration == 3)

    ret = JsonRawData()
    ret.data["iteration"] = iteration

    return done_flag, [ret]

# EOF
