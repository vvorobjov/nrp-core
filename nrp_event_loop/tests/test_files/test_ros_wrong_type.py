from nrp_core.event_loop import *


class MyClass:
    prop = 0


@RosSubscriber(keyword="input1", address="/test_sub/test", type=MyClass)
@FunctionalNode(name='function', outputs=["output1"])
def f_node_connected(input1):
    return [input1]
