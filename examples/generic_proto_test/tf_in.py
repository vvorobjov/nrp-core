import base64
import numpy as np

from nrp_core import *
from nrp_core.data.nrp_protobuf import *


@EngineDataPack(keyword='array_string', id=DataPackIdentifier('ArrayString', 'python_1'))
@EngineDataPack(keyword='array_bytes',  id=DataPackIdentifier('ArrayBytes',  'python_1'))
@EngineDataPack(keyword='array_float',  id=DataPackIdentifier('ArrayFloat',  'python_1'))
@EngineDataPack(keyword='array_double', id=DataPackIdentifier('ArrayDouble', 'python_1'))
@EngineDataPack(keyword='array_int32',  id=DataPackIdentifier('ArrayInt32',  'python_1'))
@EngineDataPack(keyword='array_int64',  id=DataPackIdentifier('ArrayInt64',  'python_1'))
@EngineDataPack(keyword='array_sint32', id=DataPackIdentifier('ArraySInt32', 'python_1'))
@EngineDataPack(keyword='array_sint64', id=DataPackIdentifier('ArraySInt64', 'python_1'))
@EngineDataPack(keyword='array_uint32', id=DataPackIdentifier('ArrayUInt32', 'python_1'))
@EngineDataPack(keyword='array_uint64', id=DataPackIdentifier('ArrayUInt64', 'python_1'))
@EngineDataPack(keyword='array_bool',   id=DataPackIdentifier('ArrayBool',   'python_1'))
@EngineDataPack(keyword='image',        id=DataPackIdentifier('Image',       'python_1'))
@EngineDataPack(keyword='bool_value',   id=DataPackIdentifier('BoolValue',   'python_1'))
@EngineDataPack(keyword='string_value', id=DataPackIdentifier('StringValue', 'python_1'))
@EngineDataPack(keyword='bytes_value',  id=DataPackIdentifier('BytesValue',  'python_1'))
@EngineDataPack(keyword='float_value',  id=DataPackIdentifier('FloatValue',  'python_1'))
@EngineDataPack(keyword='double_value', id=DataPackIdentifier('DoubleValue', 'python_1'))
@EngineDataPack(keyword='int32_value',  id=DataPackIdentifier('Int32Value',  'python_1'))
@EngineDataPack(keyword='int64_value',  id=DataPackIdentifier('Int64Value',  'python_1'))
@TransceiverFunction("python_1")
def transceiver_function(array_string,
                         array_bytes,
                         array_float,
                         array_double,
                         array_int32,
                         array_int64,
                         array_sint32,
                         array_sint64,
                         array_uint32,
                         array_uint64,
                         array_bool,
                         image,
                         bool_value,
                         string_value,
                         bytes_value,
                         float_value,
                         double_value,
                         int32_value,
                         int64_value):
    """Tests DataPacks produced by engine_grpc_1.py"""

    # Test Array* DataPacks

    assert(array_string.data.array[0] == "test")

    bytes_data = list(base64.b64decode(array_bytes.data.array[0]))
    assert(len(set(bytes_data).difference([0, 1, 2, 3, 4, 5])) == 0)

    assert(abs(array_float.data.array[0] - 1.5) < 1e-5)
    assert(abs(array_double.data.array[0] - 1.5) < 1e-5)

    assert(array_int64.data.array[0] == 11)
    assert(array_sint32.data.array[0] == 12)
    assert(array_sint64.data.array[0] == 13)
    assert(array_uint32.data.array[0] == 14)
    assert(array_uint64.data.array[0] == 15)

    assert(array_bool.data.array[0] == True)

    # Recover the multi-dimensional array from the flat buffer

    expected_array = np.array([[0, 1, 2],[3, 4, 5]])
    my_list = list(array_int32.data.array)
    shape = tuple(array_int32.data.shape)
    assert(my_list == [0, 1, 2, 3, 4, 5])
    assert(shape == (2, 3))
    assert(np.array_equal(np.reshape(my_list, shape), expected_array))

    # Test Image DataPack

    # TODO protobuf enum field names support: NRRPLT-8830
    #assert(image.data.type == nrpgenericproto.IMAGE_TYPE.RGB8)
    assert(image.data.type == 1)
    image_data = list(base64.b64decode(image.data.data))
    assert(len(set(image_data).difference([0, 1, 2, 3, 4, 5])) == 0)

    # Test *Value DataPacks

    assert(bool_value.data.value == True)
    assert(string_value.data.value == "test_value")
    assert(int.from_bytes(bytes_value.data.value, 'big') == 100)
    assert(abs(float_value.data.value - 2.5) < 1e-5)
    assert(abs(double_value.data.value - 3.5) < 1e-5)
    assert(int32_value.data.value == 101)
    assert(int64_value.data.value == 102)

    return []

# EOF
