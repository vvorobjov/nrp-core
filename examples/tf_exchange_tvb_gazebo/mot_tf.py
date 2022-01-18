from nrp_core import *
from nrp_core.data.nrp_protobuf import GazeboJointDataPack

@EngineDataPack(keyword='actors', id=DataPackIdentifier('actors', 'tvb'))
@TransceiverFunction("gazebo")
def transceiver_function(actors):
    back_left_j   = GazeboJointDataPack("husky::back_left_joint", "gazebo")
    back_right_j  = GazeboJointDataPack("husky::back_right_joint", "gazebo")
    front_left_j  = GazeboJointDataPack("husky::front_left_joint", "gazebo")
    front_right_j = GazeboJointDataPack("husky::front_right_joint", "gazebo")

    print("actors",actors)
    
    if len(actors.data.keys()) == 0:
        back_left_j.data.velocity = 0
        back_right_j.data.velocity = 0
        front_left_j.data.velocity = 0
        front_right_j.data.velocity = 0
        return [ back_left_j, back_right_j, front_left_j, front_right_j ]
    #new 0.01
    left_voltage = actors.data["move"]["left"]*0.01
    right_voltage = actors.data["move"]["right"]*0.01
    #print("->left_voltage",left_voltage)
    #print("->right_voltage",right_voltage)
    forward_vel = 20.0 * min(left_voltage, right_voltage)
    rot_vel = 70.0 * (right_voltage - left_voltage)

    back_left_j.data.velocity = (forward_vel - rot_vel) * 10
    back_right_j.data.velocity = (forward_vel + rot_vel) * 10
    front_left_j.data.velocity = (forward_vel - rot_vel) * 10
    front_right_j.data.velocity = (forward_vel + rot_vel) * 10

    print("------------------")
    print("Left voltage:  " + str(back_left_j.data.velocity))
    print("Right voltage: " + str(back_right_j.data.velocity))
    print("Forward velocity: " + str(front_left_j.data.velocity))
    print("Rotational vel:   " + str(front_right_j.data.velocity))
    # return []
    return [ back_left_j, back_right_j, front_left_j, front_right_j ]
    
