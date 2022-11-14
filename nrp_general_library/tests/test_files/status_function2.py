from nrp_core import *
from nrp_core.data.nrp_json import SimulationStatus


@EngineDataPack(keyword='datapack_engine', id=DataPackIdentifier('tf_input_engine', 'engine', 'type'))
@PreprocessedDataPack(keyword='datapack_preprocessed', id=DataPackIdentifier('tf_input_preprocessing', 'engine', 'type'))
@PreprocessedDataPack(keyword='datapack_tf', id=DataPackIdentifier('return_datapack', 'engine', 'type'))
@StatusFunction()
def transceiver_function(datapack_engine, datapack_preprocessed, datapack_tf, client_data):
    # Extract test values from the input DataPacks

    test_val1 = datapack_engine.test_value
    test_val2 = datapack_preprocessed.data["test_value"]
    test_val3 = datapack_tf.data["test_value1"]

    status = SimulationStatus()

    # Copy the values to the Status object

    status["test_value1"] = str(test_val1)
    status["test_value2"] = str(test_val2)
    status["test_value3"] = str(test_val3)

    return status, []

# EOF
