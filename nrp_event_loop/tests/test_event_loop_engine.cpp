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

#include <functional>
#include <mutex>
#include <chrono>
#include "nlohmann/json.hpp"

#include <gtest/gtest.h>

#include "nrp_general_library/config/cmake_constants.h"
#include "nrp_event_loop/event_loop/event_loop_engine.h"
#include "nrp_protobuf/enginetest.pb.h"

using namespace std::chrono_literals;

class TestGrpcDataPackController
        : public DataPackController<google::protobuf::Message>
{
public:

    TestGrpcDataPackController():
    _data(new EngineTest::TestPayload())
    { _data->set_str("initial_value"); }

    virtual void handleDataPackData(const google::protobuf::Message &data) override
    {
        // throws bad_cast
        const auto &j = dynamic_cast<const EngineTest::TestPayload &>(data);
        _data->CopyFrom(j);
    }

    virtual google::protobuf::Message *getDataPackInformation() override
    {
        auto old_data = _data;
        _data = new EngineTest::TestPayload();
        _data->CopyFrom(*old_data);
        return old_data;
    }

private:
    EngineTest::TestPayload* _data;
};


class TestEngine
        : public EngineProtoWrapper
{
public:

    const std::string dpName = "test_datapack";

    TestEngine()
            : EngineProtoWrapper("TestEngine",
                                 NRP_PLUGIN_INSTALL_DIR,
                                 nlohmann::json::array({"EngineTest"}))
    {

    }

    virtual ~TestEngine() override = default;

    void initialize(const nlohmann::json &data) override
    {
        initCalls++;
        config = data;
        this->registerDataPack(dpName, new TestGrpcDataPackController());
    }

    void reset() override
    { }

    void shutdown() override
    { shutdownCalls++;}

    SimulationTime runLoopStep(const SimulationTime timeStep) override
    {
        runLoopCalls++;
        while(holdRun)
            std::this_thread::sleep_for(100ms);

        return timeStep;
    }

    virtual bool initRunFlag() const override { return true; }

    virtual bool shutdownFlag() const override { return false; }

    int initCalls = 0;
    int runLoopCalls = 0;
    int shutdownCalls = 0;
    nlohmann::json config;
    std::atomic<bool> holdRun = false;
};

//// EVENT LOOP ENGINE
TEST(EventLoop, EVENT_LOOP_ENGINE) {
    // Initialize
    auto engine = new TestEngine();
    nlohmann::json engineConfig;
    engineConfig["test"] = true;
    EventLoopEngine ele(10ms, 1ms, 2, true, engineConfig, engine);

    ASSERT_ANY_THROW(ele.initialize());
    ASSERT_EQ(engine->initCalls, 0);

    NRPMQTTProxy::resetInstance(nlohmann::json::object());
    NRPMQTTProxy::getInstance()._doBypassBroker = true;

    ASSERT_NO_THROW(ele.initialize());
    ASSERT_EQ(engine->initCalls, 1);
    ASSERT_TRUE(engine->config.contains("test") && engine->config.at("test").get<bool>());

    // RunLoop and mqtt topic callback
    const std::string dpSubTopic = engine->getEngineName() + "/set/" + engine->dpName;
    const std::string dpPubTopic = engine->getEngineName() + "/get/" + engine->dpName;

    std::string msg = "";
    NRPMQTTProxy::getInstance().subscribe(dpPubTopic, [&] (const std::string& msgStr) {
        EngineGrpc::DataPackMessage m;
        m.ParseFromString(msgStr);
        EngineTest::TestPayload d;
        m.data().UnpackTo(&d);
        msg = d.str();
    });

    ele.runLoop(10ms);
    ASSERT_EQ(engine->runLoopCalls, 1);
    ASSERT_EQ(msg, "initial_value");

    EngineGrpc::DataPackMessage m;
    EngineTest::TestPayload d;
    d.set_str("test_1_value");
    m.mutable_data()->PackFrom(d);
    ele.runLoopAsync(0ms);

    NRPMQTTProxy::getInstance().publish(dpSubTopic, "not a serialized datapack message");
    std::this_thread::sleep_for(1s);
    ASSERT_EQ(msg, "initial_value");
    NRPMQTTProxy::getInstance().publish(dpSubTopic, m.SerializeAsString());
    std::this_thread::sleep_for(1s);
    ASSERT_EQ(msg, "initial_value");
    m.mutable_datapackid()->set_datapackname(engine->dpName);
    NRPMQTTProxy::getInstance().publish(dpSubTopic, m.SerializeAsString());
    std::this_thread::sleep_for(1s);
    ASSERT_EQ(msg, "test_1_value");

    // storeCapacity and doProcessLast
    engine->holdRun = true;
    d.set_str("test_2_value");
    m.mutable_data()->PackFrom(d);
    NRPMQTTProxy::getInstance().publish(dpSubTopic, m.SerializeAsString());
    d.set_str("test_3_value");
    m.mutable_data()->PackFrom(d);
    NRPMQTTProxy::getInstance().publish(dpSubTopic, m.SerializeAsString());
    d.set_str("test_4_value");
    m.mutable_data()->PackFrom(d);
    NRPMQTTProxy::getInstance().publish(dpSubTopic, m.SerializeAsString());
    std::this_thread::sleep_for(1s);
    engine->holdRun = false;
    std::this_thread::sleep_for(1s);
    // Engine configured to process only last msg, but queue capacity is set to 2, so last published msg has been discarded
    ASSERT_EQ(msg, "test_3_value");

    ele.stopLoop();

    // shutdown
    ele.shutdown();
    ASSERT_EQ(engine->shutdownCalls, 1);
}

// EOF
