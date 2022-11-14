import os
import signal
import psutil
import json

from abc import ABC, abstractmethod

from nrp_core.docker_handle import NRPDockerHandle


class NRPCoreServerLauncher(ABC):

    @abstractmethod
    def is_alive_nrp_process(self) -> bool:
        raise NotImplementedError

    @abstractmethod
    def kill_nrp_process(self) -> None:
        raise NotImplementedError

    @abstractmethod
    def get_exit_report(self) -> dict:
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

    def get_exit_report(self) -> dict:
        # TODO: implement
        return {'exit_code': None, 'logs': []}


class NRPCoreDockerLauncher(NRPCoreServerLauncher):

    def __init__(self, args, experiment_folder, docker_daemon_ip, image_name, log_file, get_archives):
        proc_cmd = 'nrp-run.bash xvfb-run NRPCoreSim ' + ' '.join(args)

        self._log_file = log_file
        self._get_archives = get_archives
        self._docker = NRPDockerHandle(docker_daemon_ip, image_name, experiment_folder, proc_cmd, [])

    def is_alive_nrp_process(self) -> bool:
        status = self._docker.get_status()
        return status and status['running']

    def get_exit_report(self) -> dict:
        status = self._docker.get_status()
        return {'exit_code': status['exit_code'] if status else None,
                'logs': self._docker.get_logs()}

    def kill_nrp_process(self) -> None:
        status = self._docker.get_status()
        if status:
            self._docker.get_experiment_archive(self._log_file)
            for archive in self._get_archives:
                self._docker.get_experiment_archive(archive)

            self._docker.stop()
            self._docker.remove()
