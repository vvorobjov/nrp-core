from nrp_core.engines.python_json.engine_script import EngineScript

class Script(EngineScript):

    def initialize(self):
        raise Exception("Initialization failed")

    def shutdown(self):
        raise Exception("Shutdown failed")

# EOF
