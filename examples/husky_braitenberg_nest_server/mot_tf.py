from NRPPythonModule import *
from NRPGazeboDevicesPython import PhysicsJoint

@SingleTransceiverDevice(keyword='actors', id=DeviceIdentifier('actors', 'nest'))
@TransceiverFunction("gazebo")
def transceiver_function(actors):
    back_left_j   = PhysicsJoint("husky::back_left_joint", "gazebo")
    back_right_j  = PhysicsJoint("husky::back_right_joint", "gazebo")
    front_left_j  = PhysicsJoint("husky::front_left_joint", "gazebo")
    front_right_j = PhysicsJoint("husky::front_right_joint", "gazebo")

    print(actors.data)
    left_voltage = actors.data[0]['V_m']
    right_voltage = actors.data[1]['V_m']
    
    forward_vel = 20.0 * min(left_voltage, right_voltage)
    rot_vel = 70.0 * (right_voltage - left_voltage)

    back_left_j.velocity = (forward_vel - rot_vel) * 10
    back_right_j.velocity = (forward_vel + rot_vel) * 10
    front_left_j.velocity = (forward_vel - rot_vel) * 10
    front_right_j.velocity = (forward_vel + rot_vel) * 10

    print("Left voltage:  " + str(left_voltage))
    print("Right voltage: " + str(right_voltage))

    print("Forward velocity: " + str(forward_vel))
    print("Rotational vel:   " + str(rot_vel))
    # return []
    return [ back_left_j, back_right_j, front_left_j, front_right_j ]

