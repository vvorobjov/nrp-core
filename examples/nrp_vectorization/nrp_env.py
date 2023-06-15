#!/usr/bin/env python3

import gym
from gym import spaces

from nrp_core.client import NrpCore
from nrp_core.data.nrp_protobuf.wrappers_pb2 import BoolValue, Int32Value


class NrpEnv(gym.Env):
    """
    A thin wrapper class for NRP Python Client, that makes it
    compatible with gym interface
    """

    metadata = {'render.modes': ['human']}

    def __init__(self,
                 nrp_server_address = "localhost:50050",
                 config_file = "simulation_config.json",
                 log_output = None):
        super(NrpEnv, self).__init__()

        # Define action and observation spaces

        self.action_space = spaces.Discrete(6)
        self.observation_space = spaces.Discrete(500)

        # Create an instance of NRP Core and initialize it

        self.nrp_core = NrpCore(nrp_server_address, config_file=config_file, log_output=log_output)
        self.nrp_core.initialize()


    def step(self, action):
        actions = Int32Value()
        actions.value = action

        self.nrp_core.set_proto_datapack("actions", "taxi_grpc", actions)
        done, observations = self.nrp_core.run_loop(1)

        # Int32Value
        observation = observations[0].value
        # Int32Value
        reward = observations[1].value
        # BoolValue
        is_done = observations[2].value

        return observation, reward, is_done, {}

    def reset(self):
        observations = self.nrp_core.reset()
        # Find the single non-empty value
        # TODO NRRPLT-8746 will allow to search by name
        for obs in observations:
            if obs.value:
                return obs.value

    def render(self, mode='human'):
        pass

    def close (self):
        pass

# EOF
