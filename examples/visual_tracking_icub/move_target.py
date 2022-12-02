from nrp_core import *
from nrp_core.data.nrp_protobuf import GazeboModelDataPack
from math import sin, pi

frequency = 0.3
amplitude = 0.3
center = {'x': 0, 'y': 2.42, 'z': 1.2}
t = 0


@TransceiverFunction("gazebo")
def robot_control():
    global t

    target = GazeboModelDataPack("Target", "gazebo")
    target.data.position.append(center['x'] + sin(t * frequency * 2 * pi) * (amplitude / 2))
    target.data.position.append(center['y'])
    target.data.position.append(center['z'])

    t += 0.01

    return [target]
