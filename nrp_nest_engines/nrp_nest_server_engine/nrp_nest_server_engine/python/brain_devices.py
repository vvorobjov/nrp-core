# from scipy.integrate import simps
# import numpy as np

# INPUT

# property: 'rate' /s


def PoissonSpikeGenerator(nest, neurons, n=1, conn_spec='all_to_all'):
    gen = nest.Create('poisson_generator', n)
    syn_dict = {'synapse_model': 'static_synapse',
                'weight': 0.15,
                'delay': 0.1}
    nest.Connect(gen,
                 neurons,
                 conn_spec=conn_spec,
                 syn_spec=syn_dict)
    return gen


# property: 'amplitude' : pA
def DCSource(nest, neurons, n=1, conn_spec='all_to_all'):
    gen = nest.Create('dc_generator', n)
    nest.Connect(gen,
                 neurons,
                 conn_spec=conn_spec)
    return gen


# OUTPUT

# property: 'V_m' : mV
def LeakyIntegrator(nest, neurons, n, n_type, leaky_params, conn_spec, delay):
    leaky_cells = nest.Create(n_type, n, leaky_params)
    nest.SetStatus(leaky_cells, {'V_m': leaky_params['E_L']})
    syn_dict = {'synapse_model': 'static_synapse',
                'weight': 10.0,
                'delay': delay}
    nest.Connect(neurons,
                 leaky_cells,
                 conn_spec=conn_spec,
                 syn_spec=syn_dict)
    return leaky_cells


def LeakyIntegratorAlpha(nest, neurons, n=1, conn_spec='all_to_all'):
    alpha_params = {
        'V_min': 2.2250738585072014e-308,  # sys.float_info.min workaround for NRRPLT-8950
        'V_th': 1e10,
        'C_m': 1000.0,
        'tau_m': 10.0,
        'tau_syn_ex': 2.,
        'tau_syn_in': 2.,
        'E_L': 0.0,
        'V_reset': 0.0,
        't_ref': 0.1,
        'I_e': 0.0}
    return LeakyIntegrator(nest, neurons, n, 'iaf_psc_alpha',
                           alpha_params, conn_spec, 0.1)


def LeakyIntegratorExp(nest, neurons, n=1, conn_spec='all_to_all'):
    exp_params = {'V_th': 1e10,
                  'C_m': 1000.0,
                  'tau_m': 20.0,
                  'tau_syn_ex': .5,
                  'tau_syn_in': .5,
                  'E_L': 0.0,
                  'V_reset': 0.0,
                  't_ref': 0.1,
                  'I_e': 0.0}
    return LeakyIntegrator(nest, neurons, n, 'iaf_psc_exp', exp_params, conn_spec,
                           {'distribution': 'uniform', 'low': 0.1, 'high': 2.0})


# property: 'V_m' : mV
# def NestPopulationRate(nest, name, neurons, n=1, conn_spec='all_to_all'):
#     params = {
#         'V_th': float('inf'),
#         'C_m': 1000.0,
#         'E_L': 0.0,
#         'tau_m': 20.0,
#         'tau_syn_ex': 10.0
#     }
#
#     cell = nest.Create('iaf_psc_exp', n, params)
#     nest.SetStatus(cell, {'V_m': params['E_L']})
#
#     tau_c = (1. / params["tau_syn_ex"] - 1. / params["tau_m"]) ** -1
#     t_end = -np.log(1e-10) * params["tau_m"]
#     x_new = np.arange(0., t_end, 0.1)
#     y_new = tau_c / params["C_m"] * (np.exp(
#         -x_new / params["tau_m"]) - np.exp(
#         -x_new / params["tau_syn_ex"]))
#     weight = 1.0 / simps(y_new, dx=nest.GetKernelStatus('resolution'))
#
#     nest.Connect(neurons,
#                  cell,
#                  conn_spec=conn_spec,
#                  syn_spec={'synapse_model': 'static_synapse',
#                            'weight': weight * 1000,
#                            'delay': nest.GetKernelStatus('resolution')})
#     return cell
