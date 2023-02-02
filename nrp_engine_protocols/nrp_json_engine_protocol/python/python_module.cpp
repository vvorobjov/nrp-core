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
#include "nrp_json_engine_protocol/datapack_interfaces/json_datapack.h"

#include "nrp_general_library/utils/json_converter.h"
#include "nrp_general_library/config/cmake_constants.h"

#include "python/python_module.h"


/*!
 * \brief Generates python error with given type and message
 *
 * \param[in] type Type of exception
 * \param[in] message Message of the exception
 * \return Py_None object
 */
static PyObject * setPythonError(PyObject * type, const std::string & message)
{
    PyErr_SetString(type, message.c_str());
    boost::python::throw_error_already_set();

    return Py_None;
}


/*!
 * \brief Returns element stored in json array under given index
 *
 * \param[in] json The object of which the method was invoked
 * \param[in] index Index to be accessed
 * \return Object stored under given index
 */
static PyObject * getItemFromJsonArray(const nlohmann::json & jsonParent, PyObject* index)
{
    if(PyLong_Check(index))
    {
        const auto indexLong = PyLong_AsUnsignedLong(index);

        if(indexLong >= jsonParent.size())
        {
            return setPythonError(PyExc_IndexError, "list index out of range");
        }

        return json_converter::convertJsonToPyObject(jsonParent[indexLong]);
    }
    else
    {
        return setPythonError(PyExc_IndexError, "list indices must be integers");
    }
}


/*!
 * \brief Returns element stored in json object under given key
 *
 * \param[in] json The object of which the method was invoked
 * \param[in] index Key to be accessed
 * \return Object stored under given key
 */
static PyObject * getItemFromJsonObject(const nlohmann::json & jsonParent, PyObject* index)
{
    if(PyUnicode_Check(index))
    {
        const std::string key = PyUnicode_AsUTF8(index);

        if(jsonParent.find(key) == jsonParent.end())
        {
            const std::string message = "key '" + key + "' doesn't exist";

            return setPythonError(PyExc_KeyError, message.c_str());
        }

        return json_converter::convertJsonToPyObject(jsonParent[key]);
    }
    else
    {
        return setPythonError(PyExc_IndexError, "object indices must be str");
    }
}


/*!
 * \brief Implements python __getitem__ method of nlohmann::json
 *
 * The function is part of the python interface for nlohmann::json.
 * It allows to access nlohmann::json objects using 'object[index]' notation.
 * Both json arrays and objects cannot be accessed this way.
 *
 * \param[in] json The object of which the method was invoked
 * \param[in] index Index (in case of arrays) or key (in case of objects) to be accessed
 * \return Object stored under given key or index
 */
static PyObject * nlohmannJsonGetItem(const nlohmann::json & jsonParent, PyObject* index)
{
    if(jsonParent.is_array())
    {
        return getItemFromJsonArray(jsonParent, index);
    }
    else if(jsonParent.is_object())
    {
        return getItemFromJsonObject(jsonParent, index);
    }
    else
    {
        const std::string message = "json type '" + std::string(jsonParent.type_name()) + "' can't be accessed with []";

        return setPythonError(PyExc_IndexError, message);
    }
}


/*!
 * \brief Implements python __setitem__ method of nlohmann::json
 *
 * The function is part of the python interface for nlohmann::json.
 * It allows to modify nlohmann::json objects using 'object[key] = value' notation.
 * Json arrays cannot be modified this way.
 *
 * \param[in] json The object of which the method was invoked
 * \param[in] index Key to be modified or created
 * \param[in] value Value to be inserted under the key
 */
static void nlohmannJsonSetItem(nlohmann::json * jsonParent, PyObject * index, PyObject * value)
{
    if(jsonParent->is_array())
    {
        setPythonError(PyExc_IndexError, "setting json array elements not supported");
    }

    if(PyUnicode_Check(index))
    {
        (*jsonParent)[PyUnicode_AsUTF8(index)] = json_converter::convertPyObjectToJson(value);
    }
    else
    {
        setPythonError(PyExc_IndexError, "object indices must be str");
    }
}


/*!
 * \brief Implements python __str__ method of nlohmann::json
 *
 * The function is part of the python interface for nlohmann::json.
 *
 * \param[in] json The object of which the method was invoked
 * \return String representation of the object
 */
static PyObject * nlohmannJsonDump(const nlohmann::json & json)
{
    return PyUnicode_FromString(json.dump().c_str());
}


/*!
 * \brief Implements python __len__ method of nlohmann::json
 *
 * The function is part of the python interface for nlohmann::json.
 *
 * \param[in] json The object of which the method was invoked
 * \return Length of the object as long int
 */
static PyObject * nlohmannJsonSize(const nlohmann::json & json)
{
    return PyLong_FromLong(json.size());
}

/*!
 * \brief Returns the type of a json object as a string
 *
 * \param[in] json The object of which the method was invoked
 * \return json type of the object
 */
static PyObject * nlohmannJsonType(const nlohmann::json & json)
{ return PyUnicode_FromString(json.type_name()); }


/*!
 * \brief Implements python keys method of nlohmann::json
 *
 * The function is part of the python interface for nlohmann::json.
 *
 * \param[in] json The object of which the method was invoked
 * \return List of keys in case of json object, list of indices as strings in case of json array
 */
static PyObject * nlohmannJsonKeys(const nlohmann::json & json)
{
    if(!json.is_object())
    {
        const std::string message = "'keys' cannot be used with json type '" + std::string(json.type_name()) + "'";
        setPythonError(PyExc_AttributeError, message);
    }

    auto list = PyList_New(0);

    for (auto& el : json.items())
    {
        const std::string key = el.key();

        // Skip primitive types (key will be empty for primitive types)

        if(!key.empty())
        {
            PyList_Append(list, PyUnicode_FromString(key.c_str()));
        }
    }

    return list;
}


static void nlohmannJsonAppend(nlohmann::json * jsonParent, PyObject * value)
{
    if(!jsonParent->is_array() && !jsonParent->is_null())
    {
        setPythonError(PyExc_AttributeError,
                       "'append' may be called on array or null types only, called on '" +
                       std::string(jsonParent->type_name()) + "'");
    }

    jsonParent->push_back(json_converter::convertPyObjectToJson(value));
}


BOOST_PYTHON_MODULE(JSON_PYTHON_MODULE_NAME)
{
    if(Py_IsInitialized())
    {
        json_converter::initNumpy();
        boost::python::numpy::initialize();
    }
    else
    {
        std::cerr << "Python interpreter has not been initialized, numpy support in " JSON_PYTHON_MODULE_NAME_STR " will be disabled" << std::endl;
    }

    // Import General NRP Python Module
    boost::python::import(PYTHON_MODULE_NAME_STR);

    boost::python::class_<nlohmann::json, nlohmann::json*, std::unique_ptr<nlohmann::json>>("NlohmannJson")
        .def("__getitem__", &nlohmannJsonGetItem)
        .def("__setitem__", &nlohmannJsonSetItem)
        .def("__str__",     &nlohmannJsonDump)
        .def("__len__",     &nlohmannJsonSize)
        .def("keys",        &nlohmannJsonKeys)
        .def("json_type",   &nlohmannJsonType)
        .def("append",      &nlohmannJsonAppend);

    // Create SimulationStatus Python class, which is simply an alias for NlohmannJson
    boost::python::scope().attr("SimulationStatus") = boost::python::scope().attr("NlohmannJson");

    JsonDataPack::create_python("JsonDataPack");
    JsonRawData::create_python("JsonRawData");
}

// EOF
