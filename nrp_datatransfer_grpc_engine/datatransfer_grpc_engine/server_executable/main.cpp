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

#include "nrp_grpc_engine_protocol/engine_server/engine_grpc_server.h"
#include "datatransfer_grpc_engine/engine_server/datatransfer_grpc_server.h"
#include "nrp_grpc_engine_protocol/engine_server/engine_grpc_opts_parser.h"

int main(int argc, char * argv[])
{
    // Parse the command line arguments

    cxxopts::ParseResult options = EngineGRPCOptsParser::parseOpts(argc, argv, EngineGRPCOptsParser::createOptionParser());

    // Spawn the server

    const auto serverAddress = options[EngineGRPCConfigConst::EngineServerAddrArg.data()].as<std::string>();
    const auto engineName    = options[EngineGRPCConfigConst::EngineNameArg.data()].as<std::string>();
    const auto protobufPluginsPath = options[EngineGRPCConfigConst::ProtobufPluginsPathArg.data()].as<std::string>();
    const auto protobufPluginsDump = options[EngineGRPCConfigConst::ProtobufPluginsArg.data()].as<std::string>();
    const auto protobufPlugins = nlohmann::json::parse(protobufPluginsDump);

    auto server = EngineGrpcServer(serverAddress, new DataTransferEngine(engineName, protobufPluginsPath, protobufPlugins));

    // Start the server

    server.startServerAsync();

    // Wait for initialization call

    while(!server.initRunFlag() || server.shutdownFlag())
    {
        sleep(0);
    }

    // Continue handling requests until a shutdown command has been received

    while(!server.shutdownFlag())
    {
        sleep(1);
    }

    // Shutdown

    server.shutdownServer();

    return 0;
}
