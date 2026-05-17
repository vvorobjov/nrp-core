import ctypes
import os
import signal
import psutil


from abc import ABC, abstractmethod


# Direct libc fork() handle. os.fork() (Python wrapper) runs every
# callback registered via os.register_at_fork() — including the
# threading module's _after_fork, which on CPython 3.10 (ubuntu22)
# walks all known threading locks and rebuilds them. Any of the
# locks held by a background thread that does NOT exist in the child
# (gRPC stub init thread, rclcpp DDS thread, etc.) trips an internal
# `if owner is _MainThread.ident` assertion and the child aborts
# with `Fatal Python error: Aborted` in threading.py:1599 _after_fork
# -> threading.py:101 RLock — long before exec() can take over. We
# still want a real fork() here (same semantics as before: parent
# keeps the child_pid; child execvp's NRPCoreSim), so call libc fork
# via ctypes to skip Python's at-fork machinery entirely. The child
# becomes a separate process exactly as with os.fork(); it just
# doesn't run any Python code between fork() and execvp(), so the
# threading hook can't fire.
_libc = ctypes.CDLL("libc.so.6", use_errno=True)
_libc.fork.restype = ctypes.c_int


def _raw_fork() -> int:
    pid = _libc.fork()
    if pid < 0:
        err = ctypes.get_errno()
        raise OSError(err, os.strerror(err), "libc fork")
    return pid


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
        self.child_pid = _raw_fork()
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
