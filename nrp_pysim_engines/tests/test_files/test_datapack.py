from nrp_core.engines.opensim import OpenSimEngineScript
from nrp_core.engines.python_json import RegisterEngine

@RegisterEngine()
class Script(OpenSimEngineScript):
    def initialize(self):
        print("Server Engine is initializing")
        self._registerDataPack("infos")
        self._setDataPack("infos", {"init": 1})

        self._registerDataPack("flag")

    def runLoop(self, timestep):
        dev = self._getDataPack("flag")
        print("Receive data is " + str(dev))