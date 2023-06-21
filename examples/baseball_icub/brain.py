import nest
from nrp_core.engines.nest_json import RegisterDataPack, CreateDataPack
from nrp_core.engines.nest_json import brain_devices as bd


nest.set_verbosity("M_WARNING")
nest.ResetKernel()

n_sensors = 20
n_motors = 1

PSC_PARAMS = {
    'tau_syn_ex': 5.0,
    'tau_syn_in': 5.0,
    'tau_m': 20.0,
    't_ref': 0.1,
    'I_e': 0.0,
    'C_m': 1000.0,
    'V_m': -65.0,
    'V_reset': -65.0,
    'V_th': -50.0,
    'E_L': -65.0
}

sensors = nest.Create('iaf_psc_exp', n_sensors, PSC_PARAMS)
motors = nest.Create('iaf_psc_exp', n_motors, PSC_PARAMS)

nest.Connect(sensors,
             motors,
             conn_spec='all_to_all',
             syn_spec={'synapse_model': 'static_synapse', 'weight': 1.0})

bd.DCSource(nest, 'sensors', sensors, 20, 'one_to_one')
bd.LeakyIntegratorAlpha(nest, 'motors', sensors)
