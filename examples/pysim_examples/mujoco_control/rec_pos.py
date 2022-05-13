from nrp_core import *
from nrp_core.data.nrp_json import *

@EngineDataPack(keyword='positions', id=DataPackIdentifier('positions', 'simple_mujoco'))
@TransceiverFunction("controller")
def transceiver_function(positions):
    rec_pos = JsonDataPack("positions", "controller")
    rec_pos.data["end_postion"] = positions.data["end_postion"]

    return [rec_pos]