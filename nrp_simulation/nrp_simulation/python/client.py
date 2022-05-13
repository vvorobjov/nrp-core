import grpc
import os
import signal
from contextlib import redirect_stdout
import psutil
from threading import Thread

from nrp_server_pb2 import EmptyMessage, RunLoopMessage
from nrp_server_pb2_grpc import NrpCoreStub


class NrpCore:

    TIMEOUT_SEC = 15

    def __init__(self, address, args, log_output=True) -> None:
        """Spawns a child process for NRP Core with given arguments"""
        self._devnull = open(os.devnull, 'w')
        self.child_pid = os.fork()
        is_child = (self.child_pid == 0)

        # Server side
        if is_child:
            args = f"{args} -m server --server_address {address} --slave"
            if log_output:
                args = args + " --logoutput=all"
            args_final = ["NRPCoreSim"] + args.split(" ")
            os.execvp("NRPCoreSim", args_final)
        # Client side
        else:
            # Connect to server
            self._channel = grpc.insecure_channel(address)
            self._stub = NrpCoreStub(self._channel)

            # Wait for the server to start
            n = self._wait_until_server_ready(self._channel)
            if n > 0:
                self._kill_nrp_core_process()
                if n >= self.TIMEOUT_SEC:
                    raise TimeoutError("Timeout during NRP Server startup.")
                else:
                    raise ChildProcessError("Error during NRP Server startup.")
            # Set simulation state
            self._sim_state = "Created"
            self._is_open = True

    def __del__(self) -> None:
        """Deletes the NRP Core objects and kills its subprocess"""
        try:
            with redirect_stdout(self._devnull):
                self.shutdown()
        except grpc.RpcError:
            pass

        self._devnull.close()

    def _kill_nrp_core_process(self):
        """Sends SIGTERM signal to the NRP Core subprocess"""
        try:
            os.kill(self.child_pid, signal.SIGTERM)
        except ProcessLookupError:
            pass

        if self._channel:
            self._channel.close()
            self._channel = None
            self._is_open = False

        print("NRP Server is closed now. This object can be deleted.")

    def _wait_until_server_ready(self, channel) -> int:
        """Waits for the gRPC server of the NRP Core process to start"""
        n = 1
        while n <= self.TIMEOUT_SEC:
            try:
                p = psutil.Process(self.child_pid)
                if p.status() == "zombie":
                    return n
            except psutil.NoSuchProcess:
                return n

            try:
                grpc.channel_ready_future(channel).result(timeout=1)
                return 0
            except grpc.FutureTimeoutError:
                if n > 5:
                    print(f"Waiting for NRP Server connection: {n} out of {self.TIMEOUT_SEC} seconds")

            n += 1

        return n

    def _call_rpc_async(self, sim_rpc, message) -> Thread:
        t = Thread(target=self._call_rpc, args=(sim_rpc, message,))
        t.start()
        return t

    def _call_rpc(self, sim_rpc, message) -> bool:
        """Processes calls to RPCs of the NRP Core Server"""
        if not self._is_open:
            print("This NRP Server is closed. Please delete this object and create a new one")
            return False

        try:
            res = sim_rpc(message)
            self._sim_state = res.currentState
            if self._sim_state == "Failed":
                print(f"Request failed with error \"{res.errorMsg}\". The only possible action now is \"shutdown()\".")
                return False
        except grpc.RpcError as rpc_error:
            if rpc_error.code() == grpc.StatusCode.CANCELLED:
                print(f"Can't process request: {rpc_error.details()}")
                return False
            elif rpc_error.code() == grpc.StatusCode.UNAVAILABLE:
                print(f"NRP Server is unavailable")
                return False
            else:
                # TODO: depending on the error, maybe the server process should be killed?
                raise rpc_error
        except ValueError:
            # Channel has been closed already
            return False
            
        return True
    
    def current_state(self):
        return self._sim_state

    def initialize(self):
        """Calls the initialize() RPC of the NRP Core Server"""
        return self._call_rpc(self._stub.initialize, EmptyMessage())

    def run_loop(self, num_iterations, run_async=False):
        """Calls the runLoop() RPC of the NRP Core Server"""
        message = RunLoopMessage()
        message.numIterations = num_iterations

        if run_async:
            return self._call_rpc_async(self._stub.runLoop, message)
        else:
            return self._call_rpc(self._stub.runLoop, message)

    def run_until_timeout(self, run_async=False):
        """Calls the runUntilTimeout() RPC of the NRP Core Server"""
        if run_async:
            return self._call_rpc_async(self._stub.runUntilTimeout, EmptyMessage())
        else:
            return self._call_rpc(self._stub.runUntilTimeout, EmptyMessage())

    def stop(self):
        """Calls the stopLoop() RPC of the NRP Core Server"""
        return self._call_rpc(self._stub.stopLoop, EmptyMessage())

    def reset(self):
        """Calls the reset() RPC of the NRP Core Server"""
        return self._call_rpc(self._stub.reset, EmptyMessage())

    def shutdown(self):
        """Calls the shutdown() RPC of the NRP Core Server"""
        with redirect_stdout(self._devnull):
            self._call_rpc(self._stub.shutdown, EmptyMessage())

        # Server is closed now
        try:
            os.waitpid(self.child_pid, 0)
        except ChildProcessError:
            pass

        self._kill_nrp_core_process()

# EOF
