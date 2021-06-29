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

#include "nrp_nest_server_engine/nrp_client/nest_engine_server_nrp_client.h"

#include "nrp_general_library/utils/nrp_exceptions.h"
#include "nrp_general_library/utils/restclient_setup.h"
#include "nrp_nest_server_engine/config/cmake_constants.h"
#include "nrp_general_library/config/cmake_constants.h"
#include "nrp_general_library/utils/utils.h"

#include <chrono>
#include <fstream>
#include <nlohmann/json.hpp>

/*!
 * \brief Anonymous namespace containing helper functions
 */
namespace
{
	/*!
	 * \brief Returns source of the brain file as string
	 *
	 * The source is supposed to be send to the NEST server, which will execute it.
	 *
	 * This is a helper function of initialize().
	 *
	 * \param brainFileName Name of the initialization file
	 * \return Source code of brain file as string
	 */
	std::string readBrainFile(const std::string & brainFileName)
	{
		NRP_LOGGER_TRACE("{} called", __FUNCTION__);

		std::string initCode;
		{
			std::ifstream initFile(brainFileName);
			initCode.assign((std::istreambuf_iterator<char>(initFile) ),
							(std::istreambuf_iterator<char>()		) );
		}

		return initCode;
	}

	/*!
	 * \brief Checks if engine process is still alive
	 *
	 * This is a helper function of initialize().
	 *
	 * \param process Pointer to process object
	 * \throws NRPException When process has stopped
	 */
	void checkProcessStatus(const ProcessLauncherInterface::unique_ptr & process)
	{
		if(process->getProcessStatus() == ProcessLauncherInterface::ENGINE_RUNNING_STATUS::STOPPED)
		{
			throw NRPException::logCreate("Nest Engine process stopped unexpectedly before the init file could be sent");
		}
	}

	/*!
	 * \brief Checks if device can be processed by the engine
	 *
	 * \param[in] device     Device ID structure to be verified
	 * \param[in] engineName Name of the current engine
	 *
	 * \return True if device may be processed by the engine, false otherwise
	 */
	bool isDeviceTypeValid(const DeviceIdentifier & device, const std::string & engineName)
	{
		// Check if type matches the NestServerDevice type
		// Skip the check if type was not set

		const bool isTypeValid = (device.Type.empty() ||
								device.Type == NestServerDevice::TypeName.m_data);

		return (isTypeValid && device.EngineName == engineName);
	}

	/*!
 	 * \brief Returns command execution timeout as SimulationTime
     *
	 * This is a helper function of initialize().
	 *
	 * \return Command execution timeout for this engine
	 */
	SimulationTime processCommandTimeout(const float commandTimeout)
	{
		auto timeout = SimulationTime::max();

		if(commandTimeout > 0.0f)
		{
			timeout = toSimulationTime<float, std::ratio<1,1> >(commandTimeout);
		}

		return timeout;
	}

	/*!
	 * \brief Extracts populations from NEST server response to brain file execution
	 *
	 * The NEST server is supposed to return a dictionary names 'populations'. The dictionary
	 * should contain a mapping of population (device) names to lists of IDs.
	 *
	 * This is a helper function of initialize().
	 *
	 * \param[in]  data  	   Body part of the response from NEST server
	 * \param[out] populations Populations mapping structure that will be filled by the function
	 */
	void extractPopulations(const std::string & data, NestEngineServerNRPClient::population_mapping_t * populations)
	{
		auto respJson = nlohmann::json::parse(data);
		for (nlohmann::json::iterator it = respJson["data"]["populations"].begin(); it != respJson["data"]["populations"].end(); ++it)
		{
			populations->insert({it.key(), it.value().dump()});
		}
	}

	/*!
	 * \brief Generic REST call function
	 *
	 * \param url   URL to query
	 * \param ctype Content type as string
	 * \param data  HTTP POST body
	 * \throws NRPException On response code different from 200
	 * \return Response body as string
	 */
	std::string nestGenericCall(const std::string & url, const std::string & ctype, const std::string & data)
	{
		auto resp = RestClient::post(url, ctype, data);

		if(resp.code != 200)
		{
			throw NRPException("REST call to \"" + url + "\" failed with code " + std::to_string(resp.code));
		}

		return resp.body;
	}

	/*!
	 * \brief Sends SetStatus request to NEST server
	 *
	 * \param serverAddress Address of the NEST server
	 * \param data Arguments for SetStatus
	 */
	void nestSetStatus(const std::string & serverAddress, const std::string & data)
	{
		nestGenericCall(serverAddress + "/api/SetStatus", "application/json", data);
	}

	/*!
	 * \brief Sends GetStatus request to NEST server
	 *
	 * \param serverAddress Address of the NEST server
	 * \param data Arguments to GetStatus
	 * \return Response from GetStatus as string
	 */
	std::string nestGetStatus(const std::string & serverAddress, const std::string & data)
	{
		return nestGenericCall(serverAddress + "/api/GetStatus", "application/json", data);
	}

    /*!
     * \brief Sends Run request to NEST server
     *
     * \param serverAddress Address of the NEST server
     * \param timeStep Step of the simulation in milliseconds
     */
    void nestSimulate(const std::string & serverAddress, const float timeStep)
    {
        nestGenericCall(serverAddress + "/api/Simulate", "application/json", "[" + std::to_string(timeStep) + "]");
    }

	/*!
	 * \brief Sends GetKernelStatus request to NEST server
	 *
	 * \param serverAddress Address of the NEST server
	 * \param data Arguments for GetKernelStatus
	 * \return Response from GetKernelStatus as string
	 */
	std::string nestGetKernelStatus(const std::string & serverAddress, const std::string & data)
	{
		return nestGenericCall(serverAddress + "/api/GetKernelStatus", "application/json", data);
	}

	/*!
	 * \brief Sends exec request to NEST server
	 *
	 * \param serverAddress Address of the NEST server
	 * \param data Python code to be executed by the server
	 */
	std::string nestExec(const std::string & serverAddress, const std::string & data)
	{
		return nestGenericCall(serverAddress + "/exec", "application/json", data);
	}
}

NestEngineServerNRPClient::NestEngineServerNRPClient(nlohmann::json &config, ProcessLauncherInterface::unique_ptr &&launcher)
    : EngineClient(config, std::move(launcher))
{
    setDefaultProperty<std::string>("EngineProcCmd", NRP_NEST_SERVER_EXECUTABLE_PATH);
    if(!this->engineConfig().contains("NestServerPort"))
        setDefaultProperty<int>("NestServerPort", findUnboundPort(this->PortSearchStart));

    RestClientSetup::ensureInstance();

	this->_serverAddress = this->engineConfig().at("NestServerHost").get<std::string>() + ":" +
	        std::to_string(this->engineConfig().at("NestServerPort").get<int>());
}

NestEngineServerNRPClient::~NestEngineServerNRPClient()
{}

void NestEngineServerNRPClient::initialize()
{
	NRP_LOGGER_TRACE("{} called", __FUNCTION__);

	const auto initCode    = readBrainFile(this->engineConfig().at("NestInitFileName"));
	const auto timeout     = processCommandTimeout(this->engineConfig().at("EngineCommandTimeout"));
	const auto startTime   = std::chrono::system_clock::now();
	bool 	   initSuccess = false;

	// Try sending data to given address. Continue until timeout
	do
	{
		// Check that process is still running

		checkProcessStatus(this->_process);

		// Try sending and executing initFile contents

		std::string response;
		try
		{
			response = nestExec(this->serverAddress(), nlohmann::json({{"source", initCode}, {"return", {"populations"}}}).dump());
		}
		catch(NRPException &e)
		{
			throw NRPException::logCreate(e, "Failed to execute init file \"" + this->engineConfig().at("NestInitFileName").get<std::string>() +
			        "\": " + response);
		}
		catch(std::exception &e)
		{
			// Server unreachable, retry if server still starting up

			std::this_thread::sleep_for(std::chrono::milliseconds(10));
			continue;
		}

		// Extract all populations from the response and cache them in the populations map

		extractPopulations(response, &this->_populations);

		// Exit the loop

		initSuccess = true;
		break;
	}
	while(std::chrono::system_clock::now() - startTime > timeout);

	// Check if timeout reached

	if(!initSuccess)
	{
		throw NRPException::logCreate("Failed to initialize Nest server. Received no response before timeout reached");
	}
	else
	{
		NRPLogger::info("Nest server is initialized.");
	}

	// Get simulation resolution and cache it

	this->_simulationResolution = std::stof(nestGetKernelStatus(this->serverAddress(), "[\"resolution\"]"));

	NRPLogger::debug("NestEngineServerNRPClient::initialize(...) completed with no errors.");
}

void NestEngineServerNRPClient::shutdown()
{
	NRP_LOGGER_TRACE("{} called", __FUNCTION__);
	// Empty
}

SimulationTime NestEngineServerNRPClient::getEngineTime() const
{
	return toSimulationTime<float, std::milli>(std::stof(nestGetKernelStatus(this->serverAddress(), "[\"time\"]")));
}

void NestEngineServerNRPClient::runLoopStep(SimulationTime timeStep)
{
	NRP_LOGGER_TRACE("{} called", __FUNCTION__);

	this->_runStepThread = std::async(std::launch::async, &NestEngineServerNRPClient::runStepFcn, this, timeStep);
}

void NestEngineServerNRPClient::waitForStepCompletion(float timeOut)
{
	NRP_LOGGER_TRACE("{} called", __FUNCTION__);

	// If thread state is invalid, loop thread has completed and waitForStepCompletion was called once before
	if(!this->_runStepThread.valid())
		return;

	if(timeOut > 0)
	{
		if(this->_runStepThread.wait_for(std::chrono::duration<double>(timeOut)) != std::future_status::ready)
			throw NRPException::logCreate("Nest loop still running after timeout reached");
	}
	else
	{
		this->_runStepThread.wait();
	}

	// runStepFcn doesn't return engine time, like in other engines. Only status of REST call is returned

	if(!this->_runStepThread.get())
	{
		throw NRPException::logCreate("Nest loop failed unexpectedly");
	}
}

const std::string & NestEngineServerNRPClient::getDeviceIdList(const std::string & deviceName) const
{
	NRP_LOGGER_TRACE("{} called", __FUNCTION__);

	// Check if the device is in the populations map

	if(this->_populations.count(deviceName) == 0)
	{
		throw NRPException::logCreate("Device \"" + deviceName + "\" not in populations map");
	}

	// Get list of device IDs mapped to this device name

	return this->_populations.at(deviceName);
}

EngineClientInterface::devices_set_t NestEngineServerNRPClient::getDevicesFromEngine(const device_identifiers_set_t &deviceIdentifiers)
{
	NRP_LOGGER_TRACE("{} called", __FUNCTION__);

	EngineClientInterface::devices_set_t retVals;

	for(const auto &devID : deviceIdentifiers)
	{
		if(isDeviceTypeValid(devID, this->engineName()))
		{
			const auto deviceName = devID.Name;
			std::string response;

			// Request status of the IDs from the list

			try
			{
				response = nestGetStatus(this->serverAddress(), "{\"nodes\":" + getDeviceIdList(deviceName) + "}");
			}
			catch(std::exception& e)
			{
				throw NRPException::logCreate(e, "Failed to get NEST status for device \"" + deviceName + "\"");
			}

			// Extract device details from the body
			// Response from GetStatus is an array of JSON objects, which contains device parameters

			const auto respJson = nlohmann::json::parse(response);
			retVals.emplace(new NestServerDevice(DeviceIdentifier(devID), respJson.dump()));
		}
	}

	return retVals;
}

void NestEngineServerNRPClient::sendDevicesToEngine(const devices_ptr_t &devicesArray)
{
	NRP_LOGGER_TRACE("{} called", __FUNCTION__);

	for(DeviceInterface * const device : devicesArray)
	{
		if(isDeviceTypeValid(device->id(), this->engineName()))
		{
			// Send command along with parameters to nest

			const auto deviceName = device->name();

			std::string setStatusStr = "{\"nodes\":" + getDeviceIdList(deviceName) + ","
			                           "\"params\":" + ((NestServerDevice const *)device)->data().serialize() + "}";

			try
			{
				nestSetStatus(this->serverAddress(), setStatusStr);
			}
			catch(std::exception& e)
			{
				throw NRPException::logCreate(e, "Failed to set NEST status for device\"" + deviceName + "\"");
			}
		}
	}
}

bool NestEngineServerNRPClient::runStepFcn(SimulationTime timeStep)
{
	// According to the NEST API documentation, Run accepts time to simulate in milliseconds and floating-point format

	const double runTimeMsRounded = getRoundedRunTimeMs(timeStep, this->_simulationResolution);

	try
	{
		nestSimulate(this->serverAddress(), runTimeMsRounded);
	}
	catch(const std::exception& e)
	{
		return false;
	}

	return true;
}

std::string NestEngineServerNRPClient::serverAddress() const
{
	return this->_serverAddress;
}

const std::vector<std::string> NestEngineServerNRPClient::engineProcEnvParams() const
{
	NRP_LOGGER_TRACE("{} called", __FUNCTION__);

    std::vector<std::string> envVars = this->engineConfig().at("EngineEnvParams");

    // Add NRP library path
    envVars.push_back("LD_LIBRARY_PATH=" NRP_LIB_INSTALL_DIR ":$LD_LIBRARY_PATH");

    // Disable RestrictedPython
    envVars.push_back("NEST_SERVER_RESTRICTION_OFF=1");

    return envVars;
}

const std::vector<std::string> NestEngineServerNRPClient::engineProcStartParams() const
{
	NRP_LOGGER_TRACE("{} called", __FUNCTION__);
	
    std::vector<std::string> startParams;

    // Add Server address
    int port = this->engineConfig().at("NestServerPort");
    startParams.push_back("start");
    startParams.push_back("-o");
    startParams.push_back("-h"); startParams.push_back(this->engineConfig().at("NestServerHost"));
    startParams.push_back("-p"); startParams.push_back(std::to_string(port));

    return startParams;
}

// EOF