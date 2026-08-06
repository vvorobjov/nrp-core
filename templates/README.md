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

Every config placed here must, on top of being valid JSON:

- Use **only engines available in the shipped `nest-gazebo` backend**:
  `gazebo_grpc`, `nest_json`, `nest_server`, `python_json`, `python_grpc`,
  `datatransfer_grpc_engine`, plus the ROS/MQTT proxies. Engines that need
  another image (OpenSim, TVB, PyBullet, EDLUT, SpiNNaker) must not appear.
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

## Current catalog

Because the glob is per-JSON, a directory with several `simulation_config*.json`
files renders **one tile per file**. The current set is **7 tiles** across 4
directories (the `SimulationName` column is exactly the label shown in the UI):

| Dir / config | UI tile (`SimulationName`) | Engines | What it shows |
|---|---|---|---|
| `husky_braitenberg/simulation_config.json` | `husky_simulation` | gazebo_grpc + nest_json | Braitenberg-2b reflex: camera → NEST → wheels. |
| `husky_braitenberg/simulation_config_nest_server_empty_launch.json` | `husky_simulation_nest_server` | gazebo_grpc + nest_server | NEST Desktop integration variant — expects a separate `nest-server` (nest-desktop compose topology), not the single-image backend. |
| `foraging_husky/simulation_config.json` | `Foraging Husky (drive-state SNN)` | gazebo_grpc + nest_json | Drive-state SNN (HUNGRY/SATED) foraging + obstacle reflex (EBR2-32). |
| `nest_simple/simulation_config.json` | `NEST Simple (noise-driven neuron)` | nest_json | Minimal NEST-only demo: noise-driven neuron with a voltmeter readout. |
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
