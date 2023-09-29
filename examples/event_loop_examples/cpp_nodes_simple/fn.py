from nrp_core.event_loop import *

var_x = 0

# This node reads a global var and pass it forward
@FunctionalNode(name="py_node_1", outputs=['python_var'], 
                exec_policy=node_policies.functional_node.exec_policy.always)
def read_var():
    return [var_x]

# This node is connected to 'py_node_1'. 
# It is a cpp_node which 'var_x' and could use it for any operations happening in the function body
# In this case it just returns 'var_x' as an 'int'
fn = createFNFromFactoryModule(module_name="libFNFactoryModule.so", 
                               function_name="from_python", 
                               node_name="cpp_node_1", 
                               exec_policy=node_policies.functional_node.exec_policy.on_new_message)
FromFunctionalNode("i1", "/py_node_1/python_var")(fn)

# Another cpp node, just relying 'var_x' as a python object to showcase connecting 2 cpp nodes
fn = createFNFromFactoryModule(module_name="libFNFactoryModule.so", 
                               function_name="to_python", 
                               node_name="cpp_node_2", 
                               exec_policy=node_policies.functional_node.exec_policy.on_new_message)
FromFunctionalNode("i1", "/cpp_node_1/o1")(fn)

# Finally this python node will write to 'var_x'
@FromFunctionalNode('i1', '/cpp_node_2/o1')
@FunctionalNode(name="py_node_2", outputs=[],
                exec_policy=node_policies.functional_node.exec_policy.on_new_message)
def write_var(i1):
    print(i1)
    global var_x
    var_x = i1 + 1
