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

const JsonDataPack * castToJsonDataPack(std::shared_ptr<const DataPackInterface> dataPack)
{
    return dynamic_cast<const JsonDataPack *>(dataPack.get());
}

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

            dataPacks.clear();
        }

        void TearDown() override
        {
            TransceiverDataPackInterface::setTFInterpreter(nullptr);
        }

        void prepareInputDataPack(const std::string & name, int testValue)
        {
            std::shared_ptr<TestOutputDataPack> dataPack(new TestOutputDataPack(TestOutputDataPack::ID(name)));
            dataPack->TestValue = testValue;

            dataPacks.insert(dataPack);
        }

        void insertInputDataPack(const DataPackInterface * dataPack)
        {
            this->dataPacks.insert(DataPackInterfaceConstSharedPtr(dataPack));
        }

        inline static const std::string engineName = "engine";

        python::dict globals;
        FunctionManagerSharedPtr functionManager;
        datapacks_set_t dataPacks;
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

    const auto &reqIDs = functionManager->getRequestedDataPackIDs();
    ASSERT_EQ(reqIDs.size(), 1);
    ASSERT_EQ(*(reqIDs.begin()), TestOutputDataPack::ID(devName));

    auto results = functionManager->executeTransceiverFunctions(this->engineName, this->dataPacks);

    // Test execution result
    // Results are a list of DataPackFunctionResult objects

    ASSERT_EQ(results.size(), 1);

    // TODO Why is this using InputDataPack?
    const auto resultDataPack = dynamic_cast<const TestInputDataPack *>(results.at(0).get());
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

    auto results = functionManager->executeTransceiverFunctions(this->engineName, this->dataPacks);
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
    ASSERT_THROW(functionManager->executeTransceiverFunctions(this->engineName, this->dataPacks), NRPException);
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
    const auto &reqIDs = functionManager->getRequestedDataPackIDs();

    ASSERT_EQ(reqIDs.size(), 1);
    ASSERT_EQ(*(reqIDs.begin()), TestOutputDataPack::ID(devName));

    // Run the preprocessing funtion

    auto results = functionManager->executePreprocessingFunctions(this->engineName, this->dataPacks);

    // Test execution result
    // Results are a list of DataPackFunctionResult objects

    ASSERT_EQ(results.size(), 1);

    auto resultDataPack = castToJsonDataPack(results.at(0));
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

    auto results = functionManager->executePreprocessingFunctions(this->engineName, this->dataPacks);
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
    const auto &reqIDs = functionManager->getRequestedDataPackIDs();

    // 3 datapacks should be requested by the preprocessing function from the engine

    ASSERT_EQ(reqIDs.size(), 3);
    ASSERT_NE(reqIDs.find(TestOutputDataPack::ID(devName1)), reqIDs.end());
    ASSERT_NE(reqIDs.find(TestOutputDataPack::ID(devName2)), reqIDs.end());
    ASSERT_NE(reqIDs.find(TestOutputDataPack::ID(devName3)), reqIDs.end());

    // Run the preprocessing funtion

    auto results = functionManager->executePreprocessingFunctions(this->engineName, this->dataPacks);

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
    const auto &reqIDs = functionManager->getRequestedDataPackIDs();

    EXPECT_EQ(reqIDs.size(), 1);
    EXPECT_EQ(*(reqIDs.begin()), TestOutputDataPack::ID(devName));

    // Run the preprocessing funtion and fail

    ASSERT_THROW(functionManager->executePreprocessingFunctions(this->engineName, this->dataPacks), NRPException);
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
    const auto &reqIDs = functionManager->getRequestedDataPackIDs();

    // There shoud be two datapacks requested by the function from the engine

    ASSERT_EQ(reqIDs.size(), 2);

    // Run the preprocessing funtion

    auto resultsPf = functionManager->executePreprocessingFunctions(this->engineName, this->dataPacks);

    // Test execution result
    // Results are a list of DataPackFunctionResult objects

    ASSERT_EQ(resultsPf.size(), 1);

    auto resultDataPack = castToJsonDataPack(resultsPf.at(0));
    ASSERT_EQ("5", resultDataPack->getData()["test_value"]);

    // Inject the returned datapack to the pool of datapacks, so that it's accesible by the transceiver function

    this->insertInputDataPack(resultDataPack);

    // Run the transceiver function

    auto resultsTf = functionManager->executeTransceiverFunctions(this->engineName, this->dataPacks);

    // Test the results

    resultDataPack = castToJsonDataPack(resultsTf.at(0));

    ASSERT_EQ(resultDataPack->getData()["test_value1"], "10");
    ASSERT_EQ(resultDataPack->getData()["test_value2"], "5");

    // Inject the returned datapack to the pool of datapacks, so that it's accesible by the status function

    this->insertInputDataPack(resultDataPack);

    functionManager->loadStatusFunction(statusFuntionName, statusFuntionFilename);
    auto results = functionManager->executeStatusFunction(this->dataPacks);

    bool doneFlag = std::get<0>(results);
    ASSERT_TRUE(doneFlag);

    auto dataPacks = std::get<1>(results);
    const auto resultsJson = castToJsonDataPack(dataPacks.at(0));
    ASSERT_EQ((*resultsJson).getData()["test_value1"], "10");
    ASSERT_EQ((*resultsJson).getData()["test_value2"], "5");
    ASSERT_EQ((*resultsJson).getData()["test_value3"], "10");
}


TEST_F(FunctionManagerTest, TestStatusFunctionDoneFlag)
{
    const std::string statusFuntionName     = "testTF";
    const std::string statusFuntionFilename = TEST_STATUS_FCN_FILE_NAME;
    const std::string devName               = "tf_input";
    const int         testValue             = 4;

    this->prepareInputDataPack(devName, testValue);

    functionManager->loadStatusFunction(statusFuntionName, statusFuntionFilename);
    auto requestedIds = functionManager->getRequestedDataPackIDs();
    ASSERT_EQ(requestedIds.size(), 1);
    ASSERT_EQ(requestedIds.begin()->Name, devName);
    auto results = functionManager->executeStatusFunction(this->dataPacks);

    bool doneFlag = std::get<0>(results);
    ASSERT_TRUE(doneFlag);

    this->dataPacks.begin()->get()->resetIsUpdated();

    results = functionManager->executeStatusFunction(this->dataPacks);

    doneFlag = std::get<0>(results);
    ASSERT_FALSE(doneFlag);
}


TEST_F(FunctionManagerTest, TestStatusFunctionData)
{
    const std::string statusFuntionName     = "testTF";
    const std::string statusFuntionFilename = TEST_STATUS_FCN_DATA_FILE_NAME;

    functionManager->loadStatusFunction(statusFuntionName, statusFuntionFilename);

    // First execution - there should be no DataPacks in the results

    auto results = functionManager->executeStatusFunction(this->dataPacks);
    datapacks_vector_t dataPacks = std::get<1>(results);
    ASSERT_EQ(dataPacks.size(), 0);

    // Second execution - there should be a single DataPack in the results

    results = functionManager->executeStatusFunction(this->dataPacks);
    dataPacks = std::get<1>(results);
    ASSERT_EQ(dataPacks.size(), 1);

    // Third execution - there should be two DataPacks in the results

    results = functionManager->executeStatusFunction(this->dataPacks);
    dataPacks = std::get<1>(results);
    ASSERT_EQ(dataPacks.size(), 2);
}


TEST_F(FunctionManagerTest, TestSimulationTimeAndIter)
{
    const std::string statusFuntionName     = "testTF";
    const std::string statusFuntionFilename = TEST_SIM_TIME_FILE_NAME;

    functionManager->loadStatusFunction(statusFuntionName, statusFuntionFilename);

    // First execution - test what happens when the time was never set

    auto results = functionManager->executeStatusFunction(this->dataPacks);
    datapacks_vector_t dataPacks = std::get<1>(results);
    ASSERT_EQ(dataPacks.size(), 1);
    ASSERT_EQ(castToJsonDataPack(dataPacks.at(0))->getData()["sim_time"], 0);
    ASSERT_EQ(castToJsonDataPack(dataPacks.at(0))->getData()["sim_iter"], 0);

    // Second execution - set the time to 1s, set the iteration to 1
    // The Status Function should call proper python functions and retrieve these values,
    // then put them into a DataPack

    functionManager->setSimulationTime(toSimulationTime<int, std::ratio<1>>(1));
    functionManager->setSimulationIteration(1);

    results = functionManager->executeStatusFunction(this->dataPacks);
    dataPacks = std::get<1>(results);
    ASSERT_EQ(dataPacks.size(), 1);
    ASSERT_EQ(castToJsonDataPack(dataPacks.at(0))->getData()["sim_time"], 1000000000);
    ASSERT_EQ(castToJsonDataPack(dataPacks.at(0))->getData()["sim_iter"], 1);
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
    ASSERT_THROW(functionManager->executeStatusFunction(this->dataPacks), NRPException);
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
    auto results = functionManager->executeStatusFunction(this->dataPacks);
    bool doneFlag = std::get<0>(results);

    ASSERT_EQ(doneFlag, false);
}


/*
 * Setup:
 * - The transceiver function takes two input datapacks from the engine using
 *   one keyword and accesing to them as a dictionary in the transceiver function.
 *   It returns one datapack
 */
TEST_F(FunctionManagerTest, TestMultiDataPacksTf)
{
    const std::string tfName     = "testTF";
    const std::string tfFilename = TEST_TRANSCEIVER_FCN3_FILE_NAME;

    this->prepareInputDataPack("datapack1", 4);
    this->prepareInputDataPack("datapack2", 10);

    // Load simple transceiver function

    functionManager->loadDataPackFunction(tfName, tfFilename);
    const auto &reqIDs = functionManager->getRequestedDataPackIDs();

    // There shoud be two datapacks requested by the function from the engine

    ASSERT_EQ(reqIDs.size(), 2);

    // Test execution result
    // Results are a list of DataPackFunctionResult objects

    auto results = functionManager->executeTransceiverFunctions(this->engineName, this->dataPacks);

    ASSERT_EQ(results.size(), 1);

    // Test the results
    // TODO Add a helper function for unpacking DataPacks
    const auto resultDataPack = castToJsonDataPack(results.at(0));

    ASSERT_EQ(resultDataPack->name(),"return_datapack");
    ASSERT_EQ(resultDataPack->getData()["test_value1"], "10");
    ASSERT_EQ(resultDataPack->getData()["test_value2"], "4");
}


/*
 * Setup:
 * - The transceiver function takes two input datapacks from the engine using
 *   one keyword and accesing to them as a dictionary in the transceiver function.
 *   It returns one datapack
 */
TEST_F(FunctionManagerTest, TestMultiDataPacksTfInvalid)
{
    const std::string tfName     = "testTF";
    const std::string tfFilename = TEST_TRANSCEIVER_FCN3_FILE_NAME;

    this->prepareInputDataPack("datapack1", 4);
    this->prepareInputDataPack("datapack3", 10);

    // Load simple transceiver function

    functionManager->loadDataPackFunction(tfName, tfFilename);
    const auto &reqIDs = functionManager->getRequestedDataPackIDs();

    // There shoud be two datapacks requested by the function from the engine

    ASSERT_EQ(reqIDs.size(), 2);

    // Test execution result
    ASSERT_THROW(functionManager->executeTransceiverFunctions(this->engineName, this->dataPacks), NRPException);
}

/*
 * Setup:
 * - One preprocessing function and one transceiver function
 * - The preprocessing function runs before the transceiver function
 * - The preprocessing function takes one input datapack from the engine and return one datapack
 * - The transceiver function takes two input datapacks from the engine and
 *   one input datapack from the preprocessing function. It returns one datapack
 */
TEST_F(FunctionManagerTest, TestFunctionChainMultiDataPacks)
{
    const std::string pfName = "testPF";
    const std::string tfName = "testTF";
    const std::string pfFilename = TEST_PREPROCESSING_FCN_FILE_NAME;
    const std::string tffilename = TEST_TRANSCEIVER_FCN4_FILE_NAME;

    this->prepareInputDataPack("pf_input", 4);
    this->prepareInputDataPack("tf_input_datapack1", 10);
    this->prepareInputDataPack("tf_input_datapack2", 20);

    // Load simple preprocessing function and transceiver function

    functionManager->loadDataPackFunction(pfName, pfFilename);
    functionManager->loadDataPackFunction(tfName, tffilename);
    const auto &reqIDs = functionManager->getRequestedDataPackIDs();

    // There shoud be two datapacks requested by the function from the engine

    ASSERT_EQ(reqIDs.size(), 3);

    // Run the preprocessing function

    auto resultsPf = functionManager->executePreprocessingFunctions(this->engineName, this->dataPacks);

    // Test execution result
    // Results are a list of DataPackFunctionResult objects

    ASSERT_EQ(resultsPf.size(), 1);

    auto resultDataPack = castToJsonDataPack(resultsPf.at(0));
    ASSERT_EQ("5", resultDataPack->getData()["test_value"]);

    // Inject the returned datapack to the pool of datapacks, so that it's accesible by the transceiver function

    this->insertInputDataPack(resultDataPack);

    // Run the transceiver function

    auto resultsTf = functionManager->executeTransceiverFunctions(this->engineName, this->dataPacks);

    // Test the results

    resultDataPack = castToJsonDataPack(resultsTf.at(0));

    ASSERT_EQ(resultDataPack->name(),"return_datapack");
    ASSERT_EQ(resultDataPack->getData()["test_value1"], "15");
    ASSERT_EQ(resultDataPack->getData()["test_value2"], "25");
}


// EOF
