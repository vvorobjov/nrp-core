import os
import signal
import psutil
from python_on_whales import DockerClient
import yaml


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

class NRPCoreComposeLauncher(NRPCoreServerLauncher):

    def __init__(self, compose_file, log_file, get_archives, address):
        local_user = os.path.expanduser('~').split('/')[-1]
        self.remote_address = address
        self._docker = DockerClient(compose_files=[compose_file.replace('HOME',local_user)])
        docker_context = self.MapServerToContext(self.remote_address)
        # docker_context = 'default'
        self._docker.context.use(docker_context)
        self._log_file = log_file
        self._get_archives = get_archives
        self._services = self.extract_services(compose_file.replace('HOME',local_user))

        self._docker.compose.up(quiet=True, detach=True)

    def MapServerToContext(self,address) -> str:

        for cont in self._docker.context.list():
            if self._docker.context.inspect(cont.name).endpoints['docker'].host.split('//')[1] == address.split(":")[0]:
                return cont.name

    def is_alive_nrp_process(self) -> bool:
        states = {}
        RunningServices = self._docker.compose.ps()
        for _service in self._services:
            try:
                states[_service], _  = self.findRunningState(self._services[_service]['container_name'], RunningServices)
            except KeyError:
                raise KeyError("The container name is not defined for the service {}".format(_service))
        for serv in states:
            if states[serv] != 'running':
                raise ChildProcessError('{} service is not running.'.format(serv))
                return False
        return True

    def extract_services(self,compose_file) -> list:
        with open(compose_file, 'r') as stream:
            compose_loaded = yaml.safe_load(stream)
        try:
            return compose_loaded['services']
        except:
            print("Bad compose file !")

    def findRunningState(self, input_serv, RunningServices):
        for serv in RunningServices:
            if serv.name == input_serv:
                return serv.state.status , serv.state.exit_code

    def get_exit_report(self) -> dict:
        states = {}
        exit_codes = {}
        # Get the list of running services
        RunningServices = self._docker.compose.ps()
        for _service in self._services:
            try:
                states[_service], exit_codes[_service] = self.findRunningState(self._services[_service]['container_name'], RunningServices)
            except KeyError:
                raise KeyError("The container name is not defined for the service {}".format(_service))
        return {'exit_code': exit_codes,
            'logs': self._docker.compose.logs()}

    def kill_nrp_process(self) -> None:
        # Since we are mounting the experiment folder inside the nrp-core container, so we will have all files generated inside /experiment
        # folder inside nrp-core container after containers are stopped.
        docker_context = self.MapServerToContext(self.remote_address)
        # docker_context = 'default'
        self._docker.context.use(docker_context)
        while True:
            try:
                self._docker.compose.down()
            except:
                continue
            else:
                break

