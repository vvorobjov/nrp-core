from nrp_core import *
from mypackage import MyPackageMyMessageDataPack
 
@EngineDataPack(keyword='datapack_1', id=DataPackIdentifier('datapack_1', 'python_1'))
@TransceiverFunction("datatransfer_engine")
def data_transfer(datapack_1):
    print(f"Relaying datapack with data: '{datapack_1.data.str}' '{datapack_1.data.integer}' to datatransfer_engine")
    datapack_1.engine_name = "datatransfer_engine"
    return [datapack_1]
