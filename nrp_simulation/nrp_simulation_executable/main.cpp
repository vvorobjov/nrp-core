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
#include "nrp_general_library/engine_interfaces/engine_launcher_manager.h"
#include "nrp_general_library/utils/nrp_exceptions.h"
#include "nrp_general_library/utils/python_interpreter_state.h"
#include "nrp_general_library/utils/restclient_setup.h"

#include "nrp_simulation/config/cmake_conf.h"
#include "nrp_simulation/simulation/simulation_parameters.h"
#include "nrp_simulation/simulation/simulation_manager.h"
#include "nrp_simulation/simulation/simulation_manager_fti.h"
#include "nrp_simulation/simulation/simulation_manager_event_loop.h"
#include "nrp_simulation/simulation/nrp_core_server.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <csignal>
#include <unistd.h>

#include <cstdlib>
extern "C" {
#include <fcntl.h>
#include <unistd.h>
}
#include <stdexcept>
#include <iostream>

namespace {
    std::function<void(int)> shutdown_handler;
    void signal_handler(int signal) {
        shutdown_handler(signal);
    }
}

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
                        std::vector<std::string> pluginsList,
                        const std::set<std::string>& engineTypes)
{
    // Add plugin path to LD_LIBRARY_PATH
    pluginManager.addPluginPath(NRP_PLUGIN_INSTALL_DIR);

    // If user specified a list of plugins use it, otherwise load them all
    if(pluginsList.empty())
        pluginsList = NRP_ENGINE_LAUNCHERS;

    // Iterate over plugin libs, separated by ' '
    for(const auto &libName : pluginsList)
        if(std::strcmp(libName.c_str(), "") != 0)
            loadPlugins(libName.c_str(), pluginManager, engines, engineTypes);
}

static int processLogOutputOption(const std::string& logOutput, std::string logFilename, std::string logDir)
{
    int enginesFD = -1;
    if (logOutput == "silent")
        enginesFD = open("/dev/null", O_WRONLY);
    else if (logOutput == "engines" || logOutput == "all") {

        if(!logDir.empty()) {
            if (!std::filesystem::is_directory(logDir)) {
                NRPLogger::info(
                        "Specified log directory does not exist, logging output to experiment directory.");
                logDir = "";
            } else
                logDir = logDir + "/";

            logFilename = logDir + logFilename;
        }
        enginesFD = open(logFilename.data(), O_WRONLY | O_CREAT | O_TRUNC, 0666);
    }
    else {
        std::cerr << "Failed to process Engines output cmd line option: " << logOutput
                  << ". Valid options are off, display, log." << std::endl;
        std::exit(1);
    }

    if (enginesFD < 0 && logOutput != "silent") {
        std::cerr << "Failed to open log file." << std::endl;
        std::exit(1);
    }
    else if(enginesFD >= 0 && logOutput != "engines") {
        if (dup2(enginesFD, STDOUT_FILENO) < 0) {
            std::perror("dup2 (stdout)");
            std::exit(1);
        }
        if (dup2(enginesFD, STDERR_FILENO) < 0) {
            std::perror("dup2 (stderr)");
            std::exit(1);
        }
        NRPLogger::info("Logging all output to file. Output printed from Python might not be logged in the right order.");
    }

    return enginesFD;
}


int main(int argc, char *argv[])
{
    //// PARSE COMMAND LINE PARAMETERS
    auto optParser = SimulationParams::createStartParamParser();
    std::unique_ptr<cxxopts::ParseResult> startParamPtr;
    try
    {
        if(argc == 1)
        {
            throw cxxopts::OptionParseException("No input parameters are given.\n"); 
        }
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

    // Set working directory and get config file from params
    jsonSharedPtr simConfig = SimulationParams::setWorkingDirectoryAndGetConfigFile(startParams);
    NRPLogger::info("Working directory: [ {} ]", std::filesystem::current_path().c_str());

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

    // Process log output option
    int enginesFD = startParams.count(SimulationParams::ParamLogOutputLong.data()) ? processLogOutputOption(
            startParams[SimulationParams::ParamLogOutputLong.data()].as<SimulationParams::ParamLogOutputT>(),
            startParams[SimulationParams::ParamLogFilenameLong.data()].as<SimulationParams::ParamFilenameT>(),
                    startParams[SimulationParams::ParamLogDirLong.data()].as<SimulationParams::ParamLogDirT>()
                            ) : -1;

    // Override simulation parameters from command
    if (startParams.count(SimulationParams::ParamSimParam.data()))
    {
        SimulationParams::parseAndSetCLISimParams(startParams[SimulationParams::ParamSimParam.data()].as<SimulationParams::ParamSimParamT>(), *simConfig);
    }

    // Validate the resulting config
    SimulationManager::validateConfig(simConfig);

    // List of plugin to load
    auto pluginsList = startParams[SimulationParams::ParamPlugins.data()].as<SimulationParams::ParamPluginsT>();

    // Running mode
    const auto mode = startParams[SimulationParams::ParamMode.data()].as<std::string>();

    // Server address
    const std::string serverAddress = startParams[SimulationParams::ParamServerAddressLong.data()].as<std::string>();

    // Slave mode
    auto isSlave = startParams[SimulationParams::ParamSlaveLong.data()].as<SimulationParams::ParamSlaveT>();

    //// CREATE EXPERIMENT OBJECTS
    // Setup Python. If EventLoop is used, allow threads
    PythonInterpreterState pythonInterp(argc, argv, simConfig->at("SimulationLoop") == "EventLoop");

    // Rest server
    RestClientSetup::ensureInstance();

    // Create Process and engine launchers
    MainProcessLauncherManager::shared_ptr processLaunchers(new MainProcessLauncherManager(enginesFD));
    PluginManager pluginManager;
    EngineLauncherManagerSharedPtr engines(new EngineLauncherManager());

    std::set<std::string> engineTypes;
    if(simConfig->contains("EngineConfigs"))
        for(auto &engineConfig : simConfig->at("EngineConfigs"))
            engineTypes.insert(engineConfig.at("EngineType").get<std::string>());


    loadEngines(pluginManager, engines, pluginsList, engineTypes);

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

    // Create simulation manager
    std::shared_ptr<SimulationManager> manager;
    if(simConfig->at("SimulationLoop") == "EventLoop")
        manager.reset(new EventLoopSimManager(simConfig, engines, processLaunchers));
    else
        manager.reset(new FTILoopSimManager(simConfig, engines, processLaunchers));

    //// RUN THE EXPERIMENT
    if(mode == "server")
    {
        if(serverAddress.empty())
        {
            NRPLogger::error("Server address not specified");
            return 1;
        }


        NrpCoreServer server(serverAddress, std::move(manager));

        // In slave mode ignore SIGINT, otherwise shut the server down nicely
        if(isSlave)
            signal(SIGINT, [] (int) {  });
        else {
            shutdown_handler = [&] (int) {
                server.stopServerLoop();
            };
            signal(SIGINT, signal_handler);
        }

        server.runServerLoop();
    }
    else if(mode == "standalone")
    {
        auto res = manager->initializeSimulation();
        if(res.currentState != SimulationManager::SimState::Failed)
            manager->runSimulationUntilTimeout();

        manager->shutdownSimulation();
    }
    else
    {
        NRPLogger::error("Unknown operational mode '" + mode + "'");
        return 1;
    }

    //// SHUT THINGS DOWN
    // Force flush stdout and stderr in Python, when output is redirected Python might fail to do it automatically
    if(enginesFD >= 0)
        boost::python::exec("import sys; sys.stdout.flush(); sys.stderr.flush()");

#ifdef MQTT_ON
    NRPMQTTProxy* mqttProxy = &(NRPMQTTProxy::getInstance());
    if(mqttProxy)
        mqttProxy->disconnect();
#endif

    if(enginesFD >= 0)
        close(enginesFD);

    NRPLogger::info("Exiting Simulation Manager");
    return 0;
}

