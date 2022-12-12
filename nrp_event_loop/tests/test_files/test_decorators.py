from nrp_core.event_loop import *

msgs = []


@FunctionalNode(name='function', outputs=["output1"], exec_policy=node_policies.functional_node.exec_policy.on_new_message)
def f_node(input1):
    global msgs
    msgs.append(input1)
    return [input1]


@FunctionalNode(name='function_always', outputs=["output1"], exec_policy=node_policies.functional_node.exec_policy.always)
def f_node(input1):
    global msgs
    msgs.append(input1)
    return [input1]


@InputDummy('input1', 'idummy1', 10)
@OutputDummy('output1', 'odummy1', publish_from_cache=True)
@FunctionalNode(name='function2', outputs=["output1"])
def f_node_connected(input1):
    return [input1]
    

@FromFunctionalNode('input1', '/function2/output1')
@OutputDummy('output1', 'odummy2', compute_period=2)
@FunctionalNode(name='function3', outputs=["output1"])
def f_node_f2f(input1):
    return [input1]