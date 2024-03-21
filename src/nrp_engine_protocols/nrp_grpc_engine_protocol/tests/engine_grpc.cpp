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

#include <iostream>

#include <gtest/gtest.h>

#include "nrp_general_library/process_launchers/process_launcher_basic.h"
#include "nrp_grpc_engine_protocol/config/engine_grpc_config.h"
#include "nrp_grpc_engine_protocol/engine_server/engine_grpc_server.h"
#include "nrp_grpc_engine_protocol/engine_client/engine_grpc_client.h"
#include "nrp_protobuf/engine_grpc.grpc.pb.h"
#include "nrp_protobuf/enginetest.pb.h"



void testSleep(unsigned sleepMs)
{
    std::chrono::milliseconds timespan(sleepMs);
    std::this_thread::sleep_for(timespan);
}


class TestGrpcDataPackController
        : public DataPackController<google::protobuf::Message>
{
    public:

        TestGrpcDataPackController()
            : _data(new EngineTest::TestPayload())
        { }

        virtual void handleDataPackData(const google::protobuf::Message &data) override
        {
            // throws bad_cast
            const auto &j = dynamic_cast<const EngineTest::TestPayload &>(data);
            _data->CopyFrom(j);
        }

        virtual google::protobuf::Message *getDataPackInformation() override
        {
            if(this->_returnEmptyDataPack)
                return nullptr;
            else {
                auto old_data = _data;
                _data = new EngineTest::TestPayload();
                _data->CopyFrom(*old_data);
                return old_data;
            }
        }

        void triggerEmptyDataPackReturn(bool value)
        {
            this->_returnEmptyDataPack = value;
        }

    private:
        EngineTest::TestPayload* _data;
        bool _returnEmptyDataPack = false;
};

struct TestEngineGRPCConfigConst
{
    static constexpr char EngineType[] = "test_engine";
    static constexpr char EngineSchema[] = "json://nrp-core/engines/engine_comm_protocols.json#/engine_grpc";
};

class TestEngineGrpcClient
: public EngineGrpcClient<TestEngineGrpcClient, TestEngineGRPCConfigConst::EngineSchema>
{
    public:
        TestEngineGrpcClient(nlohmann::json &config, ProcessLauncherInterface::unique_ptr &&launcher)
            : EngineGrpcClient(config, std::move(launcher))
        {}

        void initialize() override
        {
            this->sendInitializeCommand("test");
        }

        void reset() override
        {
            this->sendResetCommand();
        }

        void shutdown() override
        {
            this->sendShutdownCommand("test");
        }
};

class TestEngine
    : public EngineProtoWrapper
{
    public:

        TestEngine()
            : EngineProtoWrapper("TestEngine",
                               NRP_PLUGIN_INSTALL_DIR,
                               json::array({"EngineTest"}))
        {

        }

        virtual ~TestEngine() override = default;

        void initialize(const nlohmann::json &/*data*/) override
        {
            specialBehaviour();

            if(_doThrow)
            {
                _doThrow = false;
                throw std::runtime_error("Init failed");
            }
        }

        void reset() override
        {
            specialBehaviour();

            this->resetEngineTime();
        }

        void shutdown() override
        {
            specialBehaviour();

            if(_doThrow)
            {
                _doThrow = false;
                throw std::runtime_error("Shutdown failed");
            }
        }

        void timeoutOnNextCommand(int sleepTimeMs = 1500)
        {
            this->_sleepTimeMs = sleepTimeMs;
        }

        void throwOnNextCommand()
        {
            _doThrow = true;
        }

        SimulationTime runLoopStep(const SimulationTime timeStep) override
        {
            specialBehaviour();

            _time += timeStep;

            return _time;
        }

        void resetEngineTime()
        {
            this->_time = SimulationTime::zero();
        }

        SimulationTime getEngineTime()
        {
            return this->_time;
        }

        virtual bool initRunFlag() const override { return true; }

        virtual bool shutdownFlag() const override { return false; }

    private:

        void specialBehaviour()
        {
            if(this->_sleepTimeMs != 0)
            {
                testSleep(this->_sleepTimeMs);
                this->_sleepTimeMs = 0;
            }
        }

        SimulationTime _time = SimulationTime::zero();
        int            _sleepTimeMs = 0;
        bool _doThrow = false;
};

TEST(EngineGrpc, Connection)
{
    nlohmann::json config;
    config["EngineName"] = "engine";
    config["EngineType"] = "test_engine_grpc";
    config["ProtobufPluginsPath"] = NRP_PLUGIN_INSTALL_DIR;
    config["ProtobufPackages"] = json::array({"EngineTest"});

    // No need to be concerned about deleting this pointer, EngineGrpcServer takes care of that
    auto engineWrapper = new TestEngine();
    TestEngineGrpcClient client(config, ProcessLauncherInterface::unique_ptr(new ProcessLauncherBasic()));
    EngineGrpcServer server(client.serverAddress(), engineWrapper);

    // Should throw, because no server is running

    ASSERT_ANY_THROW(client.connectToServer());

    server.startServer();
    client.connectToServer();
    server.shutdownServer();
}

TEST(EngineGrpc, InitCommand)
{
    nlohmann::json config;
    config["EngineName"] = "engine";
    config["EngineType"] = "test_engine_grpc";
    config["ProtobufPluginsPath"] = NRP_PLUGIN_INSTALL_DIR;
    config["ProtobufPackages"] = json::array({"EngineTest"});

    // No need to be concerned about deleting this pointer, EngineGrpcServer takes care of that
    auto engineWrapper = new TestEngine();
    TestEngineGrpcClient client(config, ProcessLauncherInterface::unique_ptr(new ProcessLauncherBasic()));
    EngineGrpcServer server(client.serverAddress(), engineWrapper);

    nlohmann::json jsonMessage;
    jsonMessage["init"]    = true;

    // The gRPC server isn't running, so the init command should fail

    ASSERT_THROW(client.sendInitializeCommand(jsonMessage), std::runtime_error);

    // Start the server and send the init command. It should succeed

    server.startServer();
    // TODO Investigate why this is needed. It seems to be caused by the previous call to sendInitCommand function
    testSleep(1500);
    client.sendInitializeCommand(jsonMessage);

    // Force the server to return an error from the rpc
    // Check if the client receives an error response on command handling failure

    engineWrapper->throwOnNextCommand();
    ASSERT_THROW(client.sendInitializeCommand(jsonMessage), std::runtime_error);
}

TEST(EngineGrpc, InitCommandTimeout)
{
    nlohmann::json config;
    config["EngineName"] = "engine";
    config["EngineType"] = "test_engine_grpc";
    config["EngineCommandTimeout"] = 1;
    config["ProtobufPluginsPath"] = NRP_PLUGIN_INSTALL_DIR;
    config["ProtobufPackages"] = json::array({"EngineTest"});

    // No need to be concerned about deleting this pointer, EngineGrpcServer takes care of that
    auto engineWrapper = new TestEngine();
    TestEngineGrpcClient client(config, ProcessLauncherInterface::unique_ptr(new ProcessLauncherBasic()));
    EngineGrpcServer server(client.serverAddress(), engineWrapper);

    nlohmann::json jsonMessage;
    jsonMessage["init"]    = true;

    // Test init command timeout

    server.startServer();
    engineWrapper->timeoutOnNextCommand();
    ASSERT_THROW(client.sendInitializeCommand(jsonMessage), std::runtime_error);
}

TEST(EngineGrpc, ShutdownCommand)
{
    nlohmann::json config;
    config["EngineName"] = "engine";
    config["EngineType"] = "test_engine_grpc";
    config["ProtobufPluginsPath"] = NRP_PLUGIN_INSTALL_DIR;
    config["ProtobufPackages"] = json::array({"EngineTest"});

    // No need to be concerned about deleting this pointer, EngineGrpcServer takes care of that
    auto engineWrapper = new TestEngine();
    TestEngineGrpcClient client(config, ProcessLauncherInterface::unique_ptr(new ProcessLauncherBasic()));
    EngineGrpcServer server(client.serverAddress(), engineWrapper);
    
    nlohmann::json jsonMessage;
    jsonMessage["shutdown"] = true;

    // The gRPC server isn't running, so the shutdown command should fail

    ASSERT_THROW(client.sendShutdownCommand(jsonMessage), std::runtime_error);

    // Start the server and send the shutdown command. It should succeed

    server.startServer();
    // TODO Investigate why this is needed. It seems to be caused by the previous call to sendInitCommand function
    testSleep(1500);
    ASSERT_NO_THROW(client.sendShutdownCommand(jsonMessage));

    // Force the server to return an error from the rpc
    // Check if the client receives an error response on command handling failure

    engineWrapper->throwOnNextCommand();
    ASSERT_THROW(client.sendShutdownCommand(jsonMessage), std::runtime_error);
}

TEST(EngineGrpc, ShutdownCommandTimeout)
{
    nlohmann::json config;
    config["EngineName"] = "engine";
    config["EngineType"] = "test_engine_grpc";
    config["EngineCommandTimeout"] = 1;
    config["ProtobufPluginsPath"] = NRP_PLUGIN_INSTALL_DIR;
    config["ProtobufPackages"] = json::array({"EngineTest"});

    // No need to be concerned about deleting this pointer, EngineGrpcServer takes care of that
    auto engineWrapper = new TestEngine();
    TestEngineGrpcClient client(config, ProcessLauncherInterface::unique_ptr(new ProcessLauncherBasic()));
    EngineGrpcServer server(client.serverAddress(), engineWrapper);

    nlohmann::json jsonMessage;
    jsonMessage["shutdown"] = true;

    // Test shutdown command timeout

    server.startServer();
    engineWrapper->timeoutOnNextCommand();
    ASSERT_THROW(client.sendShutdownCommand(jsonMessage), std::runtime_error);
}

static SimulationTime floatToSimulationTime(float time)
{
    return toSimulationTime<float, std::ratio<1>>(time);
}

TEST(EngineGrpc, RunLoopStepCommand)
{
    nlohmann::json config;
    config["EngineName"] = "engine";
    config["EngineType"] = "test_engine_grpc";
    config["ProtobufPluginsPath"] = NRP_PLUGIN_INSTALL_DIR;
    config["ProtobufPackages"] = json::array({"EngineTest"});

    // No need to be concerned about deleting this pointer, EngineGrpcServer takes care of that
    auto engineWrapper = new TestEngine();
    TestEngineGrpcClient client(config, ProcessLauncherInterface::unique_ptr(new ProcessLauncherBasic()));
    EngineGrpcServer server(client.serverAddress(), engineWrapper);

    // The gRPC server isn't running, so the runLoopStep command should fail

    SimulationTime timeStep = floatToSimulationTime(0.1f);
    ASSERT_THROW(client.runLoopStepCallback(timeStep), std::runtime_error);

    server.startServer();

    // Engine time should never be smaller than 0

    engineWrapper->resetEngineTime();
    timeStep = floatToSimulationTime(-0.1f);
    ASSERT_THROW(client.runLoopStepCallback(timeStep), std::runtime_error);

    // Normal loop execution, the command should return engine time

    engineWrapper->resetEngineTime();
    timeStep = floatToSimulationTime(1.0f);
    ASSERT_NEAR(client.runLoopStepCallback(timeStep).count(), timeStep.count(), 0.0001);

    // Try to go back in time. The client should raise an error when engine time is decreasing

    engineWrapper->resetEngineTime();
    timeStep = floatToSimulationTime(2.0f);
    ASSERT_NO_THROW(client.runLoopStepCallback(timeStep));
    timeStep = floatToSimulationTime(-1.0f);
    ASSERT_THROW(client.runLoopStepCallback(timeStep), std::runtime_error);

    // TODO Add test for failure on server side
}

TEST(EngineGrpc, runLoopStepCommandTimeout)
{
    nlohmann::json config;
    config["EngineName"] = "engine";
    config["EngineType"] = "test_engine_grpc";
    config["ProtobufPluginsPath"] = NRP_PLUGIN_INSTALL_DIR;
    config["ProtobufPackages"] = json::array({"EngineTest"});
    config["EngineCommandTimeout"] = 1;

    // No need to be concerned about deleting this pointer, EngineGrpcServer takes care of that
    auto engineWrapper = new TestEngine();
    TestEngineGrpcClient client(config, ProcessLauncherInterface::unique_ptr(new ProcessLauncherBasic()));
    EngineGrpcServer server(client.serverAddress(), engineWrapper);

    // Test runLoopStep command timeout

    server.startServer();
    engineWrapper->timeoutOnNextCommand();
    SimulationTime timeStep = floatToSimulationTime(2.0f);
    ASSERT_THROW(client.runLoopStepCallback(timeStep), std::runtime_error);
}

TEST(EngineGrpc, ResetCommand)
{
    nlohmann::json config;
    config["EngineName"] = "engine";
    config["EngineType"] = "test_engine_grpc";
    config["ProtobufPluginsPath"] = NRP_PLUGIN_INSTALL_DIR;
    config["ProtobufPackages"] = json::array({"EngineTest"});

    // No need to be concerned about deleting this pointer, EngineGrpcServer takes care of that
    auto engineWrapper = new TestEngine();
    TestEngineGrpcClient client(config, ProcessLauncherInterface::unique_ptr(new ProcessLauncherBasic()));
    EngineGrpcServer server(client.serverAddress(), engineWrapper);

    nlohmann::json jsonMessage;
    jsonMessage["init"]    = true;

    // The gRPC server isn't running, so the reset command should fail

    ASSERT_THROW(client.sendResetCommand(), std::runtime_error);

    // Start the server and send the reset command. It should succeed

    server.startServer();
    // TODO Investigate why this is needed. It seems to be caused by the previous call to sendInitCommand function
    testSleep(1500);
    ASSERT_NO_THROW(client.sendResetCommand());

    // Normal loop execution, the reset should return time to zero

    SimulationTime timeStep = floatToSimulationTime(0.1f);
    ASSERT_NO_THROW(client.runLoopStepCallback(timeStep));
    ASSERT_NO_THROW(client.sendResetCommand());
    ASSERT_EQ(client.getEngineTime().count(), 0);
}

TEST(EngineGrpc, ResetCommandTimeout)
{
    nlohmann::json config;
    config["EngineName"] = "engine";
    config["EngineType"] = "test_engine_grpc";
    config["ProtobufPluginsPath"] = NRP_PLUGIN_INSTALL_DIR;
    config["ProtobufPackages"] = json::array({"EngineTest"});
    config["EngineCommandTimeout"] = 1;

    // No need to be concerned about deleting this pointer, EngineGrpcServer takes care of that
    auto engineWrapper = new TestEngine();
    TestEngineGrpcClient client(config, ProcessLauncherInterface::unique_ptr(new ProcessLauncherBasic()));
    EngineGrpcServer server(client.serverAddress(), engineWrapper);

    // Test runLoopStep command timeout

    server.startServer();
    engineWrapper->timeoutOnNextCommand();
    ASSERT_THROW(client.sendResetCommand(), std::runtime_error);
}

TEST(EngineGrpc, RegisterDataPacks)
{
    // No need to be concern about deleting this pointer, EngineGrpcServer takes care of that
    auto engineWrapper = new TestEngine();
    EngineGrpcServer server("localhost:9004", engineWrapper);
    TestGrpcDataPackController *dev1 = nullptr;

    ASSERT_EQ(engineWrapper->getNumRegisteredDataPacks(), 0);
    engineWrapper->registerDataPack("dev1", dev1);
    ASSERT_EQ(engineWrapper->getNumRegisteredDataPacks(), 1);
}


/*!
 * \brief Helper function that generated a shared pointer to a DataPack with given parameters
 */
static std::shared_ptr<const DataPackInterface> generateDataPack(const std::string & name,
                                                           const std::string & engineName,
                                                           EngineTest::TestPayload * payload = nullptr)
{
    if(payload)
    {
        return std::shared_ptr<const DataPackInterface>(new DataPack<EngineTest::TestPayload>(name, engineName, payload));
    }
    else
    {
        return std::shared_ptr<const DataPackInterface>(new DataPack<EngineTest::TestPayload>(name, engineName));
    }
}


TEST(EngineGrpc, SetDataPackData)
{
    const std::string datapackName = "a";
    const std::string engineName = "c";

    nlohmann::json config;
    config["EngineName"] = engineName;
    config["EngineType"] = "test_engine_grpc";
    config["ProtobufPluginsPath"] = NRP_PLUGIN_INSTALL_DIR;
    config["ProtobufPackages"] = json::array({"EngineTest"});

    // No need to be concerned about deleting this pointer, EngineGrpcServer takes care of that
    auto engineWrapper = new TestEngine();
    TestEngineGrpcClient client(config, ProcessLauncherInterface::unique_ptr(new ProcessLauncherBasic()));
    EngineGrpcServer server(client.serverAddress(), engineWrapper);

    datapacks_set_t input_datapacks;

    std::shared_ptr<TestGrpcDataPackController> datapackController(new TestGrpcDataPackController()); // Server side
    engineWrapper->registerDataPack(datapackName, datapackController.get());

    auto dataPack1 = generateDataPack(datapackName, engineName); // Client side
    input_datapacks.insert(dataPack1);

    // The gRPC server isn't running, so the sendDataPacksToEngine command should fail
    ASSERT_THROW(client.sendDataPacksToEngine(input_datapacks), NRPException::exception);

    // Starts the Engine
    server.startServer();
    testSleep(1500);

    auto d = new EngineTest::TestPayload();
    d->set_integer(111);
    dataPack1 = generateDataPack(datapackName, engineName, d);

    input_datapacks.clear();
    input_datapacks.insert(dataPack1);

    // Normal command execution
    client.sendDataPacksToEngine(input_datapacks);
    d = dynamic_cast<EngineTest::TestPayload *>(datapackController->getDataPackInformation());

    ASSERT_EQ(d->integer(), 111);

    // Test setting data on a datapack that wasn't registered in the engine server
    const std::string datapackName2 = "b";
    auto dataPack2 = generateDataPack(datapackName2, engineName);
    input_datapacks.clear();
    input_datapacks.insert(dataPack2);

    ASSERT_THROW(client.sendDataPacksToEngine(input_datapacks), NRPException::exception);
    // TODO Add test for setData timeout
}

TEST(EngineGrpc, GetDataPackData)
{
    const std::string datapackName = "a";
    const std::string datapackType = "b";
    const std::string engineName = "c";

    nlohmann::json config;
    config["EngineName"] = engineName;
    config["EngineType"] = "test_engine_grpc";
    config["ProtobufPluginsPath"] = NRP_PLUGIN_INSTALL_DIR;
    config["ProtobufPackages"] = json::array({"EngineTest"});

    // No need to be concerned about deleting this pointer, EngineGrpcServer takes care of that
    auto engineWrapper = new TestEngine();
    TestEngineGrpcClient client(config, ProcessLauncherInterface::unique_ptr(new ProcessLauncherBasic()));
    EngineGrpcServer server(client.serverAddress(), engineWrapper);

    // Client sends a request to the server

    datapacks_set_t input_datapacks;

    DataPackIdentifier         devId(datapackName, engineName, datapackType);
    auto dataPack1 = generateDataPack(datapackName, engineName);
    std::shared_ptr<TestGrpcDataPackController> datapackController(new TestGrpcDataPackController()); // Server side

    engineWrapper->registerDataPack(datapackName, datapackController.get());

    input_datapacks.insert(dataPack1);

    datapack_identifiers_set_t datapackIdentifiers;
    datapackIdentifiers.insert(devId);

    // The gRPC server isn't running, so the updateDataPacksFromEngine command should fail

    ASSERT_THROW(client.getDataPacksFromEngine(datapackIdentifiers), std::runtime_error);

    server.startServer();
    testSleep(1500);
    client.sendDataPacksToEngine(input_datapacks);

    // Return an empty datapack from the server
    // It should be inserted into the engines cache, but should be marked as empty

    datapackController->triggerEmptyDataPackReturn(true);

    auto output = client.getDataPacksFromEngine(datapackIdentifiers);

    ASSERT_EQ(output.size(), 1);
    ASSERT_EQ(output.begin()->get()->name(),       datapackName);
    ASSERT_EQ(output.begin()->get()->engineName(), engineName);
    ASSERT_EQ(output.begin()->get()->isEmpty(),    true);

    datapackController->triggerEmptyDataPackReturn(false);

    // Normal command execution
    // Engine cache should be updated with a non-empty datapack

    output = client.getDataPacksFromEngine(datapackIdentifiers);

    ASSERT_EQ(output.size(), 1);
    ASSERT_EQ(output.begin()->get()->name(),       datapackName);
    ASSERT_EQ(output.begin()->get()->type(),       dataPack1->type());
    ASSERT_EQ(output.begin()->get()->engineName(), engineName);
    ASSERT_EQ(output.begin()->get()->isEmpty(),    false);

    // Trigger return of an empty datapack again

    datapackController->triggerEmptyDataPackReturn(true);

    output = client.getDataPacksFromEngine(datapackIdentifiers);

    ASSERT_EQ(output.size(), 1);
    ASSERT_EQ(output.begin()->get()->isEmpty(), true);

    datapackController->triggerEmptyDataPackReturn(false);

    // Test requesting a datapack that wasn't registered in the engine server

    const std::string datapackName2 = "b";

    DataPackIdentifier         devId2(datapackName2, engineName, datapackType);
    datapackIdentifiers.insert(devId2);

    ASSERT_THROW(client.getDataPacksFromEngine(datapackIdentifiers), std::runtime_error);
    // TODO Add test for getData timeout
}

TEST(EngineGrpc, GetDataPackData2)
{
    const std::string engineName = "c";

    const std::string datapackName1 = "a";
    const std::string datapackType1 = "test_type1";
    const std::string datapackName2 = "b";
    const std::string datapackType2 = "test_type2";

    nlohmann::json config;
    config["EngineName"] = engineName;
    config["EngineType"] = "test_engine_grpc";;
    config["ProtobufPluginsPath"] = NRP_PLUGIN_INSTALL_DIR;
    config["ProtobufPackages"] = json::array({"EngineTest"});

    // No need to be concerned about deleting this pointer, EngineGrpcServer takes care of that
    auto engineWrapper = new TestEngine();
    TestEngineGrpcClient client(config, ProcessLauncherInterface::unique_ptr(new ProcessLauncherBasic()));
    EngineGrpcServer server(client.serverAddress(), engineWrapper);

    // Client sends a request to the server

    datapacks_set_t input_datapacks;

    DataPackIdentifier         devId1(datapackName1, engineName, datapackType1);
    DataPackIdentifier         devId2(datapackName2, engineName, datapackType2);
    auto dataPack1 = generateDataPack(datapackName1, engineName);
    auto dataPack2 = generateDataPack(datapackName1, engineName);
    std::shared_ptr<TestGrpcDataPackController> datapackController1(new TestGrpcDataPackController()); // Server side
    std::shared_ptr<TestGrpcDataPackController> datapackController2(new TestGrpcDataPackController()); // Server side

    engineWrapper->registerDataPack(datapackName1, datapackController1.get());
    engineWrapper->registerDataPack(datapackName2, datapackController2.get());

    input_datapacks.insert(dataPack1);
    input_datapacks.insert(dataPack2);

    server.startServer();
    client.sendDataPacksToEngine(input_datapacks);

    datapack_identifiers_set_t datapackIdentifiers;
    datapackIdentifiers.insert(devId1);
    datapackIdentifiers.insert(devId2);

    /*const auto output = client.getDataPacksFromEngine(datapackIdentifiers);

    ASSERT_EQ(output.size(), 2);
    ASSERT_EQ(output.at(0)->engineName(), engineName);
    ASSERT_EQ(output.at(1)->engineName(), engineName);
    ASSERT_EQ(output.at(0)->type(), dev1.type());
    ASSERT_EQ(output.at(1)->type(), dev1.type());

    if(output.at(0)->name().compare(datapackName1) == 0)
    {
        ASSERT_EQ(output.at(0)->name(), datapackName1);
        ASSERT_EQ(output.at(1)->name(), datapackName2);
    }
    else
    {
        ASSERT_EQ(output.at(0)->name(), datapackName2);
        ASSERT_EQ(output.at(1)->name(), datapackName1);
    }*/
}

// EOF
