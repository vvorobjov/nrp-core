# Foraging Husky — drive-state SNN demo

A reference experiment whose **control is genuinely in NEST**, not
in Python TFs. The husky must search for food when hungry, eat it,
look for rest zones when sated, **park at a rest zone until hunger
returns**, then resume searching for food. Every state transition,
every steering bias and the brake itself is a population dynamic
inside the SNN; the TFs only route data and write randomised arena
layouts.

Tracked in EBR2-32.

## The SNN — five populations + their wiring

```
                              tonic Poisson (60 Hz, w=4000 pA)
                                          |
                                          v
   eat_kick_dc (DC source set by   ┌──────────────┐    mutual inhibition
   drive_update on consumption     │ POP_HUNGRY   │←──────────┐
   events, held for                │ 50 × iaf_psc │           │
   EAT_KICK_DURATION_TICKS)        └──────┬───────┘           │
        |                                 │                   │
        v                                 │ search bias       │
   ┌──────────────┐    mutual inhib       │ (→ motor_l only)  │
   │ POP_SATED    ├────────────────────────┘                  │
   │ 50 × iaf_psc │                                           │
   └──────┬───┬───┘                                           │
          │   │                                               │
   search │   │ AND-gate                                      │
   bias   │   │ input                                         │
   (→     │   │                                               │
   motor_r│   │                                               │
   only)  │   v                                               │
          │  ┌─────────────┐    near_blue_poisson rate        │
          │  │ BRAKE       │←─── (drive_update sets to 150 Hz │
          │  │ 20 × iaf_psc│      when husky_to_rest dist < 0.9 m)
          │  └──────┬──────┘                                  │
          │         │ strong inhibition                       │
          │         │ (BRAKE_INHIB_WEIGHT = -2000 pA)         │
          v         v                                         │
   ┌──────────────────────┐         ┌──────────────────────┐  │
   │ MOTOR_L              │         │ MOTOR_R              │  │
   │ 20 × iaf_psc         │         │ 20 × iaf_psc         │  │
   │                      │         │                      │  │
   │  ← cam_green_right   │         │  ← cam_green_left    │  │
   │    (Braitenberg 2b)  │         │    (crossing)        │  │
   │  ← cam_blue_right    │         │  ← cam_blue_left     │  │
   │  ← cam_grey_left     │         │  ← cam_grey_right    │  │
   │    (inhibitory)      │         │    (inhibitory)      │  │
   │  ← motor_bias_dc     │         │  ← motor_bias_dc     │  │
   └──────────┬───────────┘         └──────────┬───────────┘  │
              │                                │              │
              v                                v              │
   ┌──────────────────────┐         ┌──────────────────────┐  │
   │ motor_l_voltage      │         │ motor_r_voltage      │  │
   │ (LeakyIntegrator)    │         │ (LeakyIntegrator)    │  │
   └──────────┬───────────┘         └──────────┬───────────┘  │
              │ V_m read                       │ V_m read     │
              v                                v              │
                       motors_to_husky.py                     │
                              │                               │
                              v                               │
                  husky::*_joint velocity                     │
                                                              │
                       (POP_HUNGRY recovers via tonic         │
                        once eat_kick_dc decays back to 0;    │
                        completes the HUNGRY → SATED → HUNGRY │
                        cycle.)                               │
```

## Run

From inside the canonical jammy container
(`nrp-local/nrp-nest-gazebo:local`):

```bash
cd /workspace/examples/foraging_husky/
NRPCoreSim -c simulation_config.json
```

Default `SimulationTimeout` is 60 s — long enough to see ~4-5 full
drive cycles under nominal parameters.

For the host-side compose flow with browser-accessible visualisation
(EBR2-32 promoted this to the mandatory acceptance gate):

```bash
bash examples/run_docker-compose_example.sh --foraging          # xvfb (headless)
bash examples/run_docker-compose_example.sh --foraging --xpra   # xpra HTML5
```

xpra HTML5 is at `http://localhost:${NRP_XPRA_HOST_PORT:-9876}`.
The compose wrapper pipes the run through `check_functionality.sh`
which asserts all five functional gates (see below) and exits 1 if
any fail.

## How to tell the SNN is actually doing the work

The compose log is sprinkled with `FORAGING_DIAG` lines from each
TF — that's the single source of truth for what the experiment is
doing. Five lines matter most:

### `FORAGING_DIAG snn …` — the SNN observer

```
FORAGING_DIAG snn tick=N hungry_v=0.42 sated_v=0.05 brake_v=0.00 \
                          motor_l_v=0.81 motor_r_v=0.74
```

* **`hungry_v` / `sated_v`** — V_m on `bd.LeakyIntegratorAlpha`
  cells reading `pop_hungry` / `pop_sated`. Mutual inhibition makes
  these **anti-correlate**: when one rises the other falls. There
  is no way for a Python TF to produce that pattern — it requires
  the NEST kernel to be running the iaf_psc_alpha + Poisson + DC +
  inhibitory synapse machinery. `check_functionality.sh` asserts
  at least one tick where `hungry_v > sated_v` AND at least one
  where `sated_v > hungry_v`; that's the cleanest functional proof
  that the SNN is actually arbitrating.

* **`brake_v`** — V_m on the brake population's readout. The brake
  is a coincidence detector (POP_SATED AND near_blue both above
  threshold → brake fires); its V_m should go from ~0 during
  search to high during a park-at-rest event. Watching `brake_v`
  jump around the same time `min_rest` drops below 0.9 m proves
  the AND gate is wired correctly.

* **`motor_l_v` / `motor_r_v`** — V_m on the motor leaky-integrator
  readouts. Wheel velocities are `min(norm_l, norm_r) * MAX_FORWARD_VEL`
  forward + `(norm_r - norm_l) * MAX_ROT_VEL` rotational, where
  `norm_*` is `V_m / VOLTAGE_SCALE` clamped to [0, 1]. When the
  brake fires you'll see both motor voltages drop sharply.

### `FORAGING_DIAG drive …` — host-side state

```
FORAGING_DIAG drive tick=N state=SATED husky_xy=(1.20,-0.30) \
                            food_0=(-2.10,1.50) dist0=3.92 \
                            min_rest=0.65 near_blue=1 \
                            eat_amp=6000 sated_ticks_left=412
```

* **`state`** — current drive label (HUNGRY vs SATED), derived from
  `_eat_kick_remaining > 0`. Lets you spot the transitions
  visually.
* **`min_rest`** — minimum distance to any rest tile. When this
  drops below `REST_DISTANCE_M` (0.9 m) and `near_blue` flips to 1,
  the brake pathway is armed; if the husky is also SATED, the
  brake fires.
* **`eat_amp` / `sated_ticks_left`** — the eat-kick DC amplitude
  and the remaining hold ticks. Eat events set this to 800 (TF
  calls), decrements per tick.

### `FORAGING_DIAG ate …` — consumption events (un-throttled)

```
FORAGING_DIAG ate food_2 husky_xy=(0.85,-1.20) respawn=(-2.40,0.30)
```

Logged every time `drive_update` triggers an eat. Records which
food was consumed, the husky's position at the moment, and the
new random respawn pose. The check script counts these — at
least one is required for the run to be GREEN.

### `FORAGING_DIAG init …` — randomised arena layout

```
FORAGING_DIAG init food_0 spawn=(2.40,-1.10)
FORAGING_DIAG init rest_2 spawn=(-0.30,2.80)
```

Each run prints seven of these at startup (`FOOD_COUNT=4` plus
`REST_COUNT=3`). Confirms the arena is freshly randomised — the
SDF default poses are bootstrap placeholders only.

### `FORAGING_DIAG cam …` — what the camera sees

```
FORAGING_DIAG cam tick=N empty=0 green_l=180.0 green_r=42.0 \
                          blue_l=0.0 blue_r=0.0 grey_l=12.0 grey_r=8.0
```

* HSV per-channel pixel fractions × peak rate. When `obstacle_to_avoidance`
  reads the drive state (via the `eat_kick_dc` amplitude readback)
  it zeros the **blue** channel while HUNGRY and the **green**
  channel while SATED — the husky only chases the target colour
  that matches its drive state.

## The five functional gates (`check_functionality.sh`)

The compose wrapper runs the check after every `--foraging` run.
Any FAIL makes the wrapper exit non-zero.

| Gate | Asserts |
|---|---|
| `camera_alive` | At least one `cam` line has any colour rate > 0 (camera works, HSV detector works). |
| `brain_alive` | At least one `motor` line has `v_l > 0` or `v_r > 0` (NEST motor populations fire). |
| `husky_alive` | At least one `motor` line has any wheel velocity ≠ 0 (control path Gazebo). |
| `snn_arbitration` | Both `hungry_v > sated_v` AND `sated_v > hungry_v` happen at some point in the run — mutual-inhibition arbitration is live. |
| `ate_anything` | At least one `ate` line (the husky reached a food cylinder and consumed it). |

## Expected emergent cycle

Under default parameters, with `SimulationTimeout=60 s`, you should
see ~4-5 full cycles. One cycle looks like:

1. **HUNGRY search.** `pop_hungry` is winning. The asymmetric
   `pop_hungry → motor_l` search bias rotates the husky in place
   while `motor_bias_dc` keeps a small forward bias. As the camera
   sweeps the arena, when a green pixel cluster enters the FOV the
   `cam_green_*` Braitenberg-2b crossing pulls the contralateral
   motor up; the husky drives toward the food.

2. **Eat.** Husky reaches `EAT_DISTANCE_M = 1.0 m` of any green
   cylinder → `drive_update` logs `FORAGING_DIAG ate …`,
   re-randomises the cylinder's pose, sets `eat_kick_dc.amplitude`
   to 6000 pA for the next 800 TF calls.

3. **SATED transition.** The DC kick into POP_SATED + mutual
   inhibition flips the winner; `sated_v` rises, `hungry_v` falls.
   `obstacle_to_avoidance` reads back the DC amplitude (its
   "drive state indicator") and zeros the green camera channel —
   the husky no longer chases food.

4. **Approach a rest zone.** With green gated off, the blue
   Braitenberg crossing pulls the husky toward the nearest blue
   tile. The asymmetric `pop_sated → motor_r` search bias kicks in
   between sightings.

5. **Park.** When `min_rest < 0.9 m` AND POP_SATED is still
   winning, the brake population's coincidence-detector inputs
   both clear threshold. Brake fires at ~50 Hz × 20 neurons →
   each motor gets ~ -4000 pA of inhibition. Wheels stop. Husky
   sits at the rest tile.

6. **Re-HUNGRY.** Over `EAT_KICK_DURATION_TICKS = 800` TF calls
   (~8 s), the eat-kick DC decays and the tonic Poisson recovers
   POP_HUNGRY's firing. The brake's POP_SATED input drops below
   threshold; brake silent; motors resume firing. Husky leaves
   the rest tile to look for more food.

7. **Repeat.** Cycle continues for the rest of the sim window.

## Parameter table

All tunables live in **`params.py`**. Brain, TF files and the unit
tests under `tests/test_tfs.py` import from this module — no magic
numbers are duplicated. Key knobs:

| Name | Default | Meaning |
|---|---|---|
| `NEST_RNG_SEED` | 4242 | NEST master RNG seed for reproducibility |
| `DRIVE_POP_SIZE` | 50 | neurons per drive population (HUNGRY / SATED) |
| `TONIC_HUNGER_RATE` | 60 Hz | constant Poisson drive into POP_HUNGRY |
| `TONIC_TO_DRIVE_WEIGHT` | 4000 pA | tonic Poisson → POP_HUNGRY weight |
| `DRIVE_INHIB_WEIGHT` | -1500 pA | HUNGRY ↔ SATED mutual inhibition |
| `EAT_KICK_AMPLITUDE` | 100000 pA | DC kick into POP_SATED on eat events |
| `EAT_KICK_DURATION_TICKS` | 800 | how long the kick is held (TF calls) |
| `GOAL_POISSON_PEAK` | 2000 Hz | full-half-image green/blue → Poisson rate |
| `OBSTACLE_POISSON_PEAK` | 400 Hz | full-half-image grey → reflex Poisson rate |
| `MOTOR_POP_SIZE` | 20 | neurons per motor population |
| `MOTOR_BIAS_DC_AMPLITUDE` | 300 pA | sub-threshold DC keeping motors near-firing |
| `SEARCH_BIAS_WEIGHT` | 80 pA | drive-pop → ipsilateral motor (asymmetric search) |
| `WEIGHT_GREEN_TO_MOTOR` | 2500 pA | green Braitenberg crossing |
| `WEIGHT_BLUE_TO_MOTOR` | 2500 pA | blue Braitenberg crossing |
| `WEIGHT_OBSTACLE_TO_MOTOR` | -1000 pA | grey ipsilateral inhibition |
| `BRAKE_POP_SIZE` | 20 | brake-population size |
| `WEIGHT_NEAR_BLUE_TO_BRAKE` | 1000 pA | near_blue_poisson → brake (sub-threshold alone) |
| `WEIGHT_SATED_TO_BRAKE` | 350 pA | POP_SATED → brake (sub-threshold alone) |
| `BRAKE_INHIB_WEIGHT` | -2000 pA | brake → motor inhibition (motor stop) |
| `REST_DISTANCE_M` | 0.9 m | near_blue activation radius |
| `NEAR_BLUE_POISSON_RATE` | 150 Hz | near_blue_poisson rate when armed |
| `EAT_DISTANCE_M` | 1.0 m | proximity threshold for an eat event |
| `RESPAWN_BOUNDS_XY` | ((-3.5,3.5),(-3.5,3.5)) | random respawn arena |
| `FOOD_COUNT` / `REST_COUNT` | 4 / 3 | model counts (must match world SDF) |
| `MAX_FORWARD_VEL` / `MAX_ROT_VEL` | 5 / 7 rad/s | wheel angular velocity ceilings |
| `VOLTAGE_SCALE` | 0.03 mV | motor_l/r V_m saturation point |
| `DIAG_LOG_EVERY_N_TICKS` | 50 | one diag line per N TF calls |

## Files

| File | Purpose |
|---|---|
| `simulation_config.json` | Standalone NRPCoreSim config (gazebo_grpc + nest_json, in-image NEST). |
| `simulation_config_docker-compose.json` | Compose variant: nest as `nest-simulator:3.9` sidecar, gazebo as `nrp-gazebo`. |
| `foraging_world.sdf` | Compact world: ground plane + 4 food cylinders + 3 rest boxes + 3 obstacles. SDF poses are bootstrap placeholders — drive_update re-randomises on tick 1. |
| `husky.sdf` | Standard husky model with NRPGazeboGrpcLinkControllerPlugin so `husky::base_link` is readable. |
| `brain.py` | NEST topology — drive populations, motor populations, brake coincidence detector, all leaky-integrator readouts. |
| `brain_nest_server.py` | Mirror of brain.py without nrp_core imports, for the nest-server compose variant. |
| `params.py` | Single source of truth for all tunable constants. |
| `camera_to_sensory.py` | Camera frame → per-channel left/right rates (HSV detector). |
| `obstacle_to_avoidance.py` | Sensory rates → NEST Poisson generators with drive-state channel gating (green off when SATED, blue off when HUNGRY) + obstacle alarm. |
| `drive_update.py` | Eat-event detection + SATED hold timer + food/rest respawn + near_blue_poisson rate + FORAGING_DIAG snn/drive/ate/init logging. |
| `motors_to_husky.py` | Motor leaky-integrator V_m → husky wheel velocities. |
| `helpers.py` | Pure-Python TF arithmetic (image_to_channel_rates, voltage_to_wheel_vels, etc) — nrp_core-free so tests/ can exercise it. |
| `check_functionality.sh` | Post-run gate parsing FORAGING_DIAG lines (5 mandatory assertions). |
| `tests/test_tfs.py` | Pytest unit tests over the pure-Python TF helpers. |
| `mosquitto.conf` | Permissive broker config for the compose flow. |

## Hacking notes

- **Adding more food / rest models**: bump `FOOD_COUNT` or `REST_COUNT`
  in `params.py`, add corresponding `<model name='food_N'>` /
  `<model name='rest_N'>` blocks to `foraging_world.sdf` with the
  matching ModelControllerPlugin, and add `@EngineDataPack` decorators
  + function parameters to `drive_update.py`. The decorator stack is
  intentionally static (nrp_core binds keyword names at decoration
  time, so you cannot loop the decorators).
- **Drive cycle balance**: if SATED phases are too short / too long,
  scale `EAT_KICK_AMPLITUDE` and `EAT_KICK_DURATION_TICKS` together —
  a longer hold gives the husky time to park, but holds the cycle
  back. If you change `DRIVE_INHIB_WEIGHT`, also rescale
  `EAT_KICK_AMPLITUDE` so the kick can still flip the winner.
- **Brake calibration**: the brake threshold is approximately
  (rate × weight × τ_syn). If you raise `REST_DISTANCE_M` or
  `NEAR_BLUE_POISSON_RATE` you may need to drop
  `WEIGHT_NEAR_BLUE_TO_BRAKE` to keep "near_blue alone" sub-threshold.
- **Verifying the SNN is in play**: when in doubt, grep the run log
  for `FORAGING_DIAG snn ` and check that `hungry_v` and `sated_v`
  swap rank multiple times. If they don't, mutual inhibition isn't
  running (likely an iaf_psc_alpha parameter or a synapse weight bug).
