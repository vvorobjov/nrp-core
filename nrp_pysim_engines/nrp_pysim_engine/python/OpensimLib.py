import opensim as osim


class OpensimInterface(object):
    def __init__(self, model_name, start_visualizer, time_step):
        super().__init__()

        self.n_step = 0
        self.integrator_accuracy = 5e-5
        self.max_forces = []
        self.cur_forces = []

        self.step_size = time_step
        self.model = osim.Model(model_name)
        self.model.initSystem()
        self.model.setUseVisualizer(start_visualizer)
        self.brain = osim.PrescribedController()
        self.muscle_set = self.model.getMuscles()
        for m_index, muscle in enumerate(self.muscle_set):
            self.brain.addActuator(muscle)
            self.brain.prescribeControlForActuator(m_index, osim.Constant(1.0))

            self.max_forces.append(muscle.getMaxIsometricForce())
            self.cur_forces.append(1.0)

        self.model.addController(self.brain)
        self.state = self.model.initSystem()
        self.reset()

        self.joint_set = self.model.getJointSet()
        self.force_set = self.model.getForceSet()

    # Run simulation step by step
    def run_one_step(self, action, timestep_ns):
        self.actuate(action)
        # Define the new endtime of the simulation
        self.n_step = self.n_step + 1
        # Integrate till the new endtime
        try:
            self.state = self.manager.integrate(self.n_step * timestep_ns / 1e9)
            self.model.realizeDynamics(self.state)
        except Exception as e:
            print(e)

    def reset(self):
        self.state = self.model.initializeState()
        self.state.setTime(0)
        self.n_step = 0

        self.reset_manager()
        self.model.realizeDynamics(self.state)

    def shutdown(self):
        pass

    # Set the value of controller
    def actuate(self, action):
        brain = osim.PrescribedController.safeDownCast(
            self.model.getControllerSet().get(0))
        function_set = brain.get_ControlFunctions()
        for j in range(function_set.getSize()):
            func = osim.Constant.safeDownCast(function_set.get(j))
            if len(action) == 0:
                func.setValue(0)
            else:
                func.setValue(float(action[j]))

    # Obtain datapack names, which can also be found in the model file "*.osim"
    def get_model_properties(self, p_type):
        if p_type == "Joint":
            return [joint.getName() for joint in self.joint_set]
        elif p_type == "Force":
            return [force.getName() for force in self.force_set]
        else:
            print(f"Wrong Type {p_type}. Supported property types are 'Joint' and 'Force'")
            return []

    # Obtain the value of one datapack by the datapack name
    def get_model_property(self, p_name, p_type):
        if p_type == "Joint":
            return self.joint_set.get(p_name).getCoordinate().getValue(self.state)
        elif p_type == "Force":
            force = self.force_set.get(p_name)
            if not force.get_appliesForce():
                print("Force type is not applied")
                return []
            return [force.getRecordValues(self.state).getAsVec3()[i] for i in range(3)]  # SimTK::Vec3 doesn't support slicing
        else:
            print(f"Wrong Type {p_type}. Supported property types are 'Joint' and 'Force'")
            return []

    def get_model_all_properties(self, p_type):
        if p_type == "Joint":
            j_set = self.joint_set
            return {j_set.get(i).getName(): j_set.get(i).getCoordinate().getValue(self.state)
                    for i in range(1, j_set.getSize())}
        elif p_type == "Force":
            return {applied_force.getName():[applied_force.getRecordValues(self.state).getAsVec3()[i] for i in range(3)]
                    for applied_force in [force for force in self.force_set if force.get_appliesForce()]}
        else:
            print(f"Wrong Type {p_type}. Supported property types are 'Joint' and 'Force'")
            return {}

    def get_sim_time(self):
        return self.state.getTime()

    def reset_manager(self):
        self.manager = osim.Manager(self.model)
        self.manager.setIntegratorAccuracy(self.integrator_accuracy)
        self.manager.initialize(self.state)
