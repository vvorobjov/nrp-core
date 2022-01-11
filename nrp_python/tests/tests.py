import unittest

from engine_script import EngineScript

class Script(EngineScript):
    def initialize(self):
        pass
 
    def runLoop(self):
        pass
 
    def shutdown(self):
        pass

class TestServer(unittest.TestCase):

    def test_instantiate(self):
        script = Script()
        script.initialize()
        script.runLoop()
        script.shutdown()

    def test_datapack_register(self):
        script = Script()
        datapack_name = "testDatapack"
        script._registerDataPack(datapack_name)
        self.assertEqual(len(script.datapacks), 1)
        self.assertEqual(script.datapacks[datapack_name], None)

    def test_datapack_set(self):
        script = Script()
        datapack_name = "testDatapack"
        data = {"test": 1}
        script._registerDataPack(datapack_name)

        script._setDataPack(datapack_name, data)
        self.assertEqual(script.datapacks[datapack_name], data)

    def test_datapack_get(self):
        script = Script()
        datapack_name = "testDatapack"
        data = {"test": 1}
        script._registerDataPack(datapack_name)

        script._setDataPack(datapack_name, data)
        self.assertEqual(script._getDataPack(datapack_name), data)

if __name__ == '__main__':
    unittest.main()