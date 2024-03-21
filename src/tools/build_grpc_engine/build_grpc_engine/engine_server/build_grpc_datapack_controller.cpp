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

#include "build_grpc_engine/engine_server/build_grpc_datapack_controller.h"

BuildGrpcDataPackController::BuildGrpcDataPackController(const std::string & datapackName,
                                                         const std::string & engineName)
    : _datapackName(datapackName), _engineName(engineName)
{

}

void BuildGrpcDataPackController::handleDataPackData(const google::protobuf::Message &data)
{
    // In order to access the data from the message, you need to cast it to the proper type

    std::cout << "Message Received: " << dynamic_cast<const EngineTest::TestPayload &>(data).integer() << std::endl;
}

google::protobuf::Message * BuildGrpcDataPackController::getDataPackInformation()
{
    static int data = 0;

    // Create a new protobuf message and fill it

    auto payload = new EngineTest::TestPayload();
    payload->set_integer(data++);

    // Return the protobuf message

    return payload;
}

// EOF
