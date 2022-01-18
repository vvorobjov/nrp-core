from http import server
import unittest
import nrp_core.engines.python_json.server_callbacks as server_callbacks


class TestServer(unittest.TestCase):

    def test_initialize(self):
        """
        Try to initialize the Script class using proper callback.
        The initialize() method should succeed and the callback should return a status message.
        """
        request_json = {"PythonFileName": "test_files/test_script.py"}
        result = server_callbacks.initialize(request_json)
        self.assertTrue(result["InitExecStatus"])


    def test_initialize_failure(self):
        """
        Try to initialize the Script class using proper callback.
        The initialize() method should raise an exception.
        It should be caught by the callback and translated into a status message.
        """
        request_json = {"PythonFileName": "test_files/test_script_raise.py"}
        result = server_callbacks.initialize(request_json)
        self.assertFalse(result["InitExecStatus"])
        self.assertEqual(result["Message"], "Initialization failed")


    def test_shutdown(self):
        """
        Try to shutdown the Script class using proper callback.
        The shutdown() method should succeed and the callback should return a status message.
        """
        request_json = {"PythonFileName": "test_files/test_script.py"}
        server_callbacks.initialize(request_json)
        result = server_callbacks.shutdown(request_json)
        self.assertTrue(result["ShutdownExecStatus"])


    def test_shutdown_failure(self):
        """
        Try to shutdown the Script class using proper callback.
        The shutdown() method should raise an exception.
        It should be caught by the callback and translated into a status message.
        """
        request_json = {"PythonFileName": "test_files/test_script_raise.py"}
        server_callbacks.initialize(request_json)
        result = server_callbacks.shutdown(request_json)
        self.assertFalse(result["ShutdownExecStatus"])
        self.assertEqual(result["Message"], "Shutdown failed")


    def test_reset(self):
        """
        Try to resest the Script class using proper callback.
        The reset() method of the script class should succeed and the callback
        should return a status message.
        """
        request_json = {"PythonFileName": "test_files/test_script.py"}
        server_callbacks.initialize(request_json)
        result = server_callbacks.reset(request_json)
        self.assertTrue(result["ResetExecStatus"])


    def test_reset_failure(self):
        """
        Try to reset the Script class using proper callback.
        The reset() method of the Script class should raise an exception.
        It should be caught by the callback and translated into a status message.
        """
        request_json = {"PythonFileName": "test_files/test_script_raise.py"}
        server_callbacks.initialize(request_json)
        result = server_callbacks.reset(request_json)
        self.assertFalse(result["ResetExecStatus"])
        self.assertEqual(result["Message"], "Reset failed")


    def test_run_loop(self):
        """
        Try to run loop step of the Script class using proper callback.
        The runLoop() method of the script class should succeed and the callback
        should return a status message and integrated simulation time.
        """
        request_json = {"PythonFileName": "test_files/test_script.py", "time_step": 20000000}
        server_callbacks.initialize(request_json)
        result = server_callbacks.run_loop(request_json)
        self.assertTrue(result["RunLoopExecStatus"])
        self.assertEqual(result["time"], 20000000)
        result = server_callbacks.run_loop(request_json)
        self.assertTrue(result["RunLoopExecStatus"])
        self.assertEqual(result["time"], 40000000)


    def test_run_loop_failure(self):
        """
        Try to run loop step of the Script class using proper callback.
        The runLoop() method of the Script class should raise an exception.
        It should be caught by the callback and translated into a status message.
        """
        request_json = {"PythonFileName": "test_files/test_script_raise.py", "time_step": 20000000}
        server_callbacks.initialize(request_json)
        result = server_callbacks.run_loop(request_json)
        self.assertFalse(result["RunLoopExecStatus"])
        self.assertEqual(result["Message"], "RunLoop failed")


if __name__ == '__main__':
    unittest.main()


# EOF
