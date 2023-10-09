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

// Mock class
class MockUpdateDataTopics {
public:
    MOCK_METHOD2(call, void(const std::string&, const std::string&));
};

TEST(TestDatatransferGrpcEngine, StreamDataPackController)
{
    // Mock NRPMQTTClient with connected status
    auto nrpMQTTClientMock = std::make_shared<NRPMQTTClientMock>(true);

    MockUpdateDataTopics mockUpdateDataTopics;

    ::testing::InSequence sequence;

    std::string dataPackName = "datapack1";

    // The expected behavior is to update the list of topics with empty topic type
    EXPECT_CALL(mockUpdateDataTopics, call("nrp_simulation/0/data/" + dataPackName, ""))
        .Times(1);

    // Launch StreamDataPackController
    std::vector<std::unique_ptr<protobuf_ops::NRPProtobufOpsIface>> dummyOps;
    StreamDataPackController controller(dataPackName, "datatransfer_engine", dummyOps, nrpMQTTClientMock, "nrp_simulation/0", std::bind(&MockUpdateDataTopics::call, &mockUpdateDataTopics, std::placeholders::_1, std::placeholders::_2));

    // Create a simple DataPack load
    Dump::String data;
    data.set_string_stream("test");

    // The expected behavior is to update the list of topics
    EXPECT_CALL(mockUpdateDataTopics, call("nrp_simulation/0/data/" + dataPackName, "Dump.String"))
        .Times(1);

    ASSERT_NO_THROW(controller.handleDataPackData(data));
}
