from nrp_core.event_loop import *

@OutputDummy('output_wrong', 'odummy1')
@FunctionalNode(name='function5', outputs=["output1"])
def wrong_o_port(input1):
    return [input1]

