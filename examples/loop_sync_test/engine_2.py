"""Python Engine 2. Will get current engine time and make it accessible as a device"""

from NRPPythonEngineModule import EngineScript,RegisterEngine

@RegisterEngine()
class Script(EngineScript):
    def initialize(self):
        """Initialize device2 with time"""
        print("Engine 2 is initializing. Registering device...")
        self._registerDevice("device2")
        self._setDevice("device2", { "time" : self._time.count(), "timestep": 0 })

    def runLoop(self, timestep):
        """Update device2 at every timestep"""
        self._setDevice("device2", { "time" : self._time.count(), "timestep": timestep.count() })

    def shutdown(self):
        print("Engine 2 is shutting down")

# EOF
