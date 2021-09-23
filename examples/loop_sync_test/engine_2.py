"""Python Engine 2. Will get current engine time and make it accessible as a datapack"""

from NRPPythonEngineModule import EngineScript,RegisterEngine

@RegisterEngine()
class Script(EngineScript):
    def initialize(self):
        """Initialize datapack2 with time"""
        print("Engine 2 is initializing. Registering datapack...")
        self._registerDataPack("datapack2")
        self._setDataPack("datapack2", { "time" : self._time.count(), "timestep": 0 })

    def runLoop(self, timestep):
        """Update datapack2 at every timestep"""
        self._setDataPack("datapack2", { "time" : self._time.count(), "timestep": timestep.count() })

    def shutdown(self):
        print("Engine 2 is shutting down")

# EOF
