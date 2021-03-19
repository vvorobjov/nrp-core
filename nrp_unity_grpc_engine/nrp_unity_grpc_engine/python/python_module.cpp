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

#include "devices/info.h"
#include "devices/camera.h"

#include "nrp_general_library/device_interface/device.h"
#include "nrp_general_library/device_interface/device_python_wrappers.h"
#include "nrp_general_library/config/cmake_constants.h"

#include "nrp_unity_grpc_engine/config/cmake_constants.h"

#include <boost/python.hpp>
#include <boost/python/suite/indexing/vector_indexing_suite.hpp>
using namespace boost::python;

BOOST_PYTHON_MODULE(UNITY_PYTHON_MODULE_NAME)
{
	import(PYTHON_MODULE_NAME_STR);

	python_property_device_class<GetInfo>::create();
	python_property_device_class<SetInfo>::create();
	python_property_device_class<Camera>::create();
}
