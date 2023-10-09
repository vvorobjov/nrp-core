from nrp_core.event_loop import *


@FromFunctionalNode('i1', '/node_1/o1')
@OutputDummy('o1', 'odummy1')
@FunctionalNode(name='node_2', outputs=["o1"])
def f_node_f2f(i1):
    print(i1)
    # is this being copied?
    return [i1]