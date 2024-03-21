from nrp_core.event_loop import *
from  nrp_core.data.nrp_ros.std_msgs import Bool

fn = createFNFromFactoryModule(module_name="libFNFactoryModule.so", function_name="my_function", node_name="my_fn", exec_policy=node_policies.functional_node.exec_policy.on_new_message)
RosSubscriber(keyword="i1", address="/test_sub", type=Bool)(fn)
RosPublisher(keyword="o1", address="/test_pub", type=Bool)(fn)