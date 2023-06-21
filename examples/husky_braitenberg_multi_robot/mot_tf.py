from nrp_core import *
from nrp_core.data.nrp_protobuf import GazeboJointDataPack


def robot_control(actors, model_name):
    back_left_j = GazeboJointDataPack(model_name+"::back_left_joint", "gazebo")
    back_right_j = GazeboJointDataPack(model_name+"::back_right_joint", "gazebo")
    front_left_j = GazeboJointDataPack(model_name+"::front_left_joint", "gazebo")
    front_right_j = GazeboJointDataPack(model_name+"::front_right_joint", "gazebo")

    left_voltage = actors.data[0]['V_m']
    right_voltage = actors.data[1]['V_m']

    forward_vel = 20.0 * min(left_voltage, right_voltage)
    rot_vel = 70.0 * (right_voltage - left_voltage)

    back_left_j.data.velocity = (forward_vel - rot_vel) * 10
    back_right_j.data.velocity = (forward_vel + rot_vel) * 10
    front_left_j.data.velocity = (forward_vel - rot_vel) * 10
    front_right_j.data.velocity = (forward_vel + rot_vel) * 10

    # print("------------------")
    # print(f"Left voltage:  {left_voltage}")
    # print(f"Right voltage: {right_voltage}")
    # print(f"Forward velocity: {forward_vel}")
    # print(f"Rotational vel:   {rot_vel}")

    return [back_left_j, back_right_j, front_left_j, front_right_j]


@EngineDataPack(keyword='actors1', id=DataPackIdentifier('actors', 'husky_brain_1'))
@EngineDataPack(keyword='actors2', id=DataPackIdentifier('actors', 'husky_brain_2'))
@TransceiverFunction("gazebo")
def tf(actors1, actors2):
    return robot_control(actors1, "husky_1") + robot_control(actors2, "husky_2")
