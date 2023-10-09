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


import unittest
from nrp_core.data.nrp_json import JsonDataPack
import nrp_core.engines.python_json.server_callbacks as server_callbacks


class TestServer(unittest.TestCase):

    engine_name = "test_engine"
    time_step = 20000000
    ratio = [1, 1000000000]

    # Dictionary used for run_loop() requests
    run_loop_json = {"time_step": time_step}

    # Dictionary used for initialize() requests
    # Contains a path to valid EngineScript class
    init_json = {"PythonFileName": "test_files/test_script.py",
                 "TimeRatio": ratio,
                 "EngineName": engine_name}

    # Dictionary used for initialize() requests
    # Contains unsupported time ratio
    init_json_unsupported_ratio = {"PythonFileName": "test_files/test_script.py",
                                   "TimeRatio": [1, 1000000],
                                   "EngineName": engine_name}

    # Dictionary used for initialize() requests.
    # Methods of the EngineScript class included in PythonFileName raise exceptions
    init_json_raise = {"PythonFileName": "test_files/test_script_raise.py",
                       "TimeRatio": ratio,
                       "EngineName": engine_name}

    # Dictionary used for initialize() requests.
    # The reset() method of EngineScript class included in PythonFileName raises an exception
    init_json_reset_raise = {"PythonFileName": "test_files/test_script_reset_raise.py",
                             "TimeRatio": ratio,
                             "EngineName": engine_name}

    # Dictionary used for initialize() requests.
    # The Script class doesn't inherit from EngineScript
    init_json_fake = {"PythonFileName": "test_files/test_script_fake.py",
                      "TimeRatio": ratio,
                      "EngineName": engine_name}

    # Dictionary used for initialize() requests.
    # The initialize() method of EngineScript class included in PythonFileName is the only method that does not raises an exception
    init_json_srr_raise = {"PythonFileName": "test_files/test_script_srr_raise.py",
                      "TimeRatio": ratio,
                      "EngineName": engine_name}

    # Dictionary used for set_datapack() requests
    set_datapack_json = {}
    set_datapack_json["test_datapack"] = {"engine_name": engine_name,
                                          "type": JsonDataPack.getType(),
                                          "data": {"test_int": 1}}

    # Dictrionary used for get_datapack() requests
    get_datapack_json = {"test_datapack": {"engine_name": engine_name, "type": JsonDataPack.getType()}}

    def test_initialize(self):
        """
        Initialize the Script class using proper callback.
        The initialize() method of EngineScript class should succeed
        and the callback should return True.
        """
        server_callbacks.initialize(self.init_json)
        self.assertEqual(server_callbacks.script._name, self.init_json["EngineName"])
        self.assertEqual(server_callbacks.script._config, self.init_json)


    def test_initialize_script_inheritance(self):
        """
        Initialize the Script class using proper callback.
        The Script class doesn't inherit from EngineScript
        and the callback should return False and an error message
        """
        with self.assertRaisesRegex(Exception, "Script class must inherit from EngineScript class"):
            server_callbacks.initialize(self.init_json_fake)


    def test_initialize_failure(self):
        """
        Initialize the Script class using proper callback.
        The initialize() method of EngineScript class should raise an exception
        and the callback should return False and an error message.
        """
        with self.assertRaisesRegex(Exception, "Initialization failed"):
            server_callbacks.initialize(self.init_json_raise)


    def test_initialize_unsupported_ratio(self):
        """
        Initialize the Script class using proper callback.
        The initialize() callback should raise an exception (because of unsupported time units)
        and return False and an error message.
        """
        with self.assertRaisesRegex(Exception, "PythonJSONEngine only support nanoseconds"):
            server_callbacks.initialize(self.init_json_unsupported_ratio)


    def test_shutdown(self):
        """
        Shutdown the Script class using proper callback.
        The shutdown() method of EngineScript class should succeed
        and increment a counter.
        """
        server_callbacks.initialize(self.init_json)
        self.assertEqual(server_callbacks.script.shutdown_num_execs, 0)
        server_callbacks.shutdown({})
        self.assertEqual(server_callbacks.script.shutdown_num_execs, 1)


    def test_shutdown_failure(self):
        """
        Shutdown the Script class using proper callback.
        The shutdown() method of EngineScript class should raise an exception.
        """
        server_callbacks.initialize(self.init_json_srr_raise)
        with self.assertRaisesRegex(Exception, "Shutdown failed"):
            server_callbacks.shutdown({})


    def test_reset(self):
        """
        Reset the Script class using proper callback.
        The reset() method of EngineScript class should succeed,
        the time should be set to 0 and the callback should return True.
        """
        server_callbacks.initialize(self.init_json)
        server_callbacks.run_loop(self.run_loop_json)
        self.assertEqual(server_callbacks.script._time_ns, self.time_step)

        server_callbacks.reset({})
        self.assertEqual(server_callbacks.script._time_ns, 0)


    def test_reset_failure(self):
        """
        Reset the Script class using proper callback.
        The reset() method of EngineScript class should raise an exception
        and the callback should return False and an error message.
        The simulation time should not be reset!
        """
        server_callbacks.initialize(self.init_json_reset_raise)
        server_callbacks.run_loop(self.run_loop_json)
        self.assertEqual(server_callbacks.script._time_ns, self.time_step)

        with self.assertRaisesRegex(Exception, "Reset failed"):
            server_callbacks.reset({})
        self.assertEqual(server_callbacks.script._time_ns, self.time_step)


    def test_run_loop(self):
        """
        Run loop step of the Script class using proper callback.
        The runLoop() method of EngineScript class should succeed and increment a counter,
        and the callback should return an integrated simulation time.
        """
        server_callbacks.initialize(self.init_json)

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


    def test_run_loop_failure(self):
        """
        Run loop step of the Script class using proper callback.
        The runLoop() method of EngineScript class should raise an exception.
        """
        server_callbacks.initialize(self.init_json_srr_raise)
        with self.assertRaisesRegex(Exception, "RunLoop failed"):
            server_callbacks.run_loop(self.run_loop_json)


    def test_set_get_datapack(self):
        """
        Set and then retrieve datapack data using proper callbacks.
        The data passed to set_datapack() and retrieved from get_datapack()
        callbacks should match.
        """
        server_callbacks.initialize(self.init_json)

        # Test get_datapack with empty request
        self.assertEqual(server_callbacks.get_datapacks({}), {})

        # Test set_datapack with empty request
        server_callbacks.set_datapacks({})

        # If the data was never set, get_datapack() should return None accompanied by some metadata
        datapacks = server_callbacks.get_datapacks(self.get_datapack_json)
        self.assertEqual(datapacks["test_datapack"]["engine_name"], self.engine_name)
        self.assertEqual(datapacks["test_datapack"]["type"], JsonDataPack.getType())
        self.assertEqual(datapacks["test_datapack"]["data"], None)

        # Set the data
        server_callbacks.set_datapacks(self.set_datapack_json)

        # Retrieve the data. It should match the data that was set.
        datapacks = server_callbacks.get_datapacks(self.get_datapack_json)

        self.assertEqual(datapacks["test_datapack"], self.set_datapack_json["test_datapack"])


    def test_set_datapack_unregistered(self):
        """
        Set datapack data using proper callback.
        The _setDataPack method of EngineScript class should raise an exception
        because of unregistered datapack name.
        """
        server_callbacks.initialize(self.init_json_srr_raise)
        with self.assertRaisesRegex(Exception, "Attempting to set data on an unregistered DataPack .*"):
            server_callbacks.set_datapacks(self.set_datapack_json)


    def test_set_datapack_malformed(self):
        """
        Set datapack data using proper callback.
        The _setDataPack method of EngineScript class should raise an exception
        because of datapack type missing in the request.
        """
        server_callbacks.initialize(self.init_json_srr_raise)
        request_json = {}
        request_json["test_datapack"] = {"engine_name": self.engine_name,
                                         "data": {"test_int": 1}}

        with self.assertRaisesRegex(Exception, "Malformed DataPack. .*"):
            server_callbacks.set_datapacks(request_json)


    def test_get_datapack_unregistered(self):
        """
        Get datapack data using proper callback.
        The _getDataPack method of EngineScript class should raise an exception
        because of unregistered datapack name.
        """
        server_callbacks.initialize(self.init_json_srr_raise)
        with self.assertRaisesRegex(Exception, "Attempting to get data from an unregistered DataPack .*"):
            server_callbacks.get_datapacks(self.get_datapack_json)


    def test_get_datapack_incorrect_engine_name(self):
        """
        Get datapack data using proper callback.
        The _getDataPack method of EngineScript class should raise an exception
        because of engine name in the request not matching the actual engine name.
        """
        server_callbacks.initialize(self.init_json_srr_raise)
        request_json = {}
        request_json["test_datapack"] = {"engine_name": "other_engine_name",
                                         "type": JsonDataPack.getType()}

        with self.assertRaisesRegex(Exception, "Requesting DataPack .* from incorrect engine .*"):
            server_callbacks.get_datapacks(request_json)


if __name__ == '__main__':
    unittest.main()


# EOF
