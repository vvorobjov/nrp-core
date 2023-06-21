# Overview

This example shows how to vectorize NRP Core environments by wrapping NRP Core Python client in a gym environment,
and using vectorization tools provided by stable baselines.

# Files

The example consists of the following files
- master_script_vectorized.py - the main script that performs environment vectorization and learning
- nrp_env.py - gym wrapper for NRP Core Python client
- status_function.py - Status Function that allows to pass data from the engines to the master script
- grpc_taxi.py - a toy Python gRPC engine, which wraps the Taxi gym environment.
- simulation_config.json - configuration file

# Running

To start learning using the vectorized environment, simply run:
```
./master_script_vectorized.py
```