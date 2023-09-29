import gymnasium as gym
from stable_baselines3.common.utils import obs_as_tensor, safe_mean
from gymnasium import spaces
import torch as th

from stable_baselines3 import PPO
from stable_baselines3.common.env_util import make_vec_env

# Parallel environments
vec_env = make_vec_env("CartPole-v1", n_envs=1)
model = PPO.load("ppo_cartpole")

obs = vec_env.reset()
for i in range (1000):
    action, _states = model.predict(obs)
    obs, rewards, dones, info = vec_env.step(action)
    vec_env.render("human")