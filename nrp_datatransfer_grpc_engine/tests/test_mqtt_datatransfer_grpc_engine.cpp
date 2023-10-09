//
// NRP Core - Backend infrastructure to synchronize simulations
//
// Copyright 2020-2022 NRP Team
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
#include <gmock/gmock.h>

#include "tests/nrp_mqtt_client_mock.h"

#include "datatransfer_grpc_engine/config/datatransfer_config.h"
#include "datatransfer_grpc_engine/config/cmake_constants.h"
#include "datatransfer_grpc_engine/engine_server/datatransfer_grpc_server.h"

#include "nrp_general_library/config/cmake_constants.h"
#include "nrp_general_library/utils/json_schema_utils.h"

#include "tests/test_env_cmake.h"

#define MQTT_WELCOME "nrp_simulation/0/welcome"
#define MQTT_DATA "nrp_simulation/0/data"

TEST(TestDatatransferGrpcEngine, ServerConnectedMock)
{
    // Engine config
    auto simConfigFile = std::fstream(TEST_ENGINE_SIMPLE_CONFIG_FILE, std::ios::in);
    nlohmann::json engine_config(nlohmann::json::parse(simConfigFile));
    json_utils::validateJson(engine_config, "json://nrp-core/engines/engine_datatransfer.json#/engine_datatransfer_base");

    // Launch DataTransferEngine
    DataTransferEngine engine(engine_config["EngineName"],
                                  NRP_PLUGIN_INSTALL_DIR, engine_config["ProtobufPackages"]);

    // Create a client with "connected" status
    auto nrpMQTTClientMock = std::make_shared<NRPMQTTClientMock>(true);
    nrpMQTTClientMock->DelegateToFake();

    // The expected calls
    EXPECT_CALL(*nrpMQTTClientMock, isConnected())
            .Times(testing::AtLeast(3));
    EXPECT_CALL(*nrpMQTTClientMock, publish(MQTT_WELCOME, "NRP-core is connected!", testing::_))
            .Times(1);
        // Check that the topics list is cleared
    EXPECT_CALL(*nrpMQTTClientMock, publish(MQTT_DATA, "[]", testing::_))
            .Times(1);
    EXPECT_CALL(*nrpMQTTClientMock, publish(MQTT_WELCOME, "Bye! NRP-core is disconnecting!", testing::_))
            .Times(1);
    // check that the data topics list is appended and updated
    nlohmann::json jsonList = nlohmann::json::array();
    for (size_t i = 0; i < engine_config["dumps"].size(); i++){
        nlohmann::json dump = engine_config["dumps"].at(i);
        std::string topic = std::string(MQTT_DATA) + std::string("/") + dump["name"].get<std::string>();
        jsonList.push_back(
                    {{"topic", topic},
                     {"type", ""}});
            EXPECT_CALL(*nrpMQTTClientMock,
                        publish(
                            MQTT_DATA,
                            jsonList.dump(),
                            testing::_))
                .Times(1);
    }
    for (size_t i = 0; i < engine_config["dumps"].size(); i++){
        jsonList.at(i)["type"] = "Data.Type";
        EXPECT_CALL(*nrpMQTTClientMock,
                    publish(
                        MQTT_DATA,
                        jsonList.dump(),
                        testing::_))
            .Times(1);
    }
    EXPECT_CALL(*nrpMQTTClientMock, disconnect())
            .Times(1);
    EXPECT_CALL(*nrpMQTTClientMock, clearRetained())
            .Times(1);

    // The expected calls (above) are in these operations
    ASSERT_NO_THROW(engine.setNRPMQTTClient(nrpMQTTClientMock));
    ASSERT_NO_THROW(engine.initialize(engine_config));
    // Update topics list
    for (size_t i = 0; i < engine_config["dumps"].size(); i++){
        nlohmann::json dump = engine_config["dumps"].at(i);
        std::string topic = std::string(MQTT_DATA) + std::string("/") + dump["name"].get<std::string>();
        engine.updateDataTopics(topic, "Data.Type");
    }
    ASSERT_NO_THROW(engine.shutdown());

    // TODO: The datapack controller is not destroyed and shared ptr survives
    testing::Mock::AllowLeak(nrpMQTTClientMock.get());
}


TEST(TestDatatransferGrpcEngine, ServerDisconnectedMock)
{
    // Engine config
    auto simConfigFile = std::fstream(TEST_ENGINE_SIMPLE_CONFIG_FILE, std::ios::in);
    nlohmann::json engine_config(nlohmann::json::parse(simConfigFile));
    json_utils::validateJson(engine_config, "json://nrp-core/engines/engine_datatransfer.json#/engine_datatransfer_base");

    // Launch DataTransferEngine
    DataTransferEngine engine(engine_config["EngineName"],
                                  NRP_PLUGIN_INSTALL_DIR, engine_config["ProtobufPackages"]);

    // Create a client with "disconnected" status
    auto nrpMQTTClientMock = std::make_shared<NRPMQTTClientMock>(false);
    nrpMQTTClientMock->DelegateToFake();

    // The expected calls (we do not expect anything to be published)
    EXPECT_CALL(*nrpMQTTClientMock, isConnected())
            .Times(testing::AtLeast(2));
    EXPECT_CALL(*nrpMQTTClientMock, publish(testing::_, testing::_, testing::_))
            .Times(0);
    EXPECT_CALL(*nrpMQTTClientMock, disconnect())
            .Times(0);
    EXPECT_CALL(*nrpMQTTClientMock, clearRetained())
            .Times(0);

    ASSERT_NO_THROW(engine.setNRPMQTTClient(nrpMQTTClientMock));
    ASSERT_NO_THROW(engine.initialize(engine_config));
    ASSERT_NO_THROW(engine.shutdown());
}

TEST(TestDatatransferGrpcEngine, ServerBroker)
{
    // Engine config
    auto simConfigFile = std::fstream(TEST_ENGINE_SIMPLE_CONFIG_FILE, std::ios::in);
    nlohmann::json engine_config(nlohmann::json::parse(simConfigFile));
    json_utils::validateJson(engine_config, "json://nrp-core/engines/engine_datatransfer.json#/engine_datatransfer_base");

    // MQTT client config
    nlohmann::json mqtt_config;
    mqtt_config["MQTTBroker"] = "localhost:1883";
    mqtt_config["ClientName"] = "datatransfer_engine";

    // Launch DataTransferEngine
    DataTransferEngine engine(engine_config["EngineName"],
                                  NRP_PLUGIN_INSTALL_DIR, engine_config["ProtobufPackages"]);

    // We can try connection to non-existent real broker, but it may fail if broker exists
    auto nrpMQTTClientMock = std::make_shared<NRPMQTTClientMock>(mqtt_config);

    // The expected calls as for disconnected client
    EXPECT_CALL(*nrpMQTTClientMock, isConnected())
            .Times(testing::AtLeast(2));
    EXPECT_CALL(*nrpMQTTClientMock, publish(testing::_, testing::_, testing::_))
            .Times(0);
    EXPECT_CALL(*nrpMQTTClientMock, disconnect())
            .Times(0);
    EXPECT_CALL(*nrpMQTTClientMock, clearRetained())
            .Times(0);

    ASSERT_NO_THROW(engine.setNRPMQTTClient(nrpMQTTClientMock));
    ASSERT_NO_THROW(engine.initialize(engine_config));
    ASSERT_NO_THROW(engine.shutdown());
}
