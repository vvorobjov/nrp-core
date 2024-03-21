from nrp_core.event_loop import *
from nrp_core.data.nrp_protobuf import DumpString

fn = createFNFromFactoryModule(module_name="libFNFactoryModule.so", function_name="my_function", node_name="my_fn", exec_policy=node_policies.functional_node.exec_policy.on_new_message)
MQTTSubscriber(keyword='i1', address="test_topic_str", type=str)(fn)
MQTTPublisher(keyword="o1", address="test_topic_proto", type=DumpString)(fn)