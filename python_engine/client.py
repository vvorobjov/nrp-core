from nrp_core.engines.python_json import EngineScript,RegisterEngine
 
@RegisterEngine()
class Script(EngineScript):
    def initialize(self):
        print("Python engine is initializing. Registering datapack...")
        self._registerDataPack("JointAngles")
        self._registerDataPack("device1")
        self._setDataPack("JointAngles", { "joint_angles" : [1, 1, 1]})
 
    def runLoop(self, timestep):
        import random

        # Get data from transceiver function (tf_1.py)
        dev = self._getDataPack("device1")
        # print(dev)

        # Send data to transceiver function (tf_2.py)
        self._setDataPack("JointAngles", { "joint_angles" : [(random.random() - 0.5) * 100, 1, 1]})
 
    def shutdown(self):
        print("Python engine is shutting down")
