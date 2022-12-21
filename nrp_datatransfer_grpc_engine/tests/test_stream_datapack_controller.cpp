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

#include "datatransfer_grpc_engine/engine_server/stream_datapack_controller.h"

#include "tests/test_env_cmake.h"

TEST(TestDatatransferGrpcEngine, StreamDataPackController)
{
    // Mock NRPMQTTClient with connected status
    auto nrpMQTTClientMock = std::make_shared<NRPMQTTClientMock>(true);

    std::string dataPackName = "datapack1";
    
    // Expected to announce the topic data address in StreamDataPackController constructor
    EXPECT_CALL(*nrpMQTTClientMock, publish("nrp/0/data", "nrp/0/data/" + dataPackName))
        .Times(1);

    // Launch StreamDataPackController
    std::vector<std::unique_ptr<protobuf_ops::NRPProtobufOpsIface>> dummyOps;
    StreamDataPackController controller(dataPackName, "datatransfer_engine", dummyOps, nrpMQTTClientMock, "nrp/0");

    // Create a simple DataPack load
    Dump::String data;
    data.set_string_stream("test");

    // The expected behavior is to send message and type to corresponding topics
    EXPECT_CALL(*nrpMQTTClientMock, publish("nrp/0/data/" + dataPackName, testing::_))
        .Times(1);
    EXPECT_CALL(*nrpMQTTClientMock, publish("nrp/0/data/" + dataPackName + "/type", "Dump.String"))
        .Times(1);

    ASSERT_NO_THROW(controller.handleDataPackData(data));
}
