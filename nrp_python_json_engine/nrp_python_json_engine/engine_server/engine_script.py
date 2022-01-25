class EngineScript:

    def __init__(self) -> None:
        """Constructor"""
        self._datapacks = {}
        self._time = 0
        self._name = ""


    def _advanceTime(self, timestep: int) -> None:
        """Advances the simultaion time by given timestep"""
        self._time = self._time + timestep


    def _registerDataPack(self, datapack_name: str) -> None:
        """
        Registers a datapack with given name in the engine.
        The datapack-handling functions can be called only on registered datapacks"
        """
        if(datapack_name in self._datapacks):
            raise Exception("DataPack with specified name (" + datapack_name + ") already registered")

        self._datapacks[datapack_name] = None


    def _getDataPack(self, datapack_name: str) -> dict:
        """Returns cached data of the datapack with given name"""
        if(datapack_name not in self._datapacks):
            raise Exception("Attempting to get data from an unregistered DataPack (" + datapack_name + ")")

        return self._datapacks[datapack_name]


    def _setDataPack(self, datapack_name: str, data: dict) -> None:
        """Sets cached data of the datapack with given name"""
        if(datapack_name not in self._datapacks):
            raise Exception("Attempting to set data on an unregistered DataPack (" + datapack_name + ")")

        self._datapacks[datapack_name] = data


# EOF
