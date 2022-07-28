from nrp_core.client import NrpCore

import sys
import os
import unittest
from threading import Thread


class TestNrpServer(unittest.TestCase):


    def setUp(self) -> None:
        """Test fixture setup method, spawns an instance of NRP Core client."""
        address = "localhost:50051"
        config_file = "simulation_config_status.json"

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

        self.nrp_core.run_loop(5)
        self.assertEqual(self.nrp_core.current_state(), 'Stopped')

        self.nrp_core.reset()
        self.assertEqual(self.nrp_core.current_state(), 'Initialized')

        self.nrp_core.run_until_timeout()
        self.assertEqual(self.nrp_core.current_state(), 'Stopped')

        self.nrp_core.shutdown()
        self.assertEqual(self.nrp_core.current_state(), 'Created')


    def test_async(self):
        """Tests normal async execution."""
        # run simulation for high number of iterations
        self.nrp_core.initialize()
        t = self.nrp_core.run_loop(int(1e7), True)
        self.assertEqual(type(t), Thread)
        self.assertEqual(t.is_alive(), True)
        # stop simulation
        self.nrp_core.stop()
        # give some time for the simulation to stop but not as much as it would take to complete 1e7 iterations
        t.join(10)
        self.assertEqual(t.is_alive(), False)


    def test_constructor_errors(self):
        # Destroy the default NrpCore object created by SetUp method

        self.tearDown()

        # Pass an incorrect address

        with self.assertRaises(Exception):
            NrpCore("garbage_address", config_file="simulation_config.json", server_timeout=1)

        # Pass a wrong config file

        with self.assertRaises(Exception):
            NrpCore("localhost:50051", config_file="wrong_file", server_timeout=1)


    def test_runloop_status(self):
        self.nrp_core.initialize()

        # On first iteration run_loop should return a list of integer

        status = self.nrp_core.run_loop(1)
        expected_dict = {"integers": [0, 1, 2]}
        self.assertDictEqual(status, expected_dict)

        # On second iteration run_loop should return a list of booleans

        status = self.nrp_core.run_loop(1)
        expected_dict = {"flags": [True, False]}
        self.assertDictEqual(status, expected_dict)

        # On third iteration run_loop should return an empty status message

        status = self.nrp_core.run_loop(1)
        self.assertEqual(status, None)


    def test_runloop_status_multiple_steps(self):
        self.nrp_core.initialize()

        # Skip the first iteration
        # On the second iteration run_loop should return a list of booleans

        status = self.nrp_core.run_loop(2)
        expected_dict = {"flags": [True, False]}
        self.assertDictEqual(status, expected_dict)

        # On third iteration run_loop should return an empty status message

        status = self.nrp_core.run_loop(1)
        self.assertEqual(status, None)


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
os.chdir(sys.argv[4])

if __name__ == '__main__':
    unittest.main(argv=['first-arg-is-ignored'])

# EOF
