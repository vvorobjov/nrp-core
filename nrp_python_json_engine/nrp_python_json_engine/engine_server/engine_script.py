class EngineScript:
    def __init__(self) -> None:
        self.datapacks = {}

    def _registerDataPack(self, datapack_name):
        self.datapacks[datapack_name] = None

    def _getDataPack(self, datapack_name):
        return self.datapacks[datapack_name]

    def _setDataPack(self, datapack_name, data):
        self.datapacks[datapack_name] = data

# EOF
