from nrp_core.event_loop import *

fn = createFNFromFactoryModule(module_name="libFNFactoryModule.so", function_name="my_function", node_name="my_fn", exec_policy=node_policies.functional_node.exec_policy.on_new_message)
InputDummy("i1", "idummy", 12)(fn)
OutputDummy("o1", "odummy")(fn)