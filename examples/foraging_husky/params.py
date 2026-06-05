"""Tunable constants for the foraging_husky experiment.

Single source of truth: brain.py, the TF files and the unit tests under
tests/test_tfs.py all import from this module. No magic numbers should be
duplicated in those files; if a number lives in more than one place, it
belongs here.

Tracked in EBR2-32.
"""

# ---------------------------------------------------------------------------
# Reproducibility
# ---------------------------------------------------------------------------

# Seed for NEST's master RNG. Fix-pinning here keeps the experiment
# bit-reproducible across runs at the same wall-clock load; using the
# same seed across re-runs is the contract brain.py relies on.
NEST_RNG_SEED = 4242

# ---------------------------------------------------------------------------
# Drive populations (POP_HUNGRY vs POP_SATED — mutually inhibitory)
# ---------------------------------------------------------------------------

# Number of neurons per drive population. ~50 is the size that gives a
# spike rate readable above noise at the 100-ms TF cadence used in this
# experiment without dominating the brain footprint.
DRIVE_POP_SIZE = 50

# Tonic Poisson rate (Hz) driving POP_HUNGRY upward over time. POP_SATED
# is held by mutual inhibition until a consumption event injects a
# transient current; the slow tonic into POP_HUNGRY is what makes the
# whole drive eventually flip back.
TONIC_HUNGER_RATE = 60.0

# Mutual-inhibition synaptic weight (pA). The two drive populations are
# connected hungry→sated and sated→hungry with this negative weight; the
# winner-take-all dynamic emerges from the asymmetry between the tonic
# hunger Poisson and the eating-event transient SATED kick.
#
# iaf_psc_alpha synapses take weights in pA. A single Poisson spike of
# weight W drives a peak EPSP ≈ W * τ_syn * exp(-1) / C_m mV (here
# τ_syn=2ms, C_m=250pF default), so reaching the 15mV V_th gap from
# rest requires a steady combined input around 4000 pA. The values
# here are scaled accordingly — much larger than they'd be for a
# conductance-based iaf_cond_alpha brain (cf. husky_braitenberg).
DRIVE_INHIB_WEIGHT = -1500.0

# Tonic Poisson → POP_HUNGRY weight (pA). For iaf_psc_alpha to reach
# threshold (15 mV gap, 40 MΩ membrane R), the steady current must
# exceed ~375 pA. With alpha synapses, mean current = ν * W * τ_syn
# (here τ_syn=2ms), so ν=60Hz × W=4000 pA × 2ms = 480 pA — pushes
# POP_HUNGRY into firing in the 15-25 Hz range, enough to drive
# motors through WEIGHT_DRIVE_TO_CHANNEL.
TONIC_TO_DRIVE_WEIGHT = 4000.0

# Eating-event injection: amplitude (pA) of the DC source delivered to
# POP_SATED when the husky's base_link is within EAT_DISTANCE_M of a
# food model. Empirically POP_HUNGRY at hungry_v ≈ 1 mV is firing at
# ~100 Hz/neuron × 50 neurons → ~15 nA of mutual-inhibition current
# into POP_SATED. The kick must clearly exceed that to flip the
# winner. Empirically, with DRIVE_INHIB_WEIGHT=-1500 + 50 hungry
# neurons firing tonically the integrated inhibitory PSC peaks
# routinely hit ~-60 nA, so the kick has to overshoot well past
# 30000 pA to keep pop_sated above threshold rather than clamped
# at V_reset. 100000 pA gives a clear winner-flip in 1 ms timesteps
# and the SATED-wins ticks survive across the whole hold window.
# (30000 pA used to pass snn_arbitration by numerical noise alone —
# sated_v never actually rose above 0 even during 800-tick SATED.)
EAT_KICK_AMPLITUDE = 100000.0

# How long the eat-kick stays on after a consumption event, counted
# in drive_update TF calls (not raw NEST sim ticks — NRP-core fires
# the TF roughly every 3 NEST ticks, so 800 TF calls ≈ 2400 sim
# ticks ≈ 8 s of wall-clock at the default 1 ms NEST timestep). The
# drive arbitration cycle is: tonic Poisson ramps POP_HUNGRY → eat
# event kicks POP_SATED → POP_SATED holds for this many ticks
# (blue-seeking phase) → kick decays → POP_HUNGRY recovers via tonic
# and wins again → green-seeking phase resumes. 800 fits two-to-three
# full HUNGRY → eat → SATED → re-HUNGRY → eat cycles in
# SimulationTimeout=60 s; longer windows (15 s) only allowed one
# cycle.
EAT_KICK_DURATION_TICKS = 800

# ---------------------------------------------------------------------------
# Vision (camera_to_sensory.py): HSV ranges for green / blue / grey
# ---------------------------------------------------------------------------

# OpenCV HSV space: H is 0-179, S/V are 0-255.
# Green channel — food. Hue ~60 covers a comfortable margin around pure
# green. Saturation and value lower bounds reject washed-out greens
# (e.g. ground tint) so the channel only fires on the planted food.
HSV_GREEN_LOWER = (40, 80, 60)
HSV_GREEN_UPPER = (80, 255, 255)

# Blue channel — rest zones. Hue ~120 is centre-blue.
HSV_BLUE_LOWER = (95, 80, 60)
HSV_BLUE_UPPER = (135, 255, 255)

# Grey channel — obstacles. The obstacle cylinders use Gazebo material
# `0.5 0.5 0.5` which renders close to RGB (128,128,128). Gazebo's
# default scene background (skybox horizon) renders at much higher V
# in HSV terms but with similar near-zero saturation, so the earlier
# range (V 60-200, S 0-40) caught the sky on every frame. Tighten S
# to <=15 and clamp V to 100-160 so only the obstacle cylinders fire
# this channel.
HSV_GREY_LOWER = (0, 0, 100)
HSV_GREY_UPPER = (179, 15, 160)

# Rate-scaling factor used by camera_to_sensory.py: a fully-coloured
# half-image maps to GOAL_POISSON_PEAK Hz on the left or right Poisson
# generator for that channel.
# A saturated half-image green stimulus at 2000 Hz × WEIGHT_GREEN_TO_MOTOR
# 1500 pA × τ_syn 2 ms ≈ 6000 pA mean per motor neuron — well above the
# 1000 pA DC motor bias, so the camera-channel asymmetry dominates the
# motor-population firing rate when food is in view. Without this
# headroom the DC bias drowns out the steering signal and the husky
# drives in a straight line regardless of where food is (user observation:
# "Husky is just moving straight").
GOAL_POISSON_PEAK = 2000.0

# Grey-obstacle channel kept at a modest peak — even with the tightened
# HSV-grey range above, the Gazebo skybox / ambient lighting still
# registers as low-saturation mid-V pixels at ~90 Hz from the husky's
# camera. At OBSTACLE_POISSON_PEAK=400 that produces only ~36 pA of
# inhibition per motor neuron (90/400 of the 400 Hz peak × 1000 pA
# obstacle weight × 2 ms), well below the 1000 pA DC bias so motors
# keep firing on background. When a real grey obstacle is in frame
# the rate spikes much higher and the inhibition does override the
# DC bias on the ipsilateral side.
OBSTACLE_POISSON_PEAK = 400.0

# ---------------------------------------------------------------------------
# Motor populations (MOTOR_L / MOTOR_R)
# ---------------------------------------------------------------------------

MOTOR_POP_SIZE = 20

# Synaptic weights into motor populations (pA, current-based
# iaf_psc_alpha). Goal-seeking is Braitenberg 2b: green-left → motor-
# right (turn toward food) and vice-versa. The camera channels are
# the *primary* motor drive — the tonic motor bias below only
# provides a small wandering baseline so the husky moves at all when
# nothing colourful is in view. Earlier tunings symmetrically drove
# motors from the drive populations all-to-all, which saturated both
# sides equally and made the husky drive straight regardless of the
# camera asymmetry; the user observation "now it just moves straight"
# was exactly that. Boosted weights here so even a small per-side
# camera-rate difference produces a meaningful wheel asymmetry.
WEIGHT_GREEN_TO_MOTOR = 2500.0
WEIGHT_BLUE_TO_MOTOR = 2500.0

# Obstacle weight: ipsilateral motor gets an inhibitory kick so the
# husky turns away from the obstacle, regardless of drive state.
# Scaled with WEIGHT_GREEN_TO_MOTOR so the obstacle channel overrules
# the goal pathway on a same-side conflict.
WEIGHT_OBSTACLE_TO_MOTOR = -1000.0

# Constant DC injection (pA) into both motor populations — provides a
# sub-threshold baseline. iaf_psc_alpha threshold is ~375 pA steady
# current; 300 pA alone is BELOW threshold so the motors don't fire
# from the DC bias alone — they need an additional input source.
# That input is either the SEARCH_BIAS (POP_HUNGRY → motor_l only)
# during wandering, or the camera channels (Braitenberg 2b) when a
# coloured target is in view. The asymmetric routing of those two
# input sources is what makes the wheel command asymmetric, which is
# what makes the husky steer. A symmetric strong DC bias drowns out
# the asymmetry and the husky just drives straight.
MOTOR_BIAS_DC_AMPLITUDE = 300.0

# Drive-population → single motor weight (pA). POP_HUNGRY connects
# all-to-all to motor_l only (and POP_SATED to motor_r only). With
# POP_HUNGRY firing at ~30 Hz × 50 neurons × 80 pA × 2 ms = 240 pA
# extra to motor_l, motor_l sits at 300+240=540 pA (above threshold)
# while motor_r sits at 300 pA (sub-threshold). That's enough
# asymmetry to rotate the husky during the no-target search phase,
# but small enough that the SYMMETRIC camera-channel signal (when a
# target is centered ahead) clearly outvotes the search bias and the
# husky drives forward instead of veering off. Earlier 200 pA gave
# ~600 pA search current per motor, on par with the camera-channel
# input and visibly tugging the husky off centered targets.
SEARCH_BIAS_WEIGHT = 80.0

# ---------------------------------------------------------------------------
# Rest-zone brake (SNN-side coincidence detector that parks the husky
# at a blue rest tile while SATED is winning)
# ---------------------------------------------------------------------------

# Distance threshold (m) under which the husky is considered "at" a
# rest tile and the brake pathway is armed. The blue tiles are 0.8 m
# squares, so 0.9 m captures the husky once a corner of its body
# overlaps the tile.
REST_DISTANCE_M = 0.9

# Rate (Hz) of the near_blue_poisson generator while the husky is
# within REST_DISTANCE_M of any rest tile. Zero otherwise. Feeds the
# brake population's coincidence detector. 300 Hz delivered enough
# Poisson-tail coincidence on weight 1000 to fire the brake by
# itself (~3500 spikes per brake neuron from near_blue alone in
# spikes3) so the husky parked permanently at the first rest tile
# it bumped into. 150 Hz halves the tail energy and keeps brake
# silent until POP_SATED also drives it (true AND-gate behaviour).
NEAR_BLUE_POISSON_RATE = 150.0

# Brake population size + weights. The brake is a coincidence detector:
# it fires only when BOTH near_blue_poisson AND POP_SATED are
# simultaneously active. Brake neurons run with V_th = -15 mV
# (set in brain.py), giving a 55 mV swing threshold. With these
# weights and NEAR_BLUE_POISSON_RATE=150 Hz, near_blue alone
# delivers ~110 pA mean (~4 mV) and SATED alone ~1300 pA mean
# (~52 mV) — both sub-threshold individually. Both together
# overshoot and the brake fires hard, parking the husky at a
# rest tile while SATED but releasing it when HUNGRY returns.
BRAKE_POP_SIZE = 20
WEIGHT_NEAR_BLUE_TO_BRAKE = 1000.0
WEIGHT_SATED_TO_BRAKE = 350.0

# Brake → motor inhibition weight (pA). With the brake population at
# ~50 Hz × 20 neurons = 1000 Hz spike rate into each motor population
# all-to-all, a weight of -2000 pA delivers ~ -4000 pA mean current
# per motor neuron — a hard inhibition that crushes the DC motor
# bias and any camera-channel input, so the husky physically stops.
BRAKE_INHIB_WEIGHT = -2000.0

# ---------------------------------------------------------------------------
# Eating mechanics (motors_to_husky.py + drive_update.py)
# ---------------------------------------------------------------------------

# Distance threshold (m) under which a food model counts as "eaten".
# The husky's body radius plus a margin large enough that the
# Braitenberg-2b approach trajectory (which tends to orbit the target
# at a ~1 m radius as the camera-asymmetry signal swings around
# closing distance) actually closes the gap and fires an eat event.
# Earlier 0.6 m had the husky stuck orbiting food at ~0.95 m without
# triggering — a one-cycle run instead of the intended two-or-three.
EAT_DISTANCE_M = 1.0

# When a food model is eaten, respawn it at a random free location
# within this bounding box (uniform on [x_min, x_max] × [y_min, y_max]).
# Tightened to the inner arena so respawned cylinders stay within the
# husky's drivable area (the 20×20 m ground plane gets edge-clipped
# in xpra view at ±5 m; ±3.5 m keeps everything well inside camera
# reach from any arena position).
RESPAWN_BOUNDS_XY = ((-3.5, 3.5), (-3.5, 3.5))

# How many food models the world ships. Names are food_0 .. food_(N-1).
FOOD_COUNT = 4
# How many rest models the world ships. Names are rest_0 .. rest_(N-1).
REST_COUNT = 3

# ---------------------------------------------------------------------------
# Motor command conversion (motors_to_husky.py)
# ---------------------------------------------------------------------------

# Maximum forward wheel angular velocity (rad/s) at full motor drive.
# Empirically the husky shoots forward at ~5× the rad/s value here
# in practice (Gazebo wheel radius + chassis dynamics), so 5 rad/s
# nets a ground speed around 1.7 m/s. Fast enough to cross the arena
# (5-7 m wide) in 3-4 seconds, so a full HUNGRY → eat → SATED →
# blue approach → re-HUNGRY → eat cycle fits comfortably in
# SimulationTimeout=60 s.
MAX_FORWARD_VEL = 5.0
# Rotational scaling. Differential drive on a husky. Higher than
# MAX_FORWARD_VEL so the SEARCH_BIAS asymmetry produces a tight
# enough turn radius to sweep the arena within SimulationTimeout.
MAX_ROT_VEL = 7.0

# Camera frame width in pixels — must match the Gazebo camera config in
# husky.sdf. Used by camera_to_sensory for the left/right split.
CAMERA_FRAME_WIDTH = 320
CAMERA_FRAME_HEIGHT = 240

# ---------------------------------------------------------------------------
# Functional-check diagnostics
# ---------------------------------------------------------------------------

# Every Nth TF tick, each TF prints a single grep-friendly
# `FORAGING_DIAG <subsys> ...` line capturing its salient state.
# check_functionality.sh asserts that the captured log contains
# evidence the SNN actually drove the husky — without this the
# compose runs would exit 0 even when NEST does nothing and the
# robot sits still. 50 ticks at the default TF cadence is roughly
# every 5 simulated seconds, which is dense enough to detect activity
# inside a 60 s sim without spamming the log.
DIAG_LOG_EVERY_N_TICKS = 50
