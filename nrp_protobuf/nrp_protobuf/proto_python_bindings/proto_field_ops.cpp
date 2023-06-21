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

#include "nrp_protobuf/proto_python_bindings/proto_field_ops.h"

namespace proto_field_ops {

    bpy::object GetScalarField(gpb::Message &m, const gpb::FieldDescriptor *field) {
        switch(field->cpp_type()) {
            case gpb::FieldDescriptor::CPPTYPE_INT32: // TYPE_INT32, TYPE_SINT32, TYPE_SFIXED32
                return bpy::object(m.GetReflection()->GetInt32(m, field));
            case gpb::FieldDescriptor::CPPTYPE_INT64: // TYPE_INT64, TYPE_SINT64, TYPE_SFIXED64
                return bpy::object(m.GetReflection()->GetInt64(m, field));
            case gpb::FieldDescriptor::CPPTYPE_UINT32: // TYPE_UINT32, TYPE_FIXED32
                return bpy::object(m.GetReflection()->GetUInt32(m, field));
            case gpb::FieldDescriptor::CPPTYPE_UINT64: // TYPE_UINT64, TYPE_FIXED64
                return bpy::object(m.GetReflection()->GetUInt64(m, field));
            case gpb::FieldDescriptor::CPPTYPE_DOUBLE: // TYPE_DOUBLE
                return bpy::object(m.GetReflection()->GetDouble(m, field));
            case gpb::FieldDescriptor::CPPTYPE_FLOAT: // TYPE_FLOAT
                return bpy::object(m.GetReflection()->GetFloat(m, field));
            case gpb::FieldDescriptor::CPPTYPE_BOOL: // TYPE_BOOL
                return bpy::object(m.GetReflection()->GetBool(m, field));
            case gpb::FieldDescriptor::CPPTYPE_ENUM: // TYPE_ENUM
                return bpy::object(m.GetReflection()->GetEnumValue(m, field));
            default:
                break;
        }

        // CPPTYPE_STRING
        switch(field->type()) {
            case gpb::FieldDescriptor::TYPE_STRING:
                return bpy::object(m.GetReflection()->GetString(m, field));
            case gpb::FieldDescriptor::TYPE_BYTES: {
                // Attempt to avoid copying the field twice. In order to avoid copying at all MemoryView could be tried:
                //  https://stackoverflow.com/questions/23064407/expose-c-buffer-as-python-3-bytes
                std::string *s2 = nullptr;
                const std::string &s = m.GetReflection()->GetStringReference(m, field, s2);
                return bpy::object(bpy::handle<>(PyBytes_FromStringAndSize(s.c_str(), s.length())));
            }
            default:
                break;
        }

        throw NRPException::logCreate("Getting value from a field of non scalar type or repeated");
    }

    // TODO: address code duplication
    std::string GetScalarFieldAsString(const gpb::Message &m, const gpb::FieldDescriptor *field) {
        switch(field->cpp_type()) {
            case gpb::FieldDescriptor::CPPTYPE_INT32: // TYPE_INT32, TYPE_SINT32, TYPE_SFIXED32
                return std::to_string(m.GetReflection()->GetInt32(m, field));
            case gpb::FieldDescriptor::CPPTYPE_INT64: // TYPE_INT64, TYPE_SINT64, TYPE_SFIXED64
                return std::to_string(m.GetReflection()->GetInt64(m, field));
            case gpb::FieldDescriptor::CPPTYPE_UINT32: // TYPE_UINT32, TYPE_FIXED32
                return std::to_string(m.GetReflection()->GetUInt32(m, field));
            case gpb::FieldDescriptor::CPPTYPE_UINT64: // TYPE_UINT64, TYPE_FIXED64
                return std::to_string(m.GetReflection()->GetUInt64(m, field));
            case gpb::FieldDescriptor::CPPTYPE_DOUBLE: // TYPE_DOUBLE
                return std::to_string(m.GetReflection()->GetDouble(m, field));
            case gpb::FieldDescriptor::CPPTYPE_FLOAT: // TYPE_FLOAT
                return std::to_string(m.GetReflection()->GetFloat(m, field));
            case gpb::FieldDescriptor::CPPTYPE_BOOL: // TYPE_BOOL
                return std::to_string(m.GetReflection()->GetBool(m, field));
            case gpb::FieldDescriptor::CPPTYPE_ENUM: // TYPE_ENUM
                return std::to_string(m.GetReflection()->GetEnumValue(m, field));
            default:
                break;
        }

        // CPPTYPE_STRING
        switch(field->type()) {
            case gpb::FieldDescriptor::TYPE_STRING:
                return m.GetReflection()->GetString(m, field);
            case gpb::FieldDescriptor::TYPE_BYTES: {
                return m.GetReflection()->GetString(m, field);
            }
            default:
                break;
        }

        throw NRPException::logCreate("Getting value from a field of non scalar type or repeated");
    }

    bpy::object GetRepeatedScalarField(gpb::Message &m, const gpb::FieldDescriptor *field, int index) {
        switch(field->cpp_type()) {
            case gpb::FieldDescriptor::CPPTYPE_INT32: // TYPE_INT32, TYPE_SINT32, TYPE_SFIXED32
                return bpy::object(m.GetReflection()->GetRepeatedInt32(m, field, index));
            case gpb::FieldDescriptor::CPPTYPE_INT64: // TYPE_INT64, TYPE_SINT64, TYPE_SFIXED64
                return bpy::object(m.GetReflection()->GetRepeatedInt64(m, field, index));
            case gpb::FieldDescriptor::CPPTYPE_UINT32: // TYPE_UINT32, TYPE_FIXED32
                return bpy::object(m.GetReflection()->GetRepeatedUInt32(m, field, index));
            case gpb::FieldDescriptor::CPPTYPE_UINT64: // TYPE_UINT64, TYPE_FIXED64
                return bpy::object(m.GetReflection()->GetRepeatedUInt64(m, field, index));
            case gpb::FieldDescriptor::CPPTYPE_DOUBLE: // TYPE_DOUBLE
                return bpy::object(m.GetReflection()->GetRepeatedDouble(m, field, index));
            case gpb::FieldDescriptor::CPPTYPE_FLOAT: // TYPE_FLOAT
                return bpy::object(m.GetReflection()->GetRepeatedFloat(m, field, index));
            case gpb::FieldDescriptor::CPPTYPE_BOOL: // TYPE_BOOL
                return bpy::object(m.GetReflection()->GetRepeatedBool(m, field, index));
            case gpb::FieldDescriptor::CPPTYPE_ENUM: // TYPE_ENUM
                return bpy::object(m.GetReflection()->GetRepeatedEnumValue(m, field, index));
            default:
                break;
        }

        // CPPTYPE_STRING
        switch(field->type()) {
            case gpb::FieldDescriptor::TYPE_STRING:
                return bpy::object(m.GetReflection()->GetRepeatedString(m, field, index));
            case gpb::FieldDescriptor::TYPE_BYTES: {
                std::string *s2 = nullptr;
                const std::string &s = m.GetReflection()->GetRepeatedStringReference(m, field, index, s2);
                return bpy::object(bpy::handle<>(PyBytes_FromStringAndSize(s.c_str(), s.length())));
            }
            default:
                break;
        }

        throw NRPException::logCreate("Getting value from a field of non scalar type");
    }

    void SetScalarField(gpb::Message &m, const gpb::FieldDescriptor *field, const bpy::object &value) {
        switch(field->cpp_type()) {
            case gpb::FieldDescriptor::CPPTYPE_INT32: // TYPE_INT32, TYPE_SINT32, TYPE_SFIXED32
                m.GetReflection()->SetInt32(&m, field, bpy::extract<int32_t>(value));
                break;
            case gpb::FieldDescriptor::CPPTYPE_INT64: // TYPE_INT64, TYPE_SINT64, TYPE_SFIXED64
                m.GetReflection()->SetInt64(&m, field, bpy::extract<int64_t>(value));
                break;
            case gpb::FieldDescriptor::CPPTYPE_UINT32: // TYPE_UINT32, TYPE_FIXED32
                m.GetReflection()->SetUInt32(&m, field, bpy::extract<uint32_t>(value));
                break;
            case gpb::FieldDescriptor::CPPTYPE_UINT64: // TYPE_UINT64, TYPE_FIXED64
                m.GetReflection()->SetUInt64(&m, field, bpy::extract<uint64_t>(value));
                break;
            case gpb::FieldDescriptor::CPPTYPE_DOUBLE: // TYPE_DOUBLE
                m.GetReflection()->SetDouble(&m, field, bpy::extract<double>(value));
                break;
            case gpb::FieldDescriptor::CPPTYPE_FLOAT: // TYPE_FLOAT
                m.GetReflection()->SetFloat(&m, field, bpy::extract<float>(value));
                break;
            case gpb::FieldDescriptor::CPPTYPE_BOOL: // TYPE_BOOL
                m.GetReflection()->SetBool(&m, field, bpy::extract<bool>(value));
                break;
            case gpb::FieldDescriptor::CPPTYPE_STRING: // TYPE_STRING, TYPE_BYTES
                // TODO: if it's a bytes object, which decoder is used?
                m.GetReflection()->SetString(&m, field, bpy::extract<std::string>(value));
                break;
            case gpb::FieldDescriptor::CPPTYPE_ENUM: // TYPE_ENUM
                m.GetReflection()->SetEnumValue(&m, field, bpy::extract<int>(value));
                break;
            default:
                throw NRPException::logCreate("Setting value for a field of non scalar type");
        }
    }

    void SetRepeatedScalarField(gpb::Message &m, const gpb::FieldDescriptor *field, const bpy::object &value, int index) {
        switch(field->cpp_type()) {
            case gpb::FieldDescriptor::CPPTYPE_INT32: // TYPE_INT32, TYPE_SINT32, TYPE_SFIXED32
                m.GetReflection()->SetRepeatedInt32(&m, field, index, bpy::extract<int32_t>(value));
                break;
            case gpb::FieldDescriptor::CPPTYPE_INT64: // TYPE_INT64, TYPE_SINT64, TYPE_SFIXED64
                m.GetReflection()->SetRepeatedInt64(&m, field, index, bpy::extract<int64_t>(value));
                break;
            case gpb::FieldDescriptor::CPPTYPE_UINT32: // TYPE_UINT32, TYPE_FIXED32
                m.GetReflection()->SetRepeatedUInt32(&m, field, index, bpy::extract<uint32_t>(value));
                break;
            case gpb::FieldDescriptor::CPPTYPE_UINT64: // TYPE_UINT64, TYPE_FIXED64
                m.GetReflection()->SetRepeatedUInt64(&m, field, index, bpy::extract<uint64_t>(value));
                break;
            case gpb::FieldDescriptor::CPPTYPE_DOUBLE: // TYPE_DOUBLE
                m.GetReflection()->SetRepeatedDouble(&m, field, index, bpy::extract<double>(value));
                break;
            case gpb::FieldDescriptor::CPPTYPE_FLOAT: // TYPE_FLOAT
                m.GetReflection()->SetRepeatedFloat(&m, field, index, bpy::extract<float>(value));
                break;
            case gpb::FieldDescriptor::CPPTYPE_BOOL: // TYPE_BOOL
                m.GetReflection()->SetRepeatedBool(&m, field, index, bpy::extract<bool>(value));
                break;
            case gpb::FieldDescriptor::CPPTYPE_STRING: // TYPE_STRING, TYPE_BYTES
                m.GetReflection()->SetRepeatedString(&m, field, index, bpy::extract<std::string>(value));
                break;
            case gpb::FieldDescriptor::CPPTYPE_ENUM: // TYPE_ENUM
                m.GetReflection()->SetRepeatedEnumValue(&m, field, index, bpy::extract<int>(value));
                break;
            default:
                throw NRPException::logCreate("Setting value for a field of non scalar type");
        }
    }

    void AddRepeatedScalarField(gpb::Message &m, const gpb::FieldDescriptor *field, const bpy::object &value) {
        switch(field->cpp_type()) {
            case gpb::FieldDescriptor::CPPTYPE_INT32: // TYPE_INT32, TYPE_SINT32, TYPE_SFIXED32
                m.GetReflection()->AddInt32(&m, field, bpy::extract<int32_t>(value));
                break;
            case gpb::FieldDescriptor::CPPTYPE_INT64: // TYPE_INT64, TYPE_SINT64, TYPE_SFIXED64
                m.GetReflection()->AddInt64(&m, field, bpy::extract<int64_t>(value));
                break;
            case gpb::FieldDescriptor::CPPTYPE_UINT32: // TYPE_UINT32, TYPE_FIXED32
                m.GetReflection()->AddUInt32(&m, field, bpy::extract<uint32_t>(value));
                break;
            case gpb::FieldDescriptor::CPPTYPE_UINT64: // TYPE_UINT64, TYPE_FIXED64
                m.GetReflection()->AddUInt64(&m, field, bpy::extract<uint64_t>(value));
                break;
            case gpb::FieldDescriptor::CPPTYPE_DOUBLE: // TYPE_DOUBLE
                m.GetReflection()->AddDouble(&m, field, bpy::extract<double>(value));
                break;
            case gpb::FieldDescriptor::CPPTYPE_FLOAT: // TYPE_FLOAT
                m.GetReflection()->AddFloat(&m, field, bpy::extract<float>(value));
                break;
            case gpb::FieldDescriptor::CPPTYPE_BOOL: // TYPE_BOOL
                m.GetReflection()->AddBool(&m, field, bpy::extract<bool>(value));
                break;
            case gpb::FieldDescriptor::CPPTYPE_STRING: // TYPE_STRING, TYPE_BYTES
                m.GetReflection()->AddString(&m, field, bpy::extract<std::string>(value));
                break;
            case gpb::FieldDescriptor::CPPTYPE_ENUM: // TYPE_ENUM
                m.GetReflection()->AddEnumValue(&m, field, bpy::extract<int>(value));
                break;
            default:
                throw NRPException::logCreate("Adding value for a field of non scalar type");
        }
    }

}
