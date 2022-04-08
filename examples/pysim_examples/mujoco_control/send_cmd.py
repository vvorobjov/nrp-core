from nrp_core import *
from nrp_core.data.nrp_json import *

@EngineDataPack(keyword='action', id=DataPackIdentifier('action', 'controller'))
@TransceiverFunction("simple_mujoco")
def transceiver_function(action):
    rec_cmd = JsonDataPack("control_cmd", "simple_mujoco")
    rec_cmd.data["act_list"] = action.data["act_list"]
    
    return [ rec_cmd ]