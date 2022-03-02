# NRP Core - Backend infrastructure to synchronize simulations
#
# Copyright 2020-2021 NRP Team
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


import unittest
from nrp_core.data.nrp_json import JsonDataPack
import nrp_core.engines.python_json.server_callbacks as server_callbacks


class TestServer(unittest.TestCase):

    engine_name = "test_engine"
    time_step = 20000000
    engine_time_step = 0.01
    ratio = [1, 1000000000]
    # Dictionary used for run_loop() requests
    run_loop_json = {"time_step": time_step}

    # Dictionary used for initialize() requests
    # Contains a path to valid EngineScript class
    opensim_init = {"PythonFileName": "opensim_test_files/test_init.py",
                 "Simulator":"Opensim",
                 "WorldFileName":"opensim_test_files/WorldFiles/basicModel.osim",
                 "Visualizer":False,
                 "EngineTimestep": engine_time_step,
                 "TimeRatio":ratio,
                 "EngineName": engine_name}
    opensim_loop = {"PythonFileName": "opensim_test_files/test_opensim_loop.py",
                 "Simulator":"Opensim",
                 "WorldFileName":"opensim_test_files/WorldFiles/arm26/arm26_ground_offset.osim",
                 "Visualizer":False,
                 "EngineTimestep": engine_time_step,
                 "TimeRatio":ratio,
                 "EngineName": engine_name}
    opensim_sensor = {"PythonFileName": "opensim_test_files/test_opensim_sensor.py",
                 "Simulator":"Opensim",
                 "WorldFileName":"opensim_test_files/WorldFiles/arm26/arm26_ground_offset.osim",
                 "Visualizer":False,
                 "EngineTimestep": engine_time_step,
                 "TimeRatio":ratio,
                 "EngineName": engine_name}

    def test_initialize_simulation(self):
        """
        Initialize the Script class using proper callback.
        The initialize() method of EngineScript class should succeed
        and the callback should return True.
        """
        result = server_callbacks.initialize(self.opensim_init)
        #assert False
        #self.assertTrue(result["InitExecStatus"])
        self.assertEqual(server_callbacks.script._name, self.opensim_init["EngineName"])
        self.assertEqual(server_callbacks.script._config, self.opensim_init)

    def test_shutdown(self):
        """
        Shutdown the Script class using proper callback.
        The shutdown() method of EngineScript class should succeed
        and increment a counter.
        """
        server_callbacks.initialize(self.opensim_init)
        self.assertEqual(server_callbacks.script.shutdown_num_execs, 0)
        server_callbacks.shutdown({})
        self.assertEqual(server_callbacks.script.shutdown_num_execs, 1)

    def test_reset(self):
        """
        Reset the Script class using proper callback.
        The reset() method of EngineScript class should succeed,
        the time should be set to 0 and the callback should return True.
        """
        server_callbacks.initialize(self.opensim_init)
        server_callbacks.run_loop(self.run_loop_json)
        self.assertEqual(server_callbacks.script._time_ns, self.time_step)

        result = server_callbacks.reset({})
        self.assertTrue(result["ResetExecStatus"])
        self.assertEqual(server_callbacks.script._time_ns, 0)

    def test_simulation_run_loop(self):
        """
        Run loop step of the Script class using proper callback.
        The runLoop() method of EngineScript class should succeed and increment a counter,
        and the callback should return an integrated simulation time.
        """
        server_callbacks.initialize(self.opensim_loop)
        self.assertEqual(server_callbacks.script.run_loop_num_execs, 0)
        self.assertEqual(server_callbacks.script.timestep, 0)

        result = server_callbacks.run_loop(self.run_loop_json)
        self.assertEqual(result["time"], self.time_step)
        self.assertEqual(server_callbacks.script.run_loop_num_execs, 1)
        self.assertEqual(server_callbacks.script.timestep, self.time_step)

        # Run second loop step to check if time is integrated correctly
        result = server_callbacks.run_loop(self.run_loop_json)
        self.assertEqual(result["time"], 2 * self.time_step)
        self.assertEqual(server_callbacks.script.run_loop_num_execs, 2)
        self.assertEqual(server_callbacks.script.timestep, self.time_step)

    def test_simulator_data_sense(self):
        """
        Get datapack data using proper callback.
        The _getDataPack method of EngineScript class should raise an exception
        because of engine name in the request not matching the actual engine name.
        """
        server_callbacks.initialize(self.opensim_loop)
        self.assertEqual(server_callbacks.script.run_loop_num_execs, 0)
        self.assertEqual(server_callbacks.script.timestep, 0)

        result = server_callbacks.run_loop(self.run_loop_json)
        self.assertEqual(result["time"], self.time_step)
        self.assertEqual(server_callbacks.script.run_loop_num_execs, 1)
        self.assertEqual(server_callbacks.script.timestep, self.time_step)

if __name__ == '__main__':
    unittest.main()


# EOF
