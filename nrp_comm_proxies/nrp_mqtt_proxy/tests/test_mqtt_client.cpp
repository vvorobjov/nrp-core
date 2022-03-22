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

#include "tests/test_env_cmake.h"


TEST(TestDatatransferGrpcEngine, MQTTClient)
{
    // MQTT client config
    nlohmann::json mqtt_config;
    mqtt_config["MQTTBroker"] = "localhost:1883";
    mqtt_config["ClientName"] = "datatransfer_engine";

    // Real client should be unable to connect
    NRPMQTTClient nrpMQTTClient(mqtt_config);
    EXPECT_FALSE(nrpMQTTClient.isConnected());

    // Mock client should be unable to connect with real client methods
    NRPMQTTClientMock nrpMQTTClientMock1(mqtt_config);
    EXPECT_FALSE(nrpMQTTClientMock1.isConnected());

    // Mock client should be able to connect with fake methods
    NRPMQTTClientMock nrpMQTTClientMock2(true);
    nrpMQTTClientMock2.DelegateToFake();
    EXPECT_TRUE(nrpMQTTClientMock2.isConnected());
    nrpMQTTClientMock2.disconnect();
    EXPECT_FALSE(nrpMQTTClientMock2.isConnected());

    // Mock client should be disconnected with fake methods if specified so
    NRPMQTTClientMock nrpMQTTClientMock3(false);
    nrpMQTTClientMock3.DelegateToFake();
    EXPECT_FALSE(nrpMQTTClientMock3.isConnected());
}
