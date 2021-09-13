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

#include "nrp_nest_json_engine/engine_server/nest_json_server.h"

#include "nrp_general_library/utils/python_error_handler.h"
#include "nrp_general_library/utils/python_interpreter_state.h"

#include "nrp_nest_json_engine/config/cmake_constants.h"
#include "nrp_nest_json_engine/engine_server/nest_engine_device_controller.h"
#include "nrp_nest_json_engine/python/create_device_class.h"

#include "nrp_nest_json_engine/config/nest_config.h"

#include <fstream>

namespace python = boost::python;


NestJSONServer::NestJSONServer(const std::string &serverAddress, python::dict globals)
    : EngineJSONServer(serverAddress),
      _pyGlobals(globals)
{
	NRP_LOGGER_TRACE("{} called", __FUNCTION__);
}

NestJSONServer::NestJSONServer(const std::string &serverAddress, const std::string &engineName, const std::string &registrationAddress, python::dict globals)
    : EngineJSONServer(serverAddress, engineName, registrationAddress),
      _pyGlobals(globals)
{
	NRP_LOGGER_TRACE("{} called", __FUNCTION__);
}

NestJSONServer::~NestJSONServer()
{
	NRP_LOGGER_TRACE("{} called", __FUNCTION__);
	
	try
	{
		// If Nest has been initialized, run cleanup
		if(this->_nestPreparedFlag && this->_pyNest.has_key("Cleanup"))
		{
			this->_nestPreparedFlag = false;
			this->_pyNest["Cleanup"]();
		}

		// Shutdown any running threads
		this->_shutdownFlag = true;
		this->shutdownServer();
	}
	catch(python::error_already_set &)
	{
		// If an error occured, print the error
		NRPException::logCreate("Nest JSON python shutdown failure: " + handle_pyerror());
	}
	catch(std::exception &e)
	{
		NRPException::logCreate(e, "Nest JSON python failure");
	}
}

bool NestJSONServer::initRunFlag() const
{
	return this->_initRunFlag;
}

bool NestJSONServer::shutdownFlag() const
{
	return this->_shutdownFlag;
}

SimulationTime NestJSONServer::runLoopStep(SimulationTime timeStep)
{
	NRP_LOGGER_TRACE("{} called", __FUNCTION__);

	PythonGILLock lock(this->_pyGILState, true);

	try
	{
		const double runTimeMsRounded = getRoundedRunTimeMs(timeStep, python::extract<double>(this->_pyNest["GetKernelStatus"]("resolution")));

		// Commented out in the context of https://hbpneurorobotics.atlassian.net/browse/NRRPLT-8209
		// this->_pyNest["Run"](runTimeMsRounded);
		this->_pyNest["Simulate"](runTimeMsRounded);

		// The time field of dictionary returned from GetKernelStatus contains time in milliseconds
		return toSimulationTime<float, std::milli>(python::extract<float>(this->_pyNest["GetKernelStatus"]("biological_time")));
	}
	catch(python::error_already_set &)
	{
		// If an error occured, print the error
		throw NRPException::logCreate("Failed to run Nest step: " + handle_pyerror());
	}
}

nlohmann::json NestJSONServer::initialize(const nlohmann::json &data, EngineJSONServer::lock_t&)
{
	NRP_LOGGER_TRACE("{} called", __FUNCTION__);

	_initData = data;

	PythonGILLock lock(this->_pyGILState, true);
	try
	{
		// Import modules
		NRPLogger::debug("NestJSONServer: importing nest module...");
		python::object nestModule = python::import("nest");
		this->_pyNest = python::dict(nestModule.attr("__dict__"));

		python::object nrpNestModule = python::import(NRP_NEST_PYTHON_MODULE_STR);
		this->_pyNRPNest = python::dict(nrpNestModule.attr("__dict__"));

		this->_pyGlobals["nest"] = nestModule;
		this->_pyGlobals[NRP_NEST_PYTHON_MODULE_STR] = nrpNestModule;
		NRPLogger::debug("NestJSONServer: importing nest module is finished");
	}
	catch(python::error_already_set &)
	{
		// If an error occured, return the message to the NRP server without setting the initRunFlag
		const auto msg = handle_pyerror();
		NRPLogger::error(msg);
		return this->formatInitErrorMessage(msg);
	}

	// Empty device mapping
	this->_devMap.clear();

	// Read init file if present
	const std::string &initFileName = data.at("NestInitFileName");
	NRPLogger::debug("NestJSONServer: reading init file: {}", initFileName);
	if(!initFileName.empty())
	{
		std::fstream initFile(initFileName, std::ios_base::in);
		if(!initFile.good())
		{
			NRPLogger::error("Could not find init file {}", initFileName);
			return this->formatInitErrorMessage("Could not find init file " + initFileName);
		}

		// Execute Init File
		try
		{
			python::exec_file(python::str(initFileName), this->_pyGlobals, this->_pyGlobals);
		}
		catch(python::error_already_set &)
		{
			// If an error occured, return the message to the NRP server without setting the initRunFlag
			const auto msg = handle_pyerror();
			NRPLogger::error(msg);
			return this->formatInitErrorMessage(msg);
		}

		initFile.close();
	}

	nlohmann::json jsonDevMap;
	try
	{
		NRPLogger::debug("NestJSONServer: registering devices");
		// Read device map
		python::dict jsonModule = static_cast<python::dict>(python::import("json").attr("__dict__"));
		python::object jsonSerialize = jsonModule["dumps"];

		const std::string jsonStr = python::extract<std::string>(jsonSerialize(this->_devMap));
		jsonDevMap = nlohmann::json::parse(jsonStr);

		// Register devices
		this->_devMap = python::dict(this->_pyNRPNest["GetDevMap"]());
		python::list devMapKeys = this->_devMap.keys();
		const long numDevices = python::len(devMapKeys);
		//const long numDevices = python::len(this->_pyNRPNest["GetDevMap"]());
		for(long i=0; i < numDevices; ++i)
		{
			const python::object &devKey = devMapKeys[i];
			const std::string devName = python::extract<std::string>(python::str(devKey));
			python::object devNodes = this->_devMap[devKey];
			NRPLogger::debug("NestJSONServer: registering device {:d} {}", i, devName);

			auto devController = std::shared_ptr<NestEngineJSONDeviceController<NestDevice> >(new
			            NestEngineJSONDeviceController<NestDevice>(DeviceIdentifier(devName, data.at("EngineName"), NestDevice::TypeName.data()),
												 devNodes, this->_pyNest));

			this->_deviceControllerPtrs.push_back(devController);
			this->registerDeviceNoLock(devName, devController.get());
			NRPLogger::debug("NestJSONServer: device {:d} {} is registered", i, devName);
		}

		// Prepare Nest for execution
        // Commented out in the context of https://hbpneurorobotics.atlassian.net/browse/NRRPLT-8209
		// this->_pyNest["Prepare"]();
		// this->_nestPreparedFlag = true;
	}
	catch(python::error_already_set &)
	{
		// If an error occured, print the error
		const auto msg = handle_pyerror();
		NRPLogger::error(msg);
		return this->formatInitErrorMessage(msg);
	}

	// Init has run once
	this->_initRunFlag = true;

	NRPLogger::debug("NestJSONServer::initialize(...) completed with no errors.");

	// Return success and parsed devmap
	return nlohmann::json({{NestConfigConst::InitFileExecStatus, true}, {NestConfigConst::InitFileParseDevMap, jsonDevMap}});
}

nlohmann::json NestJSONServer::reset(EngineJSONServer::lock_t &simLock)
{
	NRP_LOGGER_TRACE("{} called", __FUNCTION__);

	PythonGILLock lock(this->_pyGILState, true);

	if (!this->initRunFlag())
	{
		return nlohmann::json({{NestConfigConst::ResetExecStatus, false}, {NestConfigConst::ErrorMsg, "Cannot reset non-initialized instance"}});
	}

	try
	{	
		this->shutdown(_initData);

		// Revert _shutdownFlag in order the surver could survive
		this->_shutdownFlag = false;
		
		this->initialize(_initData, simLock);

		return nlohmann::json({{NestConfigConst::ResetExecStatus, true}});
	}
	catch (python::error_already_set &)
	{
		const auto msg = handle_pyerror();
		NRPLogger::error("Failed to reset NEST instance: {}", msg);
		return nlohmann::json({{NestConfigConst::ResetExecStatus, false}, {NestConfigConst::ErrorMsg, msg}});
	}
}

nlohmann::json NestJSONServer::shutdown(const nlohmann::json &)
{
	NRP_LOGGER_TRACE("{} called", __FUNCTION__);
	
	PythonGILLock lock(this->_pyGILState, true);

	this->_shutdownFlag = true;

	this->_pyNest["ResetKernel"]();

	// TODO: _nestPreparedFlag can't be tru currently.
	// Probably, due to the context of https://hbpneurorobotics.atlassian.net/browse/NRRPLT-8209
	if(this->_nestPreparedFlag)
	{
		this->_nestPreparedFlag = false;
		this->_pyNest["Cleanup"]();
	}

	// Remove device controllers
	this->clearRegisteredDevices();
	this->_deviceControllerPtrs.clear();

	return nlohmann::json();
}

nlohmann::json NestJSONServer::formatInitErrorMessage(const std::string &errMsg)
{
	return nlohmann::json({{NestConfigConst::InitFileExecStatus, 0}, {NestConfigConst::ErrorMsg, errMsg}});
}

nlohmann::json NestJSONServer::getDeviceData(const nlohmann::json &reqData)
{
	PythonGILLock lock(this->_pyGILState, true);
	return this->EngineJSONServer::getDeviceData(reqData);
}

nlohmann::json NestJSONServer::setDeviceData(const nlohmann::json &reqData)
{
	PythonGILLock lock(this->_pyGILState, true);
	return this->EngineJSONServer::setDeviceData(reqData);
}
