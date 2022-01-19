"""Python Engine 1. Will get current engine time and make it accessible as a datapack"""

from nrp_core.engines.python_json import EngineScript

class Script(EngineScript):
    def initialize(self):
        """Initialize datapack3 with time"""
        print("Engine 3 is initializing. Registering datapack...")
        self._registerDataPack("datapack3")
        self._setDataPack("datapack3", { "time" : self._time, "timestep": 0 })

    def runLoop(self, timestep):
        """Update datapack3 at every timestep"""
        self._setDataPack("datapack3", { "time" : self._time, "timestep": timestep.count() })

    def shutdown(self):
        print("Engine 3 is shutting down")

# EOF
