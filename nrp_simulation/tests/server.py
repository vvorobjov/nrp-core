from nrp_core.client import NrpCore

import sys
import os
import unittest
from threading import Thread


class TestNrpServer(unittest.TestCase):

    def setUp(self) -> None:
        """Test fixture setup method, spawns an instance of NRP Core client."""
        address = "localhost:50051"
        args = "-p NRPNestJSONEngine.so,NRPGazeboGrpcEngine.so,NRPPythonJSONEngine.so"

        self.nrp_core = NrpCore(address, args=args)

    def tearDown(self) -> None:
        """
        Test fixture teardown method.
        Destroys the instance of NRP Core client created by the setup function.
        """
        del self.nrp_core

    def test_basic(self):
        """Tests normal simulation execution."""
        self.assertEqual(self.nrp_core.current_state(), 'Created')
        self.assertEqual(self.nrp_core.initialize(), True)
        self.assertEqual(self.nrp_core.current_state(), 'Initialized')
        self.assertEqual(self.nrp_core.run_loop(5), True)
        self.assertEqual(self.nrp_core.current_state(), 'Stopped')
        self.assertEqual(self.nrp_core.reset(), True)
        self.assertEqual(self.nrp_core.current_state(), 'Initialized')
        self.assertEqual(self.nrp_core.run_until_timeout(), True)
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
        self.nrp_core.shutdown()
        address = "localhost:50051"
        args = "-p NRPNestJSONEngine.so,NRPGazeboGrpcEngine.so,NRPPythonJSONEngine.so"

        # Pass an incorrect address so the client can't connect
        old_timeout = NrpCore.TIMEOUT_SEC
        NrpCore.TIMEOUT_SEC = 3
        self.assertRaises(TimeoutError, NrpCore, 'wrong_address', args=args)
        NrpCore.TIMEOUT_SEC = old_timeout
        # Pass wrong config file, make NRPCoreSim process die
        self.assertRaises(ChildProcessError, NrpCore, address, config_file="wrong_file", args=args)

    def test_runloop_no_init(self):
        """
        Tests calling runLoop() before initialize().
        It should not be possible to call runLoop() before initialize().
        """
        self.assertEqual(self.nrp_core.run_loop(5), False)

    def test_shutdown_no_init(self):
        """
        Tests calling shutdown() before initialize().
        It should be possible to call shutdown() even without calling initialize().
        """
        self.nrp_core.shutdown()

    def test_double_init(self):
        """
        Tests calling initialize() twice.
        The second call to initialize() should raise an exception.
        """
        self.nrp_core.initialize()
        self.assertEqual(self.nrp_core.initialize(), False)


# Change directory to the experiment's directory
os.chdir(sys.argv[4])

if __name__ == '__main__':
    unittest.main(argv=['first-arg-is-ignored'])

# EOF
