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
#include <boost/python/numpy.hpp>
#include <nlohmann/json.hpp>

#ifndef ENGINE_JSON_JSON_CONVERTER_H
#define ENGINE_JSON_JSON_CONVERTER_H

namespace json_converter
{

/*!
 * \brief Initializes numpy array API for this module
 *
 * The function must be called before using the module.
 * If it's not done, some of the other functions in the module may fail with an exception.
 * The function must be called after Py_Initialize().
 */
void initNumpy();

/*!
 * \brief Converts given JSON object into a python object
 */
PyObject * convertJsonToPyObject(const nlohmann::json & json);


/*!
 * \brief Converts given python object into a JSON object
 */
nlohmann::json convertPyObjectToJson(PyObject* value);

} // namespace json_converter

#endif // ENGINE_JSON_JSON_CONVERTER_H

// EOF
