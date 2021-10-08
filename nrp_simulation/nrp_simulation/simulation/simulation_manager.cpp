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

#include "nrp_simulation/simulation/simulation_manager.h"

#include "nrp_general_library/utils/file_finder.h"
#include "nrp_general_library/utils/nrp_exceptions.h"
#include "nrp_simulation/config/cmake_conf.h"
#include "nrp_general_library/utils/nrp_logger.h"

#include <iostream>
#include <fstream>
#include <future>
#include <filesystem>


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
            (SimulationParams::ParamLogDirLong.data(), SimulationParams::ParamLogDirDesc.data(),
             cxxopts::value<SimulationParams::ParamLogDirT>()->default_value("logs"))
        (SimulationParams::ParamModeLong.data(), SimulationParams::ParamModeDesc.data(),
             cxxopts::value<SimulationParams::ParamModeT>()->default_value("standalone"))
            (SimulationParams::ParamServerAddressLong.data(), SimulationParams::ParamServerAddressDesc.data(),
             cxxopts::value<SimulationParams::ParamServerAddressT>()->default_value(""));

    return opts;
}

nlohmann::json SimulationParams::parseJSONFile(const std::string &fileName)
{
    // Try to open file
    std::ifstream cfgFile(fileName);
    if(!cfgFile.is_open())
    {
        throw std::invalid_argument("Error: Could not open config file " + fileName);
    }

    // Parse JSON from file
    nlohmann::json cfgJSON;
    try
    {
        cfgFile >> cfgJSON;
    }
    catch(std::exception &e)
    {
        throw std::invalid_argument("Error: Could not parse config file " + fileName + "\n" + e.what());
    }

    return cfgJSON;
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


SimulationManager::SimulationManager(const jsonSharedPtr &simulationConfig)
    : _simConfig(simulationConfig)
{}

SimulationManager::~SimulationManager()
{
    NRP_LOGGER_TRACE("{} called", __FUNCTION__);

    this->shutdownLoop();
}


jsonSharedPtr SimulationManager::configFromParams(const cxxopts::ParseResult &args)
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
        NRPLogger::debug("Got configuration file from parameters [ {} ]", simCfgFileName);

        // Change working directory and config path if needed
        auto simCfgFileDir = std::filesystem::path(simCfgFileName).parent_path();
        if (!directoryIsSet && std::filesystem::is_directory(simCfgFileDir)){
            NRPLogger::debug("Setting working directory from config file name [ {} ]", simCfgFileDir.c_str());
            // If the WD wasn't set with parameter, set it as parent path of the config
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

    simConfig.reset(new nlohmann::json(SimulationParams::parseJSONFile(simCfgFileName)));

    return simConfig;
}


SimulationManager SimulationManager::createFromConfig(jsonSharedPtr &config)
{
    json_utils::validate_json(*config, "https://neurorobotics.net/simulation.json#Simulation");

    // Set default values

    json_utils::set_default<std::vector<std::string>>(*config, "DataPackProcessingFunctions", std::vector<std::string>());

    return SimulationManager(config);
}

FTILoopConstSharedPtr SimulationManager::simulationLoop() const
{
    return this->_loop;
}

jsonSharedPtr SimulationManager::simulationConfig()
{
    return this->_simConfig;
}

jsonConstSharedPtr SimulationManager::simulationConfig() const
{
    return this->_simConfig;
}

void SimulationManager::initFTILoop(const EngineLauncherManagerConstSharedPtr &engineLauncherManager,
                                           const MainProcessLauncherManager::const_shared_ptr &processLauncherManager)
{
    NRP_LOGGER_TRACE("{} called", __FUNCTION__);

    // Create and initialize loop
    NRPLogger::info("Initializing simulation loop");

    if(this->_loop == nullptr)
    {
        this->_loop.reset(new FTILoop(this->createSimLoop(engineLauncherManager, processLauncherManager)));
    }
    else
    {
        throw NRPException::logCreate("Simulation already initialized");
    }

    this->_loop->initLoop();
}

bool SimulationManager::runSimulationUntilTimeout(int frac)
{
    NRP_LOGGER_TRACE("{} called", __FUNCTION__);

    if(this->_loop == nullptr)
        return false;

    bool hasTimedOut = false;

    while(1)
    {
        hasTimedOut = hasSimTimedOut(this->_loop->getSimTime(), toSimulationTime<unsigned, std::ratio<1>>(frac * int(this->_simConfig->at("SimulationTimeout"))));

        if(hasTimedOut)
            break;

        SimulationTime timeStep = toSimulationTime<float, std::ratio<1>>(this->_simConfig->at("SimulationTimestep"));

        this->_loop->runLoop(timeStep);
    }

    return hasTimedOut;
}

bool SimulationManager::resetSimulation()
{
    NRP_LOGGER_TRACE("{} called", __FUNCTION__);

    NRPLogger::info("SimulationManager: resetting simulation...");

/*  if (this->isRunning()){
        NRPLogger::error("Cannot reset the running simulation");
        return false;
    }
    if(simLock.owns_lock())
        simLock.unlock();*/

    //sim_lock_t internalLock(this->_internalLock);

    try{
        if(this->_loop != nullptr) {
            //simLock.lock();

            this->_loop->resetLoop();

            //simLock.unlock();
            //std::this_thread::yield();
        }
        else{
            throw NRPException::logCreate("SimulationManager: cannot reset the loop, the loop doesn't exist");
        }
    }
    catch(NRPException &e) {
        throw NRPException::logCreate(e, "SimulationManager: Loop reset has FAILED");
    }

    NRPLogger::info("SimulationManager: simulation is reset.");
    return true;
}

void SimulationManager::runSimulation(unsigned numIterations)
{
    NRP_LOGGER_TRACE("{} called", __FUNCTION__);

    if(this->_loop == nullptr)
        throw NRPException::logCreate("Simulation must be initialized before calling runLoop");

    const SimulationTime timeStep = toSimulationTime<float, std::ratio<1>>(this->_simConfig->at("SimulationTimestep"));

    unsigned iteration = 0;

    while(iteration++ < numIterations)
    {
        this->_loop->runLoop(timeStep);
    }
}

void SimulationManager::shutdownLoop()
{
    NRP_LOGGER_TRACE("{} called", __FUNCTION__);

    try {
        if(this->_loop != nullptr) {
            this->_loop->shutdownLoop();

            this->_loop = nullptr;
        }
    }
    catch(NRPException &e) {
        throw NRPException::logCreate(e, "SimulationManager: Loop shutdown has FAILED");
    }

}

FTILoop SimulationManager::createSimLoop(const EngineLauncherManagerConstSharedPtr &engineManager, const MainProcessLauncherManager::const_shared_ptr &processLauncherManager)
{
    NRP_LOGGER_TRACE("{} called", __FUNCTION__);

    DataPackHandle::engine_interfaces_t engines;
    auto &engineConfigs = this->_simConfig->at("EngineConfigs");

    // Create all engines required by simConfig
    engines.reserve(engineConfigs.size());
    for(auto &engineConfig : engineConfigs)
    {
        // Get engine launcher associated with type
        const std::string engineType = engineConfig.at("EngineType");
        auto engineLauncher = engineManager->findLauncher(engineType);
        if(engineLauncher == nullptr)
        {
            const auto errMsg = "Failed to find engine interface \"" + engineType + "\"";
            NRPLogger::error(errMsg);

            throw std::invalid_argument(errMsg);
        }

        // Create and launch engine
        try
        {
            engines.push_back(engineLauncher->launchEngine(engineConfig, processLauncherManager->createProcessLauncher(this->_simConfig->at("ProcessLauncherType"))));
        }
        catch(std::exception &e)
        {
            throw NRPException::logCreate(e, "Failed to launch engine interface \"" + engineLauncher->engineType() + "\"");
        }
    }

    return FTILoop(this->_simConfig, engines);
}
