from nrp_core import *
from nrp_core.data.nrp_protobuf import GazeboJointDataPack

doInitialize = True

@EngineDataPack(keyword='motors', id=DataPackIdentifier('motors', 'nest'))
@TransceiverFunction("gazebo")
def robot_control(motors):
    r_shoulder = GazeboJointDataPack("iCub::r_shoulder_yaw", "gazebo")
    r_shoulder.data.position = 30. * motors.data[0]['V_m']

    global doInitialize
    if doInitialize:
        doInitialize = False
        r_elbow = GazeboJointDataPack("iCub::r_elbow", "gazebo")
        r_elbow.data.position = 2.
        eye_tilt = GazeboJointDataPack("iCub::eye_tilt", "gazebo")
        eye_tilt.data.position = -0.8
        return [r_shoulder, r_elbow, eye_tilt]
    else:
        return [r_shoulder]
