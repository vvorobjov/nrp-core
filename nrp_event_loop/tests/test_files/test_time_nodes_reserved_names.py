from nrp_core.event_loop import *


@InputDummy('i1', 'clock_node', 10)
@InputDummy('i2', 'iteration_node', 10)
@FunctionalNode(name='function', outputs=[])
def f_node(i1, i2):
    return []
