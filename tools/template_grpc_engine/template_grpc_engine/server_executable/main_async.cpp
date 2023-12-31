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

#include "nrp_event_loop/event_loop/event_loop_engine.h"
#include "nrp_event_loop/event_loop/event_loop_engine_opts_parser.h"
#include "nrp_general_library/utils/json_schema_utils.h"
#include "${engine_name_lowercase}_grpc_engine/engine_server/${engine_name_lowercase}_grpc_server.h"
#include "${engine_name_lowercase}_grpc_engine/engine_client/${engine_name_lowercase}_grpc_client.h"
#include "${engine_name_lowercase}_grpc_engine/config/${engine_name_lowercase}_config.h"

namespace {
    std::function<void(int)> stop_handler;
    void signal_handler(int signal) {
        stop_handler(signal);
    }
}

int main(int argc, char * argv[])
{
    // Parse the command line arguments
    cxxopts::ParseResult options = ELEOptsParser::parseOpts(argc, argv, ELEOptsParser::createOptionParser());
    const auto configFile = options["c"].as<std::string>();
    const auto logLevel = options["loglevel"].as<std::string>();

    if (!std::filesystem::is_regular_file(configFile)){
        throw std::invalid_argument("The provided configuration file [ " + configFile + " ] is invalid");
    }

    // Create default logger
    auto logger = NRPLogger("AsyncEngine", NRPLogger::level_t::off, NRPLogger::level_from_string(logLevel), "", true);

    // Process Engine configuration
    nlohmann::json config(json_utils::parseJSONFile(configFile));
    json_utils::setDefault<std::string>(config.at("EngineConfig"), "EngineType", ${engine_name}GrpcConfigConst::EngineType);
    json_utils::validateJson(config, "json://nrp-core/event_loop.json#/event_loop_engine");

    auto eTstep = config.at("Timestep").get<float>();
    auto eTout  = config.at("Timeout").get<float>();
    auto eTstepWarn = config.at("TimestepWarnThreshold").get<float>();

    auto timestep = std::chrono::milliseconds((int)(1000 * eTstep));
    auto timeout = std::chrono::milliseconds((int)(1000 * eTout));
    auto timestepWarn = std::chrono::milliseconds((int)(1000 * eTstepWarn));

    std::stringstream info_msg;
    info_msg << "Creating Event Loop with configuration: timestep=" << timestep.count() << "(ms), timeout=" << timeout.count() << "(ms)";
    NRPLogger::info(info_msg.str());

    ${engine_name}EngineGrpcClient client(config.at("EngineConfig"), ProcessLauncherInterface::unique_ptr{});

    if(client.engineConfig().at("EngineTimestep").get<float>() != eTstep)
        NRPLogger::info("Engine Timestep parameter " + std::to_string(client.engineConfig().at("EngineTimestep").get<float>()) +
                        " overwritten to " + std::to_string(eTstep) +
                        " to match EventLoop time step");

    const auto protobufPluginsPath = client.engineConfig().at("ProtobufPluginsPath").get<std::string>();
    const auto protobufPlugins = client.engineConfig().at("ProtobufPackages");

    json_utils::setDefault<nlohmann::json>(config, "MQTTConfig", nlohmann::json::object());
    json_utils::setDefault<std::string>(config.at("MQTTConfig"), "ClientName", client.engineName());

    // Connect to MQTT server
    auto& mqttProxy = NRPMQTTProxy::resetInstance(config.at("MQTTConfig"));

    // Instantiate Engine
    auto wrapper = new ${engine_name}Engine(client.engineName(), protobufPluginsPath, protobufPlugins);
    EventLoopEngine engine(timestep, timestepWarn,
                           config.at("DataQueueSize").get<size_t>(),
                                   config.at("ProcessLastMsg").get<bool>(),
                                           client.engineConfig(), wrapper);

    // add sigint handle
    stop_handler = [&] (int) {
        NRPLogger::info("Interruption Signal received. Stopping the simulation");
        engine.stopLoop();
    };
    signal(SIGINT, signal_handler);
    signal(SIGTERM, signal_handler);

    // Start the server
    engine.initialize();
    engine.runLoop(timeout);
    engine.shutdown();

    mqttProxy.clearRetained();
    mqttProxy.disconnect();

    return 0;
}
