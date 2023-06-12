import time
from nrp_core.client import NrpCore

nrp_1 = "10.1.1.163:50050"
nrp_2 = "10.1.1.102:50050"
compose_file = "/home/HOME/nrp-core/examples/husky_braitenberg/docker_compose_deploy_husky.yaml"
config_file = "/home/HOME/nrp-core/examples/husky_braitenberg/simulation_config_docker-compose.json"

# This line will run the compose file in the next step

nrp_core_1 = NrpCore(nrp_1, compose_file=compose_file,config_file=config_file)
print("NRPCore 1 inited !!!")


nrp_core_2 = NrpCore(nrp_2, compose_file=compose_file,config_file=config_file)
print("NRPCore 2 inited !!!")

nrp_core_1.initialize()
print("NRPCore 1 initialized !!!")


nrp_core_2.initialize()
print("NRPCore 2 initialized !!!")


time.sleep(10)
for i in range(100):
    nrp_core_1.run_loop(1)
    nrp_core_2.run_loop(1)

nrp_core_1.shutdown()
nrp_core_2.shutdown()
