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


if __name__ == '__main__':
    unittest.main()


# EOF
