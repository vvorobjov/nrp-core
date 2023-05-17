# Link of mujoco_py:
# -- https://github.com/openai/mujoco-py
# Documentation of mujoco_py:
# -- https://openai.github.io/mujoco-py/build/html/index.html
# Documentation of MjData:
# -- https://mujoco.readthedocs.io/en/latest/APIreference.html?highlight=mjData#mjdata

from mujoco_py import load_model_from_path, MjSim, MjViewer
import glfw


class MujocoInterface(object):
	def __init__(self, model_name, start_visualizer, time_step, extra_params):
		super(MujocoInterface, self).__init__()
		self.model = load_model_from_path(model_name)
		self.sim = MjSim(self.model)
		self.sim_state = self.sim.get_state()

		# Mini timestep of Mujoco is 2ms
		self.basic_timestep = self.model.opt.timestep
		self.step_size = int(time_step/self.basic_timestep)
		if self.step_size == 0:
			raise ValueError(f"The minimal timestep of Mujoco is {self.basic_timestep}")

		self.start_visualizer = start_visualizer
		self.viewer = None
		if start_visualizer:
			self.viewer = MjViewer(self.sim)
			self.viewer.cam.distance = self.model.stat.extent * 0.25

	def run_one_step(self, action, timestep_ns):
		self.sim.data.ctrl[:] = action
		cur_step_size = int(timestep_ns / 1e9 / self.basic_timestep)
		for i in range(cur_step_size):
			self.sim.step()
		if self.start_visualizer:
			self.viewer.render()

	def reset(self):
		self.sim.reset()
		return 0

	def shutdown(self):
		glfw.destroy_window(self.viewer.window)

	def get_model_properties(self, p_type):
		try:
			return getattr(self.sim.model, f'{p_type}_names')
		except AttributeError as e:
			glfw.destroy_window(self.viewer.window)
			raise ValueError(f'Property type {p_type} is not supported with error {str(e)}.')

	def get_model_property(self, p_name, p_type):
		try:
			p_func = getattr(self.sim.data, f'get_{p_type}')
			return p_func(p_name)
		except AttributeError as e:
			glfw.destroy_window(self.viewer.window)
			raise ValueError(f'Fail on data. {p_type} is not supported with error {str(e)}.')

	def get_model_all_properties(self, p_type):
		try:
			return getattr(self.sim.data, p_type)
		except AttributeError as e:
			glfw.destroy_window(self.viewer.window)
			raise ValueError(f'Property type {p_type} is not supported with error {str(e)}.')

	def get_sim_time(self):
		return self.sim.data.time
