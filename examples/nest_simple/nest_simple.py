"""Init File. Imports nest and sets up a poisson generator, neuron, and voltmeter"""

import nest
import nest.voltage_trace
from NRPNestJSONPythonModule import RegisterDevice, CreateDevice

nest.set_verbosity("M_WARNING")
nest.ResetKernel()

noise = CreateDevice("noise", "poisson_generator", 2)
neuron = nest.Create("iaf_psc_alpha")
voltmeter = nest.Create("voltmeter")
RegisterDevice('voltage', voltmeter)

nest.SetStatus(noise, {"rate": 15000.0})

nest.Connect(noise, neuron, syn_spec={'weight': [[1.2, -1.0]], 'delay': 1.0})
nest.Connect(voltmeter, neuron)

# EOF
