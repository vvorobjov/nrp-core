from nrp_core.event_loop import *

@InputDummy('input_wrong', 'idummy1', 10)
@FunctionalNode(name='function4', outputs=["output1"])
def wrong_i_port(input1):
    return [input1]
