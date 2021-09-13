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

#include "nrp_json_engine_protocol/config/engine_json_config.h"
#include "nrp_json_engine_protocol/engine_server/engine_json_server.h"
#include "nrp_json_engine_protocol/device_interfaces/json_device.h"

#include "tests/test_engine_json_device_controllers.h"

#include <future>
#include <restclient-cpp/restclient.h>

using namespace testing;

class TestEngineJSONServer
        : public EngineJSONServer
{
	public:
	template<class ...T>
	TestEngineJSONServer(T &&...properties)
	    : EngineJSONServer(std::forward<T>(properties)...)
	{}

	virtual ~TestEngineJSONServer() override = default;

	SimulationTime curTime = SimulationTime::zero();

	SimulationTime runLoopStep(SimulationTime timeStep) override
	{
		if(timeStep < SimulationTime::zero())
			throw std::invalid_argument("error");

		curTime += timeStep;

		return curTime;
	}

	nlohmann::json initialize(const nlohmann::json &data, EngineJSONServer::lock_t&) override
	{
		return nlohmann::json({{"status", "success"}, {"original", data}});
	}

	nlohmann::json reset(EngineJSONServer::lock_t&) override
	{
		return nlohmann::json({{"status", "success"}});
	}

	// TODO: Test shutdown
	nlohmann::json shutdown(const nlohmann::json &data) override
	{
		return nlohmann::json({{"status", "shutdown"}, {"original", data}});
	}

	template<class EXCEPTION = std::exception>
	void startCatchException()
	{
		ASSERT_THROW(this->startServer(), EXCEPTION);
		this->_serverRunning = false;
	}
};

// TODO Split the test into multiple simpler ones. Every test should test a single use case.
TEST(EngineJSONServerTest, Functions)
{
    const std::string address = "localhost:5434";
	TestEngineJSONServer server(address);

	// Create test device

	const std::string deviceName = "test_name";
	const std::string engineName = "test_engine_name";
	auto data = new nlohmann::json({{deviceName, {{"data", 1}}}});
	auto dev1 = JsonDevice(deviceName, engineName, data);

	// Register device controllers

	auto dev1Ctrl = TestJSONDeviceController(DeviceIdentifier(dev1.id()));
	server.registerDevice(dev1.name(), &dev1Ctrl);

	// Test setting empty data
	// The JSON object in the controller should not be updated

	auto retData = server.setDeviceData(nlohmann::json());
	ASSERT_FALSE(dev1Ctrl.data().contains("data"));
	ASSERT_TRUE(retData.empty());

	// Test setting data for an unregistered device
	// The JSON object in the controller should not be updated

	retData = server.setDeviceData(nlohmann::json({{"fakeDevice", {}}}));
	ASSERT_FALSE(dev1Ctrl.data().contains("data"));
	ASSERT_STREQ(retData.find("fakeDevice")->get<std::string>().data(), "");
	ASSERT_EQ(retData.size(), 1);

	// Test setting data for a registered device
	// The JSON object in the controller should be updated

	retData = server.setDeviceData(*data);
	ASSERT_EQ(retData.size(), 1);
	ASSERT_TRUE(dev1Ctrl.data().contains(deviceName));
	ASSERT_EQ(dev1Ctrl.data()[deviceName]["type"       ], JsonDevice::getType());
	ASSERT_EQ(dev1Ctrl.data()[deviceName]["engine_name"], engineName);
	ASSERT_EQ(dev1Ctrl.data()[deviceName]["data"       ], dev1.getData()[deviceName]["data"]);

	// Test getting empty data

	retData = server.getDeviceData(nlohmann::json());
	ASSERT_TRUE(retData.empty());

	// Test getting data for an unregistered device
	// The response should contain an empty JSON

	retData = server.getDeviceData(nlohmann::json({{"fakeDevice", {}}}));
	ASSERT_TRUE(retData.find("fakeDevice")->empty());
	ASSERT_EQ(retData.size(), 1);

	// Test getting data for a registered device
	// The response should contain an empty JSON

	auto request = nlohmann::json();
	request[deviceName] = {{"engine_name", engineName}};
	retData = server.getDeviceData(request);
	ASSERT_EQ(retData.size(), 1);
	ASSERT_TRUE(retData.contains("test_name"));
	ASSERT_EQ(retData[deviceName]["type"       ], JsonDevice::getType());
	ASSERT_EQ(retData[deviceName]["engine_name"], engineName);
	ASSERT_EQ(retData[deviceName]["data"       ], (*data)[deviceName]["data"]);

	// Clear devices

	server.clearRegisteredDevices();
	ASSERT_EQ(server._devicesControllers.size(), 0);
}

TEST(EngineJSONServerTest, HttpRequests)
{
	const std::string address = "localhost:5434";
	TestEngineJSONServer server(address);

	// Create test device

	const std::string deviceName = "test_name";
	const std::string engineName = "test_engine_name";
	auto devData = new nlohmann::json({{deviceName, {{"data", 1}}}});
	auto dev1 = JsonDevice(deviceName, "test_engine_name", devData);

	// Register device controllers

	auto dev1Ctrl = TestJSONDeviceController(DeviceIdentifier(dev1.id()));
	server.registerDevice(dev1.name(), &dev1Ctrl);

	ASSERT_FALSE(server.isServerRunning());
	server.startServerAsync();
	ASSERT_TRUE(server.isServerRunning());

	// Init command
	nlohmann::json data;
	data.emplace("init", nlohmann::json());
	auto resp = RestClient::post(address + "/" + EngineJSONConfigConst::EngineServerInitializeRoute.data(), EngineJSONConfigConst::EngineServerContentType.data(), data.dump());
	nlohmann::json retData = nlohmann::json::parse(resp.body);
	ASSERT_STREQ(retData["status"].get<std::string>().data(), "success");

	SimulationTime runTime = toSimulationTime<int, std::milli>(1);
	// Run step command
	data.clear();
	data[EngineJSONConfigConst::EngineTimeStepName.data()] = runTime.count();
	resp = RestClient::post(address + "/" + EngineJSONConfigConst::EngineServerRunLoopStepRoute.data(), EngineJSONConfigConst::EngineServerContentType.data(), data.dump());
	ASSERT_EQ(server.curTime, runTime);

	// Run set server command
	auto request = nlohmann::json();
	request[deviceName] = {{"engine_name", engineName}, {"data", 2}};
	resp = RestClient::post(address + "/" + EngineJSONConfigConst::EngineServerSetDevicesRoute.data(), EngineJSONConfigConst::EngineServerContentType.data(), request.dump());
	retData = nlohmann::json::parse(resp.body);
	// Device name with no data should be returned as confirmation
	ASSERT_EQ(retData.size(), 1);
	ASSERT_EQ(retData[deviceName], "");

	// Run get server command
	request.clear();
	request[deviceName] = {{"engine_name", engineName}};
	resp = RestClient::post(address + "/" + EngineJSONConfigConst::EngineServerGetDevicesRoute.data(), EngineJSONConfigConst::EngineServerContentType.data(), request.dump());
	retData = nlohmann::json::parse(resp.body);
	ASSERT_EQ(retData.size(), 1);
	ASSERT_TRUE(retData.contains(deviceName));
	ASSERT_EQ(retData[deviceName]["type"       ], JsonDevice::getType());
	ASSERT_EQ(retData[deviceName]["engine_name"], engineName);
	// We should get the same value as was set by the setDevices function above
	ASSERT_EQ(retData[deviceName]["data"       ], 2);
}
