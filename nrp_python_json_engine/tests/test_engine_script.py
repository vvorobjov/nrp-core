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

from nrp_core.engines.python_json.engine_script import EngineScript

class Script(EngineScript):
    pass


class TestServer(unittest.TestCase):

    def test_datapack_register(self):
        """
        Try to register a datapack with given name.
        The datapack should be added to the _datapacks dict.
        """
        script = Script()
        datapack_name = "testDatapack"
        script._registerDataPack(datapack_name)
        self.assertEqual(len(script._datapacks), 1)
        self.assertEqual(script._datapacks[datapack_name], None)


    def test_datapack_register_overwrite(self):
        """Try to register the same datapack twice. Should raise an exception"""
        script = Script()
        datapack_name = "testDatapack"
        script._registerDataPack(datapack_name)
        with self.assertRaises(Exception):
            script._registerDataPack(datapack_name)
        self.assertEqual(len(script._datapacks), 1)


    def test_datapack_set(self):
        """Try to set data on a registered datapack"""
        script = Script()
        datapack_name = "testDatapack"
        data = {"test": 1}
        script._registerDataPack(datapack_name)
        script._setDataPack(datapack_name, data)
        self.assertEqual(script._datapacks[datapack_name], data)


    def test_datapack_set_unregistered(self):
        """Try to set data on a unregistered datapack. Should raise an exception."""
        script = Script()
        datapack_name = "testDatapack"
        data = {"test": 1}
        with self.assertRaises(Exception):
            script._setDataPack(datapack_name, data)
        self.assertEqual(len(script._datapacks), 0)


    def test_datapack_get(self):
        """
        Try to get data from a registered datapack.
        Should return None if the data was never set, and the latest data otherwise.
        """
        script = Script()
        datapack_name = "testDatapack"
        data = {"test": 1}
        script._registerDataPack(datapack_name)
        self.assertEqual(script._getDataPack(datapack_name), None)
        script._setDataPack(datapack_name, data)
        self.assertEqual(script._getDataPack(datapack_name), data)


    def test_datapack_get_unregistered(self):
        """Try to get data from an unregistered datapack. Should raise an exception."""
        script = Script()
        datapack_name = "testDatapack"
        with self.assertRaises(Exception):
            script._getDataPack(datapack_name)


    def test_time(self):
        """Try to advance the simulation time."""
        script = Script()
        self.assertEqual(script._time_ns, 0)
        timestep = 1000000
        script._advanceTime(timestep)
        self.assertEqual(script._time_ns, timestep)
        script._advanceTime(timestep)
        self.assertEqual(script._time_ns, 2 * timestep)


if __name__ == '__main__':
    unittest.main()

# EOF
