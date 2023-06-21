import numpy as np

from nrp_core import *
from nrp_core.data.nrp_protobuf import *


def create_datapack_generic(name, type, value):
    datapack = type(name, "python_2")
    datapack.data.array.append(value)
    return datapack


def create_datapack_wrapper(name, type, value):
    datapack = type(name, "python_2")
    datapack.data.value = value
    return datapack


@TransceiverFunction("python_2")
def transceiver_function():
    """Produces DataPacks that will be tested by engine_grpc_2"""
    datapacks = []

    datapacks.append(create_datapack_generic("ArrayString", NrpGenericProtoArrayStringDataPack, "test"))
    datapacks.append(create_datapack_generic("ArrayBytes",  NrpGenericProtoArrayBytesDataPack,  (100).to_bytes(1, "big")))
    datapacks.append(create_datapack_generic("ArrayFloat",  NrpGenericProtoArrayFloatDataPack,  1.5))
    datapacks.append(create_datapack_generic("ArrayDouble", NrpGenericProtoArrayDoubleDataPack, 2.5))
    datapacks.append(create_datapack_generic("ArrayInt64",  NrpGenericProtoArrayInt64DataPack,  2))
    datapacks.append(create_datapack_generic("ArraySInt32", NrpGenericProtoArraySInt32DataPack, 3))
    datapacks.append(create_datapack_generic("ArraySInt64", NrpGenericProtoArraySInt64DataPack, 4))
    datapacks.append(create_datapack_generic("ArrayUInt32", NrpGenericProtoArrayUInt32DataPack, 5))
    datapacks.append(create_datapack_generic("ArrayUInt64", NrpGenericProtoArrayUInt64DataPack, 6))

    # An example of multi-dimensional array

    datapack = NrpGenericProtoArrayInt32DataPack("ArrayInt32", "python_2")
    my_array = np.array([[0, 1, 2],[3, 4, 5]])
    datapack.data.array[:] = my_array.flatten().tolist()
    datapack.data.shape[:] = my_array.shape
    datapacks.append(datapack)

    datapacks.append(create_datapack_wrapper("StringValue", WrappersStringValueDataPack, "test_value"))
    datapacks.append(create_datapack_wrapper("BytesValue",  WrappersBytesValueDataPack,  (101).to_bytes(1, "big")))
    datapacks.append(create_datapack_wrapper("FloatValue",  WrappersFloatValueDataPack,  10.5))
    datapacks.append(create_datapack_wrapper("DoubleValue", WrappersDoubleValueDataPack, 20.5))
    datapacks.append(create_datapack_wrapper("Int32Value",  WrappersInt32ValueDataPack,  10))
    datapacks.append(create_datapack_wrapper("Int64Value",  WrappersInt64ValueDataPack,  20))
    datapacks.append(create_datapack_wrapper("BoolValue",   WrappersBoolValueDataPack,   True))

    return datapacks

# EOF