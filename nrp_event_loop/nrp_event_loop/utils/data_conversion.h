/* * NRP Core - Backend infrastructure to synchronize simulations
 *
 * Copyright 2020-2021 NRP Team
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * This project has received funding from the European Union’s Horizon 2020
 * Framework Programme for Research and Innovation under the Specific Grant
 * Agreement No. 945539 (Human Brain Project SGA3).
 */

#ifndef DATA_CONVERSION_H
#define DATA_CONVERSION_H

#include <boost/python.hpp>

#include "nrp_general_library/utils/nrp_exceptions.h"


/*!
 * \brief Template for data conversion functions used in InputPorts upon new message arrival.
 *
 * Concrete conversions are implemented as specializations
 */
template <class T_IN, class T_OUT>
struct dataConverter {
    static void convert(const T_IN */*d1*/, T_OUT &/*d2*/) {
        std::stringstream s;
        s << "data conversion from " << typeid(T_IN).name() << " to type " << typeid(T_OUT).name()
          << " is not implemented.";
        throw NRPException::logCreate(s.str());
    }
};

// dataConverter specializations

namespace bpy = boost::python;

template <class T_IN>
struct dataConverter<T_IN, bpy::object> {
    static void convert(const T_IN *d1, bpy::object &d2)
    {
        try {
            if constexpr((std::is_class_v<T_IN> || std::is_union_v<T_IN>) && !std::is_same_v<std::string, T_IN> )
                d2 = bpy::object(bpy::ptr(d1));
            else
                d2 = bpy::object(*d1);
        }
        catch (const boost::python::error_already_set&) {
            std::string error_msg = "An error occurred while converting C++ data to Python. Please be sure you imported the Python module/s supporting the conversion of this data type";
            PyErr_Print();
            throw NRPException::logCreate(error_msg);
        }
    }
};

template <class T_OUT>
struct dataConverter<bpy::object, T_OUT> {
    static void convert(const bpy::object  *d1, T_OUT &d2)
    {
        try {
            d2 = boost::python::extract<T_OUT>(*d1);
        }
        catch (const boost::python::error_already_set&) {
            std::string error_msg = "An error occurred while converting Python to C++ data";
            PyErr_Print();
            throw NRPException::logCreate(error_msg);
        }
    }
};

#endif //DATA_CONVERSION_H
