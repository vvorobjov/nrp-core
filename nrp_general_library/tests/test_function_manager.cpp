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

#include "nrp_general_library/utils/utils.h"
#include "nrp_general_library/config/cmake_constants.h"
#include "tests/test_transceiver_function_manager.h"
#include "nrp_general_library/datapack_interface/datapack.h"
#include "nrp_general_library/utils/json_converter.h"
#include "tests/test_env_cmake.h"

using JsonDataPack = DataPack<nlohmann::json>;

using namespace boost;

/*!
 * \brief Fixture class for tests of TransceiverFunctionInterpreter
 */
class FunctionManagerTest : public testing::Test {
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

            functionManager.reset(new FunctionManager(globals));

            TransceiverDataPackInterface::setTFInterpreter(functionManager.get());

            devs.clear();
            functionManager->setEngineDataPacks({{this->engineName, &this->devs}});
        }

        void TearDown() override
        {
            TransceiverDataPackInterface::setTFInterpreter(nullptr);
        }

        void prepareInputDataPack(const std::string & name, int testValue)
        {
            std::shared_ptr<TestOutputDataPack> dev(new TestOutputDataPack(TestOutputDataPack::ID(name)));
            dev->TestValue = testValue;

            devs.push_back(dev);
            // TODO devs are passed by rvalue reference to the functionManager, not sure if that's safe...
        }

        inline static const std::string engineName = "engine";

        python::dict globals;
        FunctionManagerSharedPtr functionManager;
        EngineClientInterface::datapacks_t devs;
};

/*
 * Setup:
 * - One transceiver function that takes a single datapack as input and returns a single datapack
 */
TEST_F(FunctionManagerTest, TestTransceiverFunction)
{
    const std::string tfName     = "testTF";
    const std::string tfFilename = TEST_TRANSCEIVER_FCN_FILE_NAME;
    const std::string devName    = "tf_input";
    const int         testValue  = 4;

    this->prepareInputDataPack(devName, testValue);

    // Load and execute simple python function
    functionManager->loadDataPackFunction(tfName, tfFilename);

    const auto &reqIDs = functionManager->updateRequestedDataPackIDs();
    ASSERT_EQ(reqIDs.size(), 1);
    ASSERT_EQ(*(reqIDs.begin()), TestOutputDataPack::ID(devName));

    auto results = functionManager->executeTransceiverFunctions(this->engineName);

    // Test execution result
    // Results are a list of DataPackFunctionResult objects

    ASSERT_EQ(results.size(), 1);

    // TODO Why is this using InputDataPack?
    const auto resultDataPack = dynamic_cast<const TestInputDataPack *>(results.begin()->DataPacks.at(0));
    ASSERT_EQ(resultDataPack->id(), TestInputDataPack::ID());
    ASSERT_EQ(testValue, std::stoi(resultDataPack->TestValue));
}

/*
 * Setup:
 * - Try to run an unregistered (not loaded) Transceiver Function
 * - The run function should throw
 */
TEST_F(FunctionManagerTest, TestTransceiverFunctionUnregistered)
{
    const std::string tfName        = "testTF";
    const std::string tfFilename    = TEST_TRANSCEIVER_FCN_FILE_NAME;
    const std::string tfNameInvalid = "testTFinvalid";

    // Try to run a TF without any TFs registered in the functionManager

    auto results = functionManager->executeTransceiverFunctions(this->engineName);
    ASSERT_EQ(results.size(), 0);
}

/*
 * Setup:
 * - Try to load multiple status functions
 * - Only the first load should be successful
 */
TEST_F(FunctionManagerTest, TestTransceiverFunctionNoFile)
{
    const std::string transceiverFuntionName     = "testTF";
    const std::string transceiverFuntionFilename = "garbage_filename";

    ASSERT_THROW(functionManager->loadDataPackFunction(transceiverFuntionName, transceiverFuntionFilename), NRPException);
}


/*
 * Setup:
 * - Load a valid TF, then try to load another one with the same name
 * - The second load should fail
 */
TEST_F(FunctionManagerTest, TestTransceiverFunctionDuplicated)
{
    const std::string tfName     = "testTF";
    const std::string tfFilename = TEST_TRANSCEIVER_FCN_FILE_NAME;
    const std::string devName    = "tf_input";
    const int         testValue  = 4;

    // Load a valid TF

    this->prepareInputDataPack(devName, testValue);
    functionManager->loadDataPackFunction(tfName, tfFilename);

    // Try to load another TF with the same name

    ASSERT_THROW(functionManager->loadDataPackFunction(tfName, tfFilename), NRPException);
}


/*
 * Setup:
 * - Try to load multiple status functions
 * - Only the first load should be successful
 */
TEST_F(FunctionManagerTest, TestTransceiverFunctionInvalid)
{
    const std::string tfName     = "testTF";
    const std::string tfFilename = TEST_INVALID_TRANSCEIVER_FCN_FILE_NAME;

    functionManager->loadDataPackFunction(tfName, tfFilename);
    ASSERT_THROW(functionManager->executeTransceiverFunctions(this->engineName), NRPException);
}


/*
 * Setup:
 * - One preprocessing function that takes a single datapack as input and returns a single datapack
 */
TEST_F(FunctionManagerTest, TestPreprocessingFunction)
{
    const std::string tfName     = "testTF";
    const std::string tfFilename = TEST_PREPROCESSING_FCN_FILE_NAME;
    const std::string devName    = "pf_input";

    this->prepareInputDataPack(devName, 5);

    // Load simple preprocessing function

    functionManager->loadDataPackFunction(tfName, tfFilename);
    const auto &reqIDs = functionManager->updateRequestedDataPackIDs();

    ASSERT_EQ(reqIDs.size(), 1);
    ASSERT_EQ(*(reqIDs.begin()), TestOutputDataPack::ID(devName));

    // Run the preprocessing funtion

    auto results = functionManager->executePreprocessingFunctions(this->engineName);

    // Test execution result
    // Results are a list of DataPackFunctionResult objects

    ASSERT_EQ(results.size(), 1);

    const auto resultDataPack = dynamic_cast<const JsonDataPack *>(results.begin()->DataPacks.at(0));
    ASSERT_EQ("6", resultDataPack->getData()["test_value"]);
}

/*
 * Setup:
 * - Try to run an unregistered (not loaded) Preprocessing Function
 * - The run function should throw
 */
TEST_F(FunctionManagerTest, TestPreprocessingFunctionUnregistered)
{
    const std::string pfName        = "testPF";
    const std::string pfFilename    = TEST_PREPROCESSING_FCN_FILE_NAME;
    const std::string pfNameInvalid = "testPFinvalid";

    // Try to run a PF without any PFs registered in the functionManager

    auto results = functionManager->executePreprocessingFunctions(this->engineName);
    ASSERT_EQ(results.size(), 0);
}

/*
 * Setup:
 * - One preprocessing function that takes a multiple datapacks as input and returns multiple datapacks
 */
TEST_F(FunctionManagerTest, TestPreprocessingFunctionMultipleDataPacks)
{
    const std::string tfName     = "testTF";
    const std::string tfFilename = TEST_PREPROCESSING_FCN2_FILE_NAME;
    const std::string devName1   = "pf_input1";
    const std::string devName2   = "pf_input2";
    const std::string devName3   = "pf_input3";

    this->prepareInputDataPack(devName1, 5);
    this->prepareInputDataPack(devName2, 15);
    this->prepareInputDataPack(devName3, 25);

    // Load the preprocessing function

    functionManager->loadDataPackFunction(tfName, tfFilename);
    const auto &reqIDs = functionManager->updateRequestedDataPackIDs();

    // 3 datapacks should be requested by the preprocessing function from the engine

    ASSERT_EQ(reqIDs.size(), 3);
    ASSERT_NE(reqIDs.find(TestOutputDataPack::ID(devName1)), reqIDs.end());
    ASSERT_NE(reqIDs.find(TestOutputDataPack::ID(devName2)), reqIDs.end());
    ASSERT_NE(reqIDs.find(TestOutputDataPack::ID(devName3)), reqIDs.end());

    // Run the preprocessing funtion

    auto results = functionManager->executePreprocessingFunctions(this->engineName);

    // Test execution result

    ASSERT_EQ(results.size(), 1);

    // TODO How to extract the datapacks and test return values?
}

/*
 * Setup:
 * - PF raises exception because of wrong input datapack
 */
TEST_F(FunctionManagerTest, TestPreprocessingFunctionWrongInputDataPack)
{
    const std::string tfName     = "testTFFail";
    const std::string tfFilename = TEST_INVALID_INPUT_PREPROCESSING_FCN_FILE_NAME;
    const std::string devName    = "pf_input";

    this->prepareInputDataPack(devName, 5);

    // Load simple preprocessing function and fail

    ASSERT_THROW(functionManager->loadDataPackFunction(tfName, tfFilename), NRPException);
}

/*
 * Setup:
 * - PF raises exception because of wrong output datapack
 */
TEST_F(FunctionManagerTest, TestPreprocessingFunctionWrongOutputDataPack)
{
    const std::string tfName     = "testTF";
    const std::string tfFilename = TEST_INVALID_OUTPUT_PREPROCESSING_FCN_FILE_NAME;
    const std::string devName    = "pf_input";

    this->prepareInputDataPack(devName, 5);

    // Load simple preprocessing function

    functionManager->loadDataPackFunction(tfName, tfFilename);
    const auto &reqIDs = functionManager->updateRequestedDataPackIDs();

    EXPECT_EQ(reqIDs.size(), 1);
    EXPECT_EQ(*(reqIDs.begin()), TestOutputDataPack::ID(devName));

    // Run the preprocessing funtion and fail

    ASSERT_THROW(functionManager->executePreprocessingFunctions(this->engineName), NRPException);
}

/*
 * Setup:
 * - One preprocessing function, one transceiver function and one status function
 * - The preprocessing function runs first
 * - The preprocessing function takes one input datapack from the engine and returns one datapack
 * - The transceiver function takes one input datapack from the engine and
 *   one input datapack from the preprocessing function. It returns one datapack
 * - The status function takes one input datapack from the preprocessing function,
 *   one from the transceiver function, and one from the engine
 */
TEST_F(FunctionManagerTest, TestFunctionChain)
{
    const std::string pfName     = "testPF";
    const std::string pfFilename = TEST_PREPROCESSING_FCN_FILE_NAME;
    const std::string tfName     = "testTF";
    const std::string tfFilename = TEST_TRANSCEIVER_FCN2_FILE_NAME;
    const std::string statusFuntionName     = "testStatusFunction";
    const std::string statusFuntionFilename = TEST_STATUS_FCN2_FILE_NAME;

    // Prepare datapacks

    this->prepareInputDataPack("pf_input", 4);
    this->prepareInputDataPack("tf_input_engine", 10);

    // Load simple preprocessing function

    functionManager->loadDataPackFunction(tfName, tfFilename);
    functionManager->loadDataPackFunction(pfName, pfFilename);
    const auto &reqIDs = functionManager->updateRequestedDataPackIDs();

    // There shoud be two datapacks requested by the function from the engine

    ASSERT_EQ(reqIDs.size(), 2);

    // Run the preprocessing funtion

    auto resultsPf = functionManager->executePreprocessingFunctions(this->engineName);

    // Test execution result
    // Results are a list of DataPackFunctionResult objects

    ASSERT_EQ(resultsPf.size(), 1);

    auto resultDataPack = dynamic_cast<JsonDataPack *>(resultsPf.begin()->DataPacks.at(0));
    ASSERT_EQ("5", resultDataPack->getData()["test_value"]);

    // Inject the returned datapack to the pool of datapacks, so that it's accesible by the transceiver function

    this->devs.push_back(resultDataPack->moveToSharedPtr());

    // Run the transceiver function

    auto resultsTf = functionManager->executeTransceiverFunctions(this->engineName);

    // Test the results

    resultDataPack = dynamic_cast<JsonDataPack *>(resultsTf.begin()->DataPacks.at(0));

    ASSERT_EQ(resultDataPack->getData()["test_value1"], "10");
    ASSERT_EQ(resultDataPack->getData()["test_value2"], "5");

    // Inject the returned datapack to the pool of datapacks, so that it's accesible by the status function

    this->devs.push_back(resultDataPack->moveToSharedPtr());

    functionManager->loadStatusFunction(statusFuntionName, statusFuntionFilename);
    std::unique_ptr<nlohmann::json> results = functionManager->executeStatusFunction();

    ASSERT_EQ((*results)["test_value1"], "10");
    ASSERT_EQ((*results)["test_value2"], "5");
    ASSERT_EQ((*results)["test_value3"], "10");
}


/*
 * Setup:
 * - Try to load and run a basic status function that accepts a single datapack
 * - Everything should work without errors
 */
TEST_F(FunctionManagerTest, TestStatusFunction)
{
    const std::string statusFuntionName     = "testTF";
    const std::string statusFuntionFilename = TEST_STATUS_FCN_FILE_NAME;
    const std::string devName               = "tf_input";
    const int         testValue             = 4;

    this->prepareInputDataPack(devName, testValue);

    functionManager->loadStatusFunction(statusFuntionName, statusFuntionFilename);
    std::unique_ptr<nlohmann::json> results = functionManager->executeStatusFunction();

    ASSERT_EQ((*results)["test_int"], 456);
    ASSERT_TRUE((*results)["test_flags"][0]);
    ASSERT_FALSE((*results)["test_flags"][1]);
}


/*
 * Setup:
 * - Try to load multiple status functions
 * - Only the first load should be successful
 */
TEST_F(FunctionManagerTest, TestStatusFunctionDuplicated)
{
    const std::string statusFuntionName     = "testTF";
    const std::string statusFuntionFilename = TEST_STATUS_FCN_FILE_NAME;
    const std::string devName               = "tf_input";
    const int         testValue             = 4;

    this->prepareInputDataPack(devName, testValue);

    functionManager->loadStatusFunction(statusFuntionName, statusFuntionFilename);
    ASSERT_THROW(functionManager->loadStatusFunction(statusFuntionName, statusFuntionFilename), NRPException);
}


/*
 * Setup:
 * - Try to load multiple status functions
 * - Only the first load should be successful
 */
TEST_F(FunctionManagerTest, TestStatusFunctionInvalid)
{
    const std::string statusFuntionName     = "testTF";
    const std::string statusFuntionFilename = TEST_INVALID_STATUS_FCN_FILE_NAME;

    functionManager->loadStatusFunction(statusFuntionName, statusFuntionFilename);
    ASSERT_THROW(functionManager->executeStatusFunction(), NRPException);
}


/*
 * Setup:
 * - Try to load multiple status functions
 * - Only the first load should be successful
 */
TEST_F(FunctionManagerTest, TestStatusFunctionNoFile)
{
    const std::string statusFuntionName     = "testTF";
    const std::string statusFuntionFilename = "garbage_filename";

    ASSERT_THROW(functionManager->loadStatusFunction(statusFuntionName, statusFuntionFilename), NRPException);
}


/*
 * Setup:
 * - Try to execute status function without loading it
 * - The call to the run function should not fail, but should return early without executing anything
 */
TEST_F(FunctionManagerTest, TestStatusFunctionUndefined)
{
    ASSERT_EQ(functionManager->executeStatusFunction(), nullptr);
}


// EOF
