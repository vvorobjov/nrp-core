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

#ifndef JSON_DEVICE_CONTROLLER_H
#define JSON_DEVICE_CONTROLLER_H

#include "nrp_general_library/engine_interfaces/device_controller.h"
#include "nrp_general_library/device_interface/device_interface.h"

#include "nrp_json_engine_protocol/device_interfaces/json_device.h"

#include <nlohmann/json.hpp>
#include <iostream>

/*!
 * @brief Base controller class for JSON devices
 *
 * The class provides helper methods and common members for controllers
 * that are used to handle JSON devices.
 */

// TODO It seems that the metadata stored in _deviceId and the _data members are
//      duplicated. Let's see if this can be improved.

class JsonDeviceController
    : public DeviceController<nlohmann::json>
{
    public:
        const nlohmann::json & getEmptyDevice() const
        {
            return this->_emptyDevice;
        }

    protected:

        /*!
         * @brief Constructor that should be called by the derived class
         */
        JsonDeviceController(const DeviceIdentifier & deviceId) :
            _deviceId(deviceId)
        {
            if(deviceId.Type != JsonDevice::getType())
            {
                throw NRPException::logCreate("Device type \"" + deviceId.Type + "\" cannot be handled by JsonDeviceController");
            }

            this->_data[this->_deviceId.Name]["type"       ] = this->_deviceId.Type;
            this->_data[this->_deviceId.Name]["engine_name"] = this->_deviceId.EngineName;
            this->_data[this->_deviceId.Name]["data"       ] = nullptr;

            this->_emptyDevice[this->_deviceId.Name]["type"       ] = this->_deviceId.Type;
            this->_emptyDevice[this->_deviceId.Name]["engine_name"] = this->_deviceId.EngineName;
        }

        /*!
         * @brief Sets the "data" part of the cached JSON object
         */
        // TODO The data could be swapped, but the reference received from the caller is const.
        //      It would require modifying of the DeviceController interface.
        void setCachedData(const nlohmann::json &data)
        {
            // TODO Check name, type and engine name
            // TODO Format of the received JSON data is different than the data cached by the controller
            //      (the <device_name> label is missing). Try to change that.
            this->_data[this->_deviceId.Name]["data"] = data["data"];
        }

        /*!
         * @brief Returns reference to the "data" part of the cached JSON object
         */
        nlohmann::json * getCachedData()
        {
            return &this->_data[this->_deviceId.Name].at("data");
        }

        DeviceIdentifier _deviceId;

        /*!
         * @brief Cached incoming/outgoing data in JSON format
         *
         * The format of the data is the following:
         * { <device_name> : { type : <device_type>, engine_name : <engine_name>, data : <data> } }
         */
        nlohmann::json _data;
        nlohmann::json _emptyDevice;
};

#endif // JSON_DEVICE_CONTROLLER_H

// EOF
