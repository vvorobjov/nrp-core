from nrp_core import *
from nrp_core.data.nrp_protobuf import GazeboJointDataPack
from nrp_core.event_loop import *

@ToEngine(keyword="front_right_j", address="/gazebo/husky::front_right_joint")
@ToEngine(keyword="front_left_j", address="/gazebo/husky::front_left_joint")
@ToEngine(keyword="back_right_j", address="/gazebo/husky::back_right_joint")
@ToEngine(keyword="back_left_j", address="/gazebo/husky::back_left_joint")
@FromSpinnaker(keyword="right_voltage_msg", address="right_wheel_motor_voltage")
@FromSpinnaker(keyword="left_voltage_msg", address="left_wheel_motor_voltage")
@FunctionalNode(name="gazebo", outputs=['back_left_j', 'back_right_j', 'front_left_j', 'front_right_j'])
def transceiver_function(left_voltage_msg, right_voltage_msg):

    back_left_j   = GazeboJointDataPack("husky::back_left_joint", "gazebo")
    back_right_j  = GazeboJointDataPack("husky::back_right_joint", "gazebo")
    front_left_j  = GazeboJointDataPack("husky::front_left_joint", "gazebo")
    front_right_j = GazeboJointDataPack("husky::front_right_joint", "gazebo")

    if left_voltage_msg is None or right_voltage_msg is None:
        [ back_left_j, back_right_j, front_left_j, front_right_j ]

    left_voltage = left_voltage_msg["voltages"][0]["voltage"]
    right_voltage = right_voltage_msg["voltages"][0]["voltage"]

    forward_vel = 20.0 * min(left_voltage, right_voltage)
    rot_vel = 70.0 * (right_voltage - left_voltage)

    back_left_j.data.velocity = (forward_vel - rot_vel) * 10
    back_right_j.data.velocity = (forward_vel + rot_vel) * 10
    front_left_j.data.velocity = (forward_vel - rot_vel) * 10
    front_right_j.data.velocity = (forward_vel + rot_vel) * 10

    # print("------------------")
    # print("Left voltage:  " + str(left_voltage))
    # print("Right voltage: " + str(right_voltage))
    # print("Forward velocity: " + str(forward_vel))
    # print("Rotational vel:   " + str(rot_vel))

    return [ back_left_j, back_right_j, front_left_j, front_right_j ]

