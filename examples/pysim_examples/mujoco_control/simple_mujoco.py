"""
A Py_Sim Engine for simulation
--> obtain information from simulation and send them to controller engine
--> receive controller command to run the simulation
"""
from nrp_core.engines.py_sim import PySimEngineScript

class Script(PySimEngineScript):
    def initialize(self):
        # Initialize datapack of sensors and controller
        self._registerDataPack("positions")
        self._setDataPack("positions", { "end_postion" : [0,0,0]})

        self._registerDataPack("control_cmd")

    def runLoop(self, timestep):
        #  Receive control data and sensor information
        self.action = self._getDataPack("control_cmd").get("act_list")
        site_list = self.sim_manager.get_model_properties("site")
        end_pos = self.sim_manager.get_model_property(site_list[0], "site_xpos")
        #  Execute action in the simulator and send sensor information
        self.sim_manager.run_step(self.action, timestep)
        self._setDataPack("positions", { "end_postion" : end_pos.tolist()})

    def shutdown(self):
        self.sim_manager.shutdown()
        print("Mujoco is ending !!!")
