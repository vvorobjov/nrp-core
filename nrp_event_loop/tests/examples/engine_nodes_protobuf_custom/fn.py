from nrp_core.event_loop import *

fn = createFNFromFactoryModule(module_name="libFNFactoryModule.so", function_name="my_function", node_name="my_fn", exec_policy=node_policies.functional_node.exec_policy.on_new_message)
FromEngine(keyword='i1', address='/python_1/datapack_1')(fn)
ToEngine(keyword="o1", address="/datatransfer_engine")(fn)