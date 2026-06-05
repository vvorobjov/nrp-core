# -*- coding: utf-8 -*-
"""Foraging-husky brain — nest_server / docker-compose variant.

This script runs **inside the docker-registry.ebrains.eu/nest/nest-simulator:3.9
container** spawned by docker-compose-foraging-nest-gazebo.yaml. That
container ships pure NEST 3.9 plus the nest-server REST API; it does
NOT have nrp_core installed AND does not mount the experiment dir, so
neither `from nrp_core...` nor `import params` is reachable here. NRP-
core POSTs this file's contents to /exec and the nest-server evaluates
them in its own namespace; the script must therefore be self-contained.

The params block below mirrors params.py one-for-one; if you change a
value in either file, change it in both. The standalone variant is
brain.py and uses params.py directly. (husky_braitenberg also keeps a
brain.py / braitenberg_nest_server_compose.py pair — same trade-off.)

Topology mirrors brain.py one-for-one; populations exposed to NRP-core
via the conventional `populations` dict at the bottom.

Tracked in EBR2-32.
"""
# pragma: no cover

import nest


# --- Inlined params (kept in lockstep with params.py) ---
class P:
    NEST_RNG_SEED = 4242
    DRIVE_POP_SIZE = 50
    TONIC_HUNGER_RATE = 60.0
    DRIVE_INHIB_WEIGHT = -1500.0
    TONIC_TO_DRIVE_WEIGHT = 4000.0
    EAT_KICK_DURATION_TICKS = 800
    BRAKE_POP_SIZE = 20
    WEIGHT_NEAR_BLUE_TO_BRAKE = 1000.0
    WEIGHT_SATED_TO_BRAKE = 350.0
    BRAKE_INHIB_WEIGHT = -2000.0
    MOTOR_POP_SIZE = 20
    WEIGHT_GREEN_TO_MOTOR = 2500.0
    WEIGHT_BLUE_TO_MOTOR = 2500.0
    WEIGHT_OBSTACLE_TO_MOTOR = -1000.0
    MOTOR_BIAS_DC_AMPLITUDE = 300.0
    SEARCH_BIAS_WEIGHT = 80.0

nest.set_verbosity("M_WARNING")
nest.ResetKernel()
nest.rng_seed = P.NEST_RNG_SEED
# Spike recorders use record_to='ascii' for perf; nest-server keeps
# the kernel alive across /api/exec calls, so the previous run's
# /tmp/fh_*.dat files are still around when this brain is re-loaded.
# overwrite_files=True lets the recorder enroll re-spawn without
# RecordingBackendASCII::enroll() crashing on "file already exists".
nest.SetKernelStatus({'overwrite_files': True})

# Drive populations
pop_hungry = nest.Create('iaf_psc_alpha', P.DRIVE_POP_SIZE)
pop_sated = nest.Create('iaf_psc_alpha', P.DRIVE_POP_SIZE)

tonic_hunger = nest.Create('poisson_generator',
                           params={'rate': P.TONIC_HUNGER_RATE})
nest.Connect(tonic_hunger, pop_hungry,
             syn_spec={'weight': P.TONIC_TO_DRIVE_WEIGHT, 'delay': 0.1})

nest.Connect(pop_hungry, pop_sated, 'all_to_all',
             syn_spec={'weight': P.DRIVE_INHIB_WEIGHT, 'delay': 0.1})
nest.Connect(pop_sated, pop_hungry, 'all_to_all',
             syn_spec={'weight': P.DRIVE_INHIB_WEIGHT, 'delay': 0.1})

# Eat-event DC injector (drive_update.py writes 'amplitude' per tick).
eat_kick_dc = nest.Create('dc_generator', 1, {'amplitude': 0.0})
nest.Connect(eat_kick_dc, pop_sated, 'all_to_all')

# Motor populations + leaky-integrator readouts
motor_l = nest.Create('iaf_psc_alpha', P.MOTOR_POP_SIZE)
motor_r = nest.Create('iaf_psc_alpha', P.MOTOR_POP_SIZE)

# Leaky-integrator readouts. iaf_psc_exp (not iaf_psc_alpha) because
# iaf_psc_alpha defaults V_min to -inf, which nest-server serialises
# as JSON `-Infinity` — and NRP-core's strict-JSON parser rejects it
# with a parse error on the first GetStatus call. iaf_psc_exp's
# defaults are all finite so the REST round-trip succeeds. The husky
# braitenberg_nest_server_compose.py uses the same iaf_psc_exp for
# the same reason.
_LEAKY_PARAMS = {
    'V_th': 1e10, 'C_m': 1000.0, 'tau_m': 10.0,
    'tau_syn_ex': 2.0, 'tau_syn_in': 2.0,
    'E_L': 0.0, 'V_reset': 0.0, 't_ref': 0.1, 'I_e': 0.0,
}
motor_l_voltage = nest.Create('iaf_psc_exp', 1, _LEAKY_PARAMS)
nest.SetStatus(motor_l_voltage, {'V_m': 0.0})
nest.Connect(motor_l, motor_l_voltage, 'all_to_all',
             syn_spec={'synapse_model': 'static_synapse',
                       'weight': 10.0, 'delay': 0.1})

motor_r_voltage = nest.Create('iaf_psc_exp', 1, _LEAKY_PARAMS)
nest.SetStatus(motor_r_voltage, {'V_m': 0.0})
nest.Connect(motor_r, motor_r_voltage, 'all_to_all',
             syn_spec={'synapse_model': 'static_synapse',
                       'weight': 10.0, 'delay': 0.1})

# Drive-population voltage readouts. snn_observer.py logs these as
# FORAGING_DIAG snn hungry_v / sated_v lines to expose the mutual-
# inhibition winner-take-all dynamics.
pop_hungry_voltage = nest.Create('iaf_psc_exp', 1, _LEAKY_PARAMS)
nest.SetStatus(pop_hungry_voltage, {'V_m': 0.0})
nest.Connect(pop_hungry, pop_hungry_voltage, 'all_to_all',
             syn_spec={'synapse_model': 'static_synapse',
                       'weight': 10.0, 'delay': 0.1})

pop_sated_voltage = nest.Create('iaf_psc_exp', 1, _LEAKY_PARAMS)
nest.SetStatus(pop_sated_voltage, {'V_m': 0.0})
nest.Connect(pop_sated, pop_sated_voltage, 'all_to_all',
             syn_spec={'synapse_model': 'static_synapse',
                       'weight': 10.0, 'delay': 0.1})

# Camera-driven Poissons (rates written by obstacle_to_avoidance.py)
cam_green_left_poisson = nest.Create('poisson_generator', 1, {'rate': 0.0})
cam_green_right_poisson = nest.Create('poisson_generator', 1, {'rate': 0.0})
cam_blue_left_poisson = nest.Create('poisson_generator', 1, {'rate': 0.0})
cam_blue_right_poisson = nest.Create('poisson_generator', 1, {'rate': 0.0})
cam_grey_left_poisson = nest.Create('poisson_generator', 1, {'rate': 0.0})
cam_grey_right_poisson = nest.Create('poisson_generator', 1, {'rate': 0.0})

nest.Connect(cam_green_left_poisson, motor_r, 'all_to_all',
             syn_spec={'weight': P.WEIGHT_GREEN_TO_MOTOR, 'delay': 0.1})
nest.Connect(cam_green_right_poisson, motor_l, 'all_to_all',
             syn_spec={'weight': P.WEIGHT_GREEN_TO_MOTOR, 'delay': 0.1})
nest.Connect(cam_blue_left_poisson, motor_r, 'all_to_all',
             syn_spec={'weight': P.WEIGHT_BLUE_TO_MOTOR, 'delay': 0.1})
nest.Connect(cam_blue_right_poisson, motor_l, 'all_to_all',
             syn_spec={'weight': P.WEIGHT_BLUE_TO_MOTOR, 'delay': 0.1})
nest.Connect(cam_grey_left_poisson, motor_l, 'all_to_all',
             syn_spec={'weight': P.WEIGHT_OBSTACLE_TO_MOTOR, 'delay': 0.1})
nest.Connect(cam_grey_right_poisson, motor_r, 'all_to_all',
             syn_spec={'weight': P.WEIGHT_OBSTACLE_TO_MOTOR, 'delay': 0.1})

tonic_motor_bias = nest.Create('dc_generator', 1,
                               params={'amplitude': P.MOTOR_BIAS_DC_AMPLITUDE})
nest.Connect(tonic_motor_bias, motor_l, 'all_to_all')
nest.Connect(tonic_motor_bias, motor_r, 'all_to_all')

# Search bias — see brain.py for the rationale.
nest.Connect(pop_hungry, motor_l, 'all_to_all',
             syn_spec={'weight': P.SEARCH_BIAS_WEIGHT, 'delay': 0.1})
nest.Connect(pop_sated, motor_r, 'all_to_all',
             syn_spec={'weight': P.SEARCH_BIAS_WEIGHT, 'delay': 0.1})

# Rest-zone brake — coincidence detector (POP_SATED + near_blue both
# above threshold → brake fires → inhibits motors → husky parks).
near_blue_poisson = nest.Create('poisson_generator', 1, {'rate': 0.0})
brake = nest.Create('iaf_psc_alpha', P.BRAKE_POP_SIZE,
                    params={'V_th': -15.0})
nest.Connect(near_blue_poisson, brake, 'all_to_all',
             syn_spec={'weight': P.WEIGHT_NEAR_BLUE_TO_BRAKE, 'delay': 0.1})
nest.Connect(pop_sated, brake, 'all_to_all',
             syn_spec={'weight': P.WEIGHT_SATED_TO_BRAKE, 'delay': 0.1})
nest.Connect(brake, motor_l, 'all_to_all',
             syn_spec={'weight': P.BRAKE_INHIB_WEIGHT, 'delay': 0.1})
nest.Connect(brake, motor_r, 'all_to_all',
             syn_spec={'weight': P.BRAKE_INHIB_WEIGHT, 'delay': 0.1})

# Brake voltage readout for the SNN observer.
brake_voltage = nest.Create('iaf_psc_exp', 1, _LEAKY_PARAMS)
nest.SetStatus(brake_voltage, {'V_m': 0.0})
nest.Connect(brake, brake_voltage, 'all_to_all',
             syn_spec={'synapse_model': 'static_synapse',
                       'weight': 10.0, 'delay': 0.1})

# Spike recorders for the per-population analysis (see brain.py).
# record_to='ascii' keeps NEST's n_events counter live for the
# JsonDataPack reads in drive_update while preventing the recorder
# from accumulating the full events list in memory — without that
# offload the run drops to ~1/60 realtime within ~10 s of sim time.
_SR_KW = {'record_to': 'ascii'}
pop_hungry_spikes = nest.Create('spike_recorder',
                                params={**_SR_KW, 'label': '/tmp/fh_pop_hungry_spikes'})
nest.Connect(pop_hungry, pop_hungry_spikes)
pop_sated_spikes = nest.Create('spike_recorder',
                               params={**_SR_KW, 'label': '/tmp/fh_pop_sated_spikes'})
nest.Connect(pop_sated, pop_sated_spikes)
motor_l_spikes = nest.Create('spike_recorder',
                             params={**_SR_KW, 'label': '/tmp/fh_motor_l_spikes'})
nest.Connect(motor_l, motor_l_spikes)
motor_r_spikes = nest.Create('spike_recorder',
                             params={**_SR_KW, 'label': '/tmp/fh_motor_r_spikes'})
nest.Connect(motor_r, motor_r_spikes)
brake_spikes = nest.Create('spike_recorder',
                           params={**_SR_KW, 'label': '/tmp/fh_brake_spikes'})
nest.Connect(brake, brake_spikes)

# Datapack-name → NEST object mapping for NRP-core's nest_server engine.
# Stable names — TF files refer to them by string. Names match brain.py's
# RegisterDataPack calls one-for-one.
populations = {
    'cam_green_left_poisson':  cam_green_left_poisson,
    'cam_green_right_poisson': cam_green_right_poisson,
    'cam_blue_left_poisson':   cam_blue_left_poisson,
    'cam_blue_right_poisson':  cam_blue_right_poisson,
    'cam_grey_left_poisson':   cam_grey_left_poisson,
    'cam_grey_right_poisson':  cam_grey_right_poisson,
    'eat_kick_dc':             eat_kick_dc,
    'motor_l_voltage':         motor_l_voltage,
    'motor_r_voltage':         motor_r_voltage,
    'pop_hungry_voltage':      pop_hungry_voltage,
    'pop_sated_voltage':       pop_sated_voltage,
    'near_blue_poisson':       near_blue_poisson,
    'brake_voltage':           brake_voltage,
    'pop_hungry_spikes':       pop_hungry_spikes,
    'pop_sated_spikes':        pop_sated_spikes,
    'motor_l_spikes':          motor_l_spikes,
    'motor_r_spikes':          motor_r_spikes,
    'brake_spikes':            brake_spikes,
}
