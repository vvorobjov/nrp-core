from nrp_core.event_loop import *


@FromEngine('input1', '/fake_engine/my_data_pack')
@ToEngine('output1', '/fake_engine/my_data_pack')
@ToEngine('output1', '/fake_engine/wrong_id')
@ToEngine('output1', '/another_fake_engine/my_data_pack')
@FunctionalNode(name='function', outputs=["output1"])
def f_node_connected(input1):
    return [input1]
