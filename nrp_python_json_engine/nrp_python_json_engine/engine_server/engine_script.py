class EngineScript:

    def __init__(self) -> None:
        self._datapacks = {}
        self._time = 0
        self._name = ""


    def _advanceTime(self, timestep) -> None:
        self._time = self._time + timestep


    def _registerDataPack(self, datapack_name):
        if(datapack_name in self._datapacks):
            raise Exception("DataPack with specified name (" + datapack_name + ") already registered")

        self._datapacks[datapack_name] = None


    def _getDataPack(self, datapack_name):
        if(datapack_name not in self._datapacks):
            raise Exception("Attempting to get data from an unregistered DataPack (" + datapack_name + ")")

        return self._datapacks[datapack_name]


    def _setDataPack(self, datapack_name, data):
        if(datapack_name not in self._datapacks):
            raise Exception("Attempting to set data on an unregistered DataPack (" + datapack_name + ")")

        self._datapacks[datapack_name] = data


# EOF
