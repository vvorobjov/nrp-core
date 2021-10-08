"""Init File. Imports nest and sets up a poisson generator, neuron, and voltmeter"""

import nest
import nest.voltage_trace
from nrp_core.engines.nest_json import RegisterDataPack, CreateDataPack

nest.set_verbosity("M_WARNING")
nest.ResetKernel()

noise = CreateDataPack("noise", "poisson_generator", 2)
neuron = nest.Create("iaf_psc_alpha")
voltmeter = nest.Create("voltmeter")
RegisterDataPack('voltage', voltmeter)

nest.Connect(noise, neuron, syn_spec={'weight': [[1.2, -1.0]], 'delay': 1.0})
nest.Connect(voltmeter, neuron)

# EOF
