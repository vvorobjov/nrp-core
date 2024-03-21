from multiprocessing import Process, Manager
import time

from .NrpCoresProcess import NrpCores
from .Trainer import PPOTrainer

def nrp_process(nrp_id, config_json, flag_dict, data_buffer, model_paras):
	nrp_name = 'nrp' + str(nrp_id)
	nrp_server_address = "localhost:5005" + str(nrp_id)
	log_file_name = ".out" + str(nrp_id) + ".log"
	cur_nrp = NrpCores(nrp_server_address,
		config_json,
		log_output=log_file_name)
	step_num = flag_dict['step_num']
	step_counter = 0
	max_len = 0
	flag_dict['start'] = True

	run_flag = True
	while step_counter < step_num:
		if run_flag:
			cur_data = cur_nrp.step(model_paras['use_paras'])
			if len(cur_data) > 0:
				data_buffer[nrp_id] = cur_data[:]
				run_flag = False
			step_counter += 1
		elif flag_dict['update']:
			data_buffer[nrp_id] = []
			run_flag = True
		else:
			time.sleep(1e-3)

	cur_nrp.shutdown()
	flag_dict['end'] = True


def nrp_func(nrp_id, config_json, flag_dict, data_buffer, model_paras):
	nrp_process(nrp_id, config_json,
		flag_dict, data_buffer, model_paras)


class LaunchNRPs(object):
	"""docstring for LaunchNRPs"""
	def __init__(self, env_name, config_json, thread_num, batch_size):
		super(LaunchNRPs, self).__init__()
		self.model = PPOTrainer(env_name, 1)

		self.thread_num = thread_num
		self.batch_size = batch_size
		self.thread_list = []

		self.model_paras = Manager().dict()
		self.data_buffer = Manager().list([[],[]])
		self.flag_dict = Manager().dict()

		self.model_paras['paras'] = self.model.get_parameters().copy()
		self.model_paras['use_paras'] = self.model.get_parameters().copy()
		self.flag_dict['start'] = False
		self.flag_dict['update'] = False
		self.flag_dict['end'] = False
		self.flag_dict['step_num'] = 1000

		for i in range(thread_num):
			cur_nrp_id = i
			self.thread_list.append(Process(target=nrp_func,
				args=(cur_nrp_id, config_json, self.flag_dict, self.data_buffer, self.model_paras,)))
		
		for i in range(thread_num):
			self.thread_list[i].start()
			
	def running(self):
		sim_start_flag = False
		temp_data_buffer = []
		for i in range(self.thread_num):
			temp_data_buffer.append([])

		while not self.flag_dict['end']:
			if not self.flag_dict['start'] and not sim_start_flag:
				time.sleep(0.01)
				continue
			sim_start_flag = True

			done_flag = True
			for i in range(self.thread_num):
				if len(self.data_buffer[i]) == 0:
					done_flag = False
					self.flag_dict['update'] = False
				else:
					temp_data_buffer[i] = self.data_buffer[i].copy()
			
			if done_flag:
				cur_data_buffer = []
				for i in range(self.thread_num):
					cur_data_buffer.extend(temp_data_buffer[i])
				self.flag_dict['update'] = True
				self.model.model_train(cur_data_buffer)
				self.model_paras['paras'] = self.model.get_parameters()
				self.model_paras['use_paras'] = self.model_paras['paras']
			time.sleep(0.001)

	def shutdown(self):
		for i in range(self.thread_num):
			self.thread_list[i].join()
		self.model.model_save("ppo_cartpole")
		self.model.shutdown()


		
