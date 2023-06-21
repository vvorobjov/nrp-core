# NRP Core - Backend infrastructure to synchronize simulations
#
# Copyright 2020-2023 NRP Team
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#   http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#
# This project has received funding from the European Union’s Horizon 2020
# Framework Programme for Research and Innovation under the Specific Grant
# Agreement No. 945539 (Human Brain Project SGA3).

import time
import logging
from threading import Event, Lock
from abc import ABC, abstractmethod


class EventLoopInterface(ABC):

    def __init__(self, timestep: float, timestep_thres: float):
        self._timestep = timestep
        self._timestep_thres = timestep_thres
        
        self._is_initialized = False
        self._stop_request = Event()
        self._run_lock = Lock()

        logger = logging.getLogger(__name__)
        logger.setLevel(logging.INFO)

    def run_loop_once(self, start_time: float):
        self._run_loop_cb()
        step_duration = time.perf_counter() - start_time
        if step_duration < self._timestep:
            time.sleep(self._timestep - step_duration)

    def run_loop(self, timeout: float):
        if not self._is_initialized:
            logging.debug("EventLoop is not initialized, runLoop can't be called")
            return
        
        logging.info("Starting Event Loop")

        with self._run_lock:
            use_timeout = timeout != 0.

            start_loop_time = time.perf_counter()
            start_step_old = start_loop_time

            while not self._stop_request.is_set():
                start_step_new = time.perf_counter()
                step_duration = start_step_new - start_step_old

                if use_timeout and start_step_new - start_loop_time >= timeout:
                    break

                if step_duration > self._timestep + self._timestep_thres:
                    logging.warning("Event Loop can't run at the target frequency. Actual step duration: {} (s). "
                                    "Target step duration:  {} (s).".format(step_duration, self._timestep))

                self.run_loop_once(start_step_new)

                start_step_old = start_step_new

            self._stop_request.clear()

        logging.info("Completed Event Loop")

    def stop_loop(self, wait=False):
        self._stop_request.set()
        if wait:
            self.wait_for_loop_end()

    def wait_for_loop_end(self):
        # Waiting for run loop end
        with self._run_lock:
            pass

    def initialize(self):
        if self._is_initialized:
            logging.debug("EventLoop is already initialized, initialize can't be called")
            return

        self._initialize_cb()
        self._is_initialized = True

    def shutdown(self):
        if not self._is_initialized:
            logging.debug("EventLoop is not initialized, shutdown can't be called")
            return

        self.stop_loop()
        self._shutdown_cb()
        self._is_initialized = False

    @abstractmethod
    def _initialize_cb(self):
        raise NotImplementedError("not implemented in EventLoop base class")

    @abstractmethod
    def _run_loop_cb(self):
        raise NotImplementedError("not implemented in EventLoop base class")

    @abstractmethod
    def _shutdown_cb(self):
        raise NotImplementedError("not implemented in EventLoop base class")
