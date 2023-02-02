from nrp_core import *
from nrp_core.data.nrp_json import JsonRawData

iteration = 0

@StatusFunction()
def status_function():
    global iteration

    iteration += 1

    ret = JsonRawData()
    ret.data["iteration"] = iteration

    return False, [ret]

# EOF
