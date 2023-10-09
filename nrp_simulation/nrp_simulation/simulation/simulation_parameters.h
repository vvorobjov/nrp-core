/* * NRP Core - Backend infrastructure to synchronize simulations
 *
 * Copyright 2020-2023 NRP Team
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

#ifndef SIMULATION_PARAMS_H
#define SIMULATION_PARAMS_H

#include "nrp_general_library/utils/nrp_exceptions.h"
#include "nrp_general_library/utils/nrp_logger.h"

#include <filesystem>
#include <iostream>
#include <fstream>
#include <string>
#include <cxxopts.hpp>
#include <nlohmann/json.hpp>

using jsonSharedPtr = std::shared_ptr<nlohmann::json>;


/*!
 * \brief NRP Simulation Startup Parameters
 */
struct SimulationParams
{
    static constexpr std::string_view NRPProgramName = "NRPCoreSim";
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
    static constexpr std::string_view ParamPluginsDesc = "Engine plugins to load. If omitted, all engines compiled with nrp-core are loaded";
    using ParamPluginsT = std::vector<std::string>;

    static constexpr std::string_view ParamExpDir = "d";
    static constexpr std::string_view ParamExpDirLong = "d,dir";
    static constexpr std::string_view ParamExpDirDesc = "The explicit location of the experiment folder";
    using ParamExpDirT = std::string;
    
    // Log paramters
    static constexpr std::string_view ParamConsoleLogLevelLong = "cloglevel";
    static constexpr std::string_view ParamConsoleLogLevelDesc = "Console minimum level of log severity";
    using ParamConsoleLogLevelT = std::string;

    static constexpr std::string_view ParamFileLogLevelLong = "floglevel";
    static constexpr std::string_view ParamFileLogLevelDesc = "File minimum level of log severity";
    using ParamFileLogLevelT = std::string;

    static constexpr std::string_view ParamLogOutputLong = "logoutput";
    static constexpr std::string_view ParamLogOutputDesc = "Option for logging stdout and stderr in the experiment: engines, all, silent";
    using ParamLogOutputT = std::string;

    static constexpr std::string_view ParamLogFilenameLong = "logfilename";
    static constexpr std::string_view ParamLogFilenameDesc = "If 'logoutput' option is 'engines' or 'all', name of the log file";
    using ParamFilenameT = std::string;

    static constexpr std::string_view ParamLogDirLong = "logdir";
    static constexpr std::string_view ParamLogDirDesc = "Directory for the file logs";
    using ParamLogDirT = std::string;

    static constexpr std::string_view ParamLogConfig = "l";
    static constexpr std::string_view ParamLogConfigLong = "l,logconfig";
    static constexpr std::string_view ParamLogConfigDesc = "Print the simulation config in use to DEBUG log";
    using ParamLogConfigT = bool;

    static constexpr std::string_view ParamMode = "m";
    static constexpr std::string_view ParamModeLong = "m,mode";
    static constexpr std::string_view ParamModeDesc = "Operational mode, standalone or server";
    using ParamModeT = std::string;

    static constexpr std::string_view ParamSimParam = "o";
    static constexpr std::string_view ParamSimParamLong = "o,param";
    static constexpr std::string_view ParamSimParamDesc = "Simulation config parameter, specified as param=value, overriding configuration file parameters";
    using ParamSimParamT = std::vector<std::string>;

    static constexpr std::string_view ParamServerAddressLong = "server_address";
    static constexpr std::string_view ParamServerAddressDesc = "Desired address of the server in server operational mode";
    using ParamServerAddressT = std::string;

    static constexpr std::string_view ParamSlaveLong = "slave";
    static constexpr std::string_view ParamSlaveDesc = "If present NRPCoreSim runs in slave mode. For internal use.";
    using ParamSlaveT = bool;

    /*!
     * \brief Create a parser for start parameters
     * \return Returns parser
     */
    static cxxopts::Options createStartParamParser();

    // TODO: in this function the WD is also set with a kind of complex logic, it should be documented in the function
    //  declaration, i.e., here
    /*!
     * \brief Get the simulation configuration from start parameters as a JSON object
     * \param args Parsed start parameters
     * \return Returns instance of simulation config
     */
    static jsonSharedPtr setWorkingDirectoryAndGetConfigFile(const cxxopts::ParseResult &args);

    /*!
     * \brief Parse cmd line simulation parameters
     * \param parseResults
     * \param simulationConfig
     */
    static void parseAndSetCLISimParams(const ParamSimParamT& parseResults, nlohmann::json &simulationConfig);

    /*!
     * \brief parsing input parameter string log level into enum type
     * \param logLevel The string expression of the log level
     */
    static NRPLogger::level_t parseLogLevel(const std::string &logLevel);

private:

    /*!
     * \brief Set cmd line simulation parameter
     * @param fullKey
     * @param value
     * @param simulationConfig
     */
    static void setCLISimParams(const std::string& fullKey, const std::string& value, nlohmann::json &simulationConfig);

    /*!
     * \brief default log level for the acses when the parameters are specified wrongly
     */
    static const NRPLogger::level_t _defaultLogLevel = NRPLogger::level_t::info;
};

#endif
