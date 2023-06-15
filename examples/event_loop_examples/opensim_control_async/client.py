"""
A Python Engine for controller
--> obtain information from OpenSim simulator
--> feedback control command to OpenSim simulator
"""
from nrp_core.engines.python_json import EngineScript


class Script(EngineScript):

    def __init__(self):
        super().__init__()

        self.reset_count = 0
        self.para_init()

    def initialize(self):
        """Initialize datapack1 with empty time"""
        print("Client Engine is initializing. Registering datapack...")
        self._registerDataPack("reset_flag")
        self._registerDataPack("joint_data")

        self._registerDataPack("action")
        self._setDataPack("action", {"act_list": self.action, "reset": 0})

    def para_init(self):
        self.e_old = 0
        self.count = 0

        self.kp = 0.39
        self.ki = 0.001
        self.kd = 0.99

        self.action = [0.0, 0.0, 0.0, 0.0, 0.0, 0.0]  # arm 26

    def pid_ctrl(self, elbow_joint):
        error = 2.0 - elbow_joint
        de = error - self.e_old
        self.e_old = error
        u = (self.kp * error + self.ki * error + self.kd * de)

        self.action = [0.0, 0.0, 0.0, 0.0 + u, 0.0, 0.0]

    def initialize(self):
        """Initialize datapack1 with empty time"""
        print("Client Engine is initializing. Registering datapack...")
        self._registerDataPack("joint_data")
        self._registerDataPack("action")
        self._setDataPack("action", {"act_list": self.action, "reset": 0})

    def runLoop(self, timestep):
        # Receive necessary joint data at every timestep
        joints = self._getDataPack("joint_data")
        if not joints:
            return

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
        self.pid_ctrl(elbow_joint)

        # The reset need two step: one step to start the reset
        # and one step to wait the end of reset
        self.reset_count += 1
        reset_c = self.reset_count % 200
        if reset_c < 2:
            self.para_init()

        self._setDataPack("action", {"act_list": self.action,
                                     "reset": 1 if reset_c == 0 else 0})

    def shutdown(self):
        print("Controller engine is shutting down")
