"""Python Engine 1. Will get current engine time and make it accessible as a device"""

from NRPPythonEngineModule import EngineScript,RegisterEngine

@RegisterEngine()
class Script(EngineScript):
    def initialize(self):
        """Initialize device1 with time"""
        print("Engine 1 is initializing. Registering device...")
        self._registerDevice("device1")
        self._setDevice("device1", { "time" : self._time.count(), "timestep": 0 })

    def runLoop(self, timestep):
        """Update device1 at every timestep"""
        self._setDevice("device1", { "time" : self._time.count(), "timestep": timestep.count() })

    def shutdown(self):
        print("Engine 1 is shutting down")

# EOF
