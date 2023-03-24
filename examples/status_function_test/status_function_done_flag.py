from nrp_core import *


@EngineDataPack(keyword='datapack_engine1', id=DataPackIdentifier('datapack_engine1', 'python_1'))
@EngineDataPack(keyword='datapack_engine2', id=DataPackIdentifier('datapack_engine2', 'python_2'))
@SimulationIteration(keyword="sim_iter")
@StatusFunction()
def status_function(datapack_engine1, datapack_engine2, sim_iter):

    # The DataPack coming from engine 'python_1' should be updated on every iteration
    assert(datapack_engine1.isUpdated())

    # The DataPack coming from engine 'python_2' should be updated on every other iteration
    if sim_iter % 2:
        assert(not datapack_engine2.isUpdated())
    else:
        assert(datapack_engine2.isUpdated())

    return datapack_engine2.isUpdated(), []

# EOF
