"""Python Engine 2. Will get time from Engine 1 and print it"""

from nrp_core.engines.python_grpc import GrpcEngineScript
from nrp_core.data.nrp_protobuf import dump_pb2

class Script(GrpcEngineScript):
    def initialize(self):
        """Initialize datapack1 with empty time"""
        print("Engine 2 is initializing. Registering datapack...")
        self._registerDataPack("rec_datapack2", dump_pb2.String)

    def runLoop(self, timestep_ns):
        """Receive datapack1 at every timestep"""
        print("Engine 2 received data is ", self._getDataPack("rec_datapack2").string_stream)

    def shutdown(self):
        print("Engine 2 is shutting down")