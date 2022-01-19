from time import time
from nrp_core.engines.python_json.engine_script import EngineScript

class Script(EngineScript):

    def __init__(self) -> None:
        super().__init__()
        self.run_loop_num_execs = 0
        self.shutdown_num_execs = 0
        self.timestep = 0

    def initialize(self):
        self._registerDataPack("test_datapack")
        pass

    def shutdown(self):
        self.shutdown_num_execs = self.shutdown_num_execs + 1

    def runLoop(self, timestep):
        self.run_loop_num_execs = self.run_loop_num_execs + 1
        self.timestep = timestep

    def reset(self):
        pass

# EOF
