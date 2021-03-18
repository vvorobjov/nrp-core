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

#include "nrp_nest_json_engine/python/create_device_class.h"

CreateDeviceClass::CreateDeviceClass(boost::python::dict nest, boost::python::dict devMap)
    : _nest(nest),
      _devMap(devMap)
{}

boost::python::object CreateDeviceClass::pyCreateDevice(boost::python::tuple args, boost::python::dict kwargs)
{
	CreateDeviceClass &self = boost::python::extract<CreateDeviceClass&>(args[0]);
	return self.createAndRegisterDevice(args, kwargs);
}

boost::python::object CreateDeviceClass::pyRegisterDevice(boost::python::tuple args, boost::python::dict)
{
	CreateDeviceClass &self = boost::python::extract<CreateDeviceClass&>(args[0]);
	self.registerDevice(boost::python::str(args[1]), args[2]);

	return boost::python::object();
}

boost::python::api::object CreateDeviceClass::createAndRegisterDevice(boost::python::tuple args, boost::python::dict kwargs)
{
	// Call Nest's Create function with all arguments except for first one (that is the label)
	boost::python::object deviceID = this->_nest["Create"](*(args.slice(1, boost::python::len(args))), **kwargs);

	// Save created device's ID
	this->registerDevice(boost::python::str(args[0]), deviceID);

	return deviceID;
}

void CreateDeviceClass::registerDevice(boost::python::str devName, boost::python::api::object nodeCollection)
{
	this->_devMap[devName] = nodeCollection;
}

boost::python::dict CreateDeviceClass::pyDevMap()
{
	return this->devMap();
}

const boost::python::dict &CreateDeviceClass::devMap() const
{
	return this->_devMap;
}