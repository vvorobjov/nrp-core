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

#ifndef BUILD_JSON_DATAPACK_CONTROLLER_SERVER_H
#define BUILD_JSON_DATAPACK_CONTROLLER_SERVER_H

#include "nrp_json_engine_protocol/engine_server/json_datapack_controller.h"

class BuildJSONDataPackController
    : public JsonDataPackController
{
    public:
        BuildJSONDataPackController(const std::string & datapackName,
                                    const std::string & engineName);

        /*!
         * \brief Processes data coming from the transceiver function
         *
         * \param[in] data The latest data from the transceiver function
         */
        void handleDataPackData(const nlohmann::json &data) override;

        /*!
         * \brief Returns the newest simulation data
         *
         * The data will be passed to the engine client through REST.
         * There it will be wrapped in a datapack object and passed to the transceiver functions.
         *
         * \return Pointer to the latest simulation data. The returned pointer should point
         *         to the cachedData object. nullptr can be returned when no new data is available.
         */
        nlohmann::json * getDataPackInformation() override;
};

#endif // BUILD_JSON_DATAPACK_CONTROLLER_SERVER_H

// EOF
