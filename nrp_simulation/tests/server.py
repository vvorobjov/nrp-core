from nrp_core.client import NrpCore
from nrp_core.data.nrp_protobuf.enginetest_pb2 import TestPayload

import sys
import os
import unittest
from threading import Thread
import json


class TestNrpServer(unittest.TestCase):


    def setUp(self) -> None:
        """Test fixture setup method, spawns an instance of NRP Core client."""
        address = "localhost:50051"
        config_file = "simulation_config.json"

        self.nrp_core = NrpCore(address, config_file=config_file)


    def tearDown(self) -> None:
        """
        Test fixture teardown method.
        Destroys the instance of NRP Core client created by the setup function.
        """
        if hasattr(self, 'nrp_core'):
            del self.nrp_core


    def test_basic(self):
        """Tests normal simulation execution."""
        self.assertEqual(self.nrp_core.current_state(), 'Created')

        self.nrp_core.initialize()
        self.assertEqual(self.nrp_core.current_state(), 'Initialized')

        self.nrp_core.run_loop(4)
        self.assertEqual(self.nrp_core.current_state(), 'Stopped')

        self.nrp_core.reset()
        self.assertEqual(self.nrp_core.current_state(), 'Initialized')

        self.nrp_core.run_until_timeout()
        self.assertEqual(self.nrp_core.current_state(), 'Stopped')

        self.nrp_core.shutdown()
        self.assertEqual(self.nrp_core.current_state(), 'Created')


    def test_async(self):
        pass
        """Tests normal async execution."""
        """# run simulation for high number of iterations
        self.nrp_core.initialize()
        t = self.nrp_core.run_loop(int(1e7), run_async=True)
        self.assertEqual(type(t), Thread)
        self.assertEqual(t.is_alive(), True)
        # stop simulation
        self.nrp_core.stop()
        # give some time for the simulation to stop but not as much as it would take to complete 1e7 iterations
        t.join(10)
        self.assertEqual(t.is_alive(), False)"""


    def test_constructor_errors(self):
        # Destroy the default NrpCore object created by SetUp method

        self.tearDown()

        # Pass an incorrect address

        with self.assertRaises(Exception):
            NrpCore("garbage_address", config_file="simulation_config.json", server_timeout=1)

        # Pass a wrong config file

        with self.assertRaises(Exception):
            NrpCore("localhost:50051", config_file="wrong_file", server_timeout=1)


    def test_done_flag(self):

        self.tearDown()

        address = "localhost:50051"
        config_file = "simulation_config_done_flag.json"

        self.nrp_core = NrpCore(address, config_file=config_file)

        self.nrp_core.initialize()

        done_flag, trajectory = self.nrp_core.run_loop(1)
        self.assertTrue(done_flag)
        done_flag, trajectory = self.nrp_core.run_loop(1)
        self.assertFalse(done_flag)


    def test_set_proto_datapack(self):

        # Prepare test payload

        data = TestPayload()
        data.integer = 555

        # Store the payload in the buffer

        self.assertEqual(len(self.nrp_core._proto_datapack_out_buffer), 0)
        self.nrp_core.set_proto_datapack("test", "test", data)
        self.assertEqual(len(self.nrp_core._proto_datapack_out_buffer), 1)

        # Send the payload to NRP Core with run_loop()
        # The buffer should be empty after the call

        self.nrp_core.initialize()
        self.nrp_core.run_loop(1)
        self.assertEqual(len(self.nrp_core._proto_datapack_out_buffer), 0)

        # The buffer should be cleared after a reset

        self.nrp_core.set_proto_datapack("test", "test", data)
        self.assertEqual(len(self.nrp_core._proto_datapack_out_buffer), 1)
        self.nrp_core.reset()
        self.assertEqual(len(self.nrp_core._proto_datapack_out_buffer), 0)


    def test_set_json_datapack(self):

        # Prepare test payload

        data = {}
        data["test_data"] = 888

        # Store the payload in the buffer

        self.assertEqual(len(self.nrp_core._json_datapack_out_buffer), 0)
        self.nrp_core.set_json_datapack("actions", "python_1", data)
        self.assertEqual(len(self.nrp_core._json_datapack_out_buffer), 1)

        # Send the payload to NRP Core with run_loop()
        # The buffer should be empty after the call

        self.nrp_core.initialize()
        self.nrp_core.run_loop(1)
        self.assertEqual(len(self.nrp_core._json_datapack_out_buffer), 0)

        # The buffer should be cleared after a reset

        self.nrp_core.set_json_datapack("actions", "python_1", data)
        self.assertEqual(len(self.nrp_core._json_datapack_out_buffer), 1)
        self.nrp_core.reset()
        self.assertEqual(len(self.nrp_core._json_datapack_out_buffer), 0)


    def test_loopback_json_datapacks(self):

        self.tearDown()

        address = "localhost:50051"
        config_file = "simulation_config_loopback.json"

        self.nrp_core = NrpCore(address, config_file=config_file)

        data = {}
        data["test_data"] = 888
        self.nrp_core.set_json_datapack("actions", "python_1", data)

        self.nrp_core.initialize()
        done_flag, trajectory = self.nrp_core.run_loop(1)

        self.assertEqual(len(trajectory), 1)
        self.assertEqual(trajectory[0]["test_data"], 0)

        done_flag, trajectory = self.nrp_core.run_loop(1)

        self.assertEqual(len(trajectory), 1)
        self.assertEqual(trajectory[0]["test_data"], 888)


    def test_trajectory_json(self):

        self.tearDown()

        address = "localhost:50051"
        config_file = "simulation_config_trajectory_json.json"

        self.nrp_core = NrpCore(address, config_file=config_file)

        self.nrp_core.initialize()

        done_flag, timeout_flag, trajectory = self.nrp_core.run_until_timeout()

        self.assertTrue(done_flag)
        self.assertFalse(timeout_flag)
        self.assertEqual(len(trajectory), 4)

        for i, datapack in enumerate(trajectory):
            self.assertEqual(datapack["iteration"], i)


    def test_trajectory_until_timeout(self):

        self.tearDown()

        address = "localhost:50051"
        config_file = "simulation_config_trajectory_timeout.json"

        self.nrp_core = NrpCore(address, config_file=config_file)

        self.nrp_core.initialize()

        done_flag, timeout_flag, trajectory = self.nrp_core.run_until_timeout()

        self.assertFalse(done_flag)
        self.assertTrue(timeout_flag)
        self.assertEqual(len(trajectory), 4)

        for i, datapack in enumerate(trajectory):
            self.assertEqual(datapack["iteration"], i)


    def test_trajectory_proto(self):

        self.tearDown()

        address = "localhost:50051"
        config_file = "simulation_config_trajectory_proto.json"

        self.nrp_core = NrpCore(address, config_file=config_file)

        self.nrp_core.initialize()

        # The done flag should be False, which means that we should not receive
        # any trajectory data after running the run_loop

        done_flag, timeout_flag, trajectory = self.nrp_core.run_until_timeout()

        self.assertTrue(done_flag)
        self.assertFalse(timeout_flag)
        self.assertEqual(len(trajectory), 8)

        # The trajectory should contain alternating Dump.String and
        # EngineTest.TestPayload proto messages
        # The order of messages coming from the Status Function should be preserved
        for i, data in enumerate(trajectory):
            if i % 2:  # 1, 3, 5...
                self.assertEqual(data.integer, i // 2)
            else:  # 0, 2, 4...
                json_data = json.loads(data.string_stream)
                self.assertEqual(json_data["int"], i // 2)


    def test_trajectory_mixed(self):
        """
        Tests trajectories with mixed JSON and proto messages
        """

        self.tearDown()

        address = "localhost:50051"
        config_file = "simulation_config_trajectory_mixed.json"

        self.nrp_core = NrpCore(address, config_file=config_file)

        self.nrp_core.initialize()

        done_flag, timeout_flag, trajectory = self.nrp_core.run_until_timeout()

        self.assertTrue(done_flag)
        self.assertFalse(timeout_flag)
        self.assertEqual(len(trajectory), 8)

        # The trajectory should contain alternating JSON objects and
        # EngineTest.TestPayload proto messages
        # The order of messages coming from the Status Function should be preserved
        for i, data in enumerate(trajectory):
            if i % 2:  # 1, 3, 5...
                self.assertEqual(data.integer, i // 2)
            else:  # 0, 2, 4...
                self.assertEqual(data["int"], i // 2)


    def test_runloop_no_init(self):
        """
        Tests calling runLoop() before initialize().
        It should not be possible to call runLoop() before initialize().
        """
        with self.assertRaises(Exception):
            self.nrp_core.run_loop(5)


    def test_shutdown_no_init(self):
        """
        Tests calling shutdown() before initialize().
        Currently it's not allowed to call shutdown in 'Created' state
        """
        self.assertEqual(self.nrp_core.current_state(), 'Created')
        with self.assertRaises(Exception):
            self.nrp_core.shutdown()


    def test_double_init(self):
        """
        Tests calling initialize() twice.
        The second call to initialize() should raise an exception.
        """
        self.nrp_core.initialize()
        with self.assertRaises(Exception):
            self.nrp_core.initialize()


# Change directory to the experiment's directory
os.chdir(sys.argv[2])

if __name__ == '__main__':
    unittest.main(argv=['first-arg-is-ignored'])

# EOF
