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

#include "nrp_communication_controller/nrp_communication_plugin.h"

#include "nrp_gazebo_grpc_engine/engine_server/nrp_communication_controller.h"
#include "nrp_grpc_engine_protocol/engine_server/engine_grpc_opts_parser.h"

void gazebo::NRPCommunicationPlugin::Load(int argc, char **argv)
{
    NRP_LOGGER_TRACE("{} called", __FUNCTION__);

    NRPLogger::info("NRP Communication plugin: Initializing...");

    std::string serverAddr, engineName, registrationAddr;
    try
    {
        // Parse options from input
        auto inputArgsParse = EngineGRPCOptsParser::parseOpts(argc, argv, EngineGRPCOptsParser::createOptionParser(true));

        // Save given URL
        serverAddr = inputArgsParse[EngineGRPCConfigConst::EngineServerAddrArg.data()].as<std::string>();
        engineName = inputArgsParse[EngineGRPCConfigConst::EngineNameArg.data()].as<std::string>();
        // TODO: remove registrationAddr parameter everywhere
        // registrationAddr = inputArgsParse[EngineGRPCConfigConst::EngineRegistrationServerAddrArg.data()].as<std::string>();
        registrationAddr = "";
    }
    catch(cxxopts::OptionException &e)
    {
        NRPLogger::error("Failed to parse options [ {} ]", e.what());

        throw;
    }

    // Create server with given URL
    auto &newController = NRPCommunicationController::resetInstance(serverAddr, engineName, registrationAddr);

    // Save bound URL
    this->_serverAddress = newController.serverAddress();
    NRPLogger::info("gazebo::NRPCommunicationPlugin::Load: starting server on {}", this->_serverAddress);

    // Start the server
    newController.startServerAsync();

    NRPLogger::info("gazebo::NRPCommunicationPlugin::Load: Server started. Waiting for input...");
}

void gazebo::NRPCommunicationPlugin::Reset()
{
    NRP_LOGGER_TRACE("{} called", __FUNCTION__);
    
    // Reset server
    NRPLogger::info("NRP Communication plugin: Resetting controller...");
    auto &newController = NRPCommunicationController::resetInstance(this->_serverAddress);

    // Start server
    newController.startServerAsync();

    NRPLogger::info("NRP Communication plugin: Server restarted. Waiting for input...");
}
