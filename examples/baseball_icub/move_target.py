from nrp_core import *
from nrp_core.data.nrp_protobuf import GazeboModelDataPack


@EngineDataPack(keyword='target', id=DataPackIdentifier('Target', 'gazebo'))
@TransceiverFunction("gazebo")
def robot_control(target):

    if target.data.position[2] < 0.1:
        target_ret = GazeboModelDataPack("Target", "gazebo")
        target_ret.data.position.append(3.)
        target_ret.data.position.append(-0.28)
        target_ret.data.position.append(0.7)
        target_ret.data.linearVelocity.append(-8.97)
        target_ret.data.linearVelocity.append(0.0)
        target_ret.data.linearVelocity.append(1.84)
        return [target_ret]
    else:
        return []
