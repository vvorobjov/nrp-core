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

#ifndef ENGINE_JSON_NRP_CLIENT_H
#define ENGINE_JSON_NRP_CLIENT_H

#include "nrp_general_library/engine_interfaces/engine_client_interface.h"
#include "nrp_json_engine_protocol/config/engine_json_config.h"
#include "nrp_json_engine_protocol/nrp_client/engine_json_registration_server.h"
#include "nrp_json_engine_protocol/device_interfaces/json_device.h"
#include "nrp_general_library/utils/nrp_exceptions.h"
#include "nrp_general_library/utils/restclient_setup.h"

#include <nlohmann/json.hpp>
#include <list>

#include <chrono>

#include <iostream>
#include <restclient-cpp/restclient.h>

/*!
 *  \brief NRP - Gazebo Communicator on the NRP side. Converts DeviceInterface classes from/to JSON objects
 *  \tparam ENGINE_INTERFACE Class derived from GeneralInterface. Currently either PhysicsInterface or BrainInterface
 */
template<class ENGINE, const char* SCHEMA>
class EngineJSONNRPClient
        : public EngineClient<ENGINE, SCHEMA>
{
	public:
		/*!
		 * \brief Constructor
		 * \param config Engine Config
		 * \param launcher Process launcher
		 */
		EngineJSONNRPClient(nlohmann::json &config, ProcessLauncherInterface::unique_ptr &&launcher)
		    : EngineClient<ENGINE, SCHEMA>(config, std::move(launcher)),
		      _serverAddress(this->engineConfig().at("ServerAddress"))
		{
			NRP_LOGGER_TRACE("{} called", __FUNCTION__);

			RestClientSetup::ensureInstance();
		}

		/*!
		 * \brief Constructor
		 * \param serverAddress Server Address to send requests to
		 * \param config Engine Config
		 * \param launcher Process launcher
		 */
		EngineJSONNRPClient(const std::string &serverAddress, nlohmann::json &config, ProcessLauncherInterface::unique_ptr &&launcher)
		    : EngineClient<ENGINE, SCHEMA>(config, std::move(launcher)),
		      _serverAddress(serverAddress)
		{
			NRP_LOGGER_TRACE("{} called", __FUNCTION__);

            this->engineConfig()["ServerAddress"] = this->_serverAddress;
			RestClientSetup::ensureInstance();
		}

		virtual ~EngineJSONNRPClient() override = default;

		virtual pid_t launchEngine() override
		{
			NRP_LOGGER_TRACE("{} called", __FUNCTION__);

			// Launch engine process.
			// enginePID == -1 means that the launcher hasn't launched a process
			auto enginePID = this->EngineClientInterface::launchEngine();

			// Wait for engine to register itself if process launching has succeeded
			if(enginePID > 0 && !this->engineConfig().at("RegistrationServerAddress").empty())
			{
				const auto serverAddr = this->waitForRegistration(20, 1);
				if(serverAddr.empty())
					throw NRPException::logCreate("Error while waiting for engine \"" + this->engineName() + "\" to register its address. Did not receive a reply");

                this->engineConfig()["ServerAddress"] = serverAddr;
				this->_serverAddress = serverAddr;
			}

			return enginePID;
		}

		virtual void sendDevicesToEngine(const typename EngineClientInterface::devices_ptr_t &devicesArray) override
		{
			NRP_LOGGER_TRACE("{} called", __FUNCTION__);

			// Convert devices to JSON format
			nlohmann::json request;
			for(const auto &curDevice : devicesArray)
			{
				if(curDevice->engineName().compare(this->engineName()) == 0)
				{
					if(curDevice->type() != JsonDevice::getType())
					{
						throw NRPException::logCreate("Engine \"" +
													this->engineName() +
													"\" cannot handle device type '" +
													curDevice->type() + "'");
					}

					// We get ownership of the device's data
					// We'll have to delete the object after we're done

					nlohmann::json * data = (dynamic_cast<JsonDevice *>(curDevice))->releaseData();

					const auto & name = curDevice->name();

					request[name]["engine_name"] = curDevice->engineName();
					request[name]["type"]        = curDevice->type();
					request[name]["data"].swap(*data);

					delete data;
				}
			}

			// Send updated devices to Engine JSON server
			EngineJSONNRPClient::sendRequest(this->_serverAddress + "/" + EngineJSONConfigConst::EngineServerSetDevicesRoute.data(),
			                                 EngineJSONConfigConst::EngineServerContentType.data(), request.dump(),
			                                 "Engine server \"" + this->engineName() + "\" failed during device handling");

			// TODO: Check if engine has processed all sent devices
		}

        virtual const std::vector<std::string> engineProcStartParams() const override
        {
			NRP_LOGGER_TRACE("{} called", __FUNCTION__);

            std::vector<std::string> startParams = this->engineConfig().at("EngineProcStartParams");

            std::string name = this->engineConfig().at("EngineName");
            startParams.push_back(std::string("--") + EngineJSONConfigConst::EngineNameArg.data() + "=" + name);

            // Add JSON Server address (will be used by EngineJSONServer)
            std::string address = this->engineConfig().at("ServerAddress");
            startParams.push_back(std::string("--") + EngineJSONConfigConst::EngineServerAddrArg.data() + "=" + address);

            // Add JSON registration Server address (will be used by EngineJSONServer)
            std::string reg_address = this->engineConfig().at("RegistrationServerAddress");
            startParams.push_back(std::string("--") + EngineJSONConfigConst::EngineRegistrationServerAddrArg.data() + "=" + reg_address);

            return startParams;
        }

        virtual const std::vector<std::string> engineProcEnvParams() const override
        {
            return this->engineConfig().at("EngineEnvParams");
        }

	protected:
		virtual typename EngineClientInterface::devices_set_t getDevicesFromEngine(const typename EngineClientInterface::device_identifiers_set_t &deviceIdentifiers) override
		{
			NRP_LOGGER_TRACE("{} called", __FUNCTION__);

			nlohmann::json request;
			for(const auto &devID : deviceIdentifiers)
			{
				if(this->engineName().compare(devID.EngineName) == 0)
				{
					request[devID.Name] = {{"engine_name", devID.EngineName}, {"type", devID.Type}};
				}

			}

			// Post request to Engine JSON server
			const auto resp(EngineJSONNRPClient::sendRequest(this->_serverAddress + "/" + EngineJSONConfigConst::EngineServerGetDevicesRoute.data(),
			                                                 EngineJSONConfigConst::EngineServerContentType.data(), request.dump(),
			                                                 "Engine server \"" + this->engineName() + "\" failed during device retrieval"));

			return this->getDeviceInterfacesFromJSON(resp);
		}

		/*!
		 * \brief Send an initialization command
		 * \param data Data that should be passed to the engine
		 * \return Returns init data from engine
		 */
		nlohmann::json sendInitCommand(const nlohmann::json &data)
		{
			NRP_LOGGER_TRACE("{} called", __FUNCTION__);

			NRPLogger::debug("EngineJSONNRPClient::sendInitCommand [ data: {} ]", data.dump());

			// Post init request to Engine JSON server
			return sendRequest(this->_serverAddress + "/" + EngineJSONConfigConst::EngineServerInitializeRoute.data(),
			                   EngineJSONConfigConst::EngineServerContentType.data(), data.dump(),
			                   "Engine server \"" + this->engineName() + "\" failed during initialization");
		}

		/*!
		 * \brief Send a reset command
		 * \param data Data that should be passed to the engine
		 * \return Returns reset data from engine
		 */
		nlohmann::json sendResetCommand(const nlohmann::json &data)
		{
			NRP_LOGGER_TRACE("{} called", __FUNCTION__);
			
			NRPLogger::debug("EngineJSONNRPClient::sendResetCommand [ data: {} ]", data.dump());

			// Post reset request to Engine JSON server
			return sendRequest(this->_serverAddress + "/" + EngineJSONConfigConst::EngineServerResetRoute.data(),
			                   EngineJSONConfigConst::EngineServerContentType.data(), data.dump(),
			                   "Engine server \"" + this->engineName() + "\" failed during reset");
		}

		/*!
		 * \brief Send a shutdown command
		 * \param data Data that should be passed to the engine
		 * \return Returns init data from engine
		 */
		nlohmann::json sendShutdownCommand(const nlohmann::json &data)
		{
			NRP_LOGGER_TRACE("{} called", __FUNCTION__);

			NRPLogger::debug("EngineJSONNRPClient::sendShutdownCommand [ data: {} ]", data.dump());

			// Post init request to Engine JSON server
			return sendRequest(this->_serverAddress + "/" + EngineJSONConfigConst::EngineServerShutdownRoute.data(),
			                   EngineJSONConfigConst::EngineServerContentType.data(), data.dump(),
			                   "Engine server \"" + this->engineName() + "\" failed during shutdown");
		}

		/*!
		 * \brief Wait for the engine registration server to receive a call from the engine
		 * \param numTries Number of times to check the registration server for an address
		 * \param waitTime Wait time (in seconds) between checks
		 * \return Returns engine server address if present, empty string otherwise
		 */
		std::string waitForRegistration(unsigned int numTries, unsigned int waitTime) const
		{
			NRP_LOGGER_TRACE("{} called", __FUNCTION__);

			auto *pRegistrationServer = EngineJSONRegistrationServer::getInstance();
			if(pRegistrationServer == nullptr)
				pRegistrationServer = EngineJSONRegistrationServer::resetInstance(this->engineConfig().at("RegistrationServerAddress"));

			if(!pRegistrationServer->isRunning())
				pRegistrationServer->startServerAsync();

			// Try to retrieve engine address
			auto engineAddr = pRegistrationServer->requestEngine(this->engineName());

			while(engineAddr.empty() && numTries > 0)
			{
				// Continue to wait for engine address for 20s
				sleep(waitTime);
				engineAddr = pRegistrationServer->retrieveEngineAddress(this->engineName());
				--numTries;
			}

			// Close server if no additional clients are waiting for their engines to register
			if(pRegistrationServer->getNumWaitingEngines() == 0)
				EngineJSONRegistrationServer::clearInstance();

			return engineAddr;
		}

	private:

		/*!
		 * \brief Server Address to send requests to
		 */
		std::string _serverAddress;

		/*!
		 * \brief Send a request to the Server
		 * \param serverName Name of the server
		 * \param contentType Content Type
		 * \param request Body of request
		 * \param exceptionMessage Message to put into exception output
		 * \return Returns body of response, parsed as JSON
		 */
		static nlohmann::json sendRequest(const std::string &serverName, const std::string &contentType, const std::string &request, const std::string_view &exceptionMessage)
		{
			// Post request to Engine JSON server
			try
			{
				auto resp = RestClient::post(serverName, contentType, request);
				if(resp.code != 200)
				{
					throw std::domain_error(exceptionMessage.data());
				}

				return nlohmann::json::parse(resp.body);
			}
			catch(std::exception &e)
			{
				throw NRPException::logCreate(e, "Communication with engine server failed");
			}
		}

		/*!
		 * \brief Thread function that executes the loop and waits for a result from the engine
		 * \param timeStep Time (in seconds) to execute the engine
		 * \return Returns current time of engine
		 */
		SimulationTime runLoopStepCallback(SimulationTime timeStep) override
		{
			nlohmann::json request;
			request[EngineJSONConfigConst::EngineTimeStepName.data()] = timeStep.count();

			// Post run loop request to Engine JSON server
			nlohmann::json resp(EngineJSONNRPClient::sendRequest(this->_serverAddress + "/" + EngineJSONConfigConst::EngineServerRunLoopStepRoute.data(),
			                                                     EngineJSONConfigConst::EngineServerContentType.data(), request.dump(),
			                                                     "Engine Server failed during loop execution"));

			// Get engine time from response
			SimulationTime engineTime;
			try
			{
				engineTime = SimulationTime(resp[EngineJSONConfigConst::EngineTimeName.data()]);
			}
			catch(std::exception &e)
			{
				throw NRPException::logCreate(e, "Error while parsing the return value of the run_step of \"" + this->engineName() + "\"");
			}

			if(engineTime < SimulationTime::zero())
				throw NRPException::logCreate("Error during execution of engine \"" + this->engineName() + "\"");

			return engineTime;
		}

		/*!
		 * \brief Extracts devices from given JSON
		 * \param devices JSON data of devices
		 * \return Returns list of devices
		 */
		typename EngineClientInterface::devices_set_t getDeviceInterfacesFromJSON(const nlohmann::json &devices) const
		{
			NRP_LOGGER_TRACE("{} called", __FUNCTION__);

			typename EngineClientInterface::devices_set_t interfaces;

			for(auto curDeviceIterator = devices.begin(); curDeviceIterator != devices.end(); ++curDeviceIterator)
			{
				try
				{
					DeviceIdentifier deviceID(curDeviceIterator.key(),
											  (*curDeviceIterator)["engine_name"],
											  (*curDeviceIterator)["type"]);

					deviceID.EngineName = this->engineName();
					interfaces.insert(this->getSingleDeviceInterfaceFromJSON(curDeviceIterator, deviceID));
				}
				catch(std::exception &e)
				{
					// TODO: Handle json device parsing error
					throw NRPException::logCreate(e, "Failed to parse JSON Device Interface");
				}
			}

			return interfaces;
		}

		/*!
		 * \brief Go through given DEVICES and try to create a DeviceInterface from the JSON object
		 * \tparam DEVICE DeviceInterface Class to check
		 * \tparam REMAINING_DEVICES Remaining DeviceInterface Classes to check
		 * \param deviceData Device data as JSON object
		 * \param deviceID ID of device
		 * \return Returns pointer to created device
		 */

		inline DeviceInterfaceConstSharedPtr getSingleDeviceInterfaceFromJSON(const nlohmann::json::const_iterator &deviceData, DeviceIdentifier &deviceID) const
		{
			NRP_LOGGER_TRACE("{} called", __FUNCTION__);

            if(deviceID.Type == JsonDevice::getType())
            {
				// Check whether the requested device has new data
				// A device that has no data will contain two JSON objects with "engine_name" and "type" keys (parts of device ID)

				if(deviceData->size() == 2)
				{
					// There's no meaningful data in the device, so create an empty device with device ID only

					return DeviceInterfaceSharedPtr(new DeviceInterface(std::move(deviceID)));
				}

                nlohmann::json * data = new nlohmann::json(std::move((*deviceData)["data"]));

                DeviceInterfaceSharedPtr newDevice(new JsonDevice(deviceID.Name, deviceID.EngineName, data));
                newDevice->setEngineName(this->engineName());

                return newDevice;
            }
			else
			{
				throw NRPException::logCreate("Device type \"" + deviceID.Type + "\" cannot be handled by the \"" + this->engineName() + "\" engine");
			}
		}
};


#endif //ENGINE_JSON_NRP_CLIENT_H
