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

#include <gtest/gtest.h>

#include "nrp_general_library/config/cmake_constants.h"
#include "tests/test_transceiver_function_interpreter.h"
#include "nrp_general_library/datapack_interface/datapack.h"
#include "nrp_general_library/utils/json_converter.h"
#include "tests/test_env_cmake.h"

using JsonDataPack = DataPack<nlohmann::json>;

using namespace boost;

void appendPythonPath(const std::string &path)
{
    boost::python::handle pathH(boost::python::borrowed(PySys_GetObject("path")));
    boost::python::list paths(pathH);
    paths.append(path);

    PySys_SetObject("path", paths.ptr());
}

TEST(TransceiverFunctionInterpreterTest, TestSimplePythonFcn)
{
    Py_Initialize();
    json_converter::initNumpy();
    boost::python::numpy::initialize();

    python::object main(python::import("__main__"));
    python::dict globals(main.attr("__dict__"));
    try
    {
        // Append simple_function path to search
        appendPythonPath(TEST_SIMPLE_TRANSCEIVER_FCN_MODULE_PATH);

        // Load simple function
        python::object simpleFcn(python::import("simple_fcn"));
        globals.update(simpleFcn.attr("__dict__"));
        globals["simple_fcn"]();
    }
    catch(boost::python::error_already_set &)
    {
        PyErr_Print();
        PyErr_Clear();

        std::cout.flush();

        throw std::invalid_argument("Python Exception");
    }

    TransceiverFunctionInterpreterSharedPtr interpreter(new TransceiverFunctionInterpreter(globals));

    TransceiverDataPackInterface::setTFInterpreter(interpreter.get());

    // Load and execute simple python function
    const std::string tfName = "testTF";
    TestSimpleTransceiverDataPack::shared_ptr tfDataPack(new TestSimpleTransceiverDataPack(globals["simple_fcn"]));
    interpreter->loadTransceiverFunction(tfName, tfDataPack);

    // Test execution result
    boost::python::list res(interpreter->runSingleTransceiverFunction(tfName));
    ASSERT_EQ((int)boost::python::extract<int>(res[0]), 3);

    // Test invalid TF name
    ASSERT_THROW(interpreter->runSingleTransceiverFunction("invalidTFName"), NRPExceptionNonRecoverable);

    TransceiverDataPackInterface::setTFInterpreter(nullptr);
}

/*!
 * \brief Fixture class for tests of TransceiverFunctionInterpreter
 */
class InterpreterTest : public testing::Test {
    protected:
        void SetUp() override
        {
            Py_Initialize();
            json_converter::initNumpy();
            boost::python::numpy::initialize();

            python::object main(python::import("__main__"));
            python::object nrpModule(python::import(PYTHON_MODULE_NAME_STR));

            appendPythonPath(TEST_PYTHON_MODULE_PATH);
            python::object testModule(python::import(TEST_PYTHON_MODULE_NAME_STR));

            globals.update(main.attr("__dict__"));
            globals.update(nrpModule.attr("__dict__"));
            globals.update(testModule.attr("__dict__"));

            interpreter.reset(new TransceiverFunctionInterpreter(globals));

            TransceiverDataPackInterface::setTFInterpreter(interpreter.get());

            interpreter->setEngineDataPacks({{this->engineName, &this->devs}});
        }

        void TearDown() override
        {
            TransceiverDataPackInterface::setTFInterpreter(nullptr);
        }

        void prepareTransceiverFunctionConfig(const std::string & name, const std::string & filename)
        {
            this->tfCfg["Name"] = name;
            this->tfCfg["FileName"] = filename;
            this->tfCfg["IsActive"] = true;
            this->tfCfg["IsPreprocessing"] = false;
        }

        void preparePreprocessingFunctionConfig(const std::string & name, const std::string & filename)
        {
            this->pfCfg["Name"] = name;
            this->pfCfg["FileName"] = filename;
            this->pfCfg["IsActive"] = true;
            this->pfCfg["IsPreprocessing"] = true;
        }

        void prepareInputDataPack(const std::string & name, int testValue)
        {
            std::shared_ptr<TestOutputDataPack> dev(new TestOutputDataPack(TestOutputDataPack::ID(name)));
            dev->TestValue = testValue;

            devs.push_back(dev);
        }

        inline static const std::string engineName = "engine";

        python::dict globals;
        nlohmann::json tfCfg;
        nlohmann::json pfCfg;
        TransceiverFunctionInterpreterSharedPtr interpreter;
        EngineClientInterface::datapacks_t devs;
};

TEST_F(InterpreterTest, TestTransceiverFcnDataPacks)
{
    const int testValue = 0;
    const std::string devName = "tf_input";

    this->prepareInputDataPack("tf_input", testValue);

    // Load and execute simple python function
    const std::string tfName = "testTF";
    TestTransceiverDataPack::shared_ptr tfDataPack(new TestTransceiverDataPack());
    interpreter->loadTransceiverFunction(tfName, tfDataPack);

    const auto &reqIDs = interpreter->updateRequestedDataPackIDs();
    ASSERT_EQ(reqIDs.size(), 1);
    ASSERT_EQ(*(reqIDs.begin()), TestOutputDataPack::ID());

    // Test execution result
    boost::python::list res;
    res = static_cast<boost::python::list>(interpreter->runSingleTransceiverFunction(tfName));

    ASSERT_EQ(boost::python::len(res), 1);

    const TestInputDataPack &inDataPack = boost::python::extract<TestInputDataPack>(res[0]);
    ASSERT_EQ(inDataPack.id(), TestInputDataPack::ID());
    ASSERT_EQ(testValue, std::stoi(inDataPack.TestValue));
}

/*
 * Setup:
 * - One transceiver function that takes a single datapack as input and returns a single datapack
 */
TEST_F(InterpreterTest, TestTransceiverFunction)
{
    const std::string tfName = "testTF";
    const std::string devName = "tf_input";
    const int testValue = 4;

    this->prepareTransceiverFunctionConfig(tfName, TEST_TRANSCEIVER_FCN_FILE_NAME);

    this->prepareInputDataPack(devName, testValue);

    // Load and execute simple python function
    interpreter->loadTransceiverFunction(this->tfCfg);

    const auto &reqIDs = interpreter->updateRequestedDataPackIDs();
    ASSERT_EQ(reqIDs.size(), 1);
    ASSERT_EQ(*(reqIDs.begin()), TestOutputDataPack::ID(devName));

    // Test execution result
    boost::python::list res(interpreter->runSingleTransceiverFunction(tfName));
    ASSERT_EQ(boost::python::len(res), 1);

    const TestInputDataPack &inDataPack = boost::python::extract<TestInputDataPack>(res[0]);
    ASSERT_EQ(inDataPack.id(), TestInputDataPack::ID());
    ASSERT_EQ(testValue, std::stoi(inDataPack.TestValue));
}

/*
 * Setup:
 * - One preprocessing function that takes a single datapack as input and returns a single datapack
 */
TEST_F(InterpreterTest, TestPreprocessingFunction)
{
    const std::string tfName = "testTF";
    const std::string devName = "pf_input";

    this->preparePreprocessingFunctionConfig(tfName, TEST_PREPROCESSING_FCN_FILE_NAME);

    this->prepareInputDataPack(devName, 5);

    // Load simple preprocessing function

    interpreter->loadTransceiverFunction(this->pfCfg);
    const auto &reqIDs = interpreter->updateRequestedDataPackIDs();

    ASSERT_EQ(reqIDs.size(), 1);
    ASSERT_EQ(*(reqIDs.begin()), TestOutputDataPack::ID(devName));

    // Run the preprocessing funtion

    boost::python::list res(interpreter->runSingleTransceiverFunction(interpreter->findTransceiverFunction(tfName)->second));

    // Test execution result

    ASSERT_EQ(boost::python::len(res), 1);
    const JsonDataPack &retDataPack = boost::python::extract<JsonDataPack>(res[0]);
    ASSERT_EQ(retDataPack.getData()["test_value"], "6");
}

/*
 * Setup:
 * - One preprocessing function that takes a multiple datapacks as input and returns multiple datapacks
 */
TEST_F(InterpreterTest, TestPreprocessingFunctionMultipleDataPacks)
{
    const std::string tfName = "testTF";
    const std::string devName1 = "pf_input1";
    const std::string devName2 = "pf_input2";
    const std::string devName3 = "pf_input3";

    this->preparePreprocessingFunctionConfig(tfName, TEST_PREPROCESSING_FCN2_FILE_NAME);

    this->prepareInputDataPack(devName1, 5);
    this->prepareInputDataPack(devName2, 15);
    this->prepareInputDataPack(devName3, 25);

    // Load the preprocessing function

    interpreter->loadTransceiverFunction(this->pfCfg);
    const auto &reqIDs = interpreter->updateRequestedDataPackIDs();

    // 3 datapacks should be requested by the preprocessing function from the engine

    ASSERT_EQ(reqIDs.size(), 3);
    ASSERT_NE(reqIDs.find(TestOutputDataPack::ID(devName1)), reqIDs.end());
    ASSERT_NE(reqIDs.find(TestOutputDataPack::ID(devName2)), reqIDs.end());
    ASSERT_NE(reqIDs.find(TestOutputDataPack::ID(devName3)), reqIDs.end());

    // Run the preprocessing funtion

    boost::python::list res(interpreter->runSingleTransceiverFunction(interpreter->findTransceiverFunction(tfName)->second));

    // Test execution result

    ASSERT_EQ(boost::python::len(res), 1);

    // TODO How to extract the datapacks and test return values?
}

/*
 * Setup:
 * - PF raises exception because of wrong input datapack
 */
TEST_F(InterpreterTest, TestPreprocessingFunctionWrongInputDataPack)
{
    const std::string tfName = "testTFFail";
    const std::string devName = "pf_input";

    this->preparePreprocessingFunctionConfig(tfName, TEST_INVALID_INPUT_PREPROCESSING_FCN_FILE_NAME);

    this->prepareInputDataPack(devName, 5);

    // Load simple preprocessing function and fail

    ASSERT_THROW(interpreter->loadTransceiverFunction(this->pfCfg), NRPException);
}

/*
 * Setup:
 * - PF raises exception because of wrong output datapack
 */
TEST_F(InterpreterTest, TestPreprocessingFunctionWrongOutputDataPack)
{
    const std::string tfName = "testTF";
    const std::string devName = "pf_input";

    this->preparePreprocessingFunctionConfig(tfName, TEST_INVALID_OUTPUT_PREPROCESSING_FCN_FILE_NAME);

    this->prepareInputDataPack(devName, 5);

    // Load simple preprocessing function

    interpreter->loadTransceiverFunction(this->pfCfg);
    const auto &reqIDs = interpreter->updateRequestedDataPackIDs();

    EXPECT_EQ(reqIDs.size(), 1);
    EXPECT_EQ(*(reqIDs.begin()), TestOutputDataPack::ID(devName));

    // Run the preprocessing funtion and fail

    ASSERT_THROW(interpreter->runSingleTransceiverFunction(interpreter->findTransceiverFunction(tfName)->second), NRPException);
}

/*
 * Setup:
 * - One preprocessing function and one transceiver function
 * - The preprocessing function runs before the transceiver function
 * - The preprocessing function takes one input datapack from the engine and return one datapack
 * - The transceiver function takes one input datapack from the engine and
 *   one input datapack from the preprocessing function. It returns one datapack
 */
TEST_F(InterpreterTest, TestFunctionChain)
{
    const std::string pfName = "testPF";
    const std::string tfName = "testTF";

    // Prepare function configurations

    this->preparePreprocessingFunctionConfig(pfName, TEST_PREPROCESSING_FCN_FILE_NAME);
    this->prepareTransceiverFunctionConfig(tfName, TEST_TRANSCEIVER_FCN2_FILE_NAME);

    this->prepareInputDataPack("pf_input", 4);
    this->prepareInputDataPack("tf_input_engine", 10);

    // Load simple preprocessing function

    interpreter->loadTransceiverFunction(this->tfCfg);
    interpreter->loadTransceiverFunction(this->pfCfg);
    const auto &reqIDs = interpreter->updateRequestedDataPackIDs();

    // There shoud be two datapacks requested by the function from the engine

    ASSERT_EQ(reqIDs.size(), 2);

    // Run the preprocessing function

    boost::python::list resPf(interpreter->runSingleTransceiverFunction(interpreter->findTransceiverFunction(pfName)->second));

    // Test the results

    ASSERT_EQ(boost::python::len(resPf), 1);
    const JsonDataPack & tmpDataPack = boost::python::extract<JsonDataPack>(resPf[0]);
    JsonDataPack retDataPack(tmpDataPack.name(), tmpDataPack.engineName(), new nlohmann::json(tmpDataPack.getData()));
    ASSERT_EQ(retDataPack.getData()["test_value"], "5");

    // Inject the returned datapack to the pool of datapacks, so that it's accesible by the transceiver function

    this->devs.push_back(retDataPack.moveToSharedPtr());

    // Run the transceiver function

    boost::python::list resTf(interpreter->runSingleTransceiverFunction(interpreter->findTransceiverFunction(tfName)->second));

    // Test the results

    const JsonDataPack &retDataPackTf = boost::python::extract<JsonDataPack>(resTf[0]);

    ASSERT_EQ(retDataPackTf.getData()["test_value1"], "10");
    ASSERT_EQ(retDataPackTf.getData()["test_value2"], "5");
}

// EOF
