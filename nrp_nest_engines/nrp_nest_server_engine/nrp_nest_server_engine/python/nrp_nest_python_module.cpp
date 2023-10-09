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

#include <boost/python.hpp>

#include "nrp_general_library/config/cmake_constants.h"
#include "nrp_nest_server_engine/config/cmake_constants.h"
#include "nrp_nest_server_engine/python/create_datapack_class.h"


namespace python = boost::python;

static CreateDataPackClass *pCreateDataPack = nullptr;

python::object CreateDataPack(python::tuple args, python::dict kwargs)
{
    assert(pCreateDataPack != nullptr);
    return pCreateDataPack->createAndRegisterDataPack(args, kwargs);
}

void RegisterDataPack(python::str devName, python::object nodeCollection)
{
    assert(pCreateDataPack != nullptr);
    return pCreateDataPack->registerDataPack(devName, nodeCollection);
}

python::dict GetDevMap()
{
    assert(pCreateDataPack != nullptr);
    return pCreateDataPack->pyDevMap();
}

BOOST_PYTHON_MODULE(NRP_NEST_PYTHON_MODULE)
{
    // Import General NRP Python Module
    python::import(PYTHON_MODULE_NAME_STR);

    // Setup CreateDataPack and import Nest
    python::class_<CreateDataPackClass>("__CreateDataPackClass", python::no_init)
            .def("CreateDataPack", python::raw_function(&CreateDataPackClass::pyCreateDataPack))
            .def("RegisterDataPack", python::raw_function(&CreateDataPackClass::pyRegisterDataPack))
            .def("GetDevMap", &CreateDataPackClass::pyDevMap);

    python::dict devMap;
    python::dict nestDict(python::import("nest").attr("__dict__"));

    python::object pyCreateDataPack(CreateDataPackClass(nestDict, devMap));
    python::scope().attr("__CreateDataPack") = pyCreateDataPack;

    CreateDataPackClass &createDataPack = python::extract<CreateDataPackClass&>(pyCreateDataPack);
    pCreateDataPack = &createDataPack;

    // Setup Nest Create and Register DataPack Function
    python::def("CreateDataPack", python::raw_function(CreateDataPack));
    python::def("RegisterDataPack", RegisterDataPack);
    python::def("GetDevMap", GetDevMap);
}
