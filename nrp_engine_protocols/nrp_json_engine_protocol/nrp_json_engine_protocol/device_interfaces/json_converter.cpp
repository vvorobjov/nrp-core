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

#include "nrp_json_engine_protocol/device_interfaces/json_converter.h"


namespace json_converter
{

namespace np = boost::python::numpy;


static PyObject * convertJsonToPyDict(const nlohmann::json & json)
{
    auto dict = PyDict_New();

    for (nlohmann::json::const_iterator it = json.cbegin(); it != json.cend(); ++it)
    {
        PyDict_SetItemString(dict, it.key().c_str(), convertJsonToPyObject(it.value()));
    }

    return dict;
}


static PyObject * convertJsonToPyList(const nlohmann::json & json)
{
    auto list = PyList_New(0);

    for (const auto& el : json)
    {
        PyList_Append(list, convertJsonToPyObject(el));
    }

    return list;
}


static void generatePythonError(const std::string & errorMessage)
{
    // Generate a python exception

    PyErr_SetString(PyExc_TypeError, errorMessage.c_str());
    boost::python::throw_error_already_set();
}


PyObject * convertJsonToPyObject(const nlohmann::json & json)
{
    namespace nl = nlohmann;

    switch(json.type())
    {
        case nl::json::value_t::null:            return Py_None;
        case nl::json::value_t::object:          return convertJsonToPyDict(json);
        case nl::json::value_t::array:           return convertJsonToPyList(json);
        case nl::json::value_t::string:          return PyUnicode_FromString(json.get<std::string>().c_str());
        case nl::json::value_t::boolean:         return (json.get<bool>() ? Py_True : Py_False);
        case nl::json::value_t::number_integer:  return PyLong_FromLong(json.get<nl::json::number_integer_t>());
        case nl::json::value_t::number_unsigned: return PyLong_FromUnsignedLong(json.get<nl::json::number_unsigned_t>());
        case nl::json::value_t::number_float:    return PyFloat_FromDouble(json.get<double>());
        default:
            generatePythonError("Conversion of nlohmann::json type " +
                                std::to_string(static_cast<int>(json.type())) +
                                " not implemented");
            return nullptr;
    }
}


template <typename VECTOR_TYPE>
static nlohmann::json convertNumpyArrayHelper(const boost::python::numpy::ndarray & npArray)
{
    const auto data = reinterpret_cast<VECTOR_TYPE *>(npArray.get_data());

    // TODO I guess this is going to copy the data twice... Can it be done better?

    std::vector<VECTOR_TYPE> vec(data, data + (*npArray.get_shape()));

    return nlohmann::json(vec);
}


template <typename BUILTIN_TYPE>
static inline bool compareDtype(np::dtype dtype)
{
    return np::equivalent(np::dtype::get_builtin<BUILTIN_TYPE>(), dtype);
}


nlohmann::json convertNumpyArrayToJson(PyObject * value)
{
    boost::python::handle pyHandle(boost::python::borrowed(value));
    boost::python::object pyObject(pyHandle);

    const auto npArray = np::from_object(pyObject);

    // Conversions of only 1-dimensional arrays are supported

    if(npArray.get_nd() != 1)
    {
        generatePythonError("Conversion of numpy array with nd = " +
                            std::to_string(npArray.get_nd()) +
                            " not implemented");

        return nullptr;
    }

    const auto dtype = npArray.get_dtype();

    if     (compareDtype<int64_t >(dtype)) return convertNumpyArrayHelper<int64_t >(npArray);
    else if(compareDtype<int32_t >(dtype)) return convertNumpyArrayHelper<int32_t >(npArray);
    else if(compareDtype<int16_t >(dtype)) return convertNumpyArrayHelper<int16_t >(npArray);
    else if(compareDtype<int8_t  >(dtype)) return convertNumpyArrayHelper<int8_t  >(npArray);
    else if(compareDtype<uint64_t>(dtype)) return convertNumpyArrayHelper<uint64_t>(npArray);
    else if(compareDtype<uint32_t>(dtype)) return convertNumpyArrayHelper<uint32_t>(npArray);
    else if(compareDtype<uint16_t>(dtype)) return convertNumpyArrayHelper<uint16_t>(npArray);
    else if(compareDtype<uint8_t >(dtype)) return convertNumpyArrayHelper<uint8_t >(npArray);
    else if(compareDtype<float   >(dtype)) return convertNumpyArrayHelper<float   >(npArray);
    else if(compareDtype<double  >(dtype)) return convertNumpyArrayHelper<double  >(npArray);
    else
    {
        // Extract dtype of the array as string

        const std::string dtypeString(boost::python::extract<char const *>(boost::python::str(npArray.get_dtype())));
        generatePythonError("Conversion of numpy dtype '" + dtypeString + "' not implemented");

        return nullptr;
    }
}


static nlohmann::json convertPyListToJson(PyObject* value)
{
    auto list = nlohmann::json::array();

    for(Py_ssize_t i = 0; i < PyList_Size(value); i++)
    {
        list.push_back(convertPyObjectToJson(PyList_GetItem(value, i)));
    }

    return list;
}


static nlohmann::json convertPyTupleToJson(PyObject* value)
{
    auto list = nlohmann::json::array();

    for(Py_ssize_t i = 0; i < PyTuple_Size(value); i++)
    {
        list.push_back(convertPyObjectToJson(PyTuple_GetItem(value, i)));
    }

    return list;
}


static nlohmann::json convertPyDictToJson(PyObject* value)
{
    auto dict = nlohmann::json::object();

    PyObject *key, *val;
    Py_ssize_t pos = 0;

    while(PyDict_Next(value, &pos, &key, &val))
    {
        dict[PyUnicode_AsUTF8(key)] = convertPyObjectToJson(val);
    }

    return dict;
}


nlohmann::json convertPyObjectToJson(PyObject* value)
{
    if(value == Py_None)            return nullptr;
    else if(PyBool_Check(value))    return static_cast<bool>(PyObject_IsTrue(value));
    else if(PyLong_Check(value))    return PyLong_AsLong(value);
    else if(PyUnicode_Check(value)) return PyUnicode_AsUTF8(value);
    else if(PyFloat_Check(value))   return PyFloat_AsDouble(value);
    else if(PyList_Check(value))    return convertPyListToJson(value);
    else if(PyTuple_Check(value))   return convertPyTupleToJson(value);
    else if(PyDict_Check(value))    return convertPyDictToJson(value);
    else                            return convertNumpyArrayToJson(value);
}

} // namespace json_converter

// EOF
