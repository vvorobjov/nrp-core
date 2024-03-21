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

#ifndef JSON_DATAPACK_CONTROLLER_H
#define JSON_DATAPACK_CONTROLLER_H

#include "nrp_general_library/engine_interfaces/datapack_controller.h"
#include "nrp_general_library/datapack_interface/datapack_interface.h"

#include "nrp_json_engine_protocol/datapack_interfaces/json_datapack.h"

#include <nlohmann/json.hpp>
#include <iostream>

/*!
 * @brief Base controller class for JSON datapacks
 *
 * The class provides helper methods and common members for controllers
 * that are used to handle JSON datapacks.
 */

// TODO It seems that the metadata stored in _datapackId and the _data members are
//      duplicated. Let's see if this can be improved.

class JsonDataPackController
    : public DataPackController<nlohmann::json>
{
    public:
        const nlohmann::json & getEmptyDataPack() const
        {
            return this->_emptyDataPack;
        }

    protected:

        /*!
         * @brief Constructor that should be called by the derived class
         */
        JsonDataPackController(const DataPackIdentifier & datapackId) :
            _datapackId(datapackId)
        {
            if(datapackId.Type != JsonDataPack::getType())
            {
                throw NRPException::logCreate("DataPack type \"" + datapackId.Type + "\" cannot be handled by JsonDataPackController");
            }

            this->_data[this->_datapackId.Name]["type"       ] = this->_datapackId.Type;
            this->_data[this->_datapackId.Name]["engine_name"] = this->_datapackId.EngineName;
            this->_data[this->_datapackId.Name]["data"       ] = nullptr;

            this->_emptyDataPack[this->_datapackId.Name]["type"       ] = this->_datapackId.Type;
            this->_emptyDataPack[this->_datapackId.Name]["engine_name"] = this->_datapackId.EngineName;
        }

        /*!
         * @brief Sets the "data" part of the cached JSON object
         */
        // TODO The data could be swapped, but the reference received from the caller is const.
        //      It would require modifying of the DataPackController interface.
        void setCachedData(const nlohmann::json &data)
        {
            // TODO Check name, type and engine name
            // TODO Format of the received JSON data is different than the data cached by the controller
            //      (the <datapack_name> label is missing). Try to change that.
            this->_data[this->_datapackId.Name]["data"] = data["data"];
        }

        /*!
         * @brief Returns reference to the "data" part of the cached JSON object
         */
        nlohmann::json * getCachedData()
        {
            return &this->_data[this->_datapackId.Name].at("data");
        }

        DataPackIdentifier _datapackId;

        /*!
         * @brief Cached incoming/outgoing data in JSON format
         *
         * The format of the data is the following:
         * { <datapack_name> : { type : <datapack_type>, engine_name : <engine_name>, data : <data> } }
         */
        nlohmann::json _data;
        nlohmann::json _emptyDataPack;
};

#endif // JSON_DATAPACK_CONTROLLER_H

// EOF
