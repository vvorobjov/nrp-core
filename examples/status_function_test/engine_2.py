"""Python Engine 2. Will get time from Engine 1 and print it"""

from nrp_core.engines.python_json import EngineScript

class Script(EngineScript):
    def initialize(self):
        """Initialize datapack1 with empty time"""
        print("Engine 2 is initializing. Registering datapack...")
        self.datapack_out_name = "datapack_engine2"
        self._registerDataPack(self.datapack_out_name)
        self._setDataPack(self.datapack_out_name, { "time" : self._time_ns, "timestep": 0 })

    def runLoop(self, timestep_ns):
        """Receive datapack1 at every timestep"""
        self._setDataPack(self.datapack_out_name, { "time" : self._time_ns, "timestep": timestep_ns })

    def shutdown(self):
        print("Engine 2 is shutting down")

    def reset(self):
        self._setDataPack(self.datapack_out_name, { "time" : self._time_ns, "timestep": 0 })
