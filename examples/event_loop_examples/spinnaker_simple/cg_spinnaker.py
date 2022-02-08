from nrp_core import *
from nrp_core.data.nrp_json import *
from nrp_core.event_loop import *

import pyNN.spiNNaker as p

p.setup(1.0)

# Create a Population into which spikes can be injected.  This has 25 neurons.
injector = p.Population(
    25, p.external_devices.SpikeInjector(notify=False), label="injector")

# Make the Population send live spikes out.
p.external_devices.activate_live_output_for(injector, notify=False)

@FromSpinnaker(keyword='spinnaker_in', address="injector")
@ToSpinnaker(keyword="spinnaker_out", address="injector")
@FunctionalNode(name = "spinnaker_fn", outputs=["spinnaker_out"], exec_policy=node_policies.functional_node.exec_policy.always)
def transceiver_function_in(spinnaker_in):
    out = NlohmannJson()
    if spinnaker_in is not None:
        print('Message from spinnaker: {}'.format(spinnaker_in))
    out['neuron_ids'] = list(range(25))
    return [ out ]
