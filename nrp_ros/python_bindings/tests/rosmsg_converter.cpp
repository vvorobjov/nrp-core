//
// NRP Core - Backend infrastructure to synchronize simulations
//
// Copyright 2020-2021 NRP Team
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
#include <boost/python/numpy.hpp>

#include <nrp_ros_msgs/Test.h>

#include "nrp_general_library/utils/utils.h"
#include "nrp_general_library/utils/nrp_exceptions.h"
#include "nrp_general_library/utils/nrp_logger.h"

#include "python_bindings/config/cmake_constants.h"


using namespace boost;
using namespace nrp_ros_msgs;


#define CONCATENATE_TOKENS_EXPANDED(A, B) A ## B
#define CONCATENATE_TOKENS(A, B) CONCATENATE_TOKENS_EXPANDED(A, B)


class RosMsgConverter : public testing::Test
{
protected:

    static void SetUpTestSuite()
    {

        Py_Initialize();

        python::numpy::initialize();
        main    = new python::object(python::import("__main__"));
        globals = new python::dict(main->attr("__dict__"));

        // Append simple_function path to search
        appendPythonPath(TEST_PYTHON_FUNCTIONS_MODULE_PATH);
        boost::python::import("nrp_core.data.nrp_ros.nrp_ros_msgs");


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
    static const std::string runAndExtractExceptionMessage(const std::string & functionName, const nrp_ros_msgs::Test * inputRosMsg)
    {
        try
        {
            if(inputRosMsg != nullptr)
            {
                (*globals)[functionName](boost::ref(inputRosMsg));
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
    static void assertArrayEqual(const std::vector<TYPE> & result, const std::array<TYPE, 3> & expected)
    {
        ASSERT_EQ(result.size(), expected.size());

        for(size_t i = 0; i < result.size(); i++)
        {
            ASSERT_EQ(result[i], expected[i]);
        }
    }

    static void assertPoseEqual(const geometry_msgs::Pose & result)
    {
        geometry_msgs::Pose expected = geometry_msgs::Pose();
        expected.position.x = 1;
        expected.position.y = 2;
        expected.position.z = 3;
        expected.orientation.x = 1;
        expected.orientation.y = 2;
        expected.orientation.z = 3;
        expected.orientation.w = 4;
        ASSERT_EQ(result.position.x, expected.position.x);
        ASSERT_EQ(result.position.y, expected.position.y);
        ASSERT_EQ(result.position.z, expected.position.z);

        ASSERT_EQ(result.orientation.x, expected.orientation.x);
        ASSERT_EQ(result.orientation.y, expected.orientation.y);
        ASSERT_EQ(result.orientation.z, expected.orientation.z);
        ASSERT_EQ(result.orientation.w, expected.orientation.w);
    }


    static python::object * main;
    static python::dict   * globals;


};

python::object * RosMsgConverter::main = nullptr;
python::dict   * RosMsgConverter::globals = nullptr;


/*!
 * \brief Tests conversion from nlohmann::json to python using JsonDataPack
 */
TEST_F(RosMsgConverter, TestRosMsgToPython)
{
    try
    {
        // Create input datapack with MSG data

        nrp_ros_msgs::Test * inputRosMsg = new nrp_ros_msgs::Test();
        (*inputRosMsg).bool_msg = true;

        (*inputRosMsg).int8_msg = 20;
        (*inputRosMsg).uint8_msg = 21;
        (*inputRosMsg).int16_msg = 22;
        (*inputRosMsg).uint16_msg = 23;
        (*inputRosMsg).int32_msg = 24;
        (*inputRosMsg).uint32_msg = 25;
        (*inputRosMsg).int64_msg = 26;
        (*inputRosMsg).uint64_msg = 27;

        (*inputRosMsg).float32_msg = 100.13;
        (*inputRosMsg).float64_msg = 56.56;

        (*inputRosMsg).string_msg = "Armageddon";
        (*inputRosMsg).time_msg = ros::Time();
        (*inputRosMsg).duration_msg = ros::Duration();

        (*inputRosMsg).byte_array_msg = { 1, 0, 1, 0 };
        (*inputRosMsg).float_array_msg = { 1.0, 0.0, 2.0 };
        (*inputRosMsg).std_string_msg.data = "TestString";
        (*inputRosMsg).pose_msg.push_back(geometry_msgs::Pose());


        // Call the test function

        (*globals)["test_input"](boost::ref(inputRosMsg));
    }
    catch(boost::python::error_already_set &)
    {
        handlePythonException();
    }
}


/*!
 * \brief Tests failure modes of conversion from RosMsg to python using TestDataPack
 */
TEST_F(RosMsgConverter, TestRosMsgToPythonFailures)
{
    // Create input with RosMsg data
    // Use of no default constructor is not supported and should throw an exception

    nrp_ros_msgs::Test * inputRosMsg = new nrp_ros_msgs::Test();

    // Call the test function
    ASSERT_EQ(runAndExtractExceptionMessage("test_unsupported_not_default_constructor_time", inputRosMsg), "Python argument types in\n    Time.__init__(Time, int)\ndid not match C++ signature:\n    __init__(_object*)");
    ASSERT_EQ(runAndExtractExceptionMessage("test_unsupported_not_default_constructor_duration", inputRosMsg), "Python argument types in\n    Duration.__init__(Duration, int)\ndid not match C++ signature:\n    __init__(_object*)");
}




/*!
 * \brief Tests conversion from python to RosMsg using TestDataPack
 */
TEST_F(RosMsgConverter, TestPythonToRosMsg)
{
    try
    {
        // Call the test function

        nrp_ros_msgs::Test * res = boost::python::extract<nrp_ros_msgs::Test *>((*globals)["test_output"]());

        // Check basic data types

        ASSERT_EQ(res->bool_msg, true);
        ASSERT_EQ(res->int8_msg, 1);
        ASSERT_EQ(res->uint8_msg, 2);
        ASSERT_EQ(res->int16_msg, 3);
        ASSERT_EQ(res->uint16_msg, 4);
        ASSERT_EQ(res->int32_msg, 5);
        ASSERT_EQ(res->uint32_msg, 6);
        ASSERT_EQ(res->int64_msg, 7);
        ASSERT_EQ(res->uint64_msg, 8);
        ASSERT_NEAR(res->float32_msg, 43.21,1e-6);
        ASSERT_NEAR(res->float64_msg, 41.21,1e-6);
        ASSERT_EQ(res->string_msg, "string");
        ASSERT_EQ(res->time_msg, ros::Time(0.001));
        ASSERT_EQ(res->duration_msg, ros::Duration(60));
        ASSERT_EQ(res->std_string_msg.data, "test");


        assertArrayEqual(res->byte_array_msg, std::array<uint8_t , 3>({1, 0, 1}));
        assertArrayEqual(res->float_array_msg, std::array<float, 3>({4, 1, 6}));
        assertPoseEqual(res->pose_msg[0]);

    }
    catch(boost::python::error_already_set &)
    {
        handlePythonException();
    }
}





