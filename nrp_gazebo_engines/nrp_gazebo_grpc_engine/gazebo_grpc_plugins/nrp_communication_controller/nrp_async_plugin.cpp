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

#include "nrp_communication_controller/nrp_async_plugin.h"

#include "nrp_gazebo_grpc_engine/engine_server/nrp_communication_controller.h"
#include "nrp_gazebo_grpc_engine/nrp_client/gazebo_engine_grpc_nrp_client.h"
#include "nrp_event_loop/event_loop/event_loop_engine_opts_parser.h"

namespace {
    std::function<void(int)> stop_handler;
    void signal_handler(int signal) {
        stop_handler(signal);
    }
}

gazebo::NRPGazeboAsyncPlugin::~NRPGazeboAsyncPlugin()
{
    // this->shutdown();
}

void gazebo::NRPGazeboAsyncPlugin::Load(int argc, char **argv)
{
    NRP_LOGGER_TRACE("{} called", __FUNCTION__);

    // Parse the command line arguments
    cxxopts::ParseResult options = ELEOptsParser::parseOpts(argc, argv, ELEOptsParser::createOptionParser(true));
    const auto configFile = options["c"].as<std::string>();
    const auto logLevel = options["loglevel"].as<std::string>();

    if (!std::filesystem::is_regular_file(configFile)){
        throw std::invalid_argument("The provided configuration file [ " + configFile + " ] is invalid");
    }

    // Create default logger
    _logger.reset(new NRPLogger("AsyncEngine", NRPLogger::level_t::off,
                                NRPLogger::level_from_string(logLevel), "", true));
    NRPLogger::info("NRP Async plugin: Initializing...");

    // Process Engine configuration
    nlohmann::json config(json_utils::parseJSONFile(configFile));
    json_utils::setDefault<std::string>(config.at("EngineConfig"), "EngineType", GazeboGrpcConfigConst::EngineType);
    json_utils::validateJson(config, "json://nrp-core/event_loop.json#/event_loop_engine");

    auto eTstep = config.at("Timestep").get<float>();
    auto eTout  = config.at("Timeout").get<float>();
    auto eTstepWarn = config.at("RealTimeDeltaThres").get<float>();

    _timestep = std::chrono::milliseconds((int)(1000 * eTstep));
    _timeout = std::chrono::milliseconds((int)(1000 * eTout));
    _timestepWarn = std::chrono::milliseconds((int)(1000 * eTstepWarn));

    std::stringstream info_msg;
    info_msg << "Creating Event Loop with configuration: timestep=" << _timestep.count() << "(ms), timeout=" << _timeout.count() << "(ms)";
    NRPLogger::info(info_msg.str());

    GazeboEngineGrpcNRPClient client(config.at("EngineConfig"), ProcessLauncherInterface::unique_ptr{});

    _engineName = client.engineName();
    _protobufPluginsPath = client.engineConfig().at("ProtobufPluginsPath").get<std::string>();
    _protobufPlugins = client.engineConfig().at("ProtobufPackages");

    json_utils::setDefault<nlohmann::json>(config, "MQTTConfig", nlohmann::json::object());
    json_utils::setDefault<std::string>(config.at("MQTTConfig"), "ClientName", client.engineName());

    // Connect to MQTT server
    NRPMQTTProxy::resetInstance(config.at("MQTTConfig"));

    // save ele config
    _eleConfig["storeCapacity"] = config.at("DataQueueSize").get<size_t>();
    _eleConfig["doProcessLast"] = config.at("ProcessLastMsg").get<bool>();
    _eleConfig["engineConfig"] = client.engineConfig();
    _eleConfig["LogRTInfo"] = config.at("LogRTInfo").get<bool>();
    _eleConfig["UseGlobalClock"] = config.at("UseGlobalClock").get<bool>();

    // Start ELE
    this->startELE();

    // Register signal handler
    stop_handler = [this](int signal) {
        NRPLogger::info("NRP Async plugin: Received temination signal. Shutting down...");
        this->shutdown();
        gazebo::shutdown();
        std::raise(signal);
    };

    signal(SIGINT, signal_handler);
    signal(SIGTERM, signal_handler);
}

void gazebo::NRPGazeboAsyncPlugin::Reset()
{
    NRP_LOGGER_TRACE("{} called", __FUNCTION__);
    if(this->_ele)
        _ele->shutdown();
    NRPMQTTProxy::getInstance().clearRetained();
    this->startELE();
}

void gazebo::NRPGazeboAsyncPlugin::startELE()
{
    NRP_LOGGER_TRACE("{} called", __FUNCTION__);

    NRPLogger::debug("NRP Async plugin: Creating Engine");

    // Reset server
    auto newController = new NRPGazeboCommunicationController(_engineName, _protobufPluginsPath, _protobufPlugins);

    _ele.reset(new EventLoopEngine(_timestep, _timestepWarn,
                                   _eleConfig["storeCapacity"].get<size_t>(),
                                   _eleConfig["doProcessLast"].get<bool>(),
                                   _eleConfig["engineConfig"], newController,
                                   false,
                                   _eleConfig["LogRTInfo"].get<bool>(),
                                   _eleConfig["UseGlobalClock"].get<bool>()));

    CommControllerSingleton::resetInstance(newController);

    NRPLogger::debug("NRP Async plugin: starting Engine");

    // Start server
    _ele->runLoopAsync(_timeout, true);

    NRPLogger::info("NRP Async plugin: Engine started. Waiting for input...");
}

void gazebo::NRPGazeboAsyncPlugin::shutdown()
{
    if(this->_ele)
        _ele->shutdown();

    auto& mqttProxy = NRPMQTTProxy::getInstance();
    mqttProxy.clearRetained();
    mqttProxy.disconnect();
}
