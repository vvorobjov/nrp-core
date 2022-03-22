from nrp_core.data.nrp_json import NlohmannJson
from nrp_core.data.nrp_protobuf import GazeboJoint
from nrp_core.event_loop import *


@MQTTSubscriber(keyword='mqtt_in', address="test_topic", type=str)
@MQTTPublisher(keyword="mqtt_out", address="test_topic", type=str)
@FunctionalNode(name="mqtt_str", outputs=["mqtt_out"], exec_policy=node_policies.functional_node.exec_policy.always)
def transceiver_function_in(mqtt_in):
    if mqtt_in is not None:
        print('Message from mqtt str: {}'.format(mqtt_in))

    return ["Hi there!"]


@MQTTSubscriber(keyword='mqtt_in', address="test_topic_proto", type=GazeboJoint)
@MQTTPublisher(keyword="mqtt_out", address="test_topic_proto", type=GazeboJoint)
@FunctionalNode(name="mqtt_proto", outputs=["mqtt_out"], exec_policy=node_policies.functional_node.exec_policy.always)
def transceiver_function_in(mqtt_in):
    if mqtt_in is not None:
        print('Message from mqtt proto: {}'.format(mqtt_in))

    out = GazeboJoint()
    out.position = 10
    return [out]


@MQTTSubscriber(keyword='mqtt_in', address="test_topic_json", type=NlohmannJson)
@MQTTPublisher(keyword="mqtt_out", address="test_topic_json", type=str)
@FunctionalNode(name="mqtt_json", outputs=["mqtt_out"], exec_policy=node_policies.functional_node.exec_policy.always)
def transceiver_function_in(mqtt_in):
    if mqtt_in is not None:
        print('Message from mqtt json: {}'.format(mqtt_in))

    return ['{"hi": "there"}']
