from .ConvertLib import dictionary_to_tensor
from .ConvertLib import for_json_dump

from nrp_client import NrpCore
from nrp_protobuf import dump_pb2

import numpy as np
import json


class NrpCores():
	"""docstring for NrpCores"""
	def __init__(self,
				nrp_server_address = "localhost:50050",
				config_file = "simulation_config.json",
				log_output = None):
		super(NrpCores, self).__init__()
		
		self.nrp_core = NrpCore(nrp_server_address,
			config_file=config_file,
			log_output=log_output)
		self.nrp_core.initialize()

	def step(self, model_paras):
		trg_policy = dump_pb2.String()
		trg_policy.string_stream = for_json_dump(model_paras)
		self.nrp_core.set_proto_datapack("net_weight", "ppo_model", trg_policy)
		done, observations = self.nrp_core.run_loop(1)
		
		cur_data = json.loads(observations[0].string_stream)
		return cur_data["data"]

	def reset(self):
		self.nrp_core.reset()

	def shutdown(self):
		self.nrp_core.shutdown()
		
