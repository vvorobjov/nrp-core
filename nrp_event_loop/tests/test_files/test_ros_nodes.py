from nrp_core.event_loop import *
from nrp_core.data.nrp_ros.nrp_ros_msgs import Test


@RosSubscriber(keyword="input1", address="/test_sub/test", type=Test)
@RosPublisher(keyword="output1", address="/test_pub/test", type=Test)
@FunctionalNode(name='function_wrong', outputs=["output1"])
def f_node_connected(input1):
    return [input1]
