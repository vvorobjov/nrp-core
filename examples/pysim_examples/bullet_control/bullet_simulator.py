"""
A Py_Sim Engine for a PyBullet car simulation
--> obtain information from simulation and send them to controller engine
--> receive controller command from engine to run the simulation
"""
from nrp_core.engines.py_sim import PySimEngineScript


class Script(PySimEngineScript):

    def __init__(self):
        super().__init__()
        self.action = None
        self.wheels = []

    def initialize(self):
        self.wheels = [2, 3, 5, 7]
        # Initialize datapack of sensors and controller
        self._registerDataPack("positions")
        self._setDataPack("positions", {"body": [0, 0, 0]})

        self._registerDataPack("control_cmd")

    def runLoop(self, timestep):
        #  Receive control data
        self.action = self._getDataPack("control_cmd").get("act_list")

        act_list = {"Joint": [{'index': wheel,
                               'controlMode': 'VELOCITY_CONTROL',
                               'force': 10,
                               'targetVelocity': self.action[0]} for wheel in self.wheels]}
        # Execute
        self.sim_manager.run_step(act_list, timestep)

        # Collect observation information
        self._setDataPack("positions",
                          {"body": self.sim_manager.get_model_property('chassis', "Link")[0]})
        
    def shutdown(self):
        self.sim_manager.shutdown()
        print("Simulation End !!!")
