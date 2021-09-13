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

#include "nrp_python_json_engine/engine_server/python_engine_json_device_controller.h"
#include "nrp_json_engine_protocol/device_interfaces/json_converter.h"

#include "python/python_module.h"

#include <iostream>

PythonEngineJSONDeviceController::PythonEngineJSONDeviceController(const DeviceIdentifier & devID)
    : JsonDeviceController(devID)
{}

void PythonEngineJSONDeviceController::handleDeviceData(const nlohmann::json &data)
{
	this->_deviceData = boost::python::object(boost::python::handle<>(json_converter::convertJsonToPyObject(data["data"])));
}

nlohmann::json * PythonEngineJSONDeviceController::getDeviceInformation()
{
	*(getCachedData()) = json_converter::convertPyObjectToJson(this->_deviceData.ptr());

	return &(this->_data);
}

boost::python::object & PythonEngineJSONDeviceController::data()
{
	return this->_deviceData;
}

boost::python::object PythonEngineJSONDeviceController::data() const
{
	return this->_deviceData;
}
