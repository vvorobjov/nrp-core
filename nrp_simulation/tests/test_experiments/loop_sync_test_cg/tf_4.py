from nrp_core import *
from nrp_core.data.nrp_json import *
from nrp_core.event_loop import *


@ToEngine(keyword="dummy", address="/python_3")
@FunctionalNode(name="tf_4", outputs=["dummy"], exec_policy=node_policies.functional_node.exec_policy.always)
def transceiver_function(dummy):
    """Updates iteration counter for this transceiver function"""
    global num_iterations
    num_iterations[3] += 1

    return [JsonDataPack("dummy", "python_3")]

# EOF
