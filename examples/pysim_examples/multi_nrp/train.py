from nrp_core.engines.py_sim.NRPThreads import LaunchNRPs
import time

if __name__ == '__main__':
	env_name = "CartPole-v1"
	config_json = "env_engines/simulation_config_train.json"
	thread_num = 2
	batch_size = 32
	nrps = LaunchNRPs(env_name, config_json, thread_num, batch_size)
	
	nrps.running()

	nrps.shutdown()


	# Something problem after anything shutdown
# EOF 