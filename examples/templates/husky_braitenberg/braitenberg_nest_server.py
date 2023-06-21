# -*- coding: utf-8 -*-
"""
This file contains the setup of the neuronal network running the Husky experiment with neuronal image recognition
"""
# pragma: no cover

import nest

LEAKY_PARAMS = {
    'V_th': 1e10,
    'C_m': 1000.0,
    'tau_m': 10.0,
    'tau_syn_ex': 2.,
    'tau_syn_in': 2.,
    'E_L': 0.0,
    'V_reset': 0.0,
    't_ref': 0.1,
    'I_e': 0.0
}

# Get ready for the network modifications
# Simulate() will not work if prepared
if nest.GetKernelStatus('prepared'):
  nest.Cleanup()

# Get the nodes from the NEST Desktop
population = nest.GetNodes()

# Connect neurons
CIRCUIT = population

print("----------------------------")
print(population)
print("----------------------------")

# Left side poisson generator
lpg = nest.Create('poisson_generator')

# Right side poisson generator
rpg = nest.Create('poisson_generator')

# Go poisson generator
gpg = nest.Create('poisson_generator')

# # # Connect datapacks
nest.Connect(lpg, CIRCUIT[0:3:2])
nest.Connect(rpg, CIRCUIT[1:4:2])
nest.Connect(gpg, CIRCUIT[4])

# Create and connect leaky integrator cells
leaky_cells = nest.Create('iaf_psc_exp', 2, LEAKY_PARAMS)
nest.SetStatus(leaky_cells, {'V_m': LEAKY_PARAMS['E_L']})

nest.Connect(CIRCUIT[6],
             leaky_cells[0],
             conn_spec='all_to_all',
             syn_spec={'synapse_model': 'static_synapse', 'weight': 10.0, 'delay': 0.1})

nest.Connect(CIRCUIT[7],
             leaky_cells[1],
             conn_spec='all_to_all',
             syn_spec={'synapse_model': 'static_synapse', 'weight': 10.0, 'delay': 0.1})


populations = {
    'circuit': population[:8],
    'lpg': lpg,
    'rpg': rpg,
    'gpg': gpg,
    'actors': leaky_cells
}

# EOF
