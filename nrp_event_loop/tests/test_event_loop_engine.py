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
from threading import Thread
import unittest
from nrp_core.event_loop import EventLoopInterface

from nrp_core.event_loop import EngineWrapper, EventLoopEngine


class EngineWrapperTest(EngineWrapper):

    def __init__(self):
        self._config = {}
        self._initCalls = 0
        self._runCalls = 0
        self._setDPCalls = 0
        self._dpMsg = ""
        self._shutdownCalls = 0

    def get_engine_name(self) -> str:
        return "test_engine"

    def get_registered_datapack_names(self) -> list:
        return ["test_dp"]

    def set_datapack(self, datapack):
        self._setDPCalls += 1
        self._dpMsg = datapack

    def get_datapack(self, dp_name: str):
        raise Exception("not needed in tests")

    def initialize(self, config: dict):
        self._config = config
        self._initCalls += 1

    def run_loop(self, timestep_ns: int):
        self._runCalls += 1

    def shutdown(self):
        self._shutdownCalls += 1


class EventLoopEngineTest(EventLoopEngine):

    def __init__(self, timestep: float, timestep_thres: float, store_capacity: int,
                 do_process_last: bool,
                 engine_config: dict,
                 mqtt_config: dict,
                 engine_wrapper: EngineWrapperTest):
        super().__init__(timestep, timestep_thres, store_capacity, do_process_last,
                         engine_config, mqtt_config, engine_wrapper)

    def _datapack_from_str(self, dp_name, msg_str):
        return msg_str

    def _get_datapack_as_str(self, dp_name):
        raise Exception("This function can't be tested without connecting to MQTT, should never be called")


class TestEventLoopEngine(unittest.TestCase):

    def test_event_loop_engine(self):
        engine_config = {"test": True}
        engine = EngineWrapperTest()
        ele = EventLoopEngineTest(0.01, 0.001, 2, True, engine_config, {}, engine)

        # Initialize
        ele._allowNoMqtt = True
        ele.initialize()
        self.assertEqual(engine._initCalls, 1)
        self.assertTrue("test" in engine._config and engine._config["test"])

        # RunLoop and mqtt topic callback
        ele.run_loop(0.01)
        self.assertEqual(engine._runCalls, 1)
        self.assertEqual(engine._setDPCalls, 0)

        # since mqtt client is not connected we can't test publish datapacks
        # and testing subscribe datapacks must be faked
        ele._topic_callback("not_registered_dp", "dp_data")
        ele.run_loop(0.01)
        self.assertEqual(engine._setDPCalls, 0)
        ele._topic_callback("test_dp", "dp_data")
        ele.run_loop(0.01)
        self.assertEqual(engine._setDPCalls, 1)
        self.assertEqual(engine._dpMsg, "dp_data")
        ele._topic_callback("test_dp", "data1")
        ele._topic_callback("test_dp", "data2")
        ele._topic_callback("test_dp", "data3")
        ele.run_loop(0.01)
        self.assertEqual(engine._setDPCalls, 2)
        # Engine configured to process only last msg, but queue capacity is set to 2, so last published msg
        # has been discarded
        self.assertEqual(engine._dpMsg, "data2")
        
        # Shutdown
        ele.shutdown()
        self.assertEqual(engine._shutdownCalls, 1)


if __name__ == '__main__':
    unittest.main()

# EOF
