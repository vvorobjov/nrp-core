"""Python Engine . Will get current frame and make it accessible as a datapack"""

from nrp_core.engines.python_grpc import GrpcEngineScript
from nrp_core.data.nrp_protobuf import unityproto_pb2
class Script(GrpcEngineScript):
    def initialize(self):
        """Initialize python image receiver"""
        print("Python Engine is initializing. Registering datapack...")
        self._registerDataPack("input_image", unityproto_pb2.BImage)
        self.counter = 0

    def runLoop(self, timestep_ns):
        """receive datapack and dump it on the disc"""
        snapshot = self._getDataPack("input_image").image
        self.counter += 1
        with open(f"./rgbout/frame_{self.counter}.png", 'wb') as f:
            f.write(snapshot)

    def shutdown(self):
        print("Python Engine  is shutting down")

    def reset(self):
        print("Python Engine is resetting")
