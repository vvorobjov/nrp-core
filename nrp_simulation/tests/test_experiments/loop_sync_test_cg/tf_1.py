from nrp_core import *
from nrp_core.data.nrp_json import *
from nrp_core.event_loop import *


@FunctionalNode(name="tf_1", outputs=["dummy"], exec_policy=node_policies.functional_node.exec_policy.always)
def transceiver_function():
    """Updates iteration counter for this transceiver function"""
    global num_iterations
    num_iterations[0] += 1

    return [JsonDataPack("dummy", "python_1")]

# EOF
