//
// NRP Core - Backend infrastructure to synchronize simulations
//
// Copyright 2020-2023 NRP Team
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//   http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//
// This project has received funding from the European Union’s Horizon 2020
// Framework Programme for Research and Innovation under the Specific Grant
// Agreement No. 945539 (Human Brain Project SGA3).
//

#include <array>
#include <gtest/gtest.h>
#include <boost/python.hpp>
#include <nlohmann/json.hpp>

#include "nrp_general_library/utils/utils.h"

#include "nrp_json_engine_protocol/datapack_interfaces/json_datapack.h"
#include "python/python_module.cpp"

using namespace boost;


#define CONCATENATE_TOKENS_EXPANDED(A, B) A ## B
#define CONCATENATE_TOKENS(A, B) CONCATENATE_TOKENS_EXPANDED(A, B)


class JsonConverter : public testing::Test
{
    protected:

        static void SetUpTestSuite()
        {
            PyImport_AppendInittab("test_module", CONCATENATE_TOKENS(PyInit_, JSON_PYTHON_MODULE_NAME));
            Py_Initialize();
            json_converter::initNumpy();
            python::numpy::initialize();
            main    = new python::object(python::import("__main__"));
            globals = new python::dict(main->attr("__dict__"));

            // Append simple_function path to search
            appendPythonPath(TEST_PYTHON_FUNCTIONS_MODULE_PATH);
            PyImport_ImportModule("test_module");

            // Load simple function
            python::object simpleFcn(python::import("test_functions"));
            globals->update(simpleFcn.attr("__dict__"));
        }

        static void handlePythonException()
        {
            PyErr_Print();
            PyErr_Clear();

            std::cout.flush();

            throw std::invalid_argument("Python Exception");
        }


        /*!
         * \brief Runs python function with given name, extracts the exception message and returns it as string
         */
        static const std::string runAndExtractExceptionMessage(const std::string & functionName, const JsonDataPack * inputDataPack)
        {
            try
            {
                if(inputDataPack != nullptr)
                {
                    (*globals)[functionName](boost::ref(inputDataPack));
                }
                else
                {
                    (*globals)[functionName]();
                }
            }
            catch(boost::python::error_already_set &)
            {
                PyObject *ptype, *pvalue, *ptraceback;
                PyErr_Fetch(&ptype, &pvalue, &ptraceback);

                const char *pStrErrorMessage = PyUnicode_AsUTF8(pvalue);
                return std::string(pStrErrorMessage);
            }

            return "";
        }


        /*!
         * \brief Asserts that two arrays are identical with respect to size and values
         */
        template <typename TYPE>
        static void assertArrayEqual(const nlohmann::json & result, const std::array<TYPE, 3> & expected)
        {
            ASSERT_EQ(result.size(), expected.size());

            for(size_t i = 0; i < result.size(); i++)
            {
                ASSERT_EQ(result[i], expected[i]);
            }
        }


        /*!
         * \brief Asserts that two arrays are identical with respect to size and values (with some margin)
         */
        template <typename TYPE>
        static void assertArrayNear(const nlohmann::json & result, const std::array<TYPE, 3> & expected)
        {
            ASSERT_EQ(result.size(), expected.size());

            for(size_t i = 0; i < result.size(); i++)
            {
                ASSERT_NEAR(result[i], expected[i], 1e-6);
            }
        }

        static python::object * main;
        static python::dict   * globals;
};

python::object * JsonConverter::main = nullptr;
python::dict   * JsonConverter::globals = nullptr;


/*!
 * \brief Tests conversion from nlohmann::json to python using JsonDataPack
 */
TEST_F(JsonConverter, TestJsonToPython)
{
    try
    {
        // Create input datapack with JSON data

        nlohmann::json * inputJson = new nlohmann::json();
        (*inputJson)["testNull"     ] = nullptr;
        (*inputJson)["testLong"     ] = 100;
        (*inputJson)["testDouble"   ] = 56.56;
        (*inputJson)["testString"   ] = "Armageddon";
        (*inputJson)["testBoolTrue" ] = true;
        (*inputJson)["testBoolFalse"] = false;
        (*inputJson)["testArray"    ] = { 1, 0, 2 };
        (*inputJson)["testObject"   ] = { {"key1", "value"}, {"key2", 600} };
        JsonDataPack inputDataPack("a", "b", inputJson);

        // Call the test function

        (*globals)["test_input"](boost::ref(inputDataPack));
    }
    catch(boost::python::error_already_set &)
    {
        handlePythonException();
    }
}


/*!
 * \brief Tests conversion from python to nlohmann::json using JsonDataPack
 */
TEST_F(JsonConverter, TestPythonToJson)
{
    try
    {
        // Call the test function

        JsonDataPack * res = boost::python::extract<JsonDataPack *>((*globals)["test_output"]());

        // Check basic data types

        ASSERT_EQ(res->getData()["test_null"      ], nullptr);
        ASSERT_EQ(res->getData()["test_long"      ], 1);
        ASSERT_EQ(res->getData()["test_double"    ], 43.21);
        ASSERT_EQ(res->getData()["test_string"    ], "string");
        ASSERT_EQ(res->getData()["test_bool_true" ], true);
        ASSERT_EQ(res->getData()["test_bool_false"], false);

        // Check list

        assertArrayEqual(res->getData()["test_array"], std::array<int, 3>({5, 1, 6}));

        // Check tuple

        assertArrayEqual(res->getData()["test_tuple"], std::array<int, 3>({1, 2, 3}));

        // Check dictionary

        ASSERT_EQ(res->getData()["test_object"]["key1"], "value");
        ASSERT_EQ(res->getData()["test_object"]["key2"], 600);

        // Check numpy integer arrays

        assertArrayEqual(res->getData()["test_numpy_array_int8" ], std::array<int, 3>({ 1,  2,  3}));
        assertArrayEqual(res->getData()["test_numpy_array_int16"], std::array<int, 3>({ 4,  5,  6}));
        assertArrayEqual(res->getData()["test_numpy_array_int32"], std::array<int, 3>({ 7,  8,  9}));
        assertArrayEqual(res->getData()["test_numpy_array_int64"], std::array<int, 3>({10, 11, 12}));

        // Check numpy unsigned integer arrays

        assertArrayEqual(res->getData()["test_numpy_array_uint8" ], std::array<unsigned, 3>({0,  1,  2}));
        assertArrayEqual(res->getData()["test_numpy_array_uint16"], std::array<unsigned, 3>({3,  4,  5}));
        assertArrayEqual(res->getData()["test_numpy_array_uint32"], std::array<unsigned, 3>({6,  7,  8}));
        assertArrayEqual(res->getData()["test_numpy_array_uint64"], std::array<unsigned, 3>({9, 10, 11}));

        // Check numpy floating-point arrays

        assertArrayNear(res->getData()["test_numpy_array_float32"], std::array<float,  3>({0.5, 2.3, 3.55}));
        assertArrayNear(res->getData()["test_numpy_array_float64"], std::array<double, 3>({1.5, 2.3, 3.88}));
    }
    catch(boost::python::error_already_set &)
    {
        handlePythonException();
    }
}


/*!
 * \brief Tests failure modes of conversion from python to nlohmann::json using JsonDataPack
 */
TEST_F(JsonConverter, TestPythonToJsonFailures)
{
    // Call the test function

    nlohmann::json * inputJsonArray = new nlohmann::json({ 1, 0, 2 });
    JsonDataPack inputDataPackArray("a", "b", inputJsonArray);

    nlohmann::json * inputJsonObject = new nlohmann::json({ {"key1", "value"}, {"key2", 600} });
    JsonDataPack inputDataPackObject("a", "b", inputJsonObject);

    ASSERT_EQ(runAndExtractExceptionMessage("test_index_out_of_range_object_1", &inputDataPackObject), "object indices must be str");
    ASSERT_EQ(runAndExtractExceptionMessage("test_index_out_of_range_object_2", &inputDataPackObject), "key 'non_existent' doesn't exist");

    ASSERT_EQ(runAndExtractExceptionMessage("test_index_out_of_range_array_1", &inputDataPackArray), "list index out of range");
    ASSERT_EQ(runAndExtractExceptionMessage("test_index_out_of_range_array_2", &inputDataPackArray), "list indices must be integers");
    ASSERT_EQ(runAndExtractExceptionMessage("test_index_out_of_range_array_3", &inputDataPackArray), "setting json array elements not supported");

    ASSERT_EQ(runAndExtractExceptionMessage("test_conversion_failure_unsupported_type",       nullptr), "Attempted to convert unsupported python type into nlohmann::json.");
    ASSERT_EQ(runAndExtractExceptionMessage("test_numpy_conversion_failure_unsupported_type", nullptr), "Conversion of numpy dtype '<U1' not implemented");
    ASSERT_EQ(runAndExtractExceptionMessage("test_numpy_conversion_failure_unsupported_nd",   nullptr), "Conversion of numpy array with nd = 2 not implemented");
}


/*!
 * \brief Tests __str__ method of JsonDataPack
 */
TEST_F(JsonConverter, TestJsonStrMethod)
{
    try
    {
        // Create input datapack with JSON data

        nlohmann::json * inputJson = new nlohmann::json();
        (*inputJson)["testNull"     ] = nullptr;
        (*inputJson)["testLong"     ] = 100;
        (*inputJson)["testDouble"   ] = 56.56;
        (*inputJson)["testString"   ] = "Armageddon";
        (*inputJson)["testBoolTrue" ] = true;
        (*inputJson)["testBoolFalse"] = false;
        (*inputJson)["testArray"    ] = { 1, 0, 2 };
        (*inputJson)["testObject"   ] = { {"key1", "value"}, {"key2", 600} };
        JsonDataPack inputDataPack("a", "b", inputJson);

        // Call the test function

        (*globals)["test_str_method"](boost::ref(inputDataPack));
    }
    catch(boost::python::error_already_set &)
    {
        handlePythonException();
    }
}


/*!
 * \brief Tests __len__ method of JsonDataPack
 */
TEST_F(JsonConverter, TestJsonLenMethod)
{
    try
    {
        // Create input datapack with JSON data

        nlohmann::json * inputJson = new nlohmann::json();
        (*inputJson)["testNull"     ] = nullptr;
        (*inputJson)["testLong"     ] = 100;
        (*inputJson)["testArray"    ] = { 1, 0, 2 };
        (*inputJson)["testObject"   ] = { {"key1", "value"}, {"key2", 600} };
        JsonDataPack inputDataPack("a", "b", inputJson);

        // Call the test function

        (*globals)["test_len_method"](boost::ref(inputDataPack));
    }
    catch(boost::python::error_already_set &)
    {
        handlePythonException();
    }
}


/*!
 * \brief Tests keys() method of JsonDataPack
 */
TEST_F(JsonConverter, TestJsonKeysMethod)
{
    try
    {
        // Create input datapacks with JSON data

        nlohmann::json * inputJsonArray = new nlohmann::json({ 1, 0, 2 });
        JsonDataPack inputDataPackArray("a", "b", inputJsonArray);

        nlohmann::json * inputJsonObject = new nlohmann::json({ {"key1", "value"}, {"key2", 600} });
        JsonDataPack inputDataPackObject("a", "b", inputJsonObject);

        // Call the test function

        (*globals)["test_keys_method"](boost::ref(inputDataPackArray), boost::ref(inputDataPackObject));
    }
    catch(boost::python::error_already_set &)
    {
        handlePythonException();
    }
}


/*!
 * \brief Tests append() method of JsonDataPack
 */
TEST_F(JsonConverter, TestJsonAppendMethod)
{
    try
    {
        // Call the test function

        (*globals)["test_append_method"]();
    }
    catch(boost::python::error_already_set &)
    {
        handlePythonException();
    }
}

// EOF
