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
#include "proto_python_bindings/repeated_field_proxy.h"

Py_ssize_t ExtractIndices(PyObject* indices, Py_ssize_t& from, Py_ssize_t& to, Py_ssize_t& step, Py_ssize_t length)
{
    // Single item
    if(PyLong_Check(indices)) {
        from = to = PyLong_AsLong(indices);
        return -1;
    }

    // List
    else if(PySlice_Check(indices)) {
        Py_ssize_t slicelength;
        if(PySlice_GetIndicesEx(indices, length, &from, &to, &step, &slicelength) != -1)
            return slicelength;
    }

    PyErr_SetString(PyExc_TypeError, "list indices must be integers");
    boost::python::throw_error_already_set();
    return -1;
}

bpy::object RepeatedScalarFieldProxy::GetItem(PyObject* indices)
{
    Py_ssize_t from, to, step;
    Py_ssize_t length = Len();
    Py_ssize_t slicelength = ExtractIndices(indices, from, to, step, length);

    if(slicelength == -1)
        return GetRepeatedScalarField(_m, _f, from);
    else if(slicelength == 0)
        return bpy::list();
    else {
        bpy::list items;
        if(from <= to) {
            for(Py_ssize_t index = from; index < to; index += step)
                items.append(GetRepeatedScalarField(_m, _f, index));
        }
        else {
            for(Py_ssize_t index = from; index > to; index += step)
                items.append(GetRepeatedScalarField(_m, _f, index));
        }

        return std::move(items);
    }
}

void RepeatedScalarFieldProxy::SetItem(PyObject* indices, PyObject* value)
{
    Py_ssize_t from, to, step;
    Py_ssize_t length = Len();
    Py_ssize_t slicelength = ExtractIndices(indices, from, to, step, length);

    if(slicelength == -1) {
        SetRepeatedScalarField(_m, _f, bpy::object(boost::python::handle<>(boost::python::borrowed(value))), from);
        return;
    }
    else if( !PySequence_Check(value) ) {
        PyErr_SetString(PyExc_TypeError, "can only assign an iterable");
        boost::python::throw_error_already_set();
    }

    if(step != 1 && slicelength != PySequence_Length(value)) {
        std::stringstream s;
        s << "attempt to assign sequence of size " << PySequence_Length(value) << " to extended slice of size " << slicelength;
        PyErr_SetString(PyExc_ValueError, s.str().c_str());
        boost::python::throw_error_already_set();
    }

    if(PySequence_Length(value) == slicelength) {
        Py_ssize_t n = 0;
        if(from <= to) {
            for(Py_ssize_t index = from; index < to; index += step)
                SetRepeatedScalarField(_m, _f,bpy::object(boost::python::handle<>(boost::python::borrowed(
                        PySequence_GetItem(value, n++)))), index);
        }
        else {
            for(Py_ssize_t index = from; index > to; index += step)
                SetRepeatedScalarField(_m, _f,bpy::object(boost::python::handle<>(boost::python::borrowed(
                        PySequence_GetItem(value, n++)))), index);
        }
    }
    else if(from >= length)
        Extend(bpy::object(boost::python::handle<>(boost::python::borrowed(value))));
    else {
        PyObject* full_slice(PySlice_New(nullptr, nullptr, nullptr));
        bpy::list old_items(GetItem(full_slice));

        Clear();

        for(int i=0;i<from;++i)
            AddRepeatedScalarField(_m, _f, old_items[i]);
        for(int i=0;i<PySequence_Length(value);++i)
            AddRepeatedScalarField(_m, _f,
                                   bpy::object(boost::python::handle<>(boost::python::borrowed(PySequence_GetItem(value, i)))));
        for(int i=to;i<length;++i)
            AddRepeatedScalarField(_m, _f, old_items[i]);
    }
}

bpy::object RepeatedScalarFieldProxy::Iter()
{ return  bpy::object(RepeatedScalarFieldIterProxy(_m, _f)); }

