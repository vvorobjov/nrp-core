import numpy as np
import torch
import json

class ForJSONEncoder(json.JSONEncoder):
	def default(self, obj):
		if isinstance(obj, np.ndarray):
			return obj.tolist()
		elif isinstance(obj, torch.Tensor):
			return obj.tolist()
		return json.JSONEncoder.default(self, obj)

def for_json_dump(data):
	return json.dumps(data, cls=ForJSONEncoder)

def dictionary_to_tensor(the_dict, key_name):
	target_item = the_dict[key_name]
	for key in target_item.keys():
		target_item[key] = torch.Tensor(target_item[key])
