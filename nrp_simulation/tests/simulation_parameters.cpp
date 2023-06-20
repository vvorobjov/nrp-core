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

#include <gtest/gtest.h>

#include "nrp_general_library/utils/python_interpreter_state.h"
#include "nrp_simulation/simulation/simulation_parameters.h"
#include "tests/test_env_cmake.h"

#define DEBUG_NO_CREATE_ENGINE_LAUNCHER_FCN
#include "nrp_nest_json_engine/nrp_client/nest_engine_json_nrp_client.h"
#include "nrp_gazebo_grpc_engine/nrp_client/gazebo_engine_grpc_nrp_client.h"

using namespace testing;

static std::vector<const char*> createStartParamPtr(const std::vector<std::string> &startParamDat)
{
    std::vector<const char*> retVal;
    retVal.reserve(startParamDat.size());

    for(const auto &param : startParamDat)
    {
        retVal.push_back(param.data());
    }

    return retVal;
}

TEST(SimulationParametersTest, OptParser)
{
    auto optParser(SimulationParams::createStartParamParser());

    const char *pPyArgv = "simtest";
    PythonInterpreterState pyInterp(1, const_cast<char**>(&pPyArgv));

    std::vector<std::string> startParamDat;
    std::vector<const char*> startParams;

    // Test valid parameters
    startParamDat = {"nrp_server",
                    std::string("-") + SimulationParams::ParamHelp.data(),
                    std::string("-") + SimulationParams::ParamSimCfgFile.data(), "cfgFile.json",
                    std::string("-") + SimulationParams::ParamExpDir.data(), "experiment_dir",
                    std::string("--") + SimulationParams::ParamConsoleLogLevelLong.data(), "debug",
                    std::string("--") + SimulationParams::ParamFileLogLevelLong.data(), "trace",
                    std::string("--") + SimulationParams::ParamLogDirLong.data(), ""};

    startParams = createStartParamPtr(startParamDat);

    int argc = static_cast<int>(startParams.size());
    char **argv = const_cast<char**>(startParams.data());

    ASSERT_NO_THROW(optParser.parse(argc, argv));

    // Test invalid options
    startParams = {"nrp_server", "-fdsafdaf"};
    argc = static_cast<int>(startParams.size());
    argv = const_cast<char**>(startParams.data());

    ASSERT_THROW(optParser.parse(argc, argv), cxxopts::OptionParseException);

    startParamDat = {"nrp_server", std::string("-") + SimulationParams::ParamSimCfgFile.data()};
    startParams = createStartParamPtr(startParamDat);

    argc = static_cast<int>(startParams.size());
    argv = const_cast<char**>(startParams.data());

    ASSERT_THROW(optParser.parse(argc, argv), cxxopts::OptionParseException);
}

TEST(SimulationParametersTest, SetupExperimentConfig)
{
    auto optParser(SimulationParams::createStartParamParser());

    // Test no simulation file passed
    std::vector<std::string> startParamDat;
    std::vector<const char*> startParams = {"nrp_server"};
    int argc = static_cast<int>(startParams.size());
    char **argv = const_cast<char**>(startParams.data());

    {
        auto startParamVals(optParser.parse(argc, argv));

        ASSERT_EQ(SimulationParams::setWorkingDirectoryAndGetConfigFile(startParamVals), nullptr);
    }

    // Test non-existent file
    startParamDat = {"nrp_server",
                   std::string("-") + SimulationParams::ParamSimCfgFile.data(), "noFile.json"};
    startParams = createStartParamPtr(startParamDat);

    argc = static_cast<int>(startParams.size());
    argv = const_cast<char**>(startParams.data());

    {
        auto startParamVals(optParser.parse(argc, argv));
        ASSERT_THROW(SimulationParams::setWorkingDirectoryAndGetConfigFile(startParamVals), std::invalid_argument);
    }

    // Test invalid JSON config file
    startParamDat = {"nrp_server",
                     std::string("-") + SimulationParams::ParamSimCfgFile.data(), TEST_INVALID_JSON_FILE};
    startParams = createStartParamPtr(startParamDat);

    argc = static_cast<int>(startParams.size());
    argv = const_cast<char**>(startParams.data());

    {
        auto startParamVals(optParser.parse(argc, argv));
        ASSERT_THROW(SimulationParams::setWorkingDirectoryAndGetConfigFile(startParamVals), std::invalid_argument);
    }

    // Test valid JSON config file
    startParamDat = {"nrp_server",
                     std::string("-") + SimulationParams::ParamSimCfgFile.data(), TEST_SIM_SIMPLE_CONFIG_FILE};
    startParams = createStartParamPtr(startParamDat);

    argc = static_cast<int>(startParams.size());
    argv = const_cast<char**>(startParams.data());

    {
        auto startParamVals(optParser.parse(argc, argv));

        ASSERT_NE(SimulationParams::setWorkingDirectoryAndGetConfigFile(startParamVals), nullptr);
    }
}


TEST(SimulationParametersTest, OverrideExperimentConfig)
{
    auto optParser(SimulationParams::createStartParamParser());

    // Test invalid simulation argument
    std::vector<std::string> startParamDat = {"nrp_server",
                     std::string("-") + SimulationParams::ParamSimCfgFile.data(), TEST_SIM_SIMPLE_CONFIG_FILE,
                     std::string("-") + SimulationParams::ParamSimParam.data(), "SomeInvalidParameter=SomeInvalidValue"};
    std::vector<const char*> startParams = createStartParamPtr(startParamDat);

    int argc = static_cast<int>(startParams.size());
    char **argv = const_cast<char**>(startParams.data());

    {
        auto startParamVals(optParser.parse(argc, argv));

        jsonSharedPtr simConfig = SimulationParams::setWorkingDirectoryAndGetConfigFile(startParamVals);

        EXPECT_NO_THROW(json_utils::validateJson(*simConfig, "json://nrp-core/simulation.json#Simulation"));
        ASSERT_THROW(SimulationParams::parseAndSetCLISimParams(startParamVals[SimulationParams::ParamSimParam.data()].as<SimulationParams::ParamSimParamT>(), *simConfig), std::invalid_argument);
    }

    // Test valid simulation argument
    startParamDat = {"nrp_server",
                     std::string("-") + SimulationParams::ParamSimCfgFile.data(), TEST_SIM_SIMPLE_CONFIG_FILE,
                     std::string("-") + SimulationParams::ParamSimParam.data(), "SimulationName=NewName"};
    startParams = createStartParamPtr(startParamDat);

    argc = static_cast<int>(startParams.size());
    argv = const_cast<char**>(startParams.data());

    {
        auto startParamVals(optParser.parse(argc, argv));

        jsonSharedPtr simConfig = SimulationParams::setWorkingDirectoryAndGetConfigFile(startParamVals);

        EXPECT_NO_THROW(json_utils::validateJson(*simConfig, "json://nrp-core/simulation.json#Simulation"));
        ASSERT_NO_THROW(SimulationParams::parseAndSetCLISimParams(startParamVals[SimulationParams::ParamSimParam.data()].as<SimulationParams::ParamSimParamT>(), *simConfig));
        ASSERT_EQ(simConfig->at("SimulationName").get<std::string>(), "NewName");
    }

    // Test valid nested simulation argument and comma separation
    startParamDat = {"nrp_server",
                     std::string("-") + SimulationParams::ParamSimCfgFile.data(), TEST_SIM_SIMPLE_CONFIG_FILE,
                     std::string("-") + SimulationParams::ParamSimParam.data(), "EngineConfigs.0.EngineName=NewName,EngineConfigs.0.EngineType=NewType"};
    startParams = createStartParamPtr(startParamDat);

    argc = static_cast<int>(startParams.size());
    argv = const_cast<char**>(startParams.data());

    {
        auto startParamVals(optParser.parse(argc, argv));

        jsonSharedPtr simConfig = SimulationParams::setWorkingDirectoryAndGetConfigFile(startParamVals);

        EXPECT_NO_THROW(json_utils::validateJson(*simConfig, "json://nrp-core/simulation.json#Simulation"));
        ASSERT_NO_THROW(SimulationParams::parseAndSetCLISimParams(startParamVals[SimulationParams::ParamSimParam.data()].as<SimulationParams::ParamSimParamT>(), *simConfig));
        ASSERT_EQ(simConfig->at("EngineConfigs").at(0).at("EngineName").get<std::string>(), "NewName");
        ASSERT_EQ(simConfig->at("EngineConfigs").at(0).at("EngineType").get<std::string>(), "NewType");
    }

    // Test invalid nested simulation argument
    startParamDat = {"nrp_server",
                     std::string("-") + SimulationParams::ParamSimCfgFile.data(), TEST_SIM_SIMPLE_CONFIG_FILE,
                     std::string("-") + SimulationParams::ParamSimParam.data(), "EngineConfigs.EngineName=NewName"};
    startParams = createStartParamPtr(startParamDat);

    argc = static_cast<int>(startParams.size());
    argv = const_cast<char**>(startParams.data());

    {
        auto startParamVals(optParser.parse(argc, argv));

        jsonSharedPtr simConfig = SimulationParams::setWorkingDirectoryAndGetConfigFile(startParamVals);

        EXPECT_NO_THROW(json_utils::validateJson(*simConfig, "json://nrp-core/simulation.json#Simulation"));
        ASSERT_THROW(SimulationParams::parseAndSetCLISimParams(startParamVals[SimulationParams::ParamSimParam.data()].as<SimulationParams::ParamSimParamT>(), *simConfig), std::logic_error);
    }

    // Test valid list element override with empty dictionary 
    startParamDat = {"nrp_server",
                     std::string("-") + SimulationParams::ParamSimCfgFile.data(), TEST_SIM_SIMPLE_CONFIG_FILE,
                     std::string("-") + SimulationParams::ParamSimParam.data(), "EngineConfigs.0={}"};
    startParams = createStartParamPtr(startParamDat);

    argc = static_cast<int>(startParams.size());
    argv = const_cast<char**>(startParams.data());

    {
        auto startParamVals(optParser.parse(argc, argv));

        jsonSharedPtr simConfig = SimulationParams::setWorkingDirectoryAndGetConfigFile(startParamVals);

        EXPECT_NO_THROW(json_utils::validateJson(*simConfig, "json://nrp-core/simulation.json#Simulation"));
        ASSERT_NO_THROW(SimulationParams::parseAndSetCLISimParams(startParamVals[SimulationParams::ParamSimParam.data()].as<SimulationParams::ParamSimParamT>(), *simConfig));
        ASSERT_TRUE(simConfig->at("EngineConfigs").at(0).empty());
    }

    // Test invalid nested simulation argument list id
    startParamDat = {"nrp_server",
                     std::string("-") + SimulationParams::ParamSimCfgFile.data(), TEST_SIM_SIMPLE_CONFIG_FILE,
                     std::string("-") + SimulationParams::ParamSimParam.data(), "EngineConfigs.10.EngineName=NewName"};
    startParams = createStartParamPtr(startParamDat);

    argc = static_cast<int>(startParams.size());
    argv = const_cast<char**>(startParams.data());

    {
        auto startParamVals(optParser.parse(argc, argv));

        jsonSharedPtr simConfig = SimulationParams::setWorkingDirectoryAndGetConfigFile(startParamVals);

        EXPECT_NO_THROW(json_utils::validateJson(*simConfig, "json://nrp-core/simulation.json#Simulation"));
        ASSERT_THROW(SimulationParams::parseAndSetCLISimParams(startParamVals[SimulationParams::ParamSimParam.data()].as<SimulationParams::ParamSimParamT>(), *simConfig), std::out_of_range);
    }
}

TEST(SimulationParametersTest, SetupExperimentDirectory)
{
    auto optParser(SimulationParams::createStartParamParser());

    // Test invalid experiment directory
    std::vector<std::string> startParamDat = {"nrp_server",
                     std::string("-") + SimulationParams::ParamExpDir.data(), "non/existing/directory"};
    std::vector<const char*> startParams = createStartParamPtr(startParamDat);

    int argc = static_cast<int>(startParams.size());
    char **argv = const_cast<char**>(startParams.data());

    {
        auto startParamVals(optParser.parse(argc, argv));
        ASSERT_THROW(SimulationParams::setWorkingDirectoryAndGetConfigFile(startParamVals), std::invalid_argument);
    }

    // Test valid example directory
    startParamDat = {"nrp_server",
                     std::string("-") + SimulationParams::ParamExpDir.data(), std::filesystem::path(TEST_SIM_SIMPLE_CONFIG_FILE).parent_path()};
    startParams = createStartParamPtr(startParamDat);

    argc = static_cast<int>(startParams.size());
    argv = const_cast<char**>(startParams.data());

    {
        auto startParamVals(optParser.parse(argc, argv));
        ASSERT_NO_THROW(SimulationParams::setWorkingDirectoryAndGetConfigFile(startParamVals));
    }

    // Test valid JSON config file and valid example directory
    startParamDat = {"nrp_server",
                     std::string("-") + SimulationParams::ParamSimCfgFile.data(), std::filesystem::path(TEST_SIM_SIMPLE_CONFIG_FILE).filename(),
                     std::string("-") + SimulationParams::ParamExpDir.data(), std::filesystem::path(TEST_SIM_SIMPLE_CONFIG_FILE).parent_path()};
    startParams = createStartParamPtr(startParamDat);

    argc = static_cast<int>(startParams.size());
    argv = const_cast<char**>(startParams.data());

    {
        auto startParamVals(optParser.parse(argc, argv));

        ASSERT_NE(SimulationParams::setWorkingDirectoryAndGetConfigFile(startParamVals), nullptr);
    }

    // Test valid JSON config file with absolute path
    startParamDat = {"nrp_server",
                     std::string("-") + SimulationParams::ParamSimCfgFile.data(), TEST_SIM_SIMPLE_CONFIG_FILE};
    startParams = createStartParamPtr(startParamDat);

    argc = static_cast<int>(startParams.size());
    argv = const_cast<char**>(startParams.data());

    {
        auto startParamVals(optParser.parse(argc, argv));

        ASSERT_NE(SimulationParams::setWorkingDirectoryAndGetConfigFile(startParamVals), nullptr);
    }

    // Test invalid JSON config file and valid example directory
    startParamDat = {"nrp_server",
                     std::string("-") + SimulationParams::ParamSimCfgFile.data(), "noFile.json",
                     std::string("-") + SimulationParams::ParamExpDir.data(), std::filesystem::path(TEST_SIM_SIMPLE_CONFIG_FILE).parent_path()};
    startParams = createStartParamPtr(startParamDat);

    argc = static_cast<int>(startParams.size());
    argv = const_cast<char**>(startParams.data());

    {
        auto startParamVals(optParser.parse(argc, argv));
        ASSERT_THROW(SimulationParams::setWorkingDirectoryAndGetConfigFile(startParamVals), std::invalid_argument);
    }
}