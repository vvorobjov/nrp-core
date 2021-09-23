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

#include "${engine_name_lowercase}_json_engine/engine_server/${engine_name_lowercase}_json_device_controller.h"

${engine_name}JSONDeviceController::${engine_name}JSONDeviceController(const std::string & deviceName,
                                                         const std::string & engineName)
    : JsonDeviceController(DeviceIdentifier(deviceName, engineName, JsonDevice::getType()))
{

}

void ${engine_name}JSONDeviceController::handleDeviceData(const nlohmann::json &data)
{
    std::cout << "TF output data: " << data["data"]["test_string"] << std::endl;
    setCachedData(data);
}

nlohmann::json * ${engine_name}JSONDeviceController::getDeviceInformation()
{
    static int data = 0;

    auto cachedData = this->getCachedData();

    (*cachedData)["test_int"] = data++;

    return &(this->_data);
}

// EOF
