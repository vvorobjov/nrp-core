# NRP experiment templates (UI catalog)

This directory is the source of the **experiment catalog shown in the NRP
web UI**. `nrp-user-scripts` mounts it read-only at `/nrp-templates`, and
the proxy builds the catalog by globbing `*/*.json` here — i.e. **every
`simulation_config*.json` under every subdirectory becomes one launchable
tile**, read for its `SimulationName` and `SimulationDescription`.

Templates are the curated, demo-ready subset of `examples/`. A far richer
experiment set lives under [`../examples/`](../examples/); those are run
directly with `NRPCoreSim` (or docker-compose) and are **not** part of the
mounted catalog.

## Requirements for a template to work in the UI

A UI tile launches through a different path than `examples/` do: UI → proxy →
backend (`nrp-backend`) → `NRPCoreSim`, on the single-image `nest-gazebo`
backend. An `examples/` config authored for the CLI / docker-compose run path
is **not** automatically launchable there — dropping a copy into `templates/`
(even after adding a `datatransfer_grpc_engine`) is necessary but **not
sufficient**, and can produce a tile that renders but transitions
`started → failed` at launch. Making an experiment backend-runnable is a
per-experiment adaptation, tracked in **EBR2-120**.

Every config placed here must, on top of being valid JSON:

- Use **only engines available in the shipped `nest-gazebo` backend**:
  `gazebo_grpc`, `nest_json`, `python_json`, `python_grpc`,
  `datatransfer_grpc_engine`, plus the ROS/MQTT proxies. (`nest_server` is also
  available as a client engine, but it talks to an *external* NEST server, so a
  config using it only launches when a `nest-server` sidecar is present — the
  nest-desktop compose topology.) Engines that need another image (OpenSim, TVB,
  PyBullet, EDLUT, SpiNNaker) must not appear.
- Include a **`datatransfer_grpc_engine`** entry — the backend refuses to
  launch a template without one (`experiment_configuration.validate` raises
  `No datatransfer_grpc_engine in experiment configuration`). The backend
  overrides its `simulationID` / `MQTTBroker` / `MQTTPrefix` at run time, so
  the `localhost:1883` placeholder here is fine.
- Carry a friendly **`SimulationName`** and **`SimulationDescription`** so the
  tile renders with a human-readable label.
- Prefer the **single-backend variant** of the experiment (everything launched
  in-image by `NRPCoreSim`) over a distributed / sidecar config that expects
  separate `nest-server` or `nrp-gazebo` containers. The one existing exception
  is `husky_braitenberg/simulation_config_nest_server_empty_launch.json`, the
  NEST Desktop integration tile, which only runs under the nest-desktop compose
  topology — see the table below.
- **Launch-gate it.** Before adding a tile, confirm it clones, creates, starts
  and advances the sim clock on the live `nest-gazebo` stack
  (`nrp-user-scripts` husky/acceptance gate). Static JSON validity does not
  prove backend-runnability.

## Current catalog

Because the glob is per-JSON, a directory with several `simulation_config*.json`
files renders **one tile per file**. The current set is **5 tiles** across 2
directories (the `SimulationName` column is exactly the label shown in the UI):

| Dir / config | UI tile (`SimulationName`) | Engines | What it shows |
|---|---|---|---|
| `husky_braitenberg/simulation_config.json` | `husky_simulation` | gazebo_grpc + nest_json | Braitenberg-2b reflex: camera → NEST → wheels. |
| `husky_braitenberg/simulation_config_nest_server_empty_launch.json` | `husky_simulation_nest_server` | gazebo_grpc + nest_server | NEST Desktop integration variant — expects a separate `nest-server` (nest-desktop compose topology), not the single-image backend. |
| `tf_exchange/simulation_config.json` | `tf_exchange Python` | python_json ×2 | Two Python (JSON) engines exchanging a datapack via a transceiver function. |
| `tf_exchange/simulation_config_datatransfer.json` | `tf_exchange_mqtt_dump_test` | python_json ×2 | Same, exercising the MQTT data-dump path. |
| `tf_exchange/simulation_config_grpc.json` | `tf_exchange gRPC` | python_grpc ×2 | Same exchange over the gRPC Python engines. |

## Not in the catalog

- **`opensim_tvb`** — removed (EBR2-117). It needs the OpenSim + TVB Python
  modules, which are absent from the `nest-gazebo` image, and no
  `tvb-opensim` backend variant is published, so the tile could be listed
  but never launched. The experiment is kept under
  [`../examples/opensim_tvb/`](../examples/opensim_tvb/). It can return to
  the catalog once a `tvb-opensim` backend variant exists (separate work).
- **`foraging_husky`** and **`nest_simple`** — good demo candidates, but a
  straight copy of their `examples/` configs fails at backend launch (they are
  authored for the CLI / compose run path). They stay under
  [`../examples/foraging_husky/`](../examples/foraging_husky/) and
  [`../examples/nest_simple/`](../examples/nest_simple/), where they run via
  `NRPCoreSim` / docker-compose. Promoting them to the catalog needs the
  per-experiment backend adaptation tracked in **EBR2-120**.
