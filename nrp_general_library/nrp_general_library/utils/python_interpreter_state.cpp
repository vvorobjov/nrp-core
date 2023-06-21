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

#include "nrp_general_library/utils/python_interpreter_state.h"
#include "nrp_general_library/utils/json_converter.h"

#include <boost/python/numpy.hpp>
#include <boost/filesystem.hpp>
#include <boost/algorithm/string.hpp>

void setup_venv() {
    std::vector<std::string> paths;
    std::string pathEnv = getenv("PATH");
    boost::split(paths, pathEnv, boost::is_any_of(";:"));
    for (std::string path : paths)
    {
      boost::filesystem::path pythonPath = boost::filesystem::path(path) / "python";
      if (boost::filesystem::exists(pythonPath))
      {
        std::string pythonProgramName = pythonPath.string();
        std::wstring widestring = std::wstring(pythonProgramName.begin(), pythonProgramName.end());
        Py_SetProgramName(widestring.c_str());
        break;
      }
    }
}


PythonInterpreterState::PythonInterpreterState(int argc, const char *const *argv, bool allowThreads)
    : _wcharArgs(argc, argv)
{
    assert(argc >= 1);

    Py_SetProgramName(this->_wcharArgs.getWCharTPointers()[0]);
    setup_venv();
    Py_Initialize();
    json_converter::initNumpy();
    boost::python::numpy::initialize();

    PySys_SetArgv(this->_wcharArgs.getWCharSize(), this->_wcharArgs.getWCharTPointers());

    if(allowThreads)
        this->_state = PyEval_SaveThread();
    else
        this->_state = nullptr;
}

PythonInterpreterState::PythonInterpreterState(int argc, const std::vector<const char*> &argv, bool allowThreads)
    : PythonInterpreterState(argc, argv.data(), allowThreads)
{}

PythonInterpreterState::PythonInterpreterState(bool allowThreads)
    : _wcharArgs(0, nullptr)
{
    setup_venv();
    Py_Initialize();
    json_converter::initNumpy();
    boost::python::numpy::initialize();

    if(allowThreads)
        this->_state = PyEval_SaveThread();
    else
        this->_state = nullptr;
}

void PythonInterpreterState::allowThreads()
{
    if(this->_state == nullptr)
        this->_state = PyEval_SaveThread();
}

bool PythonInterpreterState::threadsAllowed() const
{
    return (this->_state != nullptr);
}

void PythonInterpreterState::endAllowThreads()
{
    if(this->_state != nullptr)
    {
        PyEval_RestoreThread(this->_state);
        this->_state = nullptr;
    }
}

PythonInterpreterState::~PythonInterpreterState()
{
    this->endAllowThreads();
}

PythonGILLock::PythonGILLock(PyGILState_STATE &state, const bool acquire)
    : _state(&state)
{
    if(acquire)
        this->acquire();
}

PythonGILLock::~PythonGILLock()
{
    this->release();
    this->_state = nullptr;
}

void PythonGILLock::acquire()
{
    *(this->_state) = PyGILState_Ensure();
}

void PythonGILLock::release()
{
    PyGILState_Release(*(this->_state));
}
