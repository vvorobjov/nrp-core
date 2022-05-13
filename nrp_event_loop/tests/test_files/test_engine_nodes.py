from nrp_core.event_loop import *


@FromEngine('input1', '/fake_engine/my_data_pack', cache_policy=node_policies.input_node.msg_cache.keep)
@ToEngine('output1', '/fake_engine/my_data_pack')
@ToEngine('output1', '/fake_engine/wrong_id')
@ToEngine('output1', '/another_fake_engine/my_data_pack')
@FunctionalNode(name='function', outputs=["output1"], exec_policy=node_policies.functional_node.exec_policy.always)
def f_node(input1):
    return [input1]

@FromEngine('input1', '/fake_engine_2/my_data_pack', cache_policy=node_policies.input_node.msg_cache.clear)
@ToEngine('output1', '/fake_engine_2/my_data_pack')
@FunctionalNode(name='function_2', outputs=["output1"], exec_policy=node_policies.functional_node.exec_policy.always)
def f_node(input1):
    return [input1]
