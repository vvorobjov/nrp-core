from nrp_core.event_loop import *

n_iter = 0
t_step = 10  # ms

@Clock('clock_in')
@Iteration('iteration_in')
# It is needed to connect the outputs, otherwise the FN is not run because FTILoop runs the graph in OUTPUT_DRIVEN mode
@OutputDummy('clock_out', 'dummy1')
@OutputDummy('iteration_out', 'dummy2')
@FunctionalNode(name='function', outputs=["clock_out", "iteration_out"])
def f_node(clock_in, iteration_in):
    global n_iter
    if iteration_in != n_iter or clock_in != n_iter * t_step:
        raise Exception("Time nodes are not functioning as expected!")
    else:
        n_iter += 1

    return [clock_in, iteration_in]
