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

#include "nrp_communication_controller/nrp_communication_plugin.h"

#include "nrp_gazebo_json_engine/engine_server/nrp_communication_controller.h"
#include "nrp_json_engine_protocol/engine_server/engine_json_opts_parser.h"
#include "nrp_general_library/utils/nrp_exceptions.h"

void gazebo::NRPCommunicationPlugin::Load(int argc, char **argv)
{
    NRP_LOGGER_TRACE("{} called", __FUNCTION__);

    NRPLogger::info("NRP Communication plugin: Initializing...");

    std::string serverAddr, engineName, registrationAddr;
    try
    {
        // Parse options from input
        auto inputArgsParse = EngineJSONOptsParser::parseOpts(argc, argv, EngineJSONOptsParser::createOptionParser(true));

        // Save given URL
        serverAddr = inputArgsParse[EngineJSONConfigConst::EngineServerAddrArg.data()].as<std::string>();
        engineName = inputArgsParse[EngineJSONConfigConst::EngineNameArg.data()].as<std::string>();
        registrationAddr = inputArgsParse[EngineJSONConfigConst::EngineRegistrationServerAddrArg.data()].as<std::string>();
    }
    catch(cxxopts::OptionException &e)
    {
        throw NRPException::logCreate(e, "NRP Communication plugin:: Failed to parse options");
    }
    catch(std::domain_error &e)
    {
        throw NRPException::logCreate(e, "NRP Communication plugin: Failed to parse options");
    }

    // Create server with given URL
    auto &newController = NRPJSONCommunicationController::resetInstance(serverAddr, engineName, registrationAddr);

    // Save server parameters
    this->_serverAddress = newController.serverAddress();
    this->_engineName = engineName;
    this->_registrationAddress = registrationAddr;
    NRPLogger::info("NRP Communication plugin: Starting server on {}", this->_serverAddress);

    // Start the server
    newController.startServerAsync();

    NRPLogger::info("NRP Communication plugin: Server started. Waiting for input...");
}

void gazebo::NRPCommunicationPlugin::Reset()
{
    NRP_LOGGER_TRACE("{} called", __FUNCTION__);
    
    // Reset server
    NRPLogger::info("NRP Communication plugin: Resetting controller...");
    auto &newController = NRPJSONCommunicationController::resetInstance(this->_serverAddress, this->_engineName, this->_registrationAddress);

    // Start server
    newController.startServerAsync();

    NRPLogger::info("NRP Communication plugin: Server restarted. Waiting for input...");
}
