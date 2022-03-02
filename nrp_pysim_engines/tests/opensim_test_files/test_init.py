from nrp_core.engines.opensim import OpenSimEngineScript

class Script(OpenSimEngineScript):
#class Script(OpenSimEngineScript):
	def initialize(self):
		self.run_loop_num_execs = 0
		self.shutdown_num_execs = 0
		self.timestep = 0
	def runLoop(self, timestep):
		self.run_loop_num_execs = self.run_loop_num_execs + 1
		self.timestep = timestep
	def reset(self):
		self.sim_manager.reset()
	def shutdown(self):
		self.shutdown_num_execs = self.shutdown_num_execs + 1