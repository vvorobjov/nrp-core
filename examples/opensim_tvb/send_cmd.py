from nrp_core import *
from nrp_core.data.nrp_json import *

import ranges

@EngineDataPack(keyword='action', id=DataPackIdentifier('action', 'tvb'))
@TransceiverFunction("opensim")
def transceiver_function(action):
    rec_cmd = JsonDataPack("control_cmd", "opensim")
    rec_cmd.data["act_list"] = [ranges.tvb_voltage_to_opensim_action(x) for x in action.data["act_list"]]
    rec_cmd.data["reset"] = action.data["reset"]

    return [ rec_cmd ]
