"""Python Engine 1. Will get current engine time and make it accessible as a datapack"""

from nrp_core.engines.python_grpc import GrpcEngineScript
from nrp_core.data.nrp_protobuf import dump_pb2

class Script(GrpcEngineScript):
    def initialize(self):
        """Initialize datapack1 with time"""
        print("Engine 1 is initializing. Registering datapack...")
        self._registerDataPack("datapack1", dump_pb2.String)
        d = dump_pb2.String()
        d.string_stream = str(self._time_ns)
        self._setDataPack("datapack1", d)

    def runLoop(self, timestep_ns):
        """Update datapack1 at every timestep"""
        self._getDataPack("datapack1").string_stream = str(self._time_ns)
        print("DataPack 1 data is ", self._getDataPack("datapack1").string_stream)

    def shutdown(self):
        print("Engine 1 is shutting down")

    def reset(self):
        print("Engine 1 is resetting")
