"""
Python gRPC engine that wraps around Taxi gym environment.
"""

import gym

from nrp_core.engines.python_grpc import GrpcEngineScript
from nrp_protobuf import wrappers_pb2

class Script(GrpcEngineScript):

    def _prepare_datapacks(self, observation, reward, is_done):
        observation_data = wrappers_pb2.Int32Value()
        observation_data.value = observation
        self._setDataPack("observation", observation_data)

        if reward is not None:
            reward_data = wrappers_pb2.Int32Value()
            reward_data.value = 0
            self._setDataPack("reward", reward_data)

        if is_done is not None:
            is_done_data = wrappers_pb2.BoolValue()
            is_done_data.value = False
            self._setDataPack("is_done", is_done_data)


    def initialize(self):
        self._env = gym.make("Taxi-v3")

        # Incoming DataPacks
        self._registerDataPack("actions", wrappers_pb2.Int32Value)

        # Outgoing DataPacks
        self._registerDataPack("observation", wrappers_pb2.Int32Value)
        self._registerDataPack("reward",      wrappers_pb2.Int32Value)
        self._registerDataPack("is_done",     wrappers_pb2.BoolValue)


    def runLoop(self, timestep_ns):
        action = self._getDataPack("actions").value
        observation, reward, is_done, info = self._env.step(action)
        self._prepare_datapacks(observation, reward, is_done)


    def shutdown(self):
        pass


    def reset(self):
        observation = self._env.reset()
        self._prepare_datapacks(observation, None, None)

# EOF
