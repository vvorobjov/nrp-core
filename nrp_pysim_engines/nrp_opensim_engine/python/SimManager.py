from .OpensimLib import OpensimInterface
import time
		
class SimulatorManager(object):
	"""
	This class will receive the information simulator need from NRP engine script,
	and then start and run different simulators with python API
	"""
	def __init__(self, configureVal):
		super(SimulatorManager, self).__init__()

		world_file = configureVal["WorldFileName"]
		start_visualizaer = bool(configureVal["Visualizer"])
		simulator_type = configureVal["Simulator"]

		self.time_step = configureVal["EngineTimestep"]

		self.sim_interface = None
		if simulator_type == "Opensim":
			self.sim_interface = OpensimInterface(world_file, start_visualizaer, self.time_step)
		else:
			raise Exception('Simulator ' + simulator_type + 'is not installed')
		self.stepStart = time.time()

	# TODO: currently this function is not connected to reset requests from the Engine client. The PythonJSONEngine
	#  server is just shutting down and initializing again the Engine. Calling this function if it is available would be
	#  more efficient
	def reset(self):
		self.sim_interface.reset()

	def run_step(self, action):
		self.sim_interface.run_one_step(action)

	def get_device_names(self, deviceType):
		return self.sim_interface.get_device_names(deviceType)

	def get_device_val(self, deviceName, deviceType):
		return self.sim_interface.get_device_val(deviceName, deviceType)

	def get_sim_time(self):
		return self.sim_interface.get_sim_time()

