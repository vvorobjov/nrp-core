from nrp_core import *
from nrp_core.data.nrp_json import *

car_speed = 5


@EngineDataPack(keyword='positions', id=DataPackIdentifier('positions', 'bullet_simulator'))
@TransceiverFunction("bullet_simulator")
def transceiver_function(positions):
    rec_cmd = JsonDataPack("control_cmd", "bullet_simulator")
    car_pos = positions.data["body"]
    # Change direction
    global car_speed
    if car_pos[0] > 0.5:
        car_speed = -5
    elif car_pos[0] < -0.5:
        car_speed = 5
    rec_cmd.data["act_list"] = [car_speed]

    return [rec_cmd]
