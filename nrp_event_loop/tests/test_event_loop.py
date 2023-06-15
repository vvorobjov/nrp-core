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


import time
from threading import Thread
import unittest
from nrp_core.event_loop import EventLoopInterface


class EventLoopTest(EventLoopInterface):

    def __init__(self, timestep: float, timestep_thres: float):
        super().__init__(timestep, timestep_thres)

        self.init_call_count = 0
        self.run_loop_call_count = 0
        self.shutdown_call_count = 0

    def _initialize_cb(self):
        self.init_call_count += 1

    def _run_loop_cb(self):
        self.run_loop_call_count += 1

    def _shutdown_cb(self):
        self.shutdown_call_count += 1


class TestEventLoop(unittest.TestCase):

    def test_initialize(self):
        eloop = EventLoopTest(0.1, 0.01)
        self.assertFalse(eloop._is_initialized)
        eloop.initialize()
        self.assertTrue(eloop._is_initialized)
        self.assertEqual(eloop.init_call_count, 1)
        eloop.initialize()
        self.assertEqual(eloop.init_call_count, 1)

    def test_shutdown(self):
        eloop = EventLoopTest(0.1, 0.01)
        eloop.shutdown()
        self.assertEqual(eloop.shutdown_call_count, 0)
        eloop.initialize()
        eloop.shutdown()
        self.assertEqual(eloop.shutdown_call_count, 1)
        self.assertFalse(eloop._is_initialized)

    def test_run_loop(self):
        eloop = EventLoopTest(0.1, 0.01)
        eloop.run_loop(1)
        self.assertEqual(eloop.run_loop_call_count, 0)
        eloop.initialize()
        start = time.perf_counter_ns()
        eloop.run_loop(1)
        self.assertGreaterEqual(time.perf_counter_ns(), start + 1e9)
        self.assertGreaterEqual(eloop.run_loop_call_count, 10)

    def test_run_async(self):
        eloop = EventLoopTest(0.1, 0.01)
        eloop.initialize()
        t = Thread(target=eloop.run_loop, args=[100])
        start = time.perf_counter_ns()
        t.start()
        time.sleep(1)
        eloop.stop_loop()
        t.join()
        end = time.perf_counter_ns()
        self.assertLessEqual(eloop.run_loop_call_count, (end-start) / 1e8)
        self.assertGreaterEqual(eloop.run_loop_call_count, 10)


if __name__ == '__main__':
    unittest.main()

# EOF
