"""Python Engine 1. Will get current engine time and make it accessible as a datapack"""

import base64
import numpy as np

from nrp_core.engines.python_grpc import GrpcEngineScript
from nrp_protobuf import nrpgenericproto_pb2
from nrp_protobuf import wrappers_pb2

class Script(GrpcEngineScript):
    """Produces DataPacks that will be tested by tf_in.py"""

    def initialize(self):

        # Prepare test DataPacks of types "Array*"

        # base64 is used as an example of binary encoding
        self._prepare_datapack("ArrayBytes",  nrpgenericproto_pb2.ArrayBytes,  base64.b64encode(bytes([0, 1, 2, 3, 4, 5])))
        self._prepare_datapack("ArrayString", nrpgenericproto_pb2.ArrayString, "test")
        self._prepare_datapack("ArrayFloat",  nrpgenericproto_pb2.ArrayFloat,  1.5)
        self._prepare_datapack("ArrayDouble", nrpgenericproto_pb2.ArrayDouble, 1.5)
        self._prepare_datapack("ArrayInt64",  nrpgenericproto_pb2.ArrayInt64,  11)
        self._prepare_datapack("ArraySInt32", nrpgenericproto_pb2.ArraySInt32, 12)
        self._prepare_datapack("ArraySInt64", nrpgenericproto_pb2.ArraySInt64, 13)
        self._prepare_datapack("ArrayUInt32", nrpgenericproto_pb2.ArrayUInt32, 14)
        self._prepare_datapack("ArrayUInt64", nrpgenericproto_pb2.ArrayUInt64, 15)
        self._prepare_datapack("ArrayBool",   nrpgenericproto_pb2.ArrayBool,   True)

        # An example of multi-dimensional array

        self._registerDataPack("ArrayInt32", nrpgenericproto_pb2.ArrayInt32)
        datapack = nrpgenericproto_pb2.ArrayInt32()
        my_array = np.array([[0, 1, 2],[3, 4, 5]])
        datapack.array.extend(my_array.flatten())
        datapack.shape.extend(my_array.shape)
        self._setDataPack("ArrayInt32", datapack)

        # Prepare test DataPack of type "Image"

        self._registerDataPack("Image", nrpgenericproto_pb2.Image)
        datapack = nrpgenericproto_pb2.Image()
        # base64 is used as an example of binary encoding
        datapack.data = base64.b64encode(bytes([0, 1, 2, 3, 4, 5]))
        datapack.type = nrpgenericproto_pb2.IMAGE_TYPE.RGB8
        self._setDataPack("Image", datapack)

        # Prepare test DataPacks of types "*Value"

        self._prepare_datapack_wrapper("StringValue", wrappers_pb2.StringValue, "test_value")
        self._prepare_datapack_wrapper("BytesValue",  wrappers_pb2.BytesValue, (100).to_bytes(1, byteorder='big'))
        self._prepare_datapack_wrapper("BoolValue",   wrappers_pb2.BoolValue,   True)
        self._prepare_datapack_wrapper("FloatValue",  wrappers_pb2.FloatValue,  2.5)
        self._prepare_datapack_wrapper("DoubleValue", wrappers_pb2.DoubleValue, 3.5)
        self._prepare_datapack_wrapper("Int32Value",  wrappers_pb2.Int32Value,  101)
        self._prepare_datapack_wrapper("Int64Value",  wrappers_pb2.Int64Value,  102)


    def _prepare_datapack(self, datapack_name, datapack_type, value):
        self._registerDataPack(datapack_name, datapack_type)
        datapack = datapack_type()
        datapack.array.append(value)
        self._setDataPack(datapack_name, datapack)


    def _prepare_datapack_wrapper(self, datapack_name, datapack_type, value):
        self._registerDataPack(datapack_name, datapack_type)
        datapack = datapack_type()
        datapack.value = value
        self._setDataPack(datapack_name, datapack)


    def runLoop(self, timestep_ns):
        pass


    def shutdown(self):
        pass

# EOF
