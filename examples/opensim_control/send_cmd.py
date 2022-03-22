from nrp_core import *
from nrp_core.data.nrp_json import *


@EngineDataPack(keyword='action', id=DataPackIdentifier('action', 'client'))
@TransceiverFunction("server")
def transceiver_function(action):
    rec_cmd = JsonDataPack("control_cmd", "server")
    rec_cmd.data["act_list"] = action.data["act_list"]
    rec_cmd.data["reset"] = action.data["reset"]

    return [rec_cmd]
