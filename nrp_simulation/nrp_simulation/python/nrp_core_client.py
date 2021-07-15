import grpc
import os
import signal

from nrp_server_pb2 import EmptyMessage, RunLoopMessage
from nrp_server_pb2_grpc import NrpCoreStub

class NrpCore:

    TIMEOUT_SEC = 15

    def __init__(self, address, args) -> None:
        """Spawns a child process for NRP Core with given arguments"""
        self.child_pid = os.fork()
        is_child = (self.child_pid == 0)

        if is_child:
            args_final = ["NRPSimulation"] + args + ["-m", "server", "--server_address", address]

            os.execvp("NRPSimulation", args_final)
        else:
            channel = grpc.insecure_channel(address)
            self.stub = NrpCoreStub(channel)

            # Wait for the server to start
            if not self.wait_unit_server_ready(channel):
                self.kill_nrp_core_process()
                raise TimeoutError("Timeout during NRP Server startup")

    def __del__(self) -> None:
        """Deletes the NRP Core objects and kills its subprocess"""
        self.kill_nrp_core_process()

    def kill_nrp_core_process(self) -> None:
        """Sends SIGTERM signal to the NRP Core subprocess"""
        os.kill(self.child_pid, signal.SIGTERM)

    def wait_unit_server_ready(self, channel) -> bool:
        """Waits for the gRPC server of the NRP Core process to start"""
        try:
            grpc.channel_ready_future(channel).result(timeout=self.TIMEOUT_SEC)
            return True
        except grpc.FutureTimeoutError:
            return False

    def initialize(self):
        """Calls the init() RPC of the NRP Core Server"""
        return self.stub.initialize(EmptyMessage())

    def runLoop(self, num_iterations):
        """Calls the runLoop() RPC of the NRP Core Server"""
        message = RunLoopMessage()
        message.numIterations = num_iterations
        return self.stub.runLoop(message)

    def shutdown(self):
        """Calls the shutdown() RPC of the NRP Core Server"""
        return self.stub.shutdown(EmptyMessage())

# EOF
