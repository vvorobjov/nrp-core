"""Python Engine 1. Will get current engine time and make it accessible as a device"""
from nrp_core.engines.opensim import OpenSimEngineScript
from nrp_core.engines.python_json import RegisterEngine

# The API of Opensim is shown in the following link:
# https://simtk.org/api_docs/opensim/api_docs/classOpenSim_1_1Model.html

@RegisterEngine()
class Script(OpenSimEngineScript):
    def initialize(self):
        """Initialize device1 with time"""
        print("Server Engine is initializing")
        print("Registering device --> for sensors")
        self._registerDataPack("joints")
        self._setDataPack("joints", {"shoulder": 0, "elbow": 0})
        self._registerDataPack("infos")
        self._setDataPack("infos", {"time": 0})

        # To set the force of muscles, in arm_26, they are:
        # ['TRIlong', 'TRIlat', 'TRImed', 'BIClong', 'BICshort', 'BRA']
        # The default color of muscle in the visualizer is blue.
        # Once the force of a muscle is not the default value, 
        # the color of the muscle will be changed. 
        # Using this phenomenon, the controlled muscles can be found in the visualizer
        # For example, if action=[0.5, 0.0, 0.0, 0.0, 0.0, 0.0], 
        # the color of TRIlong will not be blue in shown screen
        self.action = [0.0, 0.0, 0.0, 0.0, 0.0, 0.0]
        print("Registering device --> for actuators")
        self._registerDataPack("control_cmd")

    def runLoop(self, timestep):
        # Receive control data from TF
        self.action = self._getDataPack("control_cmd").get("act_list")
        reset_falg = self._getDataPack("control_cmd").get("reset")

        if reset_falg == 1:
            self.reset()
        else:
            # All Joints and Muscles can be found in the "*.osim"
            # Obtain the joint data from model "arm_26"
            # In arm_26, the joint set is [offset, r_shoulder, r_elbow]
            s_val = self.sim_manager.get_device_val("r_shoulder", deviceType="Joint")
            e_val = self.sim_manager.get_device_val("r_elbow", deviceType="Joint") 
            # Send data to TF
            self._setDataPack("joints", {"shoulder": s_val, "elbow": e_val})
            self._setDataPack("infos", {"time": self.sim_manager.get_sim_time()})
        # Set muscles' force to change joints
        self.sim_manager.run_step(self.action)
        # To show components in the model changed by action
        # 1: To show components in a list
        #ctrl_list = self.sim_manager.theWorld.model.getControlsTable()
        # 2: To show components one by one
        #print(self.sim_manager.get_device_names("Force"))

    def reset(self):
        print("Reset !!!")
        # Reset the value of set devices
        self._setDataPack("joints", {"shoulder": 0, "elbow": 0})
        self._setDataPack("infos", {"time": 0})
        # Reset simulation model
        self.sim_manager.reset()
        
    def shutdown(self):
        print("Engine 1 is shutting down")


