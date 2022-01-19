"""Python Engine 2. Will get current engine time and make it accessible as a datapack"""

from nrp_core.engines.python_json import EngineScript

class Script(EngineScript):
    def initialize(self):
        """Initialize datapack2 with time"""
        print("Engine 2 is initializing. Registering datapack...")
        self._registerDataPack("datapack2")
        self._setDataPack("datapack2", { "time" : self._time, "timestep": 0 })

    def runLoop(self, timestep):
        """Update datapack2 at every timestep"""
        self._setDataPack("datapack2", { "time" : self._time, "timestep": timestep.count() })

    def shutdown(self):
        print("Engine 2 is shutting down")

# EOF
