import opensim as osim
import math


class OpensimInterface(object):
	step_size = 0.001

	model = None
	state = None
	state0 = None
	joints = []
	bodies = []
	brain = None
	manager = None
	verbose = False
	n_step = 0

	state_desc_n_step = None
	prev_state_desc = None
	state_desc = None
	integrator_accuracy = 5e-5

	jointSet = None
	forceSet = None

	maxforces = []
	curforces = []
	
	def __init__(self, model_name, start_visualizer, time_step):
		super(OpensimInterface, self).__init__()
		self.step_size = time_step
		self.model = osim.Model(model_name)
		self.state = self.model.initSystem()
		self.model.setUseVisualizer(start_visualizer)
		self.brain = osim.PrescribedController()
		self.muscleSet = self.model.getMuscles()
		
		for j in range(self.muscleSet.getSize()):
			func = osim.Constant(1.0)
			self.brain.addActuator(self.muscleSet.get(j))
			self.brain.prescribeControlForActuator(j, func)

			self.maxforces.append(self.muscleSet.get(j).getMaxIsometricForce())
			self.curforces.append(1.0)

		self.model.addController(self.brain)
		self.state = self.model.initSystem()
		# Enable the visualizer
		self.reset()

		self.jointSet = self.model.getJointSet()
		self.forceSet = self.model.getForceSet()

	# Run simulation step by step
	def run_one_step(self, action):
		self.actuate(action)
		# Define the new endtime of the simulation
		self.n_step = self.n_step + 1
		# Integrate till the new endtime
		try:
			self.state = self.manager.integrate(self.step_size*self.n_step)
		except Exception as e:
			print(e)

	def reset(self):
		self.state = self.model.initializeState()
		#self.state = self.model.initSystem()
		self.state.setTime(0)
		self.n_step = 0

		self.reset_manager()
		
	# Set the value of controller
	def actuate(self, action):
		brain = osim.PrescribedController.safeDownCast(
			self.model.getControllerSet().get(0))
		function_set = brain.get_ControlFunctions()
		for j in range(function_set.getSize()):
			func = osim.Constant.safeDownCast(function_set.get(j))
			func.setValue(float(action[j]))

	# Obtain datapack names, which can also be found in the model file "*.osim"
	def get_model_properties(self, p_type):
		tSet = None
		if p_type == "Joint":
			tSet = self.jointSet
		elif p_type == "Force":
			tSet = self.forceSet
		else:
			print("supported types are 'Joint' and 'Force'")
			return []
		nameList = []
		return [tSet.get(i).getName() for i in range(tSet.getSize())]


	# Obtain the value of one datapack by the datapack name
	def get_model_property(self, p_name, p_type):
		if p_type == "Joint":
			tSet = self.jointSet
			tProperty = tSet.get(p_name).getCoordinate()
			return tProperty.getValue(self.state)
		elif p_type == "Force":
			tSet = self.forceSet
			tProperty = tSet.get(p_name).get_appliesForce()
			if tProperty:
				self.model.realizeDynamics(self.state)
				return tSet.get(p_name).getRecordValues(self.state)
			else:
				print("Force type is not applied")
				return []
		else:
			print("p_type is error")
			print("In this function, it only supports Joint and Force")
			return []
		
	def get_sim_time(self):
		return self.state.getTime()

	def reset_manager(self):
		self.manager = osim.Manager(self.model)
		self.manager.setIntegratorAccuracy(self.integrator_accuracy)
		self.manager.initialize(self.state)

