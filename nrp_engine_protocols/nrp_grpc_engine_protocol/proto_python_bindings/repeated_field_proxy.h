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

#ifndef REPEATED_FIELD_PROXY_H
#define REPEATED_FIELD_PROXY_H

#include "google/protobuf/message.h"
#include <boost/python.hpp>
#include "nrp_general_library/utils/nrp_exceptions.h"
#include "proto_python_bindings/proto_field_ops.h"

#include "nrp_grpc_engine_protocol/grpc_server/engine_grpc.grpc.pb.h"


namespace bpy = boost::python;
namespace gpb = google::protobuf;
using namespace proto_field_ops;

/*!
* \brief Function which processes an index or slice object and sets from, to and step appropriately
*
* Returns the size of the described range, -1 if 'indices' is not a valid slice
*/
Py_ssize_t ExtractIndices(PyObject* indices, Py_ssize_t& from, Py_ssize_t& to, Py_ssize_t& step, Py_ssize_t length);

/*!
 * \brief Proxy class implementing a list-like python wrapper for a protobuf repeated scalar field (ie. not of Message type)
 */
class RepeatedScalarFieldProxy
{
public:

    /*!
     * \brief Creates the python wrapper
     *
     * The python wrapper supports the usual python "list" index get/set operations and iteration.
     * It does not support the next list methods: '__delitem__', 'copy', 'count', 'index', 'insert', 'remove', 'reverse', 'sort'.
     * It does not support comparison with a list.
     */
    static void create() {
        bpy::class_<RepeatedScalarFieldProxy> binder("RepeatedScalarField", bpy::no_init);
        binder.def("__len__", &RepeatedScalarFieldProxy::Len);
        binder.def("__getitem__", &RepeatedScalarFieldProxy::GetItem);
        binder.def("__setitem__", &RepeatedScalarFieldProxy::SetItem);
        binder.def("__iter__", &RepeatedScalarFieldProxy::Iter);
        binder.def("append", &RepeatedScalarFieldProxy::Append);
        binder.def("extend", &RepeatedScalarFieldProxy::Extend);
        binder.def("pop", &RepeatedScalarFieldProxy::Pop);
        binder.def("clear", &RepeatedScalarFieldProxy::Clear);
    }

    /*!
     * \brief Constructor
     *
     * @param msg Message storing the wrapped field
     * @param field Field descriptor
     */
    RepeatedScalarFieldProxy(gpb::Message & msg, const gpb::FieldDescriptor *field)
            : _m(msg), _f(field) {
        if(!field->is_repeated())
            throw NRPException::logCreate("Accessing RepeatedScalarFieldProxy from a non-repeating field");
        else if(field->cpp_type() == gpb::FieldDescriptor::CPPTYPE_MESSAGE)
            throw NRPException::logCreate("Accessing RepeatedScalarFieldProxy from a non-scalar field");
    }

    /*!
     * /brief __len__
     */
    int Len()
    { return _m.GetReflection()->FieldSize(_m, _f); }

    /*!
     * /brief __getitem__
     */
    bpy::object GetItem(PyObject* indices);

    /*!
     * /brief __setitem__
     */
    void SetItem(PyObject* indices, PyObject* value);

    /*!
     * /brief __iter__
     */
    bpy::object Iter();

    /*!
     * /brief append
     */
    void Append(const bpy::object& value)
    { AddRepeatedScalarField(_m, _f, value); }

    /*!
     * /brief __extend__
     */
    void Extend(const bpy::object& value) {
        for(int i=0; i<bpy::len(value); ++i)
            AddRepeatedScalarField(_m, _f, value[i]);
    }

    /*!
     * /brief clear
     */
    void Clear()
    { _m.GetReflection()->ClearField(&_m,_f); }

    /*!
     * /brief pop
     */
    void Pop()
    { _m.GetReflection()->RemoveLast(&_m, _f); }

private:

    gpb::Message & _m;
    const gpb::FieldDescriptor * _f;
};


/*!
 * \brief Class implementing a python iterator for a RepeatedScalarFieldProxy
 */
class RepeatedScalarFieldIterProxy
{
public:

    /*!
     * \brief Creates the python wrapper
     */
    static void create() {
        bpy::class_<RepeatedScalarFieldIterProxy> binder("RepeatedScalarFieldIter", bpy::no_init);
        binder.def("__next__", &RepeatedScalarFieldIterProxy::Next);
    }

    /*!
     * \brief Constructor
     */
    RepeatedScalarFieldIterProxy(gpb::Message & msg, const gpb::FieldDescriptor *field)
            : _p(msg, field), _ind(0) { }

    /*!
     * \brief Implementation of __next__
     */
    bpy::object Next() {
        if(_ind == _p.Len()) {
            PyErr_SetObject(PyExc_StopIteration, Py_None);
            boost::python::throw_error_already_set();
        }

        return _p.GetItem(Py_BuildValue("i", _ind++));
    }

private:

    RepeatedScalarFieldProxy _p;
    int _ind;
};

#endif // REPEATED_FIELD_PROXY_H
