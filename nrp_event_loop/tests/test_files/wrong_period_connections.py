from nrp_core.event_loop import *


@InputDummy('input1', 'idummy1', 10)
@OutputDummy('output1', 'odummy2', compute_period=2)
@FunctionalNode(name='function2', outputs=["output1"])
def f_node_connected(input1):
    return [input1]


@InputDummy('input1', 'idummy1', 10)
@OutputDummy('output1', 'odummy2', compute_period=2)
@FunctionalNode(name='function3', outputs=["output1"])
def f_node_f2f(input1):
    return [input1]
