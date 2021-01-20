from NRPPythonModule import *
from NRPGazeboDevicesPython import PhysicsJoint


@SingleTransceiverDevice(keyword='lwn', id=DeviceIdentifier('lwn', 'nest'))
@SingleTransceiverDevice(keyword='rwn', id=DeviceIdentifier('rwn', 'nest'))
@SingleTransceiverDevice(keyword='lpg', id=DeviceIdentifier('lpg', 'nest'))
@SingleTransceiverDevice(keyword='rpg', id=DeviceIdentifier('rpg', 'nest'))
@SingleTransceiverDevice(keyword='gpg', id=DeviceIdentifier('gpg', 'nest'))
@TransceiverFunction("gazebo")
def transceiver_function(lwn, rwn, lpg, rpg, gpg):
    back_left_j   = PhysicsJoint("husky::back_left_joint", "gazebo")
    back_right_j  = PhysicsJoint("husky::back_right_joint", "gazebo")
    front_left_j  = PhysicsJoint("husky::front_left_joint", "gazebo")
    front_right_j = PhysicsJoint("husky::front_right_joint", "gazebo")
    
    left_voltage = lwn.data['V_m']
    right_voltage = rwn.data['V_m']
    
    forward_vel = 20.0 * min(left_voltage, right_voltage)
    rot_vel = 70.0 * (right_voltage - left_voltage)

    back_left_j.velocity = (forward_vel - rot_vel) * 10
    back_right_j.velocity = (forward_vel + rot_vel) * 10
    front_left_j.velocity = (forward_vel - rot_vel) * 10
    front_right_j.velocity = (forward_vel + rot_vel) * 10

    print("Left Red:  " + str(lpg.data))
    print("Right Red: " + str(rpg.data))
    print("Go On:     " + str(gpg.data))

    print("Left voltage:  " + str(left_voltage))
    print("Right voltage: " + str(right_voltage))

    print("Forward velocity: " + str(forward_vel))
    print("Rotational vel:   " + str(rot_vel))
    # return []
    return [ back_left_j, back_right_j, front_left_j, front_right_j ]

