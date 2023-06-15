from nrp_core.data.nrp_json import NlohmannJson, JsonDataPack
from nrp_core.data.nrp_protobuf import GazeboJoint, GazeboJointDataPack
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


@MQTTSubscriber(keyword='mqtt_in', address="test_topic_proto_dp", type=GazeboJointDataPack)
@MQTTPublisher(keyword="mqtt_out", address="test_topic_proto_dp", type=GazeboJointDataPack)
@FunctionalNode(name="mqtt_dp_proto", outputs=["mqtt_out"], exec_policy=node_policies.functional_node.exec_policy.always)
def transceiver_function_in(mqtt_in):
    if mqtt_in is not None:
        print('Message from mqtt proto datapack: {}'.format(mqtt_in))

    out = GazeboJointDataPack("a", "b")
    out.data.position = 10
    return [out]


@MQTTSubscriber(keyword='mqtt_in', address="test_topic_json", type=NlohmannJson)
@MQTTPublisher(keyword="mqtt_out", address="test_topic_json", type=str)
@FunctionalNode(name="mqtt_json", outputs=["mqtt_out"], exec_policy=node_policies.functional_node.exec_policy.always)
def transceiver_function_in(mqtt_in):
    if mqtt_in is not None:
        print('Message from mqtt json: {}'.format(mqtt_in))

    return ['{"hi": "there"}']


@MQTTSubscriber(keyword='mqtt_in', address="test_topic_json_dp", type=JsonDataPack)
@MQTTPublisher(keyword="mqtt_out", address="test_topic_json_dp", type=JsonDataPack)
@FunctionalNode(name="mqtt_dp_json", outputs=["mqtt_out"], exec_policy=node_policies.functional_node.exec_policy.always)
def transceiver_function_in(mqtt_in):
    if mqtt_in is not None:
        print('Message from mqtt json datapack: {}'.format(mqtt_in))

    m = JsonDataPack("dp_name", "engine_name")
    m.data["hi"] = "there"

    return [m]
