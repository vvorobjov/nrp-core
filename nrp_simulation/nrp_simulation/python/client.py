import grpc
import os
from contextlib import redirect_stdout
from threading import Thread

from nrp_server_pb2 import EmptyMessage, RunLoopMessage
from nrp_server_pb2_grpc import NrpCoreStub

from nrp_core.nrp_server_launchers import *


class NrpCore:

    TIMEOUT_SEC = 15

    def __init__(self, address, experiment_folder="", config_file="simulation_config.json", args="",
                 log_output=True, docker_server_address="", image_name="") -> None:
        """Spawns NRPCoreSim with given arguments

        :param str address: the address that will be used by NRPCoreSim server
        :param str experiment_folder: path to the folder containing all files needed to execute the experiment.
                                        By default, is an empty string, which is interpreted as the current folder.
        :param str config_file: path to the experiment configuration file. It can be an absolute path or
                                relative to `experiment_folder`.
                                By default, "simulation_config.json"
        :param str args: additional NRPCoreSim command line parameters
        :param bool log_output: if True, console output from NRPCoreSim process is hidden and logged into a file named
                                .console_output.log in the experiment folder. Default is True.
        :param str docker_server_address: IP address of the \ref docker_launcher_manager "Docker Manager server" used
                                            to launch the experiment in a docker container (more detail below).
                                            Empty by default.
        :param str image_name: name of the docker image which will be used to run the experiment (more detail below).
                                Empty by default.
        """
        
        self._devnull = open(os.devnull, 'w')

        # Initialize variables in case NRPServer launching fails
        self._sim_state = "None"
        self._is_open = False
        self._channel = None
        self._stub = None

        # Spawn NRP Core server process with chosen launcher
        log_file = ".console_output.log"
        server_args = ["-c", config_file, "-m", "server", "--server_address", address]
        if args:
            server_args += args.split(" ")
        if log_output:
            server_args += ["--logoutput=all", f"--logfilename={log_file}"]

        self._launcher = None

        if docker_server_address:
            self._launcher = NRPCoreDockerLauncher(server_args,
                                                   experiment_folder,
                                                   docker_server_address,
                                                   image_name,
                                                   log_file)
        else:
            self._launcher = NRPCoreForkLauncher(server_args, experiment_folder)

        # Connect to server
        self._channel = grpc.insecure_channel(address)
        self._stub = NrpCoreStub(self._channel)

        # Wait for the server to start
        n = self._wait_until_server_ready(self._channel, self._launcher.is_alive_nrp_process)
        if n > 0:
            self._launcher.kill_nrp_process()
            if n >= self.TIMEOUT_SEC:
                raise TimeoutError("Timeout during NRP Server startup.")
            else:
                raise ChildProcessError("Error during NRP Server startup.")

        # Set simulation state
        self._sim_state = "Created"
        self._is_open = True

    def __del__(self) -> None:
        """Deletes the NRP Core objects and kills the server"""
        try:
            with redirect_stdout(self._devnull):
                self.shutdown()
                self._close_client()
        except grpc.RpcError:
            pass

        self._devnull.close()

    def _close_client(self):

        if self._channel:
            self._channel.close()
            self._channel = None
            self._is_open = False

        print("NRP Server is closed now. This object can be deleted.")

    def _wait_until_server_ready(self, channel, is_server_alive) -> int:
        """Waits for the gRPC server of the NRP Core process to start"""
        n = 1
        while n <= self.TIMEOUT_SEC:
            if not is_server_alive():
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
            if self._stub:
                self._call_rpc(self._stub.shutdown, EmptyMessage())

        self._launcher.kill_nrp_process()

# EOF
