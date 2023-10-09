from nrp_core.event_loop import *


@Clock('i1')
@Iteration('i2')
@OutputDummy('clock', 'clock_out')
@OutputDummy('iteration', 'iteration_out')
@FunctionalNode(name='function', outputs=["clock", "iteration"])
def f_node(i1, i2):
    return [i1, i2]
