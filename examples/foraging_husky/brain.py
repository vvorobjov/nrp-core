# -*- coding: utf-8 -*-
"""Drive-state SNN for the foraging_husky experiment.

Topology
--------

    [tonic Poisson] --25.0--> POP_HUNGRY
                                |
                                |   mutual inhibition (DRIVE_INHIB_WEIGHT)
                                v
                              POP_SATED <-- bd.DCSource('eat_kick_dc')

    cam_*_poisson  (6 generators, registered by name; rates written by
                   obstacle_to_avoidance.py per TF tick)
        cam_green_left_poisson   --WEIGHT_GREEN_TO_MOTOR-->  motor_r
        cam_green_right_poisson  --WEIGHT_GREEN_TO_MOTOR-->  motor_l
        cam_blue_left_poisson    --WEIGHT_BLUE_TO_MOTOR-->   motor_r
        cam_blue_right_poisson   --WEIGHT_BLUE_TO_MOTOR-->   motor_l
        cam_grey_left_poisson    --WEIGHT_OBSTACLE_TO_MOTOR-> motor_l  (inhib)
        cam_grey_right_poisson   --WEIGHT_OBSTACLE_TO_MOTOR-> motor_r  (inhib)

    motor_l, motor_r  -->  bd.LeakyIntegratorAlpha('motor_l_voltage' / '_r_')
        (the TF reads V_m on each tick; spikes_to_wheel_vels in helpers.py
         converts to a 4-tuple of wheel velocities)

    Drive gating is implemented as additive excitatory current from
    POP_HUNGRY to motor populations (favours green-channel responses by
    raising baseline motor excitability when hungry) and from POP_SATED
    to motor populations (same for blue). Each connection uses
    WEIGHT_DRIVE_TO_CHANNEL.

Tunables live in params.py. The bd helpers (brain_devices.py) hardwire
synaptic weights for their devices; we use them only for the bookkeeping
datapacks (eat_kick_dc, motor_*_voltage) and do the custom-weight wiring
manually with nest.Connect so the goal/obstacle/drive pathways have the
weights params.py specifies.

Tracked in EBR2-32.
"""
# pragma: no cover

import nest
from nrp_core.engines.nest_json import RegisterDataPack
from nrp_core.engines.nest_json import brain_devices as bd

import params as P

nest.set_verbosity("M_WARNING")
nest.ResetKernel()
nest.rng_seed = P.NEST_RNG_SEED
# Spike recorders use record_to='ascii' for perf (see below); allow
# the recorder backend to overwrite the per-population files from
# previous runs so re-invoking the experiment doesn't error out on
# "file already exists" from a stale /tmp dump.
nest.SetKernelStatus({'overwrite_files': True})


# ---------------------------------------------------------------------------
# Drive populations
# ---------------------------------------------------------------------------

pop_hungry = nest.Create('iaf_psc_alpha', P.DRIVE_POP_SIZE)
pop_sated = nest.Create('iaf_psc_alpha', P.DRIVE_POP_SIZE)

tonic_hunger = nest.Create('poisson_generator',
                           params={'rate': P.TONIC_HUNGER_RATE})
nest.Connect(tonic_hunger, pop_hungry,
             syn_spec={'weight': P.TONIC_TO_DRIVE_WEIGHT, 'delay': 0.1})

# Mutual inhibition between drives.
nest.Connect(pop_hungry, pop_sated, 'all_to_all',
             syn_spec={'weight': P.DRIVE_INHIB_WEIGHT, 'delay': 0.1})
nest.Connect(pop_sated, pop_hungry, 'all_to_all',
             syn_spec={'weight': P.DRIVE_INHIB_WEIGHT, 'delay': 0.1})

# Eat-event injector. The bd.DCSource wrapper creates a dc_generator,
# connects it to pop_sated, and registers it as a datapack so
# drive_update.py can write the 'amplitude' field per tick.
bd.DCSource(nest, 'eat_kick_dc', pop_sated)

# Drive-population voltage readouts. Same LeakyIntegratorAlpha pattern
# as the motor readouts below — a 1-neuron leaky cell receives spikes
# from the whole drive population and its V_m tracks the population
# firing rate. snn_observer reads these to log
#   FORAGING_DIAG snn hungry_v=... sated_v=...
# every diag interval; the user can see the mutual-inhibition
# winner-take-all dynamics in action and check_functionality.sh
# asserts the two voltages anti-correlate over the run (which is only
# possible if NEST is actually running the arbitration — TFs can't
# fake anti-correlated population dynamics).
bd.LeakyIntegratorAlpha(nest, 'pop_hungry_voltage', pop_hungry, n=1,
                        conn_spec='all_to_all')
bd.LeakyIntegratorAlpha(nest, 'pop_sated_voltage', pop_sated, n=1,
                        conn_spec='all_to_all')


# ---------------------------------------------------------------------------
# Motor populations + leaky-integrator readouts
# ---------------------------------------------------------------------------

motor_l = nest.Create('iaf_psc_alpha', P.MOTOR_POP_SIZE)
motor_r = nest.Create('iaf_psc_alpha', P.MOTOR_POP_SIZE)

# bd.LeakyIntegratorAlpha creates a leaky cell and connects motor_*
# into it (weight 10.0 hardwired); motors_to_husky.py reads V_m on
# this cell to derive wheel velocities. One leaky cell per side is
# enough — V_m tracks population activity.
bd.LeakyIntegratorAlpha(nest, 'motor_l_voltage', motor_l, n=1,
                        conn_spec='all_to_all')
bd.LeakyIntegratorAlpha(nest, 'motor_r_voltage', motor_r, n=1,
                        conn_spec='all_to_all')


# ---------------------------------------------------------------------------
# Camera-driven Poisson generators (rates written by obstacle_to_avoidance.py)
# ---------------------------------------------------------------------------
#
# We do not use bd.PoissonSpikeGenerator here because its built-in
# synapse weight (0.15) is wrong for the obstacle channel, which must
# be inhibitory. Manual nest.Create + RegisterDataPack + nest.Connect
# with our own weights from params.py gives the goal / obstacle /
# drive-gating pathways the asymmetry the experiment depends on.

def _named_poisson(name):
    """Create a 1-node poisson_generator and register it as a datapack."""
    gen = nest.Create('poisson_generator', 1, params={'rate': 0.0})
    RegisterDataPack(name, gen)
    return gen


cam_green_left = _named_poisson('cam_green_left_poisson')
cam_green_right = _named_poisson('cam_green_right_poisson')
cam_blue_left = _named_poisson('cam_blue_left_poisson')
cam_blue_right = _named_poisson('cam_blue_right_poisson')
cam_grey_left = _named_poisson('cam_grey_left_poisson')
cam_grey_right = _named_poisson('cam_grey_right_poisson')

# Goal pathways: Braitenberg 2b crossing (left-camera → right-motor).
nest.Connect(cam_green_left, motor_r, 'all_to_all',
             syn_spec={'weight': P.WEIGHT_GREEN_TO_MOTOR, 'delay': 0.1})
nest.Connect(cam_green_right, motor_l, 'all_to_all',
             syn_spec={'weight': P.WEIGHT_GREEN_TO_MOTOR, 'delay': 0.1})
nest.Connect(cam_blue_left, motor_r, 'all_to_all',
             syn_spec={'weight': P.WEIGHT_BLUE_TO_MOTOR, 'delay': 0.1})
nest.Connect(cam_blue_right, motor_l, 'all_to_all',
             syn_spec={'weight': P.WEIGHT_BLUE_TO_MOTOR, 'delay': 0.1})

# Obstacle reflex (ipsilateral inhibition): grey on left → motor L
# inhibited → husky turns away (right). Same geometry but inhibitory.
nest.Connect(cam_grey_left, motor_l, 'all_to_all',
             syn_spec={'weight': P.WEIGHT_OBSTACLE_TO_MOTOR, 'delay': 0.1})
nest.Connect(cam_grey_right, motor_r, 'all_to_all',
             syn_spec={'weight': P.WEIGHT_OBSTACLE_TO_MOTOR, 'delay': 0.1})


# ---------------------------------------------------------------------------
# Forward bias: a single tonic Poisson into both motor populations.
# This gives a constant baseline "move forward" command so the husky
# wanders even when nothing is visible. The earlier design connected
# the drive populations all-to-all to motors with WEIGHT_DRIVE_TO_CHANNEL,
# which saturated both motors symmetrically and prevented the goal
# pathway from steering — the husky just drove straight. Keep the
# drive populations alive for the POP_HUNGRY ↔ POP_SATED winner-take-
# all bookkeeping + the eat_kick_dc injection, but no longer feed
# them directly into motors.
# ---------------------------------------------------------------------------

tonic_motor_bias = nest.Create('dc_generator', 1,
                               params={'amplitude': P.MOTOR_BIAS_DC_AMPLITUDE})
nest.Connect(tonic_motor_bias, motor_l, 'all_to_all')
nest.Connect(tonic_motor_bias, motor_r, 'all_to_all')


# ---------------------------------------------------------------------------
# Search bias: when no goal is in view, the drive populations push the
# husky into a slow turn so it scans the arena. POP_HUNGRY → motor_l
# only (turn-right while wandering); POP_SATED → motor_r only (turn-
# left). The asymmetry is what makes the husky rotate to find a
# target instead of driving in a straight line forever. The camera
# pathway's weight is set much higher than SEARCH_BIAS_WEIGHT so the
# moment a green/blue stimulus appears, the goal pathway overrides
# the search bias and the husky orients to the target.
# ---------------------------------------------------------------------------

nest.Connect(pop_hungry, motor_l, 'all_to_all',
             syn_spec={'weight': P.SEARCH_BIAS_WEIGHT, 'delay': 0.1})
nest.Connect(pop_sated, motor_r, 'all_to_all',
             syn_spec={'weight': P.SEARCH_BIAS_WEIGHT, 'delay': 0.1})


# ---------------------------------------------------------------------------
# Rest-zone brake — SNN-side coincidence detector that parks the husky
# at a blue rest tile while SATED is winning. Either input alone is
# sub-threshold; both together push the brake population over threshold
# and it inhibits both motor populations strongly, halting the husky.
# When POP_SATED loses to POP_HUNGRY via mutual inhibition (eat-kick
# decays), the brake's POP_SATED input drops below the coincidence
# threshold and motors resume firing → husky searches for food again.
# ---------------------------------------------------------------------------

near_blue = nest.Create('poisson_generator', 1, params={'rate': 0.0})
RegisterDataPack('near_blue_poisson', near_blue)

# Brake neurons get a sharply raised V_th (-15 mV vs default -55)
# so neither coincidence input alone can drive the brake. With
# weights 1000 / 350 + the NEAR_BLUE_POISSON_RATE=150 Hz drive,
# near_blue alone delivers ~110 pA mean and SATED alone ~1300 pA
# mean — both sub-threshold for a 55 mV swing. Both together
# overshoot and the brake fires hard. Earlier V_th=-25 + 300 Hz
# left near_blue alone above threshold via Poisson-tail coincidence
# (3500 brake spikes / neuron from near_blue alone in spikes3) →
# motors crushed → husky froze at first rest tile.
brake = nest.Create('iaf_psc_alpha', P.BRAKE_POP_SIZE,
                    params={'V_th': -15.0})

nest.Connect(near_blue, brake, 'all_to_all',
             syn_spec={'weight': P.WEIGHT_NEAR_BLUE_TO_BRAKE, 'delay': 0.1})
nest.Connect(pop_sated, brake, 'all_to_all',
             syn_spec={'weight': P.WEIGHT_SATED_TO_BRAKE, 'delay': 0.1})

# Brake → both motor populations: strong inhibition. When brake fires
# at ~50 Hz across the 20-neuron pool, the total inhibitory current
# into each motor neuron overwhelms the DC motor bias + any camera
# input, and the wheel commands drop to zero.
nest.Connect(brake, motor_l, 'all_to_all',
             syn_spec={'weight': P.BRAKE_INHIB_WEIGHT, 'delay': 0.1})
nest.Connect(brake, motor_r, 'all_to_all',
             syn_spec={'weight': P.BRAKE_INHIB_WEIGHT, 'delay': 0.1})

# Expose the brake population's voltage for the SNN observer. When
# the husky parks at a rest tile, hungry_v should be low, sated_v
# high, AND brake_v high — the snn diag log makes the AND-gate
# behaviour visible.
bd.LeakyIntegratorAlpha(nest, 'brake_voltage', brake, n=1,
                        conn_spec='all_to_all')


# ---------------------------------------------------------------------------
# Spike recorders — one per top-level population. drive_update reads
# each recorder's n_events per tick, computes the delta since last
# read, and emits a FORAGING_DIAG spikes line so the run produces an
# auditable per-population spike count time series. The post-run
# `check_functionality.sh` totals them and asserts each is non-zero,
# guarding against silent "all-zero" runs where a tuning regression
# leaves a population silent through the whole sim.
# ---------------------------------------------------------------------------

def _named_spike_recorder(name, src_pop):
    # record_to='ascii' writes spike events to a file in /tmp instead
    # of accumulating them in memory. NEST's `n_events` counter still
    # increments and is read back via JsonDataPack each TF tick —
    # that's all drive_update needs to compute spike deltas. Without
    # this the default 'memory' backend keeps every spike in the
    # node's status dict, JsonDataPack serialises the dict per tick,
    # and the run grinds to a halt (60× slower than realtime) by
    # ~10 s of sim time.
    sr = nest.Create('spike_recorder', params={
        'record_to': 'ascii',
        'label': f'/tmp/foraging_husky_{name}',
    })
    nest.Connect(src_pop, sr)
    RegisterDataPack(name, sr)
    return sr


_named_spike_recorder('pop_hungry_spikes', pop_hungry)
_named_spike_recorder('pop_sated_spikes', pop_sated)
_named_spike_recorder('motor_l_spikes', motor_l)
_named_spike_recorder('motor_r_spikes', motor_r)
_named_spike_recorder('brake_spikes', brake)
