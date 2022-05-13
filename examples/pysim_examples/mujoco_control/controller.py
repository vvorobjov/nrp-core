"""
A Python Engine for controller
--> obtain information from Mujoco simulator
--> feedback control command to Mujoco simulator
"""
from nrp_core.engines.python_json import EngineScript

class Script(EngineScript):
    def initialize(self):
        # Initialize datapack of sensors and controller
        self.cur_rad = 0
        self.step_rad =-0.036
        self.step_count = 0

        self._registerDataPack("positions")

        self._registerDataPack("action")
        self._setDataPack("action", {"act_list": [self.cur_rad]})

    def runLoop(self, timestep):
        # Produce control rad for the joint
        self.cur_rad += self.step_rad
        # Receive pole end position data at every timestep
        end_pos = self._getDataPack("positions").get("end_postion")
        print("End position of pole: ", end_pos)
        # Change swing direction of the pole base on its Z-coordinate
        self.step_count += 1
        if end_pos[2] > 0.1 and self.step_count > 10:
            self.step_rad = -self.step_rad
            self.step_count = 0

        self._setDataPack("action", {"act_list": [self.cur_rad]})

    def shutdown(self):
        print("Controller engine is shutting down")
