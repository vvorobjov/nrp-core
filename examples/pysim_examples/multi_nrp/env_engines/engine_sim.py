"""
A Py_Sim Engine for openAI simulation
--> obtain information from simulation and send them to NEST engine
--> receive controller command from NEST engine to run the simulation
"""
from nrp_core.engines.python_grpc import GrpcEngineScript
from nrp_core.engines.py_sim.SimManager import SimulatorManager
from nrp_protobuf import dump_pb2, nrpgenericproto_pb2

from nrp_core.engines.py_sim.ConvertLib import for_json_dump


class Script(GrpcEngineScript):
    def initialize(self):
        self.engine_id = self._config["EngineID"]
        self.sim_manager = SimulatorManager(self._config)

        # Initialize datapack of sensors with default values
        self.act_name = "action_" + str(self.engine_id)
        self.obs_name = "observation_" + str(self.engine_id)
        # Initialize datapack of observation dats with default values
        self.sim_manager.reset()
        init_obs = self.sim_manager.get_model_all_properties("Property")
        self._registerDataPack(self.obs_name, dump_pb2.String)
        self.trg_buffer = dump_pb2.String()
        self.trg_buffer.string_stream = for_json_dump(init_obs)
        self._setDataPack(self.obs_name, self.trg_buffer)

        # Initialize datapack of controller with default values
        self._registerDataPack(self.act_name, nrpgenericproto_pb2.ArrayInt32)
        self.data_2 = nrpgenericproto_pb2.ArrayInt32()
        self.data_2.array[:] = [0]
        self._setDataPack(self.act_name, self.data_2)

    def runLoop(self, timestep) :
        act_list = self._getDataPack(self.act_name).array[:]
        self.endFlag =  self.sim_manager.run_step(act_list, timestep)
        # Collect observation information
        cur_obs = self.sim_manager.get_model_all_properties("Property")
        self.trg_buffer.string_stream = for_json_dump(cur_obs)
        self._setDataPack(self.obs_name, self.trg_buffer)
      
    def shutdown(self):
        self.sim_manager.shutdown()
        print(self.engine_id, " -- Simulation End !!!")
