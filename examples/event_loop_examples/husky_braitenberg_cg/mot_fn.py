from nrp_core import *
from nrp_core.data.nrp_protobuf import GazeboJointDataPack
from nrp_core.event_loop import *


@ToEngine(keyword="front_right_j", address="/gazebo")
@ToEngine(keyword="front_left_j", address="/gazebo")
@ToEngine(keyword="back_right_j", address="/gazebo")
@ToEngine(keyword="back_left_j", address="/gazebo")
@FromEngine(keyword='actors', address='/nest/actors')
@FunctionalNode(name="robot_control", outputs=['back_left_j', 'back_right_j', 'front_left_j', 'front_right_j'])
def robot_control(actors):
    back_left_j = GazeboJointDataPack("husky::back_left_joint", "gazebo")
    back_right_j = GazeboJointDataPack("husky::back_right_joint", "gazebo")
    front_left_j = GazeboJointDataPack("husky::front_left_joint", "gazebo")
    front_right_j = GazeboJointDataPack("husky::front_right_joint", "gazebo")

    left_voltage = actors.data[0]['V_m']
    right_voltage = actors.data[1]['V_m']

    forward_vel = 20.0 * min(left_voltage, right_voltage)
    rot_vel = 100.0 * (right_voltage - left_voltage)

    back_left_j.data.velocity = (forward_vel - rot_vel * 0.275) * 7
    back_right_j.data.velocity = (forward_vel + rot_vel * 0.275) * 7
    front_left_j.data.velocity = (forward_vel - rot_vel * 0.275) * 7
    front_right_j.data.velocity = (forward_vel + rot_vel * 0.275) * 7

    # print("------------------")
    # print(f"Left voltage:  {left_voltage}")
    # print(f"Right voltage: {right_voltage}")
    # print(f"Forward velocity: {forward_vel}")
    # print(f"Rotational vel:   {rot_vel}")

    return [back_left_j, back_right_j, front_left_j, front_right_j]

