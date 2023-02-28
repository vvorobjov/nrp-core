"""
A Py_Sim Engine for simulation
--> obtain information from simulation and send them to controller engine
--> receive controller command to run the simulation
"""
from nrp_core.engines.py_sim import PySimEngineScript

# The API of Opensim is shown in the following link:
# https://simtk.org/api_docs/opensim/api_docs


class Script(PySimEngineScript):

    def __init__(self):
        super().__init__()

        # To set the force of muscles, in arm_26, they are:
        # ['TRIlong', 'TRIlat', 'TRImed', 'BIClong', 'BICshort', 'BRA']
        # The default color of muscle in the visualizer is blue.
        # Once the force of a muscle is not the default value,
        # the color of the muscle will be changed.
        # Using this phenomenon, the controlled muscles can be found in the visualizer
        # For example, if action= [0.5, 0.0, 0.0, 0.0, 0.0, 0.0],
        # the color of TRIlong will not be blue in shown screen
        self.action = [0.0] * 6

    def initialize(self):
        print("OpensimEngine Server is initializing")
        print("Registering datapack --> sensors")
        self._registerDataPack("joints")
        self._setDataPack("joints", {"shoulder": 0, "elbow": 0})

        self._registerDataPack("infos")
        self._setDataPack("infos", {"time": 0})

        print("Registering datapack --> actuators")
        self._registerDataPack("control_cmd")

    def runLoop(self, timestep_ns):
        # Receive control data from TF
        self.action = self._getDataPack("control_cmd").get("act_list")
        reset_flag = self._getDataPack("control_cmd").get("reset")

        if reset_flag == 1:
            self.reset()
        else:
            # All Joints and Muscles can be found in the "*.osim"
            # Obtain the joint data from model "arm_26"
            # In arm_26, the joint set is [offset, r_shoulder, r_elbow]
            s_val = self.sim_manager.get_model_property("r_shoulder", datapack_type="Joint")
            e_val = self.sim_manager.get_model_property("r_elbow", datapack_type="Joint")
            # Send data to TF
            self._setDataPack("joints", {"shoulder": s_val, "elbow": e_val})
            self._setDataPack("infos", {"time": self.sim_manager.get_sim_time()})
        # Set muscles' force so to change joints
        self.sim_manager.run_step(self.action, timestep_ns)
        # To show components in the model changed by action
        # 1: To show components in a list
        # ctrl_list = self.sim_manager.theWorld.model.getControlsTable()
        # 2: To show components one by one
        # print(self.sim_manager.get_model_properties("Force"))

    def reset(self):
        print("Resetting Opensim simulation.")
        # Reset the value of set datapacks
        self._setDataPack("joints", {"shoulder": 0, "elbow": 0})
        self._setDataPack("infos", {"time": 0})
        # Reset simulation model
        self.sim_manager.reset()

    def shutdown(self):
        self.sim_manager.shutdown()
        print("Simulation engine is shutting down")
