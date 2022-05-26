# How to run containerized experiments

1. Build docker images of the simulation components

    1. For the NRP-core component, use the script `"full-nrp/install.sh"`: `"nrp:v0"` is the name and tag of the docker image. 
    2. For the simulator component of the `"tf_exchange"` example, use the script `"minimal-nrp/install.sh"`: `"nrp_minimal:v0"` is the name and tag of the docker image.  For the `"pysim_examples/opensim_control"` example, use the script `"nrp-opensim/install.sh"`: the name and tag of the docker image will be`"nrp_opensim:v0"`. For the `"nest_simple"` example, use the script `"nrp-nest/install.sh"`: the name and tag of the docker image will be`"nrp_nest:v0"`. For the `"husky_braitenberg_json"` example, use the script `"nrp-gazebo/install.sh"`: the name and tag of the docker image will be`"nrp_gazebo:v0"`.
    3. Install the simulator image in Manager host, install the NRP-core image in Follower host.

2. Set up the Manager and Follower hosts

    1. To be able to use docker without sudo:

           sudo groupadd docker
           sudo usermod -aG docker $USER
           su -s ${USER}

    2. Copy the folder "manager" to the Manager host, and run the `"install.sh"` script included.

    3. In order to start the simulator containers, the Manager REST server has be started on the Manager host; run:

           python3 manager/server/docker_launcher.py

       The port on which the server will listen for requests can be specified with the `"--port"` parameter, its default value is `"5002"`.

    4. If NRP-core on the Follower host is  containerized (see cases 2 and 4, `Running the example experiments in multiple docker containers` section of the guide), start the NRP-core component container

           docker run -it --network="host" --name <container name> nrp:v0

3. Run an example experiment:

    Change directory to one of the example experiment (`nrp-core/examples/tf_exchange`) in the NRP-core host/container, and run:

        NRPCoreSim -c simulation_config_docker.json

Note:

The example `"pysim_examples/opensim_control"` uses the docker image `"nrp_opensim:v0"`, `"tf_exchange"` uses the docker image `"minimal_nrp:v0"`, `"nest_simple"` uses the docker image `"nrp_nest:v0"`, and `"husky_braitenberg_json"` uses the docker image `"nrp_gazebo:v0"`

The IP addresses of the local and remote hosts have to be specified in the experiment configuration file:
- `"DaemonAddress"` is the IP address of the remote Manager server.
- `"RegistrationServerAddress"` is IP address of NRP-core

As an example of `"pysim_examples/opensim_control"`, the following simulator-specific configuration parameters  are required too:
- `"WorldFileName"` is the path of simulation model files inside the remote container. On the Manager host, the manager (i.e. docker_launcher.py) REST server will store them at the specified location (defaults to `"/share/experiment_data/<engine name>"`)
- `"PythonFileName"` is the python script executed by python_json_engine inside remote the container. In the example is left unchanged, so by default is `"sim_engine.py"`.

For different engines, required configuration parameters are different. Please follow their `"config_schemas"` to configure the simulation.
