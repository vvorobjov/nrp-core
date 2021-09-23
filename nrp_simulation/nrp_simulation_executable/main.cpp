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

#include "nrp_general_library/config/cmake_constants.h"
#include "nrp_general_library/plugin_system/plugin_manager.h"
#include "nrp_general_library/process_launchers/process_launcher_manager.h"
#include "nrp_general_library/utils/nrp_exceptions.h"
#include "nrp_general_library/utils/python_interpreter_state.h"
#include "nrp_general_library/utils/restclient_setup.h"
#include "nrp_simulation/config/cmake_conf.h"
#include "nrp_simulation/simulation/simulation_manager.h"
#include "nrp_simulation/simulation/nrp_core_server.h"

#include <stdlib.h>
#include <string.h>


static void loadPlugins(const char *libName, PluginManager &pluginManager, const EngineLauncherManagerSharedPtr &engines)
{
	NRP_LOGGER_TRACE("{} called [ libName: {} ]", __FUNCTION__, libName);

	// Extract plugin file name and load it
	NRPLogger::debug("Loading {} plugin", libName);
	auto engineLauncher = pluginManager.loadPlugin(libName);
	if(engineLauncher == nullptr)
		throw NRPException::logCreate(std::string("Failed to load engine launcher from plugin \"") + libName + "\"");

	NRPLogger::info("Plugin {} is loaded", libName);

	// Register launcher
	engines->registerLauncher(EngineLauncherInterfaceSharedPtr(engineLauncher.release()));
	NRPLogger::debug("Engine launcher ({}) is registered", libName);
}


static void loadEngines(PluginManager & pluginManager,
                        EngineLauncherManagerSharedPtr & engines,
                        const cxxopts::ParseResult & startParams)
{
	// Add plugin path to LD_LIBRARY_PATH
	pluginManager.addPluginPath(NRP_PLUGIN_INSTALL_DIR);

	// Iterate over default plugin libs, separated by ' '
	const auto defaultLaunchers = NRP_SIMULATION_DEFAULT_ENGINE_LAUNCHERS;
	for(const auto &libName : defaultLaunchers)
		loadPlugins(libName, pluginManager, engines);

	auto pluginsParam = startParams[SimulationParams::ParamPlugins.data()].as<SimulationParams::ParamPluginsT>();
	for(const auto &libName : pluginsParam)
		loadPlugins(libName.c_str(), pluginManager, engines);
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
					// manager.shutdownLoop() will be called on SimulationManager destruction
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

	// Create default logger for the launcher
	auto logger = NRPLogger
	(
		SimulationParams::NRPProgramName.data(), 														// Logger name
		SimulationParams::parseLogLevel(
			startParams[SimulationParams::ParamFileLogLevelLong.data()].as<SimulationParams::ParamFileLogLevelT>()
			),																							// File log level
		SimulationParams::parseLogLevel(
			startParams[SimulationParams::ParamConsoleLogLevelLong.data()].as<SimulationParams::ParamConsoleLogLevelT>()
			),																							// Console log level
		startParams[SimulationParams::ParamLogDirLong.data()].as<SimulationParams::ParamLogDirT>(), 	// Log files location
		true
	);

	NRPLogger::info("Working directory: [ {} ]", std::filesystem::current_path().c_str());

	// Setup Python
	PythonInterpreterState pythonInterp(argc, argv);

	// Create Process launchers
	MainProcessLauncherManager::shared_ptr processLaunchers(new MainProcessLauncherManager());

	// Create engine launchers
	PluginManager pluginManager;
	EngineLauncherManagerSharedPtr engines(new EngineLauncherManager());

	loadEngines(pluginManager, engines, startParams);

	// Load simulation

	SimulationManager manager = SimulationManager::createFromConfig(simConfig);

	if(manager.simulationConfig() == nullptr)
	{
		NRPLogger::error("Simulation configuration file not specified");
		return 1;
	}

	// Run the simulation in the specified mode

	const auto mode = startParams[SimulationParams::ParamMode.data()].as<std::string>();

	if(mode == "server")
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

	NRPLogger::info("Exiting Simulation Manager");
	return 0;
}

