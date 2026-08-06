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
- Use the **single-backend variant** of the experiment (everything launched
  in-image by `NRPCoreSim`), not a distributed / sidecar config that expects
  separate `nest-server` or `nrp-gazebo` containers.

## Current catalog

| Template | Engines | What it shows |
|---|---|---|
| `husky_braitenberg` | gazebo_grpc + nest_json | Braitenberg-2b reflex: camera → NEST → wheels. |
| `foraging_husky` | gazebo_grpc + nest_json | Drive-state SNN (HUNGRY/SATED) foraging + obstacle reflex (EBR2-32). |
| `nest_simple` | nest_json | Minimal NEST-only demo: noise-driven neuron with a voltmeter readout. |
| `tf_exchange` | python_json / python_grpc | Two Python engines exchanging a datapack via a transceiver function. |

## Not in the catalog

- **`opensim_tvb`** — removed (EBR2-117). It needs the OpenSim + TVB Python
  modules, which are absent from the `nest-gazebo` image, and no
  `tvb-opensim` backend variant is published, so the tile could be listed
  but never launched. The experiment is kept under
  [`../examples/opensim_tvb/`](../examples/opensim_tvb/). It can return to
  the catalog once a `tvb-opensim` backend variant exists (separate work).
