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
        self.assertEqual(server_callbacks.script.shutdown_num_execs, 0)
        server_callbacks.shutdown(request_json)
        self.assertEqual(server_callbacks.script.shutdown_num_execs, 1)


    def test_shutdown_failure(self):
        """
        Try to shutdown the Script class using proper callback.
        The shutdown() method should raise an exception.
        It should be caught by the callback and translated into a status message.
        """
        request_json = {"PythonFileName": "test_files/test_script_raise.py"}
        server_callbacks.initialize(request_json)
        with self.assertRaisesRegex(Exception, "Shutdown failed"):
            server_callbacks.shutdown(request_json)


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
        The runLoop() method of the script class should succeed (and increment a counter)
        and the callback should return an integrated simulation time.
        """
        request_json = {"PythonFileName": "test_files/test_script.py", "time_step": 20000000}
        server_callbacks.initialize(request_json)
        self.assertEqual(server_callbacks.script.run_loop_num_execs, 0)
        result = server_callbacks.run_loop(request_json)
        self.assertEqual(result["time"], 20000000)
        self.assertEqual(server_callbacks.script.run_loop_num_execs, 1)
        result = server_callbacks.run_loop(request_json)
        self.assertEqual(result["time"], 40000000)
        self.assertEqual(server_callbacks.script.run_loop_num_execs, 2)


    def test_run_loop_failure(self):
        """
        Try to run loop step of the Script class using proper callback.
        The runLoop() method of the Script class should raise an exception.
        It should be caught by the callback and translated into a status message.
        """
        request_json = {"PythonFileName": "test_files/test_script_raise.py", "time_step": 20000000}
        server_callbacks.initialize(request_json)
        with self.assertRaisesRegex(Exception, "RunLoop failed"):
            server_callbacks.run_loop(request_json)


    def test_set_get_datapack(self):
        """
        Try to set datapacks on the Script class using proper callback.
        The _setDataPack() method of the script class should succeed and the callback
        should return a status message.
        """
        request_json = {"PythonFileName": "test_files/test_script.py"}
        server_callbacks.initialize(request_json)
        request_json = {}
        request_json["test_datapack"] = {"data": {"test_int": 1}}
        server_callbacks.set_datapack(request_json)
        get_request = {"test_datapack": {"engine_name": "python", "type": ""}}
        datapacks = server_callbacks.get_datapack(get_request)
        self.assertEqual(datapacks["test_datapack"]["engine_name"], "python")
        self.assertEqual(datapacks["test_datapack"]["type"], "")
        self.assertEqual(datapacks["test_datapack"]["data"], {"test_int": 1})


    def test_set_datapack_failure(self):
        """
        Try to set datapacks on the Script class using proper callback.
        The _setDataPack() method of the script class should raise an exception.
        It should be caught by the callback and translated into a status message.
        """
        request_json = {"PythonFileName": "test_files/test_script_raise.py"}
        server_callbacks.initialize(request_json)
        request_json = {}
        request_json["test_datapack"] = {"data": {"test_int": 1}}
        with self.assertRaisesRegex(Exception, "Attempting to set data on an unregistered DataPack .*"):
            server_callbacks.set_datapack(request_json)


    def test_get_datapack_failure(self):
        """
        Try to set datapacks on the Script class using proper callback.
        The _setDataPack() method of the script class should raise an exception.
        It should be caught by the callback and translated into a status message.
        """
        request_json = {"PythonFileName": "test_files/test_script_raise.py"}
        server_callbacks.initialize(request_json)
        get_request = {"test_datapack": {"engine_name": "python", "type": ""}}
        with self.assertRaisesRegex(Exception, "Attempting to get data from an unregistered DataPack .*"):
            server_callbacks.get_datapack(get_request)


if __name__ == '__main__':
    unittest.main()


# EOF
