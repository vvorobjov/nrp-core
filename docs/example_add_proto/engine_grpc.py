from nrp_core.engines.python_grpc import GrpcEngineScript
import mypackage_pb2

class Script(GrpcEngineScript):
    def initialize(self):
        self._registerDataPack("datapack_name", mypackage_pb2.MyMessage)
        d = mypackage_pb2.MyMessage()
        d.integer = self._time_ns
        d.str = "from grpc python engine"
        self._setDataPack("datapack_name", d)

    def runLoop(self, timestep_ns):
        self._getDataPack("datapack_name").integer = int(self._time_ns / 1e6)

    def shutdown(self):
        pass

    def reset(self):
        pass