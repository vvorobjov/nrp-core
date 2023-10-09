from nrp_core.event_loop import *


@InputDummy('i1', 'idummy1', 10)
@FunctionalNode(name='node_1', outputs=["o1"])
def f_node(i1):
    # is this being copied?
    return [i1]
