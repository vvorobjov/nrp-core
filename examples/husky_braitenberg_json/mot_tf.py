from nrp_core import *

@EngineDataPack(keyword='actors', id=DataPackIdentifier('actors', 'nest'))
@TransceiverFunction("gazebo")
def transceiver_function(actors):
    back_left_j   = JsonDataPack("husky::back_left_joint", "gazebo")
    back_right_j  = JsonDataPack("husky::back_right_joint", "gazebo")
    front_left_j  = JsonDataPack("husky::front_left_joint", "gazebo")
    front_right_j = JsonDataPack("husky::front_right_joint", "gazebo")

    left_voltage = actors.data[0]['V_m']
    right_voltage = actors.data[1]['V_m']

    forward_vel = 20.0 * min(left_voltage, right_voltage)
    rot_vel = 70.0 * (right_voltage - left_voltage)

    back_left_j.data["velocity"] = (forward_vel - rot_vel) * 10
    back_right_j.data["velocity"] = (forward_vel + rot_vel) * 10
    front_left_j.data["velocity"] = (forward_vel - rot_vel) * 10
    front_right_j.data["velocity"] = (forward_vel + rot_vel) * 10

    print("------------------")
    print("Left voltage:  " + str(back_left_j.data["velocity"]))
    print("Right voltage: " + str(back_right_j.data["velocity"]))
    print("Forward velocity: " + str(front_left_j.data["velocity"]))
    print("Rotational vel:   " + str(front_right_j.data["velocity"]))
    # return []
    return [ back_left_j, back_right_j, front_left_j, front_right_j ]

