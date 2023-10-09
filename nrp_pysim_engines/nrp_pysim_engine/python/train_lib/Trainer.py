import numpy as np
import torch as th

from stable_baselines3.common.utils import obs_as_tensor, safe_mean
from stable_baselines3.common.env_util import make_vec_env
from gymnasium import spaces

from stable_baselines3 import PPO
import gym

class PPOTrainer(object):
	"""docstring for PPOTrainer"""
	def __init__(self, env_name, env_num):
		super(PPOTrainer, self).__init__()
		self.env = make_vec_env(env_name, n_envs=env_num)
		self.model = PPO("MlpPolicy", self.env, verbose=1)
		self.model.learn(total_timesteps=1)
		
		self.action_space = self.model.action_space
		self.observation_space = self.model.observation_space
		self.predict_action = 0

	def process_obs(self, obs, rewards, dones, infos):
		self.model.policy.set_training_mode(False)
		with th.no_grad():
			last_obs = np.array(obs)
			obs_tensor = obs_as_tensor(last_obs, self.model.device)
			actions, values, log_probs = self.model.policy(obs_tensor)

		actions = actions.cpu().numpy()
		self.predict_action = actions
		if isinstance(self.action_space, spaces.Box):
			self.predict_action = np.clip(actions, 
				self.action_space.low, self.action_space.high)

		if isinstance(self.action_space, spaces.Discrete):
			# Reshape in case of discrete action
			actions = actions.reshape(-1, 1)
		for idx, done in enumerate(dones):
			if (done
				and infos[idx].get("terminal_observation") is not None
				and infos[idx].get("TimeLimit.truncated", False)
				):
				terminal_obs = self.policy.obs_to_tensor(infos[idx]["terminal_observation"])[0]
				with th.no_grad():
					terminal_value = self.policy.predict_values(terminal_obs)[0]  # type: ignore[arg-type]
					rewards[idx] += self.gamma * terminal_value

		return last_obs, actions, rewards, dones, values, log_probs

	def model_train(self, data_buffer):
		data_len = len(data_buffer)
		self.model.rollout_buffer.buffer_size = data_len
		self.model.rollout_buffer.reset()
		for i in range(data_len):
			last_obs = np.array(data_buffer[i]["last_obs"])
			actions = np.array(data_buffer[i]["actions"])
			rewards = np.array(data_buffer[i]["rewards"])
			dones = np.array(data_buffer[i]["dones"])		# may be error
			values = th.as_tensor(data_buffer[i]["values"])
			log_probs = th.as_tensor(data_buffer[i]["log_probs"])
			self.model.rollout_buffer.add(
				last_obs, actions, rewards, dones, values, log_probs
			)
		self.model.train()


	def get_action(self):
		return self.predict_action.tolist()

	def model_update(self, paras):
		self.model.set_parameters(paras)

	def get_parameters(self):
		return self.model.get_parameters()

	def model_save(self, file_name):
		self.model.save(file_name)
	def shutdown(self):
		self.env.close()
