/* * NRP Core - Backend infrastructure to synchronize simulations
 *
 * Copyright 2020-2021 NRP Team
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

#ifndef PYTHON_ENGINE_JSON_DATAPACK_CONTROLLER_H
#define PYTHON_ENGINE_JSON_DATAPACK_CONTROLLER_H

#include "nrp_json_engine_protocol/engine_server/json_datapack_controller.h"
#include "nrp_general_library/datapack_interface/datapack_interface.h"

#include <boost/python.hpp>
#include <nlohmann/json.hpp>


class PythonEngineJSONDataPackController
        : public JsonDataPackController
{
    public:
        PythonEngineJSONDataPackController(const DataPackIdentifier & devID);

        virtual ~PythonEngineJSONDataPackController() override = default;

        void handleDataPackData(const nlohmann::json &data) override;
        virtual nlohmann::json * getDataPackInformation() override;

        /*!
         * \brief Get python object referenced by this controller
         */
        boost::python::object & data();

        boost::python::object data() const;

    protected:
        /*!
         * \brief DataPack Data. Used to convert to/from JSON and python dict
         */
        boost::python::object _datapackData;
};

#endif // PYTHON_ENGINE_JSON_DATAPACK_CONTROLLER_H
