# -*- coding: utf-8 -*-
"""
This file contains the setup of the neuronal network running the Husky experiment with neuronal image recognition
"""
# pragma: no cover

import nest
from nrp_core.engines.nest_json import RegisterDataPack, CreateDataPack
from nrp_core.engines.nest_json import brain_devices as bd

SENSORPARAMS = {'E_L': -60.5,
                'C_m': 25.0,
                'g_L': 25.0 / 10.,
                't_ref': 10.0,
                'tau_syn_ex': 2.5,
                'tau_syn_in': 2.5,
                'E_ex': 0.0,
                'E_in': -75.0,
                'V_th': -60.0,
                'V_reset': -60.5,
                'V_m': -60.5}

GO_ON_PARAMS = {'E_L': -60.5,
                'C_m': 25.0,
                'g_L': 25.0 / 10.,
                'E_ex': 0.0,
                'E_in': -75.0,
                'V_reset': -61.6,
                'V_th': -60.51,
                't_ref': 10.0,
                'tau_syn_ex': 2.5,
                'tau_syn_in': 2.5,
                'V_m': -60.5}

nest.set_verbosity("M_WARNING")
nest.ResetKernel()

population = nest.Create('iaf_cond_alpha', 8)
nest.SetStatus(population[0:5], SENSORPARAMS)
nest.SetStatus(population[5:6], GO_ON_PARAMS)
nest.SetStatus(population[6:8], SENSORPARAMS)

# Shared Synapse Parameters
nest.CopyModel('tsodyks_synapse', 'base_synapse', {'U': 1.0, 'tau_rec': 1.0, 'tau_fac': 1.0})

# Synaptic weights
WEIGHT_RED_TO_ACTOR = 1.5e-1
WEIGHT_RED_TO_GO_ON = 1.2  # or -1.2e-3?
WEIGHT_GREEN_BLUE_TO_ACTOR = 1.05e-1
WEIGHT_GO_ON_TO_RIGHT_ACTOR = 1.4e-1
DELAY = 0.1

# Connect neurons
CIRCUIT = population

SYN = {'synapse_model': 'base_synapse', 'weight': WEIGHT_RED_TO_ACTOR, 'delay': DELAY}
nest.Connect(CIRCUIT[2:3], CIRCUIT[7:8], 'all_to_all', SYN)
nest.Connect(CIRCUIT[3:4], CIRCUIT[6:7], 'all_to_all', SYN)

SYN = {'synapse_model': 'base_synapse', 'weight': -WEIGHT_RED_TO_GO_ON, 'delay': DELAY}
nest.Connect(CIRCUIT[0:2], CIRCUIT[4:5], 'all_to_all', SYN)
nest.Connect(CIRCUIT[0:2], CIRCUIT[5:6], 'all_to_all', SYN)

SYN = {'synapse_model': 'base_synapse', 'weight': WEIGHT_GREEN_BLUE_TO_ACTOR, 'delay': DELAY}
nest.Connect(CIRCUIT[4:5], CIRCUIT[7:8], 'all_to_all', SYN)

SYN = {'synapse_model': 'base_synapse', 'weight': WEIGHT_GO_ON_TO_RIGHT_ACTOR, 'delay': DELAY}
nest.Connect(CIRCUIT[5:6], CIRCUIT[7:8], 'all_to_all', SYN)

# Left side poisson generator
bd.PoissonSpikeGenerator(nest, 'lpg', CIRCUIT[slice(0, 3, 2)])

# Right side poisson generator
bd.PoissonSpikeGenerator(nest, 'rpg', CIRCUIT[slice(1, 4, 2)])

# Go poisson generator
bd.PoissonSpikeGenerator(nest, 'gpg', CIRCUIT[4])

# Create and connect leaky integrator cells
bd.LeakyIntegratorAlpha(nest, 'actors', CIRCUIT[6:8], n=2, conn_spec='one_to_one')

# Simulate
# sd = nest.Create('spike_recorder')
# nest.Connect(CIRCUIT[6], sd)
#
# x = []
# v = []
# v2 = []
# v3 = []
# v4 = []
# for i in range(100):
#     nest.Simulate(10)
#     x.append(i * 10)
#     v.append(CIRCUIT[6].get('V_m'))
#     v2.append(CIRCUIT[7].get('V_m'))
#     v3.append(leaky_cells[0].get('V_m'))
#     v4.append(leaky_cells[1].get('V_m'))
#
# # nest.raster_plot.from_datapack(sd, hist=True)
# # plt.plot(x,v)
# # plt.plot(x,v2)
# plt.plot(x,v3, 'b')
# plt.plot(x,v4, 'r')
# plt.show()
