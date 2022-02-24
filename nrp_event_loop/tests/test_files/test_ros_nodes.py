from nrp_core.event_loop import *
from nrp_core.data.nrp_ros.nrp_ros_msgs import Test


@RosSubscriber(keyword="input1", address="/test_sub/test", type=Test,
               publish_policy=node_policies.input_node.msg_publish.last,
               cache_policy=node_policies.input_node.msg_cache.keep)
@RosPublisher(keyword="output1", address="/test_pub/test", type=Test)
@FunctionalNode(name='function_last', outputs=["output1"], exec_policy=node_policies.functional_node.exec_policy.always)
def f_node(input1):
    return [input1]


msgs_all = []


@RosSubscriber(keyword="input1", address="/test_sub/test_all", type=Test,
               publish_policy=node_policies.input_node.msg_publish.all,
               cache_policy=node_policies.input_node.msg_cache.keep)
@FunctionalNode(name='function_all', outputs=[], exec_policy=node_policies.functional_node.exec_policy.always)
def f_node(input1):
    global msgs_all
    for m in input1:
        msgs_all.append(m)
    return []


msgs_clear = []


@RosSubscriber(keyword="input1", address="/test_sub/test_clear", type=Test,
               publish_policy=node_policies.input_node.msg_publish.last,
               cache_policy=node_policies.input_node.msg_cache.clear)
@FunctionalNode(name='function_clear', outputs=[], exec_policy=node_policies.functional_node.exec_policy.always)
def f_node(input1):
    global msgs_clear
    if input1:
        msgs_clear.append(input1)
    return []


@RosSubscriber(keyword="input1", address="/test_sub/test_clear_all", type=Test,
               publish_policy=node_policies.input_node.msg_publish.all,
               cache_policy=node_policies.input_node.msg_cache.clear)
@FunctionalNode(name='function_clear_all', outputs=[], exec_policy=node_policies.functional_node.exec_policy.always)
def f_node(input1):
    global msgs_clear
    if input1:
        msgs_clear.append(input1)

    return []
