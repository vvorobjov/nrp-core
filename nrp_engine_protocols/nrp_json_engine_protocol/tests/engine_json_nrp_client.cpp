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
#include "nrp_json_engine_protocol/nrp_client/engine_json_nrp_client.h"
#include "nrp_general_library/process_launchers/process_launcher_basic.h"

#include "tests/test_engine_json_datapack_controllers.h"

#include <future>
#include <restclient-cpp/restclient.h>

using namespace testing;

struct TestEngineJSONConfigConst
{
    static constexpr char EngineType[] = "test_engine";
    static constexpr char EngineSchema[] = "json://nrp-core/engines/engine_comm_protocols.json#/engine_json";
};

class TestEngineJSONServer
        : public EngineJSONServer
{
    public:
    template<class ...T>
    TestEngineJSONServer(T &&...properties)
        : EngineJSONServer(std::forward<T>(properties)..., "", "")
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

    nlohmann::json shutdown(const nlohmann::json &data) override
    {
        return nlohmann::json({{"shutdown", "success"}, {"original", data}});
    }

    template<class EXCEPTION = std::exception>
    void startCatchException()
    {
        ASSERT_THROW(this->startServer(), EXCEPTION);
        this->_serverRunning = false;
    }
};

class TestEngineJSONNRPClient
: public EngineJSONNRPClient<TestEngineJSONNRPClient, TestEngineJSONConfigConst::EngineSchema>
{
    public:
    template<class ...T>
    TestEngineJSONNRPClient(T &&...properties)
        : EngineJSONNRPClient(std::forward<T>(properties)...)
    {}

    virtual ~TestEngineJSONNRPClient() override = default;

    void initialize() override
    {
        auto retVal = this->sendInitCommand("initCommand");
        if(retVal["status"].get<std::string>().compare("initCommand") != 0)
            throw NRPExceptionNonRecoverable("Test init failed");
    }

    void reset() override
    {
        auto retVal = this->sendResetCommand("resetCommand");
        if (retVal["original"].get<std::string>().compare("success") != 0)
            throw NRPExceptionNonRecoverable("Test reset failed");
    };

    void shutdown() override
    {
        auto retVal = this->sendShutdownCommand("shutdownCommand");
        if (retVal["original"].get<std::string>().compare("shutdownCommand") != 0)
            throw NRPExceptionNonRecoverable("Test shutdown failed");
    }

    SimulationTime curTime = SimulationTime::zero();
};

TEST(EngineJSONNRPClientTest, EmptyDataPack)
{
    TestEngineJSONServer server("localhost:5463");

    const auto engineName = "engine1";

    auto data = new nlohmann::json({{"", {{"data", 1}}}});
    auto dev1 = JsonDataPack("datapack1", "engine_name_1", data);

    dev1.setEngineName(engineName);

    // Register datapack controllers
    auto dev1Ctrl = TestJSONDataPackController(DataPackIdentifier(dev1.id()));
    server.registerDataPack(dev1.name(), &dev1Ctrl);

    nlohmann::json config;
    config["EngineName"] = engineName;
    config["EngineType"] = "test_engine_json";

    // Start server
    server.startServerAsync();
    TestEngineJSONNRPClient client("localhost:" + std::to_string(server.serverPort()), config, ProcessLauncherInterface::unique_ptr(new ProcessLauncherBasic()));

    TestEngineJSONNRPClient::datapack_identifiers_set_t devIDs({dev1.id()});

    // Return an empty datapack from the controller, it should end up in the cache on clients side

    dev1Ctrl.triggerEmptyDataPackReturn(true);

    auto datapacks = client.updateDataPacksFromEngine(devIDs);

    ASSERT_EQ(datapacks.size(), 1);
    ASSERT_EQ(datapacks.at(0)->isEmpty(), true);

    // Return a regular datapack (with data) from the controller. It should replace the empty datapack

    dev1Ctrl.triggerEmptyDataPackReturn(false);

    datapacks = client.updateDataPacksFromEngine(devIDs);

    ASSERT_EQ(datapacks.size(), 1);
    ASSERT_EQ(datapacks.at(0)->isEmpty(), false);

    // Return another empty datapack from the controller, it should not overwrite the previous datapack

    dev1Ctrl.triggerEmptyDataPackReturn(true);

    datapacks = client.updateDataPacksFromEngine(devIDs);

    ASSERT_EQ(datapacks.size(), 1);
    ASSERT_EQ(datapacks.at(0)->isEmpty(), false);

    // It seems like the REST server needs some time to release resources before the next test can run...
    // Without the sleep, it will complain about address already taken

    sleep(1);
}

TEST(EngineJSONNRPClientTest, DISABLED_ServerCalls)
{
    // Setup test server
    TestEngineJSONServer server("localhost:5463");

    const auto engineName = "engine1";
    const auto falseEngineName = "engineFalse";

    auto data = new nlohmann::json({{"", {{"data", 1}}}});
    auto dev1 = JsonDataPack("datapack1", "engine_name_1", data);
    data = new nlohmann::json({{"", {{"data", 2}}}});
    auto dev2 =JsonDataPack("datapack2", "engine_name_2", data);
    data = new nlohmann::json({{"", {{"data", -1}}}});
    auto devThrow = JsonDataPack("datapackThrow", "engine_throw", data);

    dev1.setEngineName(engineName);
    dev2.setEngineName(engineName);
    devThrow.setEngineName(falseEngineName);

    // TODO Review! The test is currently disabled
    //dev1.data() = 3;
    //dev2.data() = 5;

    // Register datapack controllers
    auto dev1Ctrl = TestJSONDataPackController(DataPackIdentifier(dev1.id()));
    server.registerDataPack(dev1.name(), &dev1Ctrl);
    auto dev2Ctrl = TestJSONDataPackController(DataPackIdentifier(dev2.id()));
    server.registerDataPack(dev2.name(), &dev2Ctrl);

    nlohmann::json config;
    config["EngineName"] = engineName;
    config["EngineType"] = "test_engine_json";

    // Check timeout if no server is running
    TestEngineJSONNRPClient fakeClient("localhost:" + std::to_string(server.serverPort()), config, ProcessLauncherInterface::unique_ptr(new ProcessLauncherBasic()));
    ASSERT_THROW(fakeClient.initialize(), NRPExceptionNonRecoverable);

    // Start server, test init
    server.startServerAsync();
    TestEngineJSONNRPClient client("localhost:" + std::to_string(server.serverPort()), config, ProcessLauncherInterface::unique_ptr(new ProcessLauncherBasic()));
    ASSERT_NO_THROW(client.initialize());

    ASSERT_NO_THROW(client.runLoopStepAsync(toSimulationTimeFromSeconds(10)));
    ASSERT_NO_THROW(client.runLoopStepAsyncGet(toSimulationTimeFromSeconds(10)));

    ASSERT_EQ(client.getEngineTime(), server.curTime);

    // Test datapack retrieval
    TestEngineJSONNRPClient::datapack_identifiers_set_t devIDs({dev1.id(), dev2.id(), devThrow.id()});
    auto datapacks = client.updateDataPacksFromEngine(devIDs);

    // Only two datapacks (dev1, dev2) should be retrieved, as they are associated with the correct EngineName
    ASSERT_EQ(datapacks.size(), 2);

    // Assign correct datapacks, order can be arbitrary
    /*const DataPackInterface *retDev1BasePtr = nullptr, *retDev2BasePtr = nullptr;
    if(datapacks.begin()->get()->id() == dev1.id())
    {
        retDev1BasePtr = datapacks.begin()->get();
        retDev2BasePtr = (++datapacks.begin())->get();
    }
    else if(datapacks.begin()->get()->id() == dev2.id())
    {
        retDev2BasePtr = datapacks.begin()->get();
        retDev1BasePtr = (++datapacks.begin())->get();
    }

    ASSERT_NE(retDev1BasePtr, nullptr);
    ASSERT_NE(retDev2BasePtr, nullptr);

    const auto &retDev1 = dynamic_cast<const JsonDataPack&>(*retDev1BasePtr);
    const auto &retDev2 = dynamic_cast<const JsonDataPack&>(*retDev2BasePtr);

    ASSERT_EQ(retDev1.data(), dev1Ctrl.data().data());
    ASSERT_EQ(retDev2.data(), dev2Ctrl.data().data());

    auto inputDev1 = dev1;
    auto inputDev2 = dev2;
    auto inputDevThrow = devThrow;
    dev1.data() = 6;
    dev2.data() = -1;

    // Test datapack sending
    TestEngineJSONNRPClient::datapacks_ptr_t inputs;
    inputs.push_back(&inputDev1);
    inputs.push_back(&inputDev2);
    inputs.push_back(&inputDevThrow);
    ASSERT_NO_THROW(client.sendDataPacksToEngine(inputs));

    ASSERT_EQ(inputDev1.data(), dev1Ctrl.data().data());
    ASSERT_EQ(inputDev2.data(), dev2Ctrl.data().data());*/

    ASSERT_NO_THROW(client.reset());    
}
