from nrp_core import *
from nrp_core.data.nrp_json import JsonRawData


@EngineDataPack(keyword='datapack_engine1', id=DataPackIdentifier('datapack_engine1', 'python_1'))
@StatusFunction()
def status_function(datapack_engine1):

    ret = JsonRawData()
    ret.data["test_data"] = datapack_engine1.data["test_data"]

    return True, [ret]

# EOF
