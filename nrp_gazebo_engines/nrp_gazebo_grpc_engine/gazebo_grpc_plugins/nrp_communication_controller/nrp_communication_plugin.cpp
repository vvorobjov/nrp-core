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

#include "nrp_gazebo_grpc_engine/engine_server/nrp_communication_controller.h"
#include "nrp_grpc_engine_protocol/engine_server/engine_grpc_opts_parser.h"

void gazebo::NRPCommunicationPlugin::Load(int argc, char **argv)
{
    NRP_LOGGER_TRACE("{} called", __FUNCTION__);

    NRPLogger::info("NRP Communication plugin: Initializing...");

    std::string serverAddr, engineName, protobufPluginsPath;
    nlohmann::json protobufPlugins;
    try
    {
        // Parse options from input
        auto inputArgsParse = EngineGRPCOptsParser::parseOpts(argc, argv, EngineGRPCOptsParser::createOptionParser(true));

        // Save given URL
        serverAddr = inputArgsParse[EngineGRPCConfigConst::EngineServerAddrArg.data()].as<std::string>();
        engineName = inputArgsParse[EngineGRPCConfigConst::EngineNameArg.data()].as<std::string>();
        protobufPluginsPath = inputArgsParse[EngineGRPCConfigConst::ProtobufPluginsPathArg.data()].as<std::string>();
        const auto protobufPluginsDump = inputArgsParse[EngineGRPCConfigConst::ProtobufPluginsArg.data()].as<std::string>();
        protobufPlugins = nlohmann::json::parse(protobufPluginsDump);

    }
    catch(cxxopts::OptionException &e)
    {
        NRPLogger::error("NRP Communication plugin: Failed to parse Engine options");

        throw;
    }
    catch(std::domain_error &e)
    {
        NRPLogger::error("NRP Communication plugin: Failed to parse Engine options");

        throw;
    }

    // Create server with given URL
    auto newController = new NRPGazeboCommunicationController(engineName, protobufPluginsPath, protobufPlugins);
    _grpcServer.reset(new EngineGrpcServer(serverAddr, newController));
    CommControllerSingleton::resetInstance(newController);

    // Save the server parameters
    this->_serverAddress = _grpcServer->serverAddress();
    this->_engineName = engineName;
    this->_protobufPluginsPath = protobufPluginsPath;
    this->_protobufPlugins = protobufPlugins;
    NRPLogger::info("gazebo::NRPCommunicationPlugin::Load: starting server {} on {}", this->_engineName, this->_serverAddress);

    // Start the server
    _grpcServer->startServerAsync();

    NRPLogger::info("gazebo::NRPCommunicationPlugin::Load: Server started. Waiting for input...");
}

void gazebo::NRPCommunicationPlugin::Reset()
{
    NRP_LOGGER_TRACE("{} called", __FUNCTION__);
    
    // Reset server
    NRPLogger::info("NRP Communication plugin: Resetting controller...");
    auto newController = new NRPGazeboCommunicationController(this->_engineName, this->_protobufPluginsPath, this->_protobufPlugins);
    _grpcServer.reset(new EngineGrpcServer(this->_serverAddress, newController));
    CommControllerSingleton::resetInstance(newController);

    // Start server
    _grpcServer->startServerAsync();

    NRPLogger::info("NRP Communication plugin: Server restarted. Waiting for input...");
}
