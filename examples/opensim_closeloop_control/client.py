"""Python Engine 2. Will get time from Engine 1 and print it"""

from nrp_core.engines.python_json import EngineScript, RegisterEngine

@RegisterEngine()
class Script(EngineScript):
    def initialize(self):
        """Initialize device1 with empty time"""
        print("Client Engine is initializing. Registering device...")
        self._registerDataPack("reset_flag")
        self._registerDataPack("joint_data")
        #self._setDataPack("joint_data", { "shoulder" : 0, "elbow": 0})

        self.action = [0.0, 0.0, 0.0, 0.5, 0.0, 0.0] # arm 26
        self._registerDataPack("action")
        self._setDataPack("action", { "act_list": self.action, "reset": 0})

        self.e_i = 0
        self.e_old = 0

        self.count = 0
        self.u = 0
        self.kp = 0.39
        self.ki = 0.001
        self.kd = 0.99

        self.reset_count = 0

    def runLoop(self, timestep):
        """Receive device1 at every timestep"""
        #'''
        joints = self._getDataPack("joint_data")
        elbow_joint = joints.get("elbow")
        #print(elbow_joint)
        # Easy state machine
        '''
        if elbow_joint > 2.0:
            self.action = [0.0, 0.4, 0.4, 0.4, 0.0, 0.0]
        elif elbow_joint > 1.0:
            self.action = [0.0, 0.0, 0.4, 0.4, 0.4, 0.0]
        else:
            self.action = [0.0, 0.0, 0.0, 0.6, 0.6, 0.6]
        '''
        # Easy PID controller
        #'''
        if self.count % 4 == 0:
            err = 2.0 - elbow_joint
            self.u = self.pid_ctrl(err)
        self.count = self.count + 1
        self.action = [0.5-self.u, 0.0, 0.0, 0.5+self.u, 0.0, 0.0]
        #'''
        reset_flag = 0
        self.reset_count = self.reset_count + 1
        # The reset need two step: one step to start the reset
        # and one step to wait the end of reset
        if self.reset_count % 200 < 2: 
            if self.reset_count % 200 == 0:
                reset_flag = 1
            self.action = [0.0, 0.0, 0.0, 0.0, 0.0, 0.0]
            self.para_init()
        self._setDataPack("action", { "act_list": self.action, "reset": reset_flag})

    def shutdown(self):
        print("Engine 2 is shutting down")


    def para_init(self):
        self.e_i = 0
        self.e_old = 0

        self.count = 0
        self.u = 0
        self.kp = 0.39
        self.ki = 0.001
        self.kd = 0.99

    def pid_ctrl(self, error):
        self.e_i = self.e_i + error
        de = error - self.e_old
        u = self.kp*error + self.ki*self.e_i  + self.kd*de
        self.e_old = error

        return u

    

