from nrp_core.event_loop import *

fn = createFNFromFactoryModule(module_name="libFNFactoryModule.so",
                               function_name="from_python",
                               node_name="node_2",
                               exec_policy=node_policies.functional_node.exec_policy.on_new_message)
FromFunctionalNode("i1", "/node_1/o1")(fn)

fn = createFNFromFactoryModule(module_name="libFNFactoryModule.so", 
                               function_name="to_python",
                               node_name="node_3",
                               exec_policy=node_policies.functional_node.exec_policy.on_new_message)
FromFunctionalNode("i1", "/node_2/o1")(fn)
OutputDummy("o1", "odummy")(fn)

fn = createFNFromFactoryModule(module_name="libFNFactoryModule.so", 
                               function_name="print_python_int",
                               node_name="print_value",
                               exec_policy=node_policies.functional_node.exec_policy.on_new_message)
FromFunctionalNode("i1", "/node_3/o1")(fn)
