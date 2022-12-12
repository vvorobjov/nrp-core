from nrp_core import *
from nrp_core.data.nrp_json import *
from nrp_core.event_loop import *


@ToEngine(keyword="dummy", address="/nest/dummy")
@FunctionalNode(name="tf_3", outputs=["dummy"], exec_policy=node_policies.functional_node.exec_policy.always)
def transceiver_function(dummy):
    """Updates iteration counter for this transceiver function"""
    global num_iterations
    num_iterations[2] += 1

    return [JsonDataPack("dummy", "nest")]

# EOF
