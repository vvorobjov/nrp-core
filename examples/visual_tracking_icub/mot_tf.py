from nrp_core import *
from nrp_core.data.nrp_protobuf import GazeboJointDataPack
from math import pi


@EngineDataPack(keyword='actors', id=DataPackIdentifier('actors', 'nest'))
@EngineDataPack(keyword='left_eye', id=DataPackIdentifier('iCub::left_eye_pan', 'gazebo'))
@EngineDataPack(keyword='right_eye', id=DataPackIdentifier('iCub::right_eye_pan', 'gazebo'))
@TransceiverFunction("gazebo")
def robot_control(left_eye, right_eye, actors):

    def deg2rad(deg):
        return (deg / 360.) * (2. * pi)

    # Process input
    eye_version = (left_eye.data.position + right_eye.data.position) / 2.0
    eye_vergence = left_eye.data.position - right_eye.data.position
    left_voltage = actors.data[0]['V_m']
    right_voltage = actors.data[1]['V_m']
    d = right_voltage - left_voltage

    # Compute output
    max_mov = 1.0
    eye_version_ret = eye_version + deg2rad(-((d + 0.03) / 0.09 * 2 * max_mov - max_mov))

    left_eye_ret = GazeboJointDataPack("iCub::left_eye_pan", "gazebo")
    right_eye_ret = GazeboJointDataPack("iCub::right_eye_pan", "gazebo")

    left_eye_ret.data.position = eye_version_ret + (eye_vergence / 2.0)
    right_eye_ret.data.position = eye_version_ret - (eye_vergence / 2.0)

    return [left_eye_ret, right_eye_ret]
