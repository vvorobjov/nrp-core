from nrp_core.engines.opensim import OpenSimEngineScript
from nrp_core.engines.python_json import RegisterEngine

# The API of Opensim is shown in the following link:
# https://simtk.org/api_docs/opensim/api_docs

@RegisterEngine()
class Script(OpenSimEngineScript):
    def initialize(self):
        """Initialize datapack1 with time"""
        print("Server Engine is initializing")
        self.action = [0.2, 0.0, 0.0, 0.0, 0.0, 0.0]

    def runLoop(self, timestep):
        # Receive control data from TF
        Joint_val = self.sim_manager.get_model_property("r_shoulder", datapack_type="Joint")
        print("Sense Joint !!!")
        Force_val = self.sim_manager.get_model_property("BIClong", datapack_type="Force") 
        print("Sense Force !!!")
        # Set muscles' force to change joints
        self.sim_manager.run_step(self.action)
        print("Execute action !!!")

    def shutdown(self):
        print("Engine 1 is shutting down")


