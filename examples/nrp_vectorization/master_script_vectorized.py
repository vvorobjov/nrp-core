#!/usr/bin/env python3

import argparse
import signal
import sys

from stable_baselines3 import PPO
from stable_baselines3.common.vec_env import SubprocVecEnv

from nrp_env import NrpEnv


if __name__ == '__main__':
    envs = None

    def signal_handler(sig, frame):
        envs.close()
        sys.exit(0)

    signal.signal(signal.SIGINT, signal_handler)

    envs = SubprocVecEnv([lambda: NrpEnv("localhost:50050", "simulation_config.json", log_output=".out1.log"),
                          lambda: NrpEnv("localhost:50051", "simulation_config.json", log_output=".out2.log"),
                          lambda: NrpEnv("localhost:50052", "simulation_config.json", log_output=".out3.log"),
                          lambda: NrpEnv("localhost:50053", "simulation_config.json", log_output=".out4.log")])
    
    model = PPO('MlpPolicy', envs, verbose=1, ent_coef=0.01)
    model.learn(total_timesteps=int(10000))
    model.save("ppo")

    envs.close()

# EOF
