from nrp_core import *


@EngineDataPack(keyword='datapack', id=DataPackIdentifier('tf_input', 'engine', 'type'))
@StatusFunction()
def transceiver_function(datapack):
    """
    Status Function used for automated testing.
    """
    return datapack.isUpdated(), []

# EOF
