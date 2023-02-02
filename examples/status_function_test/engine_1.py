"""Python Engine 1. Will get current engine time and make it accessible as a datapack"""

from nrp_core.engines.python_json import EngineScript


class Script(EngineScript):
    def initialize(self):
        """Initialize datapack1 with time"""
        print("Engine 1 is initializing. Registering datapack...")
        self.datapack_out_name = "datapack_engine1"
        self._registerDataPack(self.datapack_out_name)
        self._registerDataPack("actions")
        self._setDataPack(self.datapack_out_name, { "time":      self._time_ns,
                                                    "timestep":  0,
                                                    "test_data": 0 })

    def runLoop(self, timestep_ns):
        """Update datapack1 at every timestep"""
        actions_datapack = self._getDataPack("actions")
        print("DataPack 'actions' data is " + str(actions_datapack))

        if actions_datapack:
            self._setDataPack(self.datapack_out_name, { "time":      self._time_ns,
                                                        "timestep":  timestep_ns,
                                                        "test_data": actions_datapack["test_data"] })
        else:
            self._setDataPack(self.datapack_out_name, { "time":      self._time_ns,
                                                        "timestep":  timestep_ns,
                                                        "test_data": 0 })


    def shutdown(self):
        print("Engine 1 is shutting down")

    def reset(self):
        print("Engine 1 is resetting")
