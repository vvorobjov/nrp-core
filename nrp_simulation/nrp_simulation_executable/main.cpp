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

#ifdef ROS_ON
#include "ros/ros.h"
#include "nrp_ros_proxy/nrp_ros_proxy.h"
#endif

#ifdef MQTT_ON
#include "nrp_mqtt_proxy/nrp_mqtt_proxy.h"
#endif

#include "nrp_general_library/config/cmake_constants.h"
#include "nrp_general_library/plugin_system/plugin_manager.h"
#include "nrp_general_library/process_launchers/process_launcher_manager.h"
#include "nrp_general_library/utils/nrp_exceptions.h"
#include "nrp_general_library/utils/python_interpreter_state.h"
#include "nrp_general_library/utils/restclient_setup.h"
#include "nrp_simulation/config/cmake_conf.h"
#include "nrp_simulation/simulation/simulation_manager.h"
#include "nrp_simulation/simulation/nrp_core_server.h"

#include "nrp_event_loop/event_loop/event_loop.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>


static void loadPlugins(const char *libName,
                        PluginManager &pluginManager,
                        const EngineLauncherManagerSharedPtr &engines,
                        const std::set<std::string>& engineTypes)
{
    NRP_LOGGER_TRACE("{} called [ libName: {} ]", __FUNCTION__, libName);

    // Extract plugin file name and load it
    NRPLogger::debug("Loading {} plugin", libName);
    auto engineLauncher = pluginManager.loadPlugin(libName);
    if(engineLauncher == nullptr)
        throw NRPException::logCreate(std::string("Failed to load engine launcher from plugin \"") + libName + "\"");

    NRPLogger::info("Plugin {} is loaded", libName);

    // Register launcher
    if(engineTypes.count(engineLauncher->engineType())) {
        engines->registerLauncher(EngineLauncherInterfaceSharedPtr(engineLauncher.release()));
        NRPLogger::debug("Engine launcher ({}) is registered", libName);
    }
}


static void loadEngines(PluginManager & pluginManager,
                        EngineLauncherManagerSharedPtr & engines,
                        const cxxopts::ParseResult & startParams,
                        const std::set<std::string>& engineTypes)
{
    // Add plugin path to LD_LIBRARY_PATH
    pluginManager.addPluginPath(NRP_PLUGIN_INSTALL_DIR);

    // If user specified a list of plugins use it, otherwise load them all
    auto pluginsList = startParams[SimulationParams::ParamPlugins.data()].as<SimulationParams::ParamPluginsT>();
    if(pluginsList.empty())
        pluginsList = NRP_ENGINE_LAUNCHERS;

    // Iterate over plugin libs, separated by ' '
    for(const auto &libName : pluginsList)
        if(std::strcmp(libName.c_str(), "") != 0)
            loadPlugins(libName.c_str(), pluginManager, engines, engineTypes);
}


static void runServerMode(EngineLauncherManagerSharedPtr & engines,
                          MainProcessLauncherManager::shared_ptr & processLaunchers,
                          SimulationManager & manager,
                          const std::string & address)
{
    NrpCoreServer server(address);

    bool isShutdown = false;

    while(true)
    {
        server.waitForRequest();

        // Check the request type and handle it accordingly

        try
        {
            switch(server.getRequestType())
            {
                case NrpCoreServer::RequestType::Init:
                    manager.initFTILoop(engines, processLaunchers);
                    break;
                case NrpCoreServer::RequestType::RunLoop:
                    manager.runSimulation(server.getNumIterations());
                    break;
                case NrpCoreServer::RequestType::Shutdown:
                    isShutdown = true;
                    // manager.stopLoop() will be called on SimulationManager destruction
                    break;
                default:
                    throw NRPException::logCreate("Unknown request received");
            }
        }
        catch(std::exception &e)
        {
            server.markRequestAsFailed(e.what());
        }

        server.markRequestAsProcessed();

        // Break out of the loop, if shutdown was requested

        if(isShutdown)
        {
            break;
        }
    };
}


static void runStandaloneMode(EngineLauncherManagerSharedPtr & engines,
                              MainProcessLauncherManager::shared_ptr & processLaunchers,
                              SimulationManager & manager)
{
    NRPLogger::info("Config file specified, launching...\n");

    manager.initFTILoop(engines, processLaunchers);
    manager.runSimulationUntilTimeout();
    // NRRPLT-8246: uncomment to test reset
    // manager.resetSimulation();
    // manager.runSimulationUntilTimeout();
}


static void runEventLoopMode(EngineLauncherManagerSharedPtr & engines,
                             MainProcessLauncherManager::shared_ptr & processLaunchers,
                             SimulationManager & manager,
                             std::unique_ptr<EventLoop> & eLoop,
                             std::chrono::milliseconds & timeout,
                             bool runFTILoop)
{
    std::future<void> runFuture;
    std::atomic<bool> runLoop = true;

    // start FTILoop
    if(runFTILoop) {
        manager.initFTILoop(engines, processLaunchers);
        std::function<void()> run_ftiloop = [&]() {
            while (runLoop)
                manager.runSimulationOnce();
        };

        runFuture = std::async(run_ftiloop);
    }

    // run EventLoop
    eLoop->runLoopAsync(timeout);
    eLoop->waitForLoopEnd();

    // stop FTILoop
    if(runFTILoop) {
        runLoop = false;
        runFuture.wait();
    }
}


int main(int argc, char *argv[])
{

    RestClientSetup::ensureInstance();

    // Parse start params
    auto optParser = SimulationParams::createStartParamParser();
    std::unique_ptr<cxxopts::ParseResult> startParamPtr;
    try
    {
        startParamPtr.reset(new cxxopts::ParseResult(optParser.parse(argc, argv)));
    }
    catch(cxxopts::OptionParseException &e)
    {
        // If options aren't well formed, output help and exit
        std::cerr << e.what() << std::endl;
        std::cerr << optParser.help();
        return 1;
    }

    auto &startParams = *startParamPtr;

    // If help output was requested, only print that, then exit
    if(startParams[SimulationParams::ParamHelp.data()].as<SimulationParams::ParamHelpT>())
    {
        std::cout << optParser.help();
        return 0;
    }

    // Setup working directory and get config based on start pars
    jsonSharedPtr simConfig = SimulationManager::configFromParams(startParams);
    json_utils::validate_json(*simConfig, "https://neurorobotics.net/simulation.json#Simulation");

    // Create default logger for the launcher
    auto logger = NRPLogger
    (
        SimulationParams::NRPProgramName.data(),                                                        // Logger name
        SimulationParams::parseLogLevel(
            startParams[SimulationParams::ParamFileLogLevelLong.data()].as<SimulationParams::ParamFileLogLevelT>()
            ),                                                                                          // File log level
        SimulationParams::parseLogLevel(
            startParams[SimulationParams::ParamConsoleLogLevelLong.data()].as<SimulationParams::ParamConsoleLogLevelT>()
            ),                                                                                          // Console log level
        startParams[SimulationParams::ParamLogDirLong.data()].as<SimulationParams::ParamLogDirT>(),     // Log files location
        true
    );

    NRPLogger::info("Working directory: [ {} ]", std::filesystem::current_path().c_str());

    // Create Process launchers
    MainProcessLauncherManager::shared_ptr processLaunchers(new MainProcessLauncherManager());

    // Create engine launchers
    PluginManager pluginManager;
    EngineLauncherManagerSharedPtr engines(new EngineLauncherManager());

    std::set<std::string> engineTypes;
    if(simConfig->contains("EngineConfigs"))
        for(auto &engineConfig : simConfig->at("EngineConfigs"))
            engineTypes.insert(engineConfig.at("EngineType").get<std::string>());


    loadEngines(pluginManager, engines, startParams, engineTypes);

    // Load simulation

    SimulationManager manager = SimulationManager::createFromConfig(simConfig);

    if(manager.simulationConfig() == nullptr)
    {
        NRPLogger::error("Simulation configuration file not specified");
        return 1;
    }

    // Start external processes
    std::vector<std::unique_ptr<ProcessLauncherInterface>> extProcs;
    for(auto &procConf : simConfig->at("ExternalProcesses")) {
        auto procLaunch = processLaunchers->createProcessLauncher(simConfig->at("ProcessLauncherType"));

        pid_t procPID;

        try {
            procPID = procLaunch->launchProcess(procConf);
        }
        catch(std::exception &e)
        {
            throw NRPException::logCreate(e, "Error when launching process \"" + procConf.at("ProcCmd").get<std::string>() + "\"");
        }

        if(procPID == 0)
            throw NRPException::logCreate("Failed to launch process \"" + procConf.at("ProcCmd").get<std::string>() + "\"");

        extProcs.push_back(std::move(procLaunch));
    }

    // Connect to ROS
#ifdef ROS_ON
    if(simConfig->contains("ConnectROS")) {
        nlohmann::json nodeProperties = simConfig->at("ConnectROS");
        ros::init(std::map<std::string, std::string>(), nodeProperties.at("NodeName"));
        NRPROSProxy::resetInstance();
    }
#else
    if(simConfig->contains("ConnectROS"))
        NRPLogger::info("nrp-core has been compiled without ROS support. Configuration parameter 'ConnectROS' will be ignored.");
#endif

    // Connect to MQTT
#ifdef MQTT_ON
    if(simConfig->contains("ConnectMQTT")) {
        NRPMQTTProxy::resetInstance(simConfig->at("ConnectMQTT"));
    }
#else
    if(simConfig->contains("ConnectMQTT"))
        NRPLogger::info("nrp-core has been compiled without MQTT support. Configuration parameter 'ConnectMQTT' will be ignored.");
#endif

    // Setup Python
    bool startELE = simConfig->at("SimulationLoop") == "EventLoop";
    PythonInterpreterState pythonInterp(argc, argv, startELE);

    // Creates and start ELE
    std::unique_ptr<EventLoop> eLoop;
    std::chrono::milliseconds eTout;
    if(startELE) {
        NRPLogger::debug("Starting simulation with Event Loop");

        // Set up configuration
        json_utils::set_default<std::vector<std::string>>(*simConfig, "ComputationalGraph", std::vector<std::string>());

        // Starts event loop
        int eTstep;

        if(simConfig->contains("EventLoopTimestep"))
            eTstep = 1000 * simConfig->at("EventLoopTimestep").get<float>();
        else
            eTstep = 1000 * simConfig->at("SimulationTimestep").get<float>();

        if(simConfig->contains("EventLoopTimeout"))
            eTout = std::chrono::milliseconds(1000 * simConfig->at("EventLoopTimeout").get<int>());
        else
            eTout = std::chrono::milliseconds(1000 * simConfig->at("SimulationTimeout").get<int>());

        eLoop.reset(new EventLoop(simConfig->at("ComputationalGraph"), std::chrono::milliseconds(eTstep),
                                  false, simConfig->contains("ConnectROS")));

    }

    // Run the simulation in the specified mode

    const auto mode = startParams[SimulationParams::ParamMode.data()].as<std::string>();

    if(eLoop) {
        bool runFTILoop = simConfig->at("EngineConfigs").size() > 0;
        runEventLoopMode(engines, processLaunchers, manager, eLoop, eTout, runFTILoop);
        eLoop->shutdown();
    }
    else if(mode == "server")
    {
        const std::string serverAddress = startParams[SimulationParams::ParamServerAddressLong.data()].as<std::string>();

        if(serverAddress.empty())
        {
            NRPLogger::error("Server address not specified");
            return 1;
        }

        runServerMode(engines, processLaunchers, manager, serverAddress);
    }
    else if(mode == "standalone")
    {
        runStandaloneMode(engines, processLaunchers, manager);
    }
    else
    {
        NRPLogger::error("Unknown operational mode '" + mode + "'");
        return 1;
    }

#ifdef MQTT_ON
    NRPMQTTProxy* mqttProxy = &(NRPMQTTProxy::getInstance());
    if(mqttProxy)
        mqttProxy->disconnect();
#endif

    NRPLogger::info("Exiting Simulation Manager");
    return 0;
}

