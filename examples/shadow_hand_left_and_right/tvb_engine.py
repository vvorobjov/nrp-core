"""Python Engine 1. Will get current engine time and make it accessible as a datapack"""

import numpy as np

from nrp_core.engines.python_json import EngineScript,RegisterEngine
from tvb.contrib.tests.cosimulation.parallel.function_tvb import TvbSim


@RegisterEngine()
class Script(EngineScript):

    def _initialize_tvb(self):
        weight = np.array([[1, 1, 0, 1],
                           [1, 1, 1, 1],
                           [0, 1, 1, 1],
                           [1, 1, 1, 1]])

        delay = np.array([[1.5, 1.5, 1.5, 1.5],
                          [1.5, 1.5, 1.5, 1.5],
                          [1.5, 1.5, 1.5, 1.5],
                          [1.5, 1.5, 1.5, 1.5]])

        self.resolution_simulation_ms = 0.1
        self.resolution_monitor_ms    = 1.0
        self.synchronization_time_ms  = 1.0

        proxy_id = [0, 1]

        self.firing_rate = np.array([[20.0, 10.0]]) * 10 ** -3  # time units in tvb is ms so the rate is in KHz

        self.sim = TvbSim(weight, delay, proxy_id, self.resolution_simulation_ms, self.synchronization_time_ms)

        self.series_time = []
        self.series_data = []

        self.time, self.result = self.sim(self.resolution_monitor_ms, [np.array([self.resolution_simulation_ms]), self.firing_rate])
        self.series_time.append(self.time)
        self.series_data.append(self.result)


    def _initialize_datapacks(self):
        self._registerDataPack("left_index_finger_joints")
        self._registerDataPack("right_index_finger_joints")

        self._registerDataPack("left_index_finger_target")
        self._registerDataPack("right_index_finger_target")

        self._setDataPack("left_index_finger_target",  { "positions" : [0.0, 0.0, 0.0, 0.0] })
        self._setDataPack("right_index_finger_target", { "positions" : [0.0, 0.0, 0.0, 0.0] })

        self.position = 0.0


    def initialize(self):
        self._initialize_tvb()
        self._initialize_datapacks()


    def runLoop(self, timestep_ns):

        # Get positions of left and right index finger's joints from previous iteration

        left_if  = self._getDataPack("left_index_finger_joints")
        right_if = self._getDataPack("right_index_finger_joints")

        print(left_if["positions"])
        print(right_if["positions"])

        # Calculate simulation time based on requested timestep

        timestep_ms = timestep_ns.count() / 1000000.0
        simulation_time_ms = (timestep_ms / self.synchronization_time_ms) * self.synchronization_time_ms

        # Run TVB simulation

        self.time, self.result = self.sim(simulation_time_ms,
                                          [self.time + self.resolution_monitor_ms,
                                          np.repeat(self.firing_rate.reshape(1, 2),
                                                    int(self.resolution_monitor_ms / self.resolution_simulation_ms), axis=0)])
        self.series_time.append(self.time)
        self.series_data.append(self.result)

        # Set target positions for left and right index fingers

        self.position = self.position + 0.01

        self._setDataPack("left_index_finger_target",  { "positions" : [self.position, self.position, self.position, 0.0] })
        self._setDataPack("right_index_finger_target", { "positions" : [self.position, self.position, self.position, 0.0] })


    def shutdown(self):
        pass

# EOF
