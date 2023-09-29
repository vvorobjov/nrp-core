from nrp_core import *
from nrp_core.data.nrp_protobuf import DumpStringDataPack
from nrp_core.data.nrp_protobuf import DumpArrayFloatDataPack
from nrp_core.data.nrp_protobuf import WrappersBoolValueDataPack
from nrp_core.data.nrp_protobuf import WrappersBytesValueDataPack
from nrp_core.data.nrp_protobuf import WrappersFloatValueDataPack
from nrp_core.data.nrp_protobuf import WrappersDoubleValueDataPack
from nrp_core.data.nrp_protobuf import WrappersInt32ValueDataPack
from nrp_core.data.nrp_protobuf import WrappersStringValueDataPack
from nrp_core.data.nrp_protobuf import NrpGenericProtoArrayBoolDataPack

import os
import numpy as np
import random
import string

@TransceiverFunction("datatransfer_engine") # type: ignore
def transceiver_function():
    dp_dump_string = DumpStringDataPack("datapack_DumpString", "datatransfer_engine")
    dp_dump_string.data.string_stream = "str(datapack_python.data)"

    rand_data = list(np.random.rand(4).flat)
    dp_dump_arrayfloat_1d = DumpArrayFloatDataPack("datapack_ArrayFloat_1d", "datatransfer_engine")
    dp_dump_arrayfloat_1d.data.float_stream.extend(rand_data)
    dp_dump_arrayfloat_1d.data.dims.extend([4])

    rand_data = list(np.random.rand(4, 5).flat)
    dp_dump_arrayfloat_2d = DumpArrayFloatDataPack("datapack_ArrayFloat_2d", "datatransfer_engine")
    dp_dump_arrayfloat_2d.data.float_stream.extend(rand_data)
    dp_dump_arrayfloat_2d.data.dims.extend([4, 5])

    rand_data = list(np.random.rand(2, 3, 4).flat)
    dp_dump_arrayfloat_3d = DumpArrayFloatDataPack("datapack_ArrayFloat_3d", "datatransfer_engine")
    dp_dump_arrayfloat_3d.data.float_stream.extend(rand_data)
    dp_dump_arrayfloat_3d.data.dims.extend([2, 3, 4])

    rand_data = list(np.random.rand(2, 3, 2, 3).flat)
    dp_dump_arrayfloat_4d = DumpArrayFloatDataPack("datapack_ArrayFloat_4d", "datatransfer_engine")
    dp_dump_arrayfloat_4d.data.float_stream.extend(rand_data)
    dp_dump_arrayfloat_4d.data.dims.extend([2, 3, 2, 3])

    dp_wrappers_bool = WrappersBoolValueDataPack("datapack_WrappersBoolValue", "datatransfer_engine")
    dp_wrappers_bool.data.value = bool(random.getrandbits(1))

    dp_wrappers_bytes = WrappersBytesValueDataPack("datapack_WrappersBytesValue", "datatransfer_engine")
    dp_wrappers_bytes.data.value = os.urandom(8)

    dp_wrappers_float = WrappersFloatValueDataPack("datapack_WrappersFloatValue", "datatransfer_engine")
    dp_wrappers_float.data.value = random.random()

    dp_wrappers_double = WrappersDoubleValueDataPack("datapack_WrappersDoubleValue", "datatransfer_engine")
    dp_wrappers_double.data.value = random.random()

    dp_wrappers_int32 = WrappersInt32ValueDataPack("datapack_WrappersInt32Value", "datatransfer_engine")
    dp_wrappers_int32.data.value = random.randint(0, 10)

    dp_wrappers_int64 = WrappersInt32ValueDataPack("datapack_WrappersInt64Value", "datatransfer_engine")
    dp_wrappers_int64.data.value = random.randint(10000000, 100000000)

    dp_wrappers_string = WrappersStringValueDataPack("datapack_WrappersStringValue", "datatransfer_engine")
    letters = string.ascii_lowercase
    dp_wrappers_string.data.value = ''.join(random.choice(letters) for i in range(10))

    rand_data = np.random.choice(a=[False, True], size=(3, 7))
    dp_nrpgeneric_arraybool = NrpGenericProtoArrayBoolDataPack("datapack_NrpGenericProtoArrayBool", "datatransfer_engine")
    dp_nrpgeneric_arraybool.data.array.extend(rand_data.flat)
    dp_nrpgeneric_arraybool.data.shape.extend([3, 7])

    return [dp_dump_string, dp_dump_arrayfloat_1d, dp_dump_arrayfloat_2d, dp_dump_arrayfloat_3d, dp_dump_arrayfloat_4d, dp_wrappers_bool, dp_wrappers_bytes, dp_wrappers_float, dp_wrappers_double, dp_wrappers_int32, dp_wrappers_int64, dp_wrappers_string, dp_nrpgeneric_arraybool]
