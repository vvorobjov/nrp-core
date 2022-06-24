import os
import signal
import psutil
import json

from abc import ABC, abstractmethod

from nrp_core import DockerConnector


class NRPCoreServerLauncher(ABC):

    @abstractmethod
    def is_alive_nrp_process(self) -> bool:
        raise NotImplementedError

    @abstractmethod
    def kill_nrp_process(self) -> None:
        raise NotImplementedError


class NRPCoreForkLauncher(NRPCoreServerLauncher):

    def __init__(self, args, experiment_folder):
        self.child_pid = os.fork()
        is_child = (self.child_pid == 0)

        # Server side
        if is_child:
            launch_command = ["NRPCoreSim", *args, "--slave"]
            if experiment_folder:
                launch_command += ["-d", experiment_folder]
            os.execvp("NRPCoreSim", launch_command)

    def is_alive_nrp_process(self) -> bool:
        try:
            p = psutil.Process(self.child_pid)
            if p.status() == "zombie":
                return False
        except psutil.NoSuchProcess:
            return False

        return True

    def kill_nrp_process(self) -> None:
        """Sends SIGTERM signal to the NRP Core subprocess"""
        try:
            os.kill(self.child_pid, signal.SIGTERM)
        except ProcessLookupError:
            pass

        # Server is closed now
        try:
            os.waitpid(self.child_pid, 0)
        except ChildProcessError:
            pass


class NRPCoreDockerLauncher(NRPCoreServerLauncher):

    def __init__(self, args, experiment_folder, docker_server_address, image_name, log_file):
        config = {"DockerServerAddress": docker_server_address,
                  "UploadFolder": experiment_folder,
                  "ProcCmd": "xvfb-run nrp-run.bash NRPCoreSim",
                  "ProcCmdArgs": args,
                  "ExecEnvironment": [],
                  "ImageName": image_name}

        self._log_file = log_file
        self._experiment_folder = experiment_folder
        self._docker = DockerConnector(json.dumps(config))
        self._docker.initializing()

    def is_alive_nrp_process(self) -> bool:
        return self._docker.get_container_status() == "RUNNING"

    def kill_nrp_process(self) -> None:
        if self.is_alive_nrp_process():
            self._docker.download_file(self._experiment_folder, self._log_file)
            self._docker.shutdown()
