from nrp_core.client import NrpCore

import sys
import os
import unittest

class TestNrpServer(unittest.TestCase):

    def setUp(self) -> None:
        """Test fixture setup method, spawns an instance of NRP Core client."""
        address = "localhost:50051"
        args = ["-c", "simulation_config.json",
                "-p", "NRPNestJSONEngine.so,NRPGazeboGrpcEngine.so"]

        self.nrp_core = NrpCore(address, args)

    def tearDown(self) -> None:
        """
        Test fixture teardown method.
        Destroys the instance of NRP Core client created by the setup function.
        """
        del self.nrp_core

    def test_basic(self):
        """Tests normal simulation execution."""
        self.nrp_core.initialize()
        self.nrp_core.runLoop(5)
        self.nrp_core.runLoop(3)
        self.nrp_core.shutdown()

    def test_runloop_no_init(self):
        """
        Tests calling runLoop() before initialize().
        It should not be possible to call runLoop() before initialize().
        """
        self.assertRaises(Exception, self.nrp_core.runLoop, 5)

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
        self.assertRaises(Exception, self.nrp_core.initialize)

# Change directory to the experiment's directory

os.chdir(sys.argv[4])

if __name__ == '__main__':
    unittest.main(argv=['first-arg-is-ignored'])

# EOF
