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

#include <boost/python.hpp>
#include <nlohmann/json.hpp>

#include "nrp_json_engine_protocol/config/cmake_constants.h"
#include "nrp_json_engine_protocol/device_interfaces/json_device.h"
#include "nrp_json_engine_protocol/device_interfaces/json_converter.h"

#include "nrp_general_library/config/cmake_constants.h"

#include "python/python_module.h"


static PyObject * nlohmannJsonGetItem(nlohmann::json & jsonParent, PyObject* index)
{
    if(PyUnicode_Check(index))
    {
        nlohmann::json json = jsonParent[PyUnicode_AsUTF8(index)];

        return json_converter::convertJsonToPyObject(json);
    }
    else if(PyLong_Check(index))
    {
        nlohmann::json json = jsonParent[PyLong_AsLong(index)];

        return json_converter::convertJsonToPyObject(json);
    }
    else
    {
        PyErr_SetString(PyExc_IndexError, "index out of range");
        boost::python::throw_error_already_set();
        return Py_None;
    }
}


static void nlohmannJsonSetItem(nlohmann::json * jsonParent, PyObject* index, PyObject* value)
{
    if(PyUnicode_Check(index))
    {
        (*jsonParent)[PyUnicode_AsUTF8(index)] = json_converter::convertPyObjectToJson(value);
    }
    else
    {
        PyErr_SetString(PyExc_IndexError, "index out of range");
        boost::python::throw_error_already_set();
    }
}


BOOST_PYTHON_MODULE(JSON_PYTHON_MODULE_NAME)
{
    // Import General NRP Python Module
    boost::python::import(PYTHON_MODULE_NAME_STR);

    boost::python::class_<nlohmann::json, nlohmann::json*, std::unique_ptr<nlohmann::json>>("NlohmannJson")
        .def("__getitem__", &nlohmannJsonGetItem)
        .def("__setitem__", &nlohmannJsonSetItem);

    JsonDevice::create_python("JsonDevice");
}

// EOF
