"""Python Engine 1. Will get current engine time and make it accessible as a datapack"""

import base64
import numpy as np

from nrp_core.engines.python_grpc import GrpcEngineScript
from nrp_core.data.nrp_protobuf import nrpgenericproto_pb2
from nrp_core.data.nrp_protobuf import wrappers_pb2


class Script(GrpcEngineScript):
    """Tests DataPacks produced by tf_out.py"""

    def initialize(self):
        self._registerDataPack("ArrayString", nrpgenericproto_pb2.ArrayString)
        self._registerDataPack("ArrayBytes",  nrpgenericproto_pb2.ArrayBytes)
        self._registerDataPack("ArrayFloat",  nrpgenericproto_pb2.ArrayFloat)
        self._registerDataPack("ArrayDouble", nrpgenericproto_pb2.ArrayDouble)
        self._registerDataPack("ArrayInt32",  nrpgenericproto_pb2.ArrayInt32)
        self._registerDataPack("ArrayInt64",  nrpgenericproto_pb2.ArrayInt64)
        self._registerDataPack("ArraySInt32", nrpgenericproto_pb2.ArraySInt32)
        self._registerDataPack("ArraySInt64", nrpgenericproto_pb2.ArraySInt64)
        self._registerDataPack("ArrayUInt32", nrpgenericproto_pb2.ArrayUInt32)
        self._registerDataPack("ArrayUInt64", nrpgenericproto_pb2.ArrayUInt64)

        self._registerDataPack("StringValue", wrappers_pb2.StringValue)
        self._registerDataPack("BytesValue",  wrappers_pb2.BytesValue)
        self._registerDataPack("FloatValue",  wrappers_pb2.FloatValue)
        self._registerDataPack("DoubleValue", wrappers_pb2.DoubleValue)
        self._registerDataPack("Int32Value",  wrappers_pb2.Int32Value)
        self._registerDataPack("Int64Value",  wrappers_pb2.Int64Value)
        self._registerDataPack("BoolValue",   wrappers_pb2.BoolValue)



    def runLoop(self, timestep_ns):
        assert(self._getDataPack("ArrayString").array[0] == "test")
        assert(int.from_bytes(self._getDataPack("ArrayBytes").array[0], "big") == 100)
        assert(abs(self._getDataPack("ArrayFloat").array[0] - 1.5) < 1e-5)
        assert(abs(self._getDataPack("ArrayDouble").array[0] - 2.5) < 1e-5)
        assert(self._getDataPack("ArrayInt64").array[0] == 2)
        assert(self._getDataPack("ArraySInt32").array[0] == 3)
        assert(self._getDataPack("ArraySInt64").array[0] == 4)
        assert(self._getDataPack("ArrayUInt32").array[0] == 5)
        assert(self._getDataPack("ArrayUInt64").array[0] == 6)

        # Recover the multi-dimensional array from the flat buffer

        expected_array = np.array([[0, 1, 2],[3, 4, 5]])
        my_list = list(self._getDataPack("ArrayInt32").array)
        shape = tuple(self._getDataPack("ArrayInt32").shape)
        assert(my_list == [0, 1, 2, 3, 4, 5])
        assert(shape == (2, 3))
        assert(np.array_equal(np.reshape(my_list, shape), expected_array))

        assert(self._getDataPack("StringValue").value == "test_value")
        assert(int.from_bytes(self._getDataPack("BytesValue").value, "big") == 101)
        assert(abs(self._getDataPack("FloatValue").value - 10.5) < 1e-5)
        assert(abs(self._getDataPack("DoubleValue").value - 20.5) < 1e-5)
        assert(self._getDataPack("Int32Value").value == 10)
        assert(self._getDataPack("Int64Value").value == 20)
        assert(self._getDataPack("BoolValue").value == True)


    def shutdown(self):
        pass

# EOF
