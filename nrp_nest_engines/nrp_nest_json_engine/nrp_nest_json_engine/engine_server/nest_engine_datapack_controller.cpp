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

#include "nrp_nest_json_engine/engine_server/nest_engine_datapack_controller.h"

#include "nrp_json_engine_protocol/datapack_interfaces/json_converter.h"

#include "nrp_general_library/utils/nrp_exceptions.h"
#include "nrp_general_library/utils/python_error_handler.h"

NestEngineJSONDataPackController::NestEngineJSONDataPackController(const DataPackIdentifier & devID, boost::python::object nodeCollection, boost::python::dict nest)
    : JsonDataPackController(devID),
	  _nest(nest),
	  _nodeCollection(nodeCollection)
{

}

void NestEngineJSONDataPackController::handleDataPackData(const nlohmann::json &data)
{
	setCachedData(data);

	this->_nest["SetStatus"](this->_nodeCollection, boost::python::handle<>(json_converter::convertJsonToPyObject(*(getCachedData()))));
}

nlohmann::json *NestEngineJSONDataPackController::getDataPackInformation()
{
	// Get datapack status from Nest
	try
	{
		*(getCachedData()) = json_converter::convertPyObjectToJson(this->getStatusFromNest().ptr());
	}
	catch(boost::python::error_already_set &)
	{
		throw NRPException::logCreate("Failed to get Nest datapack status: " + handle_pyerror());
	}

	return &(this->_data);
}

void NestEngineJSONDataPackController::setNestID(boost::python::dict nest, boost::python::object nodeCollection)
{
	NRP_LOGGER_TRACE("{} called", __FUNCTION__);
	
	this->_nest = nest;
	this->_nodeCollection = nodeCollection;
}

boost::python::list NestEngineJSONDataPackController::getStatusFromNest()
{	return getStatusFromNest(this->_nest, this->_nodeCollection);	}

boost::python::list NestEngineJSONDataPackController::getStatusFromNest(boost::python::dict &nest, const boost::python::object &nodeCollection)
{	return boost::python::list(nest["GetStatus"](nodeCollection));	}
