from nrp_core import *
from nrp_core.data.nrp_json import JsonRawData


@EngineDataPack(keyword='datapack_engine', id=DataPackIdentifier('tf_input_engine', 'engine', 'type'))
@PreprocessedDataPack(keyword='datapack_preprocessed', id=DataPackIdentifier('tf_input_preprocessing', 'engine', 'type'))
@PreprocessedDataPack(keyword='datapack_tf', id=DataPackIdentifier('return_datapack', 'engine', 'type'))
@StatusFunction()
def transceiver_function(datapack_engine, datapack_preprocessed, datapack_tf):
    """
    Status Function used for automated testing.
    It receives datapacks coming from three different sources: PF, TF, engine
    The test data is extracted from each of the input datapacks and copied
    into the observation object, that can be checked by the C++ test.
    """
    # Extract test values from the input DataPacks

    test_val1 = datapack_engine.test_value
    test_val2 = datapack_preprocessed.data["test_value"]
    test_val3 = datapack_tf.data["test_value1"]

    # Copy the values to the observation object

    observation = JsonRawData()
    observation.data["test_value1"] = str(test_val1)
    observation.data["test_value2"] = str(test_val2)
    observation.data["test_value3"] = str(test_val3)

    return True, [observation]

# EOF
