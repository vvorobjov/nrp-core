import sys
import gym
try:
    from stable_baselines3.common.env_util import make_vec_env
except ModuleNotFoundError:
    pass



class OpenAIInterface(object):
    def __init__(self, model_name, start_visualizer, time_step, extra_params):
        super(OpenAIInterface, self).__init__()
        gym_version = gym.__version__.split('.')
        self.gym_version_flag = int(gym_version[1])

        self.action = None

        if(start_visualizer):
            render_mode = "human"
        else:
            render_mode = None

        self.env_model = "None"
        if("framework" in extra_params):
            if extra_params["framework"] == 'stable_baselines3':
                self.env_model = 'stable_baselines3'
                env_number = extra_params["env_number"]
                self.env = make_vec_env(model_name, n_envs=env_number)
            else:
                self.env = gym.make(model_name, render_mode = render_mode)
        else:
            self.env = gym.make(model_name, render_mode = render_mode)

        self.sim_time = 0
        self.observation = self.env.reset()
        self.properties = {
            "observation": None,
            "reward": None,
            "truncated": None,
            "doneFlag": None,
            "trainInfo": None
        }

    def getAction(self):
        return self.action

    def getState(self):
        return self.env.state

    def run_one_step(self, action, timestep_ns):
        self.sim_time += timestep_ns / 1e9
        if self.gym_version_flag < 25 or self.env_model == 'stable_baselines3':
            observation, reward, done_flag, info = self.env.step(action)
        else:
            observation, reward, truncated, done_flag, info = self.env.step(action)
            self.properties["truncated"] = truncated
        self.action = action
        self.properties["observation"] = observation
        self.properties["reward"] = reward
        self.properties["doneFlag"] = done_flag
        self.properties["trainInfo"] = info
        if done_flag:
            self.properties["observation"] = self.reset()
        return done_flag

    def reset(self):
        if self.gym_version_flag < 22 or self.env_model == 'stable_baselines3':
            self.properties["observation"] = self.env.reset()
        else:
            self.properties["observation"], self.properties["trainInfo"] = self.env.reset()
        return self.properties["observation"]

    def shutdown(self):
        self.env.reset()
        self.env.close()

    def get_model_properties(self, p_type):
        if p_type == "Property":
            return list(self.properties.keys())
        else:
            self.env.close()
            raise ValueError(f"Wrong Type {p_type}. Supported property type is 'Property'")

    def get_model_property(self, p_name, p_type):
        if p_type == "Property":
            if p_name in self.properties.keys():
                return self.properties[p_name].tolist() \
                    if (p_name == "observation") else self.properties[p_name]
            else:
                self.env.close()
                raise ValueError("This type only support 'reward', 'observation', 'doneFlag', and 'trainInfo'")
        else:
            self.env.close()
            raise ValueError(f"Wrong Type {p_type}. Supported property type is 'Property'")

    def get_model_all_properties(self, p_type):
        if p_type == "Property":
            return self.properties
        else:
            self.env.close()
            raise ValueError(f"Wrong Type {p_type}. Supported property type is 'Property'")

    # TODO: there is no timestep in OpenAIGym
    # For example, when using OpenAIGym with mujoco, the timestep will belong to mujoco
    # In this version, setting the timestep of this class to be the same as NRP
    def get_sim_time(self):
        return self.sim_time
