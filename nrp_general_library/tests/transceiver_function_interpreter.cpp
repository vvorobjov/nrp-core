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
#include "nrp_python_device/devices/pyobject_device.h"
#include "tests/test_env_cmake.h"

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

	TransceiverDeviceInterface::setTFInterpreter(interpreter.get());

	// Load and execute simple python function
	const std::string tfName = "testTF";
	TestSimpleTransceiverDevice::shared_ptr tfDevice(new TestSimpleTransceiverDevice(globals["simple_fcn"]));
	interpreter->loadTransceiverFunction(tfName, tfDevice);

	// Test execution result
	boost::python::list res(interpreter->runSingleTransceiverFunction(tfName));
	ASSERT_EQ((int)boost::python::extract<int>(res[0]), 3);

	// Test invalid TF name
	ASSERT_THROW(interpreter->runSingleTransceiverFunction("invalidTFName"), NRPExceptionNonRecoverable);

	TransceiverDeviceInterface::setTFInterpreter(nullptr);
}

/*!
 * \brief Fixture class for tests of TransceiverFunctionInterpreter
 */
class InterpreterTest : public testing::Test {
	protected:
  		void SetUp() override
		{
			Py_Initialize();
			python::object main(python::import("__main__"));
			python::object nrpModule(python::import(PYTHON_MODULE_NAME_STR));

			appendPythonPath(TEST_PYTHON_MODULE_PATH);
			python::object testModule(python::import(TEST_PYTHON_MODULE_NAME_STR));

			globals.update(main.attr("__dict__"));
			globals.update(nrpModule.attr("__dict__"));
			globals.update(testModule.attr("__dict__"));

			interpreter.reset(new TransceiverFunctionInterpreter(globals));

			TransceiverDeviceInterface::setTFInterpreter(interpreter.get());

			interpreter->setEngineDevices({{this->engineName, &this->devs}});
		}

		void TearDown() override
		{
			TransceiverDeviceInterface::setTFInterpreter(nullptr);
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

		void prepareInputDevice(const std::string & name, int testValue)
		{
			std::shared_ptr<TestOutputDevice> dev(new TestOutputDevice(TestOutputDevice::ID(name)));
			dev->TestValue = testValue;

			devs.push_back(dev);
		}

		inline static const std::string engineName = "engine";

		python::dict globals;
		nlohmann::json tfCfg;
		nlohmann::json pfCfg;
		TransceiverFunctionInterpreterSharedPtr interpreter;
		EngineClientInterface::devices_t devs;
};

TEST_F(InterpreterTest, TestTransceiverFcnDevices)
{
	const int testValue = 0;
	const std::string devName = "tf_input";

	this->prepareInputDevice("tf_input", testValue);

	// Load and execute simple python function
	const std::string tfName = "testTF";
	TestTransceiverDevice::shared_ptr tfDevice(new TestTransceiverDevice());
	interpreter->loadTransceiverFunction(tfName, tfDevice);

	const auto &reqIDs = interpreter->updateRequestedDeviceIDs();
	ASSERT_EQ(reqIDs.size(), 1);
	ASSERT_EQ(*(reqIDs.begin()), TestOutputDevice::ID());

	// Test execution result
	boost::python::list res;
	res = static_cast<boost::python::list>(interpreter->runSingleTransceiverFunction(tfName));

	ASSERT_EQ(boost::python::len(res), 1);

	const TestInputDevice &inDevice = boost::python::extract<TestInputDevice>(res[0]);
	ASSERT_EQ(inDevice.id(), TestInputDevice::ID());
	ASSERT_EQ(testValue, std::stoi(inDevice.TestValue));
}

/*
 * Setup:
 * - One transceiver function that takes a single device as input and returns a single device
 */
TEST_F(InterpreterTest, TestTransceiverFunction)
{
	const std::string tfName = "testTF";
	const std::string devName = "tf_input";
	const int testValue = 4;

	this->prepareTransceiverFunctionConfig(tfName, TEST_TRANSCEIVER_FCN_FILE_NAME);

	this->prepareInputDevice(devName, testValue);

	// Load and execute simple python function
	interpreter->loadTransceiverFunction(this->tfCfg);

	const auto &reqIDs = interpreter->updateRequestedDeviceIDs();
	ASSERT_EQ(reqIDs.size(), 1);
	ASSERT_EQ(*(reqIDs.begin()), TestOutputDevice::ID(devName));

	// Test execution result
	boost::python::list res(interpreter->runSingleTransceiverFunction(tfName));
	ASSERT_EQ(boost::python::len(res), 1);

	const TestInputDevice &inDevice = boost::python::extract<TestInputDevice>(res[0]);
	ASSERT_EQ(inDevice.id(), TestInputDevice::ID());
	ASSERT_EQ(testValue, std::stoi(inDevice.TestValue));
}

/*
 * Setup:
 * - One preprocessing function that takes a single device as input and returns a single device
 */
TEST_F(InterpreterTest, TestPreprocessingFunction)
{
	const std::string tfName = "testTF";
	const std::string devName = "pf_input";

	this->preparePreprocessingFunctionConfig(tfName, TEST_PREPROCESSING_FCN_FILE_NAME);

	this->prepareInputDevice(devName, 5);

	// Load simple preprocessing function

	interpreter->loadTransceiverFunction(this->pfCfg);
	const auto &reqIDs = interpreter->updateRequestedDeviceIDs();

	ASSERT_EQ(reqIDs.size(), 1);
	ASSERT_EQ(*(reqIDs.begin()), TestOutputDevice::ID(devName));

	// Run the preprocessing funtion

	boost::python::list res(interpreter->runSingleTransceiverFunction(interpreter->findTransceiverFunction(tfName)->second));

	// Test execution result

	ASSERT_EQ(boost::python::len(res), 1);
	const PyObjectDevice &retDevice = boost::python::extract<PyObjectDevice>(res[0]);
	ASSERT_EQ(retDevice.data().serialize(), "{\"test_value\": \"6\"}");
}

/*
 * Setup:
 * - One preprocessing function that takes a multiple devices as input and returns multiple devices
 */
TEST_F(InterpreterTest, TestPreprocessingFunctionMultipleDevices)
{
	const std::string tfName = "testTF";
	const std::string devName1 = "pf_input1";
	const std::string devName2 = "pf_input2";
	const std::string devName3 = "pf_input3";

	this->preparePreprocessingFunctionConfig(tfName, TEST_PREPROCESSING_FCN2_FILE_NAME);

	this->prepareInputDevice(devName1, 5);
	this->prepareInputDevice(devName2, 15);
	this->prepareInputDevice(devName3, 25);

	// Load the preprocessing function

	interpreter->loadTransceiverFunction(this->pfCfg);
	const auto &reqIDs = interpreter->updateRequestedDeviceIDs();

	// 3 devices should be requested by the preprocessing function from the engine

	ASSERT_EQ(reqIDs.size(), 3);
	ASSERT_NE(reqIDs.find(TestOutputDevice::ID(devName1)), reqIDs.end());
	ASSERT_NE(reqIDs.find(TestOutputDevice::ID(devName2)), reqIDs.end());
	ASSERT_NE(reqIDs.find(TestOutputDevice::ID(devName3)), reqIDs.end());

	// Run the preprocessing funtion

	boost::python::list res(interpreter->runSingleTransceiverFunction(interpreter->findTransceiverFunction(tfName)->second));

	// Test execution result

	ASSERT_EQ(boost::python::len(res), 2);

	// TODO How to extract the devices and test return values?
}

/*
 * Setup:
 * - One preprocessing function and one transceiver function
 * - The preprocessing function runs before the transceiver function
 * - The preprocessing function takes one input device from the engine and return one device
 * - The transceiver function takes one input device from the engine and
 *   one input device from the preprocessing function. It returns one device
 */
TEST_F(InterpreterTest, TestFunctionChain)
{
	const std::string pfName = "testPF";
	const std::string tfName = "testTF";

	// Prepare function configurations

	this->preparePreprocessingFunctionConfig(pfName, TEST_PREPROCESSING_FCN_FILE_NAME);
	this->prepareTransceiverFunctionConfig(tfName, TEST_TRANSCEIVER_FCN2_FILE_NAME);

	this->prepareInputDevice("pf_input", 4);
	this->prepareInputDevice("tf_input_engine", 10);

	// Load simple preprocessing function

	interpreter->loadTransceiverFunction(this->tfCfg);
	interpreter->loadTransceiverFunction(this->pfCfg);
	const auto &reqIDs = interpreter->updateRequestedDeviceIDs();

	// There shoud be two devices requested by the function from the engine

	ASSERT_EQ(reqIDs.size(), 2);

	// Run the preprocessing function

	boost::python::list resPf(interpreter->runSingleTransceiverFunction(interpreter->findTransceiverFunction(pfName)->second));

	// Test the results

	ASSERT_EQ(boost::python::len(resPf), 1);
	const PyObjectDevice &retDevice = boost::python::extract<PyObjectDevice>(resPf[0]);
	ASSERT_EQ(retDevice.data().serialize(), "{\"test_value\": \"5\"}");

	// Inject the returned device to the pool of devices, so that it's accesible by the transceiver function

	this->devs.push_back(retDevice.moveToSharedPtr());

	// Run the transceiver function

	boost::python::list resTf(interpreter->runSingleTransceiverFunction(interpreter->findTransceiverFunction(tfName)->second));

	// Test the results

	const PyObjectDevice &retDeviceTf = boost::python::extract<PyObjectDevice>(resTf[0]);

	nlohmann::json resultJson = nlohmann::json::parse(retDeviceTf.data().serialize());

	ASSERT_EQ(resultJson["test_value1"], "10");
	ASSERT_EQ(resultJson["test_value2"], "5");
}

// EOF
