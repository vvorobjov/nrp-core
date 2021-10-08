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

#include "nrp_python_json_engine/engine_server/python_json_server.h"

#include "nrp_general_library/utils/python_error_handler.h"
#include "nrp_general_library/utils/python_interpreter_state.h"

#include "nrp_python_json_engine/config/cmake_constants.h"
#include "nrp_python_json_engine/engine_server/python_engine_json_datapack_controller.h"
#include "nrp_python_json_engine/python/py_engine_script.h"

#include <fstream>
#include <filesystem>

namespace python = boost::python;

PythonJSONServer *PythonJSONServer::_registrationPyServer = nullptr;

PythonJSONServer::PythonJSONServer(const std::string &serverAddress, python::dict globals)
    : EngineJSONServer(serverAddress),
      _pyGlobals(globals)
{}

PythonJSONServer::PythonJSONServer(const std::string &serverAddress, const std::string &engineName, const std::string &registrationAddress, python::dict globals)
    : EngineJSONServer(serverAddress, engineName, registrationAddress),
      _pyGlobals(globals)
{}

bool PythonJSONServer::initRunFlag() const
{
    return this->_initRunFlag;
}

bool PythonJSONServer::shutdownFlag() const
{
    return this->_shutdownFlag;
}

SimulationTime PythonJSONServer::runLoopStep(SimulationTime timestep)
{
    NRP_LOGGER_TRACE("{} called", __FUNCTION__);
    
    PythonGILLock lock(this->_pyGILState, true);

    try
    {
        PyEngineScript &script = python::extract<PyEngineScript&>(this->_pyEngineScript);
        return script.runLoop(timestep);
    }
    catch(python::error_already_set &)
    {
        // If an error occured, return the message to the NRP server
        throw NRPExceptionNonRecoverable(handle_pyerror());
    }
}

nlohmann::json PythonJSONServer::initialize(const nlohmann::json &data, EngineJSONServer::lock_t&)
{
    NRP_LOGGER_TRACE("{} called", __FUNCTION__);

    PythonGILLock lock(this->_pyGILState, true);
    _initData = data;
    try
    {
        // Load python
        this->_pyGlobals.update(python::dict(python::import(NRP_PYTHON_ENGINE_MODULE_STR).attr("__dict__")));
    }
    catch(python::error_already_set &)
    {
        // If an error occured, return the message to the NRP server without setting the initRunFlag
        return this->formatInitErrorMessage(handle_pyerror());
    }

    // Read python script file if present
    const std::filesystem::path fileName = data.at("PythonFileName");
    if(fileName.empty())
    {
        const auto errMsg = "No python filename given. Aborting...";
        NRPLogger::error(errMsg);
        return this->formatInitErrorMessage(errMsg);
    }

    if(!std::filesystem::exists(fileName))
    {
        const auto errMsg = "Could not find init file " + std::string(fileName);
        NRPLogger::error(errMsg);
        return this->formatInitErrorMessage(errMsg);
    }

    // Prepare registration
    PythonJSONServer::_registrationPyServer = this;

    // Read python file
    try
    {
        python::exec_file(fileName.c_str(), this->_pyGlobals, this->_pyGlobals);
    }
    catch(python::error_already_set &)
    {
        // If an error occured, return the message to the NRP server without setting the initRunFlag
        const auto msg = handle_pyerror();
        NRPLogger::error(msg);
        return this->formatInitErrorMessage(msg);
    }

    // Check that executed file also
    if(PythonJSONServer::_registrationPyServer != nullptr)
    {
        PythonJSONServer::_registrationPyServer = nullptr;
        const auto errMsg = "Failed to initialize Python server. Given python file \"" + std::string(fileName) + "\" does not register a script";
        NRPLogger::error(errMsg);
        return this->formatInitErrorMessage(errMsg);
    }

    // Run user-defined initialize function
    try
    {
        PyEngineScript &script = python::extract<PyEngineScript&>(this->_pyEngineScript);
        script.initialize();
    }
    catch(python::error_already_set &)
    {
        // If an error occured, return the message to the NRP server without setting the initRunFlag
        const auto msg = handle_pyerror();
        NRPLogger::error(msg);
        return this->formatInitErrorMessage(msg);

    }

    // Init has run once
    this->_initRunFlag = true;

    NRPLogger::debug("PythonJSONServer::initialize(...) completed with no errors.");

    // Return success and parsed devmap
    return nlohmann::json({{PythonConfigConst::InitFileExecStatus, true}});
}


nlohmann::json PythonJSONServer::reset(EngineJSONServer::lock_t &lock)
{
    NRP_LOGGER_TRACE("{} called", __FUNCTION__);

    if (!this->initRunFlag())
    {
        return nlohmann::json({{PythonConfigConst::ResetExecStatus, false}, {PythonConfigConst::ErrorMsg, "Cannot reset non-initialized instance"}});
    }

    try
    {
        this->shutdown(_initData);

        this->_shutdownFlag = false;

        this->initialize(_initData, lock);

        return nlohmann::json({{PythonConfigConst::ResetExecStatus, true}});
    }
    catch (python::error_already_set &)
    {
        const auto msg = handle_pyerror();
        NRPLogger::error("Failed to reset Python Engine instance: {}", msg);
        return nlohmann::json({{PythonConfigConst::ResetExecStatus, false}, {PythonConfigConst::ErrorMsg, msg}});
    }
}

nlohmann::json PythonJSONServer::shutdown(const nlohmann::json &)
{
    NRP_LOGGER_TRACE("{} called", __FUNCTION__);

    PythonGILLock lock(this->_pyGILState, true);

    this->_shutdownFlag = true;

    if(this->_initRunFlag)
    {
        // Run user-defined Shutdown fcn
        try
        {
            PyEngineScript &script = python::extract<PyEngineScript&>(this->_pyEngineScript);
            script.shutdown();
        }
        catch(python::error_already_set &)
        {
            // If an error occured, return the message to the NRP server
            throw NRPExceptionNonRecoverable(handle_pyerror());
        }
    }

    // Remove datapack controllers
    this->clearRegisteredDataPacks();
    this->_datapackControllerPtrs.clear();

    return nlohmann::json();
}

PyEngineScript *PythonJSONServer::registerScript(const boost::python::object &pythonScript)
{
    NRP_LOGGER_TRACE("{} called", __FUNCTION__);
    
    assert(PythonJSONServer::_registrationPyServer != nullptr);

    // Register script with server
    PythonJSONServer::_registrationPyServer->_pyEngineScript = pythonScript();

    // Register server with script
    PyEngineScript &script = boost::python::extract<PyEngineScript&>(PythonJSONServer::_registrationPyServer->_pyEngineScript);
    script.setPythonJSONServer(PythonJSONServer::_registrationPyServer);

    PythonJSONServer::_registrationPyServer = nullptr;

    return &script;
}

nlohmann::json PythonJSONServer::formatInitErrorMessage(const std::string &errMsg)
{
    return nlohmann::json({{PythonConfigConst::InitFileExecStatus, 0}, {PythonConfigConst::ErrorMsg, errMsg}});
}

nlohmann::json PythonJSONServer::getDataPackData(const nlohmann::json &reqData)
{
    PythonGILLock lock(this->_pyGILState, true);
    return this->EngineJSONServer::getDataPackData(reqData);
}

nlohmann::json PythonJSONServer::setDataPackData(const nlohmann::json &reqData)
{
    PythonGILLock lock(this->_pyGILState, true);
    return this->EngineJSONServer::setDataPackData(reqData);
}

nlohmann::json PythonJSONServer::getEngineConfig() const
{ return this->_initData; }
