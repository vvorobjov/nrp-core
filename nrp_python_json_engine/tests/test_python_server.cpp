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

#include "nrp_python_device/devices/pyobject_device.h"
#include "nrp_general_library/utils/python_interpreter_state.h"
#include "nrp_python_json_engine/engine_server/python_json_server.h"
#include "nrp_json_engine_protocol/device_interfaces/json_device_serializer.h"
#include "tests/test_env_cmake.h"

#include <boost/python.hpp>
#include <future>
#include <restclient-cpp/restclient.h>

namespace python = boost::python;

TEST(TestPythonJSONServer, TestFunc)
{
	std::string argvDat = "TestProg";
	char *argv = &argvDat[0];
	PythonInterpreterState pyState(1, &argv);

	python::dict pyGlobals = python::dict(python::import("__main__").attr("__dict__"));
	python::object pyLocals;

    nlohmann::json config;
    config["EngineName"] = "engine";
    config["EngineType"] = "test_engine_python";
    config["PythonFileName"] = TEST_PYTHON_DEVICE_FILE_NAME;
    std::string server_address = "localhost:5434";;
    config["ServerAddress"] = server_address;

	PythonJSONServer server(config.at("ServerAddress"), pyGlobals, pyLocals);

	// Test Init
	pyState.allowThreads();

	EngineJSONServer::mutex_t fakeMutex;
	EngineJSONServer::lock_t fakeLock(fakeMutex);
	nlohmann::json respParse = server.initialize(config, fakeLock);

	const auto execResult = respParse[PythonConfigConst::InitFileExecStatus.data()].get<bool>();
	ASSERT_EQ(execResult, true);
	ASSERT_EQ(server.initRunFlag(), true);

	// Test runStep REST call
	const SimulationTime timeStep(1000);
	ASSERT_EQ(server.runLoopStep(timeStep), timeStep);

	// Test getDevice REST call EngineServerGetDevicesRoute
	server.startServerAsync();

	//pyState.endAllowThreads();
	auto req = nlohmann::json({{"device1", 0}});
	auto resp = RestClient::post(server_address + "/" + EngineJSONConfigConst::EngineServerGetDevicesRoute.data(), EngineJSONConfigConst::EngineServerContentType.data(), req.dump());
	respParse = nlohmann::json::parse(resp.body);

	// TODO Why return here?
	return;

	// Test Python Device data deserialization
    auto devid = DeviceSerializerMethods<nlohmann::json>::deserializeID(respParse.begin());
	PyObjectDevice dev = DeviceSerializerMethods<nlohmann::json>::deserialize<PyObjectDevice>(std::move(devid), respParse.begin());

	dev.PyObjectDevice::data() = python::dict(dev.PyObjectDevice::data().deserialize(""));

	// TODO: Test Sending data

	ASSERT_EQ(respParse["device1"][PyObjectDeviceConst::Object.m_data].size(), python::len(dev.data()));
	float recTime = python::extract<float>(dev.data()["time"]);
	ASSERT_EQ(0, recTime);

	pyState.endAllowThreads();
	server.shutdownServer();
}

TEST(TestPythonJSONServer, TestInitError)
{
	std::string argvDat = "TestProg";
	char *argv = &argvDat[0];
	PythonInterpreterState pyState(1, &argv);

	auto pyGlobals = python::dict(python::import("__main__").attr("__dict__"));
	python::object pyLocals;

    nlohmann::json config;
    config["EngineName"] = "engine";
    config["EngineType"] = "test_engine_python";
    config["PythonFileName"] = TEST_PYTHON_INIT_ERROR_FILE_NAME;
    std::string server_address = "localhost:5434";
    config["ServerAddress"] = server_address;

	PythonJSONServer server(server_address, pyGlobals, pyLocals);
	pyState.allowThreads();

	EngineJSONServer::mutex_t fakeMutex;
	EngineJSONServer::lock_t fakeLock(fakeMutex);
	nlohmann::json respParse = server.initialize(config, fakeLock);

	pyState.endAllowThreads();
}
