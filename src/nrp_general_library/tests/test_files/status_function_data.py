from nrp_core import *
from nrp_core.data.nrp_json import JsonRawData

iteration = 0

@StatusFunction()
def status_function():
    """
    Status Function used for automated testing.
    Based on the iteration number it will return:
    - 0 observation objects if iter_num isn't 1 or 2
    - 1 observation object if iter_num is 1
    - 2 observation objects if iter_num is 2
    """
    global iteration
    observations = []

    if iteration == 1:
        observations.append(JsonRawData())
    elif iteration == 2:
        observations.append(JsonRawData())
        observations.append(JsonRawData())

    iteration += 1

    return True, observations

# EOF
