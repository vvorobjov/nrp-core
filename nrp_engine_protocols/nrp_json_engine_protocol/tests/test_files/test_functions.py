#!/usr/bin/python3

# NRP Core - Backend infrastructure to synchronize simulations
#
# Copyright 2020-2023 NRP Team
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#   http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#
# This project has received funding from the European Union’s Horizon 2020
# Framework Programme for Research and Innovation under the Specific Grant
# Agreement No. 945539 (Human Brain Project SGA3).

import unittest
import numpy as np

from test_module import JsonDataPack


def test_input(input_datapack):

    # Create a test case object, so that we can use unittest assertions

    tc = unittest.TestCase()

    # Test that all basic types of objects were translated
    # correctly from nlohmann::json into python objects

    data = input_datapack.data

    tc.assertIsNone     (data["testNull"     ])
    tc.assertEqual      (data["testLong"     ], 100)
    tc.assertAlmostEqual(data["testDouble"   ], 56.56)
    tc.assertEqual      (data["testString"   ], "Armageddon")
    tc.assertTrue       (data["testBoolTrue" ])
    tc.assertFalse      (data["testBoolFalse"])
    tc.assertEqual      (data["testArray"    ], [1, 0, 2])
    tc.assertEqual      (data["testObject"   ], {'key1': 'value', 'key2': 600})


def test_output():

    # Fill the test datapack with different types of data
    # The C++ caller code will test that they were translated
    # correctly from python objects into nlohmann::json

    test_datapack = JsonDataPack("t", "a")

    # Standard python types

    test_datapack.data["test_null"]       = None
    test_datapack.data["test_long"]       = 1
    test_datapack.data["test_double"]     = 43.21
    test_datapack.data["test_string"]     = "string"
    test_datapack.data["test_bool_true"]  = True
    test_datapack.data["test_bool_false"] = False
    test_datapack.data["test_array"]      = [5, 1, 6]
    test_datapack.data["test_tuple"]      = (1, 2, 3)
    test_datapack.data["test_object"]     = {'key1': 'value', 'key2': 600}

    # Numpy integer arrays

    test_datapack.data["test_numpy_array_int8" ] = np.array([ 1,  2,  3], dtype="int8")
    test_datapack.data["test_numpy_array_int16"] = np.array([ 4,  5,  6], dtype="int16")
    test_datapack.data["test_numpy_array_int32"] = np.array([ 7,  8,  9], dtype="int32")
    test_datapack.data["test_numpy_array_int64"] = np.array([10, 11, 12], dtype="int64")

    # Numpy unsigned integer arrays

    test_datapack.data["test_numpy_array_uint8" ] = np.array([0,  1,  2], dtype="uint8")
    test_datapack.data["test_numpy_array_uint16"] = np.array([3,  4,  5], dtype="uint16")
    test_datapack.data["test_numpy_array_uint32"] = np.array([6,  7,  8], dtype="uint32")
    test_datapack.data["test_numpy_array_uint64"] = np.array([9, 10, 11], dtype="uint64")

    # Numpy floating-point arrays

    test_datapack.data["test_numpy_array_float32"] = np.array([0.5, 2.3, 3.55], dtype="float32")
    test_datapack.data["test_numpy_array_float64"] = np.array([1.5, 2.3, 3.88], dtype="float64")

    return test_datapack


def test_index_out_of_range_array_1(input_data_pack):
    # Access index out of range
    # Should throw. The exception should be caught by C++ part of the test

    input_data_pack.data[3]


def test_index_out_of_range_array_2(input_data_pack):
    # Access array with string as index
    # Should throw. The exception should be caught by C++ part of the test

    input_data_pack.data["test"]


def test_index_out_of_range_array_3(input_data_pack):
    # Try to set element of array
    # Should throw. The exception should be caught by C++ part of the test

    input_data_pack.data[0] = 1


def test_index_out_of_range_object_1(input_data_pack):
    # Try to access object with integer as key
    # Should throw. The exception should be caught by C++ part of the test

    input_data_pack.data[0]


def test_index_out_of_range_object_2(input_data_pack):
    # Try to access a non-existent key
    # Should throw. The exception should be caught by C++ part of the test

    input_data_pack.data["non_existent"]


def test_conversion_failure_unsupported_type():
    test_data_pack = JsonDataPack("t", "a")

    class UnsupportedType:
        def __init__(self):
            pass

    # Try to convert an unsupported type
    # Should throw. The exception should be caught by C++ part of the test

    test_data_pack.data["test_unsupported"] = UnsupportedType()

    return test_data_pack


def test_numpy_conversion_failure_unsupported_type():
    test_datapack = JsonDataPack("t", "a")

    # Try to convert a numpy array with unsupported dtype
    # Should throw. The exception should be caught by C++ part of the test

    test_datapack.data["test_numpy_array_str"] = np.array(["a", "b", "c"], dtype="str")

    return test_datapack


def test_numpy_conversion_failure_unsupported_nd():
    test_datapack = JsonDataPack("t", "a")

    # Try to convert a numpy array with unsupported shape (nd != 1)
    # Should throw. The exception should be caught by C++ part of the test

    test_datapack.data["test_numpy_array"] = np.array([[1, 2], [3, 4]])

    return test_datapack


def test_str_method(input_datapack):

    # Create a test case object, so that we can use unittest assertions

    tc = unittest.TestCase()

    # Test that all basic types of objects were translated
    # correctly from nlohmann::json into python objects

    data = input_datapack.data

    expected_string = ('{"testArray":[1,0,2],'
                       '"testBoolFalse":false,'
                       '"testBoolTrue":true,'
                       '"testDouble":56.56,'
                       '"testLong":100,'
                       '"testNull":null,'
                       '"testObject":{"key1":"value","key2":600},'
                       '"testString":"Armageddon"}')

    tc.assertEqual(str(data), expected_string)


def test_len_method(input_data_pack):

    # Create a test case object, so that we can use unittest assertions

    tc = unittest.TestCase()

    data = input_data_pack.data

    tc.assertEqual(len(data), 4)
    tc.assertEqual(len(data["testArray"]), 3)
    tc.assertEqual(len(data["testObject"]), 2)


def test_keys_method(input_array, input_object):

    # Create a test case object, so that we can use unittest assertions

    tc = unittest.TestCase()

    # For arrays, keys() should raise an Exception, for objects - a list of keys

    tc.assertRaises(AttributeError, input_array.data.keys)
    # tc.assertEqual(sorted(input_array.data.keys()), sorted(["0", "1", "2"]))
    tc.assertEqual(sorted(input_object.data.keys()), sorted(["key1", "key2"]))


def test_append_method():

    # Create a test case object, so that we can use unittest assertions

    tc = unittest.TestCase()

    test_datapack = JsonDataPack("t", "a")
    test_datapack.data.append(1)
    test_datapack.data.append({'key1': 'value', 'key2': 600})

    tc.assertEqual(test_datapack.data[0], 1)
    tc.assertEqual(test_datapack.data[1], {'key1': 'value', 'key2': 600})

    # append() isn't allowed for json objects

    test_object = JsonDataPack("t", "a")
    test_object.data["test"] = 1

    with tc.assertRaises(AttributeError):
        test_object.data.append(1)


# EOF
