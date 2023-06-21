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

#include "nrp_simulation/simulation/simulation_parameters.h"
#include "nrp_general_library/utils/json_schema_utils.h"


cxxopts::Options SimulationParams::createStartParamParser()
{
    cxxopts::Options opts(SimulationParams::NRPProgramName.data(), SimulationParams::ProgramDescription.data());
    opts.add_options()
            (SimulationParams::ParamHelpLong.data(), SimulationParams::ParamHelpDesc.data(),
             cxxopts::value<SimulationParams::ParamHelpT>()->default_value("0"))
            (SimulationParams::ParamSimCfgFileLong.data(), SimulationParams::ParamSimCfgFileDesc.data(),
             cxxopts::value<SimulationParams::ParamSimCfgFileT>())
            (SimulationParams::ParamPluginsLong.data(), SimulationParams::ParamPluginsDesc.data(),
             cxxopts::value<SimulationParams::ParamPluginsT>()->default_value({}))
            (SimulationParams::ParamExpDirLong.data(), SimulationParams::ParamExpDirDesc.data(),
             cxxopts::value<SimulationParams::ParamExpDirT>())
            (SimulationParams::ParamConsoleLogLevelLong.data(), SimulationParams::ParamConsoleLogLevelDesc.data(),
             cxxopts::value<SimulationParams::ParamConsoleLogLevelT>()->default_value("info"))
            (SimulationParams::ParamFileLogLevelLong.data(), SimulationParams::ParamFileLogLevelDesc.data(),
             cxxopts::value<SimulationParams::ParamFileLogLevelT>()->default_value("off"))
            (SimulationParams::ParamLogOutputLong.data(), SimulationParams::ParamLogOutputDesc.data(),
             cxxopts::value<SimulationParams::ParamLogOutputT>())
            (SimulationParams::ParamLogFilenameLong.data(), SimulationParams::ParamLogFilenameDesc.data(),
             cxxopts::value<SimulationParams::ParamFilenameT>()->default_value(".console_output.log"))
            (SimulationParams::ParamLogDirLong.data(), SimulationParams::ParamLogDirDesc.data(),
             cxxopts::value<SimulationParams::ParamLogDirT>()->default_value("logs"))
            (SimulationParams::ParamModeLong.data(), SimulationParams::ParamModeDesc.data(),
             cxxopts::value<SimulationParams::ParamModeT>()->default_value("standalone"))
            (SimulationParams::ParamServerAddressLong.data(), SimulationParams::ParamServerAddressDesc.data(),
             cxxopts::value<SimulationParams::ParamServerAddressT>()->default_value(""))
            (SimulationParams::ParamSimParamLong.data(), SimulationParams::ParamSimParamDesc.data(),
             cxxopts::value<SimulationParams::ParamSimParamT>())
            (SimulationParams::ParamSlaveLong.data(), SimulationParams::ParamSlaveDesc.data(),
             cxxopts::value<SimulationParams::ParamSlaveT>()->default_value("0"));

    opts.custom_help("-c <experiment_config_file> [OPTION...]");

    return opts;
}


jsonSharedPtr SimulationParams::setWorkingDirectoryAndGetConfigFile(const cxxopts::ParseResult &args)
{
    NRP_LOGGER_TRACE("{} called", __FUNCTION__);

    jsonSharedPtr simConfig = nullptr;

    // Get file names from start params
    std::string simCfgFileName;

    bool directoryIsSet = false;

    // Get and check experiment directory from start params
    try
    {
        std::string simDirName = args[SimulationParams::ParamExpDir.data()].as<SimulationParams::ParamExpDirT>();
        NRPLogger::debug("Got working directory from parameters: {}", simDirName);
        if (std::filesystem::is_directory(simDirName)){
            NRPLogger::debug("Setting working directory from command line [ {} ]", simDirName);
            std::filesystem::current_path(simDirName);
            // Mark that the WD is set with parameter
            directoryIsSet = true;
        }
        else {
            throw std::invalid_argument("The provided experiment directory path [ " + simDirName + " ] is not a directory.");
        }
    }
    catch(std::domain_error&)
    {
    }

    try
    {
        // Get config file name from the parameters
        simCfgFileName = args[SimulationParams::ParamSimCfgFile.data()].as<SimulationParams::ParamSimCfgFileT>();
        // TODO: when NRPLogger is set the working directory should be final, but within this function it is changed
        //  so it should be run before, can we still use NRPLogger here?
        NRPLogger::debug("Got configuration file from parameters [ {} ]", simCfgFileName);

        // If the WD wasn't set with parameter, set it as parent path of the config
        auto simCfgFileDir = std::filesystem::path(simCfgFileName).parent_path();
        if (!directoryIsSet && std::filesystem::is_directory(simCfgFileDir)){
            NRPLogger::debug("Setting working directory from config file name [ {} ]", simCfgFileDir.c_str());
            std::filesystem::current_path(simCfgFileDir);
            // Remove directory name from config file name
            simCfgFileName = std::filesystem::path(simCfgFileName).filename().c_str();
            NRPLogger::debug("Using the relative path to the simulation config [ {} ]", simCfgFileName.c_str());
        }

        if (!std::filesystem::is_regular_file(simCfgFileName)){
            throw std::invalid_argument("The provided configuration file [ " + simCfgFileName + " ] is invalid");
        }
    }
    catch(std::domain_error&)
    {
        // If no simulation file name is present, return empty config
        NRPLogger::debug("Couldn't get configuration file from parameters, returning empty config");
        return simConfig;
    }

    simConfig.reset(new nlohmann::json(json_utils::parseJSONFile(simCfgFileName)));

    return simConfig;
}

NRPLogger::level_t SimulationParams::parseLogLevel(const std::string &strLogLevel)
{
    // try to parse log level taken from paramters
    NRPLogger::level_t level = NRPLogger::level_from_string(strLogLevel);

    // if the log level does not exist, the NRPLogger::level_from_string returns "off", what is a valid log level
    // in this case we check if the parameters are trying to turn off the log indeed
    if (level == NRPLogger::level_t::off && strLogLevel.compare(NRPLogger::level_to_string(NRPLogger::level_t::off).data()) != 0)
    {
        // if the input parameter is different from "off", then we apply default log level
        level = _defaultLogLevel;
        NRPLogger::warn(
            "Couldn't set the desired log level, using default [{}]. You specified: {}", 
            NRPLogger::level_to_string(level), strLogLevel);
    }
    return level;
}

void SimulationParams::parseAndSetCLISimParams(const ParamSimParamT &parseResults, nlohmann::json &simulationConfig)
{
    // key=value delimiter
    std::string delimiter = "=";

    for (size_t i = 0; i < parseResults.size(); i++)
    {
        auto param = parseResults[i];
        try
        {
            std::string key = param.substr(0, param.find(delimiter));
            std::string value = param.substr(param.find(delimiter) + delimiter.length());
            setCLISimParams(key, value, simulationConfig);
        }
        catch (NRPException &e)
        {
            throw NRPException::logCreate(e, "Couldn't override configuration file parameters with " + param);
        }
    }
}

void SimulationParams::setCLISimParams(const std::string &fullKey, const std::string &value, nlohmann::json &simulationConfig)
{
    // Nested.Key.Name delimiter
    std::string delimiter = ".";

    // Get the head key Name (before the first delimiter)
    std::string key = fullKey.substr(0, fullKey.find(delimiter));

    // The element for override
    nlohmann::json *element;

    // Treat differently JSON lists and dictionaries
    if (simulationConfig.is_array()){
        int idx = std::stoi(key);
        if ((size_t)idx < simulationConfig.size()){
            element = &simulationConfig.at(idx);
        }
        else {
            throw std::out_of_range("The list element ID to override is greater than the list size");
        }
    }
    else if (simulationConfig.contains(key))
    {
        element = &simulationConfig.at(key);
    }
    else
    {
        throw std::invalid_argument("The override parameter " + key + " is absent in the simulation config");
    }
    
    // If the head key coincides with initial key Name (there is no nested keys)
    if (fullKey.compare(key) == 0)
    {
        NRPLogger::info("Overriding config parameter \"{}\" with value \"{}\"", key, value);
        if (!element->is_string())
            *element = nlohmann::json::parse(value);
        else
            *element = value;
    }
    else
    {
        // Get the nested keys (after the first delimiter)
        std::string nestedKey = fullKey.substr(fullKey.find(delimiter) + delimiter.length());
        NRPLogger::debug("Overriding config parameter \"{}\" with nested key \"{}\"", key, nestedKey);
        // Make recursion to the nested key
        setCLISimParams(nestedKey, value, *element);
    }
}
