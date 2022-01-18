from nrp_core.engines.python_json.engine_script import EngineScript

class Script(EngineScript):

    def initialize(self):
        self._registerDataPack("test_datapack")
        pass

    def shutdown(self):
        pass

    def runLoop(self):
        pass

    def reset(self):
        pass

# EOF
