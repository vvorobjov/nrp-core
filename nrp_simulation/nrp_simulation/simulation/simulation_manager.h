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

#ifndef SIMULATION_MANAGER_H
#define SIMULATION_MANAGER_H

#include "nrp_general_library/config/cmake_constants.h"
#include "nrp_general_library/engine_interfaces/engine_launcher_manager.h"
#include "nrp_general_library/plugin_system/plugin_manager.h"
#include "nrp_general_library/process_launchers/process_launcher_manager.h"
#include "nrp_simulation/simulation/simulation_loop.h"

#include <mutex>
#include <cxxopts.hpp>


/*!
 * \brief NRP Simulation Startup Parameters
 */
struct SimulationParams
{
	static constexpr std::string_view NRPProgramName = "NRPSimulation";
	static constexpr std::string_view ProgramDescription = "Brain and physics simulator";

	// Simulation Executable parameters
	static constexpr std::string_view ParamHelp = "h";
	static constexpr std::string_view ParamHelpLong = "h,help";
	static constexpr std::string_view ParamHelpDesc = "Print this message";
	using ParamHelpT = bool;

	static constexpr std::string_view ParamSimCfgFile = "c";
	static constexpr std::string_view ParamSimCfgFileLong = "c,config";
	static constexpr std::string_view ParamSimCfgFileDesc = "Simulation config file";
	using ParamSimCfgFileT = std::string;

	static constexpr std::string_view ParamPlugins = "p";
	static constexpr std::string_view ParamPluginsLong = "p,plugins";
	static constexpr std::string_view ParamPluginsDesc = "Additional engine plugins to load";
	using ParamPluginsT = std::vector<std::string>;

	static constexpr std::string_view ParamExpDir = "d";
	static constexpr std::string_view ParamExpDirLong = "d,dir";
	static constexpr std::string_view ParamExpDirDesc = "The explicit location of the experiment folder";
	using ParamExpDirT = std::string;
	
	// Log paramters
	static constexpr std::string_view ParamConsoleLogLevelLong = "cloglevel";
	static constexpr std::string_view ParamConsoleLogLevelDesc = "Console minimum level of log severity (info by default)";
	using ParamConsoleLogLevelT = std::string;

	static constexpr std::string_view ParamFileLogLevelLong = "floglevel";
	static constexpr std::string_view ParamFileLogLevelDesc = "File minimum level of log severity (off by default)";
	using ParamFileLogLevelT = std::string;

	static constexpr std::string_view ParamLogDirLong = "logdir";
	static constexpr std::string_view ParamLogDirDesc = "Directory for the file logs";
	using ParamLogDirT = std::string;

	static constexpr std::string_view ParamMode = "m";
	static constexpr std::string_view ParamModeLong = "m,mode";
	static constexpr std::string_view ParamModeDesc = "Operational mode, standalone or server";
	using ParamModeT = std::string;

	static constexpr std::string_view ParamServerAddressLong = "server_address";
	static constexpr std::string_view ParamServerAddressDesc = "Desired address of the server in server operational mode";
	using ParamServerAddressT = std::string;

	/*!
	 * \brief Create a parser for start parameters
	 * \return Returns parser
	 */
	static cxxopts::Options createStartParamParser();

	/*!
	 * \brief Parse a JSON File and return it's values
	 * \param fileName File Name
	 * \return Returns parsed JSON
	 */
	static nlohmann::json parseJSONFile(const std::string &fileName);

	/*!
	 * \brief parsing input parameter string log level into enum type
	 * \param logLevel The string expression of the log level
	 */
	static NRPLogger::level_t parseLogLevel(const std::string &logLevel);

	/*!
	 * \brief default log level for the acses when the parameters are specified wrongly
	 */
	static const NRPLogger::level_t _defaultLogLevel = NRPLogger::level_t::info;
};


/*!
 * \brief Main NRP server class. Manages simulation execution, loads plugins, and creates server if requested.
 */
class SimulationManager
        : public PtrTemplates<SimulationManager>
{
	public:
		using sim_mutex_t = std::mutex;
		using sim_lock_t = std::unique_lock<sim_mutex_t>;

		/*!
		 * \brief Constructor
		 * \param serverConfig Server configuration
		 * \param simulationConfig Simulation configuration
		 */
		SimulationManager(const jsonSharedPtr &simulationConfig);

		/*!
		 * \brief Destructor. Will stop any currently running threads
		 */
		~SimulationManager();

		/*!
		 * \brief Get the config from start parameters
		 * \param args Parsed start parameters
		 * \return Returns instance of simulation config
		 */
		static jsonSharedPtr configFromParams(const cxxopts::ParseResult &args);

		/*!
		 * \brief Create SimulationManager from pointer to config
		 * \param config Pointer to a config
		 * \return Returns instance of SimulationManager
		 */
		static SimulationManager createFromConfig(jsonSharedPtr &config);

		/*!
		 * \brief Get simulation loop
		 * \return Returns pointer to simulation loop. If no loop is loaded, return nullptr
		 */
		SimulationLoopConstSharedPtr simulationLoop() const;

		/*!
		 * \brief Get simulation config
		 * \param simLock Pass simulation lock if already owned
		 * \return Returns pointer to simulation config as well as simulation lock. If no config is loaded, return nullptr
		 */
        jsonSharedPtr simulationConfig();

		/*!
		 * \brief Get simulation config
		 * \return Returns pointer to simulation config. If no config is loaded, return nullptr
		 */
        jsonConstSharedPtr simulationConfig() const;

		/*!
		 * \brief Initialize the simulation
		 * \param engineLauncherManager Engine launchers
		 * \param processLaunchers Process launchers
		 * \param simLock Simulation lock
		 * \exception Throws an exception when the initialization fails
		 */
		void initSimulationLoop(const EngineLauncherManagerConstSharedPtr &engineLauncherManager,
		                        const MainProcessLauncherManager::const_shared_ptr &processLauncherManager);

		/*!
		 * \brief Reset the currently running simulation
		 * \param lock Simulation lock
		 */
		bool resetSimulation();

		/*!
		 * \brief Runs the simulation until a separate thread stops it or simTimeout (defined in SimulationConfig) is reached. If simTimeout is zero or negative, ignore it
		 * \param simLock Pass simulation lock if already owned
		 * \return Returns true if no error was encountered, false otherwise
		 */
		bool runSimulationUntilTimeout(int frac = 1);

		/*!
		 * \brief Run the Simulation for specified amount of time
		 * \param secs Time (in seconds) to run simulation
		 * \return Returns true if no error was encountered, false otherwise
		 */
		void runSimulation(unsigned numIterations);

		/*!
		 * \brief Shuts down simulation loop. Will shutdown any running engines and transceiver functions after any currently running steps are completed
		 */
		void shutdownLoop();

	private:

		/*!
		 * \brief Simulation Configuration
		 */
        jsonSharedPtr _simConfig;

		/*!
		 * \brief Simulation loop
		 */
		SimulationLoopSharedPtr _loop;

		/*!
		 * \brief Creates a simulation loop using the engines specified in the config file
		 * \param engineManager Manager for all available engine launchers and interfaces
		 * \return Returns simulation loop
		 */
		SimulationLoop createSimLoop(const EngineLauncherManagerConstSharedPtr &engineManager, const MainProcessLauncherManager::const_shared_ptr &processLauncherManager);

		/*!
		 * \brief Checks whether simulation has timed out. If simTimeout <= 0, continue running indefinitely
		 * \param simTime Simulation time (in seconds)
		 * \param simTimeout Simulation timeout (in seconds)
		 * \return Returns true if simulation has timed out, false otherwise
		 */
		static inline bool hasSimTimedOut(const SimulationTime &simTime, const SimulationTime &simTimeout)
		{
			return (simTimeout >= SimulationTime::zero() && simTime >= simTimeout);
		}
};

using SimulationManagerSharedPtr = SimulationManager::shared_ptr;
using SimulationManagerConstSharedPtr = SimulationManager::const_shared_ptr;

#endif
