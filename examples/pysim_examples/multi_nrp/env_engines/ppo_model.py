"""Python Engine 1. Will get current engine time and make it accessible as a datapack"""

from nrp_core.engines.python_grpc import GrpcEngineScript
from nrp_protobuf import dump_pb2, nrpgenericproto_pb2

import time
from nrp_core.engines.py_sim.ConvertLib import dictionary_to_tensor
from nrp_core.engines.py_sim.ConvertLib import for_json_dump
from nrp_core.engines.py_sim.Trainer import PPOTrainer

import json

class Script(GrpcEngineScript):
    def initialize(self):
        """Initialize datapack1 with time"""
        env_num = 1
        self.sim_num = self._config["EngineNumber"]
        self.step_num = self._config["StepNumber"]
        self.agent = PPOTrainer(self._config["WorldFileName"], 1)

        self.data_list = []
        self.state_list = []
        self.action_list = []
        self.action_prob_list = []
        for i in range(self.sim_num):
            act_name = "action_" + str(i)
            obs_name = "observation_" + str(i)

            self._registerDataPack(act_name, nrpgenericproto_pb2.ArrayInt32)
            temp_data = nrpgenericproto_pb2.ArrayInt32()
            temp_data.array[:]=[0]
            self.data_list.append(temp_data)
            self._setDataPack(act_name, self.data_list[i])

            self._registerDataPack(obs_name,dump_pb2.String)
            self.action_list.append([0])

        self.done_flag = 0
        self.load_flag = 1
        self.step_counter = 0
        self.data_buffer = []
        self._registerDataPack("net_weight", dump_pb2.String)

        self.send_data = {"data":self.data_buffer}
        self._registerDataPack("data_buffer", dump_pb2.String)
        self.trg_buffer = dump_pb2.String()
        self.trg_buffer.string_stream = for_json_dump(self.send_data)
        self._setDataPack("data_buffer", self.trg_buffer)
        
            
    def runLoop(self, timestep_ns):
        if self.load_flag:
            network_paras = json.loads(self._getDataPack("net_weight").string_stream)
            dictionary_to_tensor(network_paras, 'policy')
            self.agent.model_update(network_paras)
            self.load_flag = 0
            self.done_flag = 0
        elif self.done_flag:
            self.send_data = {"data":self.data_buffer}
            self.trg_buffer.string_stream = for_json_dump(self.send_data)
            self._setDataPack("data_buffer", self.trg_buffer)
            self.data_buffer = []
            self.step_counter = 0
            self.load_flag = 1
        else:
            done_val = 0
            for i in range(self.sim_num):
                act_name = "action_" + str(i)
                obs_name = "observation_" + str(i)
                step_observation = json.loads(self._getDataPack(obs_name).string_stream)
                obs, rewards, dones, infos = [step_observation['observation'],
                    step_observation['reward'], step_observation['doneFlag'],
                    step_observation['trainInfo']]
                last_obs, actions, rewards, dones, values, log_probs = self.agent.process_obs(obs, rewards, dones, infos)

                trans = {'last_obs':last_obs, 'actions':actions,'rewards':rewards,
                    'dones':dones, "values":values, "log_probs":log_probs}
                if dones[0]:
                    done_val += 1
                self.data_buffer.append(trans)
                self.action_list[i] = self.agent.get_action()
                self.data_list[i].array[:]=self.action_list[i]
                self._setDataPack(act_name, self.data_list[i])

            self.step_counter += 1
            if done_val > 0 or self.step_counter > self.step_num:
                self.done_flag = 1
     
    def shutdown(self):
        self.agent.shutdown()
        print("PPO is shutting down")

    
