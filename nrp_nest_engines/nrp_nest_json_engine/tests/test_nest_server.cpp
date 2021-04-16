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

#include "nrp_general_library/utils/python_interpreter_state.h"
#include "nrp_nest_json_engine/engine_server/nest_json_server.h"
#include "nrp_nest_json_engine/config/nest_config.h"
#include "tests/test_env_cmake.h"

#include <boost/python.hpp>
#include <future>
#include <restclient-cpp/restclient.h>

namespace python = boost::python;

TEST(TestNestJSONServer, TestFunc)
{
	std::string argvDat = "TestProg";
	char *argv = &argvDat[0];
	PythonInterpreterState pyState(1, &argv);

	python::dict pyGlobals = python::dict(python::import("__main__").attr("__dict__"));
	python::object pyLocals;

    nlohmann::json config;
    config["EngineName"] = "engine";
    config["EngineType"] = "test_engine_nest";
    config["NestInitFileName"] = TEST_NEST_DEVICE_FILE_NAME;
    std::string server_address = "localhost:5434";
    config["ServerAddress"] = server_address;

	NestJSONServer server(server_address, pyGlobals, pyLocals);

	// Test Init
	pyState.allowThreads();

	EngineJSONServer::mutex_t fakeMutex;
	EngineJSONServer::lock_t fakeLock(fakeMutex);
	nlohmann::json respParse = server.initialize(config, fakeLock);

	const auto execResult = respParse[NestConfigConst::InitFileExecStatus.data()].get<bool>();
	ASSERT_EQ(execResult, true);
	ASSERT_EQ(server.initRunFlag(), true);

	// Test runStep REST call
	SimulationTime timeStep = toSimulationTime<int, std::milli>(1);
	ASSERT_EQ(server.runLoopStep(timeStep).count(), timeStep.count());

	// Test getDevice REST call EngineServerGetDevicesRoute
	server.startServerAsync();

	auto req = nlohmann::json({{"voltmeter", 0}});
	auto resp = RestClient::post(server_address + "/" + EngineJSONConfigConst::EngineServerGetDevicesRoute.data(), EngineJSONConfigConst::EngineServerContentType.data(), req.dump());
	respParse = nlohmann::json::parse(resp.body);

	const std::string jsonDat = respParse["voltmeter"][PyObjectDeviceConst::Object.m_data]["element_type"].get<std::string>();
	ASSERT_STREQ(jsonDat.data(), "recorder");

	pyState.endAllowThreads();

	// Test Nest Device data deserialization
	auto devid = DeviceSerializerMethods<nlohmann::json>::deserializeID(respParse.begin());
	NestDevice dev = DeviceSerializerMethods<nlohmann::json>::deserialize<NestDevice>(std::move(devid), respParse.begin());

	//dev.data() = python::dict(dev.PyObjectDevice::data().deserialize(""));

	// TODO: Test Sending data

	ASSERT_EQ(respParse["voltmeter"][PyObjectDeviceConst::Object.m_data].size(), python::len(dev.data()));
	ASSERT_EQ(jsonDat, std::string(python::extract<std::string>(dev.data()["element_type"])));

	server.shutdownServer();
}

TEST(TestNestJSONServer, TestInitError)
{
	std::string argvDat = "TestProg";
	char *argv = &argvDat[0];
	PythonInterpreterState pyState(1, &argv);

	auto pyGlobals = python::dict(python::import("__main__").attr("__dict__"));
	python::object pyLocals;

    nlohmann::json config;
    config["EngineName"] = "engine";
    config["EngineType"] = "test_engine_nest";
    config["NestInitFileName"] = TEST_NEST_INIT_ERROR_NAME;
    std::string server_address = "localhost:5434";
    config["ServerAddress"] = server_address;

	NestJSONServer server(server_address, pyGlobals, pyLocals);

	pyState.allowThreads();

	EngineJSONServer::mutex_t fakeMutex;
	EngineJSONServer::lock_t fakeLock(fakeMutex);
	nlohmann::json respParse = server.initialize(config, fakeLock);

	pyState.endAllowThreads();
}
