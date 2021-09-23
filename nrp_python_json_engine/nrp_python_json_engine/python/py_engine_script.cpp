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

#include "nrp_python_json_engine/python/py_engine_script.h"

#include "nrp_general_library/utils/nrp_exceptions.h"
#include "nrp_python_json_engine/engine_server/python_engine_json_datapack_controller.h"


PyEngineScript::~PyEngineScript()
{
	this->_pServer = nullptr;
}

void PyEngineScript::initialize()
{}

void PyEngineScript::shutdown()
{}

SimulationTime PyEngineScript::simTime() const
{	return this->_time;	}

void PyEngineScript::registerDataPack(std::string datapackName)
{
	//std::cout << "Registering datapack \"" + datapackName + "\"\n";
	assert(this->_pServer != nullptr);

	//std::cout << "Creating datapack controller for \"" + datapackName + "\"\n";
	PtrTemplates<PythonEngineJSONDataPackController>::shared_ptr
	        newController(new PythonEngineJSONDataPackController(JsonDataPack::createID(datapackName, "")));

	//std::cout << "Adding datapack controller for \"" + datapackName + "\"\n";
	this->_datapackControllers.push_back(newController);
	this->_nameDataPackMap.emplace(datapackName, &(newController->data()));

	//std::cout << "Adding datapack \"" + datapackName + "\" to server\n";
	this->_pServer->registerDataPackNoLock(datapackName, newController.get());

	//std::cout << "Finished registering datapack \"" + datapackName + "\"\n";
}

boost::python::object &PyEngineScript::getDataPack(const std::string &datapackName)
{
	auto devIt = this->_nameDataPackMap.find(datapackName);
	if(devIt == this->_nameDataPackMap.end())
		throw NRPException::logCreate("Could not find datapack with name \"" + datapackName + "\"");

	return *(devIt->second);
}

void PyEngineScript::setDataPack(const std::string &datapackName, boost::python::object data)
{	this->getDataPack(datapackName) = data;	}

void PyEngineScript::setPythonJSONServer(PythonJSONServer *pServer)
{
	this->_pServer = pServer;
}
