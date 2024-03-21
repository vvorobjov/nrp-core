from nrp_core.event_loop import *


fn = createFNFromFactoryModule(module_name="libFNFactoryModule.so",
                               function_name="from_python",
                               node_name="node_0",
                               exec_policy=node_policies.functional_node.exec_policy.on_new_message)
InputDummy("i1", "idummy", 10)(fn)

fn = createFNFromFactoryModule(module_name="libFNFactoryModule.so", 
                               function_name="to_python",
                               node_name="node_1",
                               exec_policy=node_policies.functional_node.exec_policy.on_new_message)
FromFunctionalNode("i1", "/node_0/o1")(fn)