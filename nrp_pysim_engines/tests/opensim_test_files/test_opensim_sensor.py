from nrp_core.engines.opensim import OpenSimEngineScript

# The API of Opensim is shown in the following link:
# https://simtk.org/api_docs/opensim/api_docs
class Script(OpenSimEngineScript):
    def initialize(self):
        """Initialize datapack1 with time"""
        self.run_loop_num_execs = 0
        self.shutdown_num_execs = 0
        self.timestep = 0
        self.action = [0.2, 0.0, 0.0, 0.0, 0.0, 0.0]

    def runLoop(self, timestep):
        Joint_val = self.sim_manager.get_model_property("r_shoulder", "Joint")
        print("Sense Joint !!!")
        Force_val = self.sim_manager.get_model_property("BIClong", "Force") 
        print("Sense Force !!!")
        # Set muscles' force to change joints
        self.sim_manager.run_step(self.action)
        self.run_loop_num_execs = self.run_loop_num_execs + 1
        self.timestep = timestep

    def reset(self):
        self.sim_manager.reset()
    def shutdown(self):
        self.shutdown_num_execs = self.shutdown_num_execs + 1

