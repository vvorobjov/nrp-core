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

#ifndef PROTO_PYTHON_BINDINGS_H
#define PROTO_PYTHON_BINDINGS_H

#include "google/protobuf/message.h"
#include <boost/python.hpp>
#include "nrp_general_library/utils/nrp_exceptions.h"
#include "proto_python_bindings/proto_field_ops.h"
#include "proto_python_bindings/repeated_field_proxy.h"

#include "nrp_grpc_engine_protocol/grpc_server/engine_grpc.grpc.pb.h"


namespace bpy = boost::python;
namespace gpb = google::protobuf;
using namespace proto_field_ops;

/*!
 * \brief Class implementing python wrappers for protobuf Message types
 *
 * The main access point is "create", which effectively creates bindings for a MSG_TYPE class. The generated Python class
 * resembles the original Protobuf Python API as described here:
 *  - https://developers.google.com/protocol-buffers/docs/reference/python-generated
 *  - https://googleapis.dev/python/protobuf/latest/google/protobuf/message.html
 *
 * with the next limitations:
 *  - 1. Well Known Types not supported (yet) https://developers.google.com/protocol-buffers/docs/reference/python-generated#wkt
 *  - 2. Repeated Message field not supported (yet) https://developers.google.com/protocol-buffers/docs/reference/python-generated#repeated-message-fields
 *  - 3. Map field type not supported (yet) https://developers.google.com/protocol-buffers/docs/reference/python-generated#map-fields
 *  - 4. Basic Enum support. Enum fields get/set works with int. Enum constants can't be accessed from python https://developers.google.com/protocol-buffers/docs/reference/python-generated#enum
 *  - 5. Message Python wrapper supports only the a subset of the methods listed here https://googleapis.dev/python/protobuf/latest/google/protobuf/message.html
 *       (see "create" method)
 */
template<PROTO_MSG_C MSG_TYPE, PROTO_MSG_C ...FIELD_MSG_TYPES>
class proto_python_bindings
{
public:

    static void throw_python_error(PyObject * error, std::string msg) {
        PyErr_SetString(error, msg.c_str());
        boost::python::throw_error_already_set();
    }

    /*!
     * \brief __getattr__
     */
    static bpy::object GetAttribute(MSG_TYPE& m, char const* name)
    {
        const gpb::FieldDescriptor *field = m.GetDescriptor()->FindFieldByName(name);
        if(!field) {
            std::stringstream s;
            s << m.GetDescriptor()->name() << "\" object has no attribute \"" << name << "\"";
            throw_python_error(PyExc_AttributeError, s.str());
            return bpy::object();
        }

        if(field->is_map())
            return bpy::object();
        else if(field->is_repeated())
        {
            if(field->cpp_type() == gpb::FieldDescriptor::CPPTYPE_MESSAGE)
                return bpy::object();
            else
                return bpy::object(RepeatedScalarFieldProxy(m, field));
        }
        else if(field->cpp_type() == gpb::FieldDescriptor::CPPTYPE_MESSAGE) // TYPE_MESSAGE, TYPE_GROUP
        {
            if constexpr (sizeof...(FIELD_MSG_TYPES) > 0)
                return GetMessageField<FIELD_MSG_TYPES...>(m, field);
            else
                throw NRPException::logCreate("Can't get composite field with name " + field->name());
        }
        else
            return GetScalarField(m, field);
    }

    /*!
     * \brief __setattr__
     */
    static void SetAttribute(MSG_TYPE& m, char const* name, const bpy::object& value)
    {
        const gpb::FieldDescriptor *field = m.GetDescriptor()->FindFieldByName(name);
        if(!field) {
            std::stringstream s;
            s << m.GetDescriptor()->name() << "\" object has no attribute \"" << name << "\"";
            throw_python_error(PyExc_AttributeError, s.str());
            return;
        }

        if(field->is_repeated() || field->is_map())
            throw_python_error(PyExc_AttributeError,
                               "Assignment not allowed to repeated field \"" + field->name() + "\" in protocol message object.");
        else if(field->cpp_type() == gpb::FieldDescriptor::CPPTYPE_MESSAGE) // TYPE_MESSAGE, TYPE_GROUP
            throw_python_error(PyExc_AttributeError,
                               "Assignment not allowed to field \"" + field->name() + "\" in protocol message object.");
        else
            SetScalarField(m, field, value);
    }

    /*!
     * \brief WhichOneof
     */
    static bpy::object WhichOneof(MSG_TYPE& m, char const* name)
    {
        const gpb::OneofDescriptor *fieldOne = m.GetDescriptor()->FindOneofByName(name);
        if(!fieldOne) {
            std::stringstream s;
            s << "Protocol message has no oneof \"" << name << "\" field";
            PyErr_SetString(PyExc_ValueError, s.str().c_str());
            boost::python::throw_error_already_set();
            return bpy::object();
        }

        const gpb::FieldDescriptor *field = m.GetReflection()->GetOneofFieldDescriptor(m,fieldOne);
        if(field)
            return bpy::object(field->name());
        else
            return bpy::object();
    }

    /*!
     * \brief ClearField
     */
    static void ClearField(MSG_TYPE& m, char const* name)
    {
        const gpb::FieldDescriptor *field = m.GetDescriptor()->FindFieldByName(name);
        if(field) {
            m.GetReflection()->ClearField(&m, field);
            return;
        }

        const gpb::OneofDescriptor *fieldOne = m.GetDescriptor()->FindOneofByName(name);
        if(fieldOne) {
            m.GetReflection()->ClearOneof(&m, fieldOne);
            return;
        }

        std::stringstream s;
        s << "Protocol message has no \"" << name << "\" field.";
        PyErr_SetString(PyExc_ValueError, s.str().c_str());
        boost::python::throw_error_already_set();
    }

    /*!
     * \brief HasField
     */
    static bool HasField(MSG_TYPE& m, char const* name)
    {
        const gpb::FieldDescriptor *field = m.GetDescriptor()->FindFieldByName(name);
        if(field)
            return m.GetReflection()->HasField(m, field);

        std::stringstream s;
        s << "Unknown field " << name;
        PyErr_SetString(PyExc_ValueError, s.str().c_str());
        boost::python::throw_error_already_set();
        return false;
    }

    /*!
     * \brief Creates bindings for Protobuf Message type MSG_TYPE
     */
    static bpy::class_<MSG_TYPE> create() {
        std::shared_ptr<gpb::Message> m(new MSG_TYPE());
        const gpb::Descriptor *desc = m->GetDescriptor();

        bpy::class_<MSG_TYPE> binder(desc->name().c_str());
        binder.def(bpy::init<const MSG_TYPE &>());
        binder.def("__str__", &MSG_TYPE::DebugString);
        binder.def("__getattr__", GetAttribute);
        binder.def("__setattr__", SetAttribute);
        binder.def("ClearField",ClearField);
        binder.def("HasField", HasField);
        binder.def("WhichOneof",WhichOneof);
        binder.def("IsInitialized", &MSG_TYPE::IsInitialized);
        binder.def("Clear", &MSG_TYPE::Clear);

        return binder;
    }
};

#endif // PROTO_PYTHON_BINDINGS_H
