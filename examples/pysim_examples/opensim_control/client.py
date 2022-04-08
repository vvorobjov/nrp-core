"""
A Python Engine for controller
--> obtain information from OpenSim simulator
--> feedback control command to OpenSim simulator
"""
from nrp_core.engines.python_json import EngineScript

class Script(EngineScript):
    def initialize(self):
        self.para_init()
        self.reset_count = 0
        self.action = []
        # Initialize datapack of sensors and controller
        print("Client Engine is initializing. Registering datapack...")
        self._registerDataPack("reset_flag")
        self._registerDataPack("joint_data")

        self.action = [0.0, 0.0, 0.0,
                       0.5, 0.0, 0.0]  # arm 26
        self._registerDataPack("action")
        self._setDataPack("action", {"act_list": self.action, "reset": 0})

    def para_init(self):
        self.e_i = 0
        self.e_old = 0

        self.count = 0
        self.u = 0
        self.kp = 0.39
        self.ki = 0.001
        self.kd = 0.99

    def runLoop(self, timestep):
        # Receive necessary joint data at every timestep
        joints = self._getDataPack("joint_data")
        elbow_joint = joints.get("elbow")
        # Simple state machine
        '''
        if elbow_joint > 2.0:
            self.action = [0.0, 0.4, 0.4, 0.4, 0.0, 0.0]
        elif elbow_joint > 1.0:
            self.action = [0.0, 0.0, 0.4, 0.4, 0.4, 0.0]
        else:
            self.action = [0.0, 0.0, 0.0, 0.6, 0.6, 0.6]
        '''
        # Simple PID controller
        if self.count % 4 == 0:
            err = 2.0 - elbow_joint
            self.u = self.pid_ctrl(err)

        self.count += 1
        self.action = [0.5 - self.u, 0.0, 0.0,
                       0.5 + self.u, 0.0, 0.0]

        self.reset_count += 1
        # The reset need two step: one step to start the reset
        # and one step to wait the end of reset
        reset_c = self.reset_count % 200
        if reset_c < 2:
            self.action = [0.0, 0.0, 0.0,
                           0.0, 0.0, 0.0]
            self.para_init()
        self._setDataPack("action", {"act_list": self.action,
                                     "reset": 1 if reset_c == 0 else 0})

    def shutdown(self):
        print("Controller engine is shutting down")

    def pid_ctrl(self, error):
        self.e_i += error
        de = error - self.e_old
        u = self.kp * error + self.ki * self.e_i + self.kd * de

        self.e_old = error

        return u
