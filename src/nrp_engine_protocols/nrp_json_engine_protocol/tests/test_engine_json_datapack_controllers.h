/* * NRP Core - Backend infrastructure to synchronize simulations
 *
 * Copyright 2020-2023 NRP Team
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * This project has received funding from the European Union’s Horizon 2020
 * Framework Programme for Research and Innovation under the Specific Grant
 * Agreement No. 945539 (Human Brain Project SGA3).
 */

#ifndef TEST_ENGINE_JSON_DATAPACK_INTERFACE_H
#define TEST_ENGINE_JSON_DATAPACK_INTERFACE_H

#include "nrp_general_library/datapack_interface/datapack.h"
#include "nrp_json_engine_protocol/engine_server/json_datapack_controller.h"

struct TestJSONDataPackController
        : public JsonDataPackController
{
    public:
        TestJSONDataPackController(const DataPackIdentifier & devID) :
            JsonDataPackController(devID)
        {}

        void handleDataPackData(const nlohmann::json &data) override
        {
            setCachedData(data);
        }

        constexpr const auto &data() const
        {
            return this->_data;
        }

        nlohmann::json *getDataPackInformation() override
        {
            return &this->_data;
        }
};


#endif // TEST_ENGINE_JSON_DATAPACK_INTERFACE_H
