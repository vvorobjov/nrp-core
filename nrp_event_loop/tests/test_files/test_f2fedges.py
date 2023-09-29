from nrp_core.event_loop import *

createFNFromFactoryModule(module_name="libFNFactoryModule.so",
                            function_name="forward_int",
                            node_name="int_to_int",
                            exec_policy=node_policies.functional_node.exec_policy.on_new_message)

createFNFromFactoryModule(module_name="libFNFactoryModule.so",
                            function_name="python_to_int",
                            node_name="obj_to_int",
                            exec_policy=node_policies.functional_node.exec_policy.on_new_message)

createFNFromFactoryModule(module_name="libFNFactoryModule.so",
                            function_name="int_to_python",
                            node_name="int_to_obj",
                            exec_policy=node_policies.functional_node.exec_policy.on_new_message)

@FunctionalNode(name='obj_to_obj', outputs=["o1"], exec_policy=node_policies.functional_node.exec_policy.on_new_message)
def f_node(i1):
    if i1:
        return [i1]
    else:
        return None
