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

auto parseStartParams(auto &optParser, const std::vector<std::string> &startParamDat) {
    auto startParams = createStartParamPtr(startParamDat);
    int argc = static_cast<int>(startParams.size());
    char **argv = const_cast<char**>(startParams.data());
    return optParser.parse(argc, argv);
}

jsonSharedPtr parseStartParamsAndGetConfig(auto &optParser, const std::vector<std::string> &startParamDat) {
    auto startParamVals(parseStartParams(optParser, startParamDat));
    return SimulationParams::setWorkingDirectoryAndGetConfigFile(startParamVals);
}

TEST(SimulationParametersTest, ValidParametersAreParsedWithoutExceptions)
{
    auto optParser(SimulationParams::createStartParamParser());

    std::vector<std::string> startParamDat;

    // Test valid parameters
    startParamDat = {"nrp_server",
                    std::string("-") + SimulationParams::ParamHelp.data(),
                    std::string("-") + SimulationParams::ParamSimCfgFile.data(), "cfgFile.json",
                    std::string("-") + SimulationParams::ParamExpDir.data(), "experiment_dir",
                    std::string("--") + SimulationParams::ParamConsoleLogLevelLong.data(), "debug",
                    std::string("--") + SimulationParams::ParamFileLogLevelLong.data(), "trace",
                    std::string("--") + SimulationParams::ParamLogDirLong.data(), ""};


    ASSERT_NO_THROW(parseStartParams(optParser, startParamDat));
}

TEST(SimulationParametersTest, InvalidOptionCausesParseException) {
    auto optParser = SimulationParams::createStartParamParser();

    std::vector<std::string> startParamDat = {"nrp_server", "-fdsafdaf"};
    ASSERT_THROW(parseStartParams(optParser, startParamDat), cxxopts::OptionParseException);
}

TEST(SimulationParametersTest, MissingOptionValueCausesParseException) {
    auto optParser = SimulationParams::createStartParamParser();
    std::vector<std::string> startParamDat = {"nrp_server", std::string("-") + SimulationParams::ParamSimCfgFile.data()};
    ASSERT_THROW(parseStartParams(optParser, startParamDat), cxxopts::OptionParseException);
}

TEST(SimulationParametersTest, NoSimulationFilePassedReturnsNull)
{
    auto optParser(SimulationParams::createStartParamParser());
    std::vector<std::string> startParamDat = {"nrp_server"};
    ASSERT_EQ(parseStartParamsAndGetConfig(optParser, startParamDat), nullptr);
}

TEST(SimulationParametersTest, NonExistentFileThrowsException)
{
    auto optParser = SimulationParams::createStartParamParser();
    std::vector<std::string> startParamDat = {
        "nrp_server",
        std::string("-") + SimulationParams::ParamSimCfgFile.data(),
        "noFile.json"};
    ASSERT_THROW(parseStartParamsAndGetConfig(optParser, startParamDat), std::invalid_argument);
}

TEST(SimulationParametersTest, InvalidJSONConfigFileThrowsException)
{
    auto optParser = SimulationParams::createStartParamParser();
    std::vector<std::string> startParamDat = {
        "nrp_server",
        std::string("-") + SimulationParams::ParamSimCfgFile.data(),
        TEST_INVALID_JSON_FILE};
    ASSERT_THROW(parseStartParamsAndGetConfig(optParser, startParamDat), std::invalid_argument);
}

TEST(SimulationParametersTest, ValidJSONConfigFileReturnsNonNull)
{
    auto optParser = SimulationParams::createStartParamParser();
    std::vector<std::string> startParamDat = {
        "nrp_server",
        std::string("-") + SimulationParams::ParamSimCfgFile.data(),
        TEST_SIM_SIMPLE_CONFIG_FILE};
    ASSERT_NE(parseStartParamsAndGetConfig(optParser, startParamDat), nullptr);
}

TEST(SimulationParametersTest, AddsParametersToConfig)
{
    auto optParser(SimulationParams::createStartParamParser());

    // Test non-existing simulation argument
    std::vector<std::string> startParamDat =
        {"nrp_server",
         std::string("-") + SimulationParams::ParamSimCfgFile.data(), TEST_SIM_SIMPLE_CONFIG_FILE,
         std::string("-") + SimulationParams::ParamSimParam.data(), "SomeNewParameter=\"SomeNewValue\"",
         std::string("-") + SimulationParams::ParamSimParam.data(), "SomeParameter.NestedParameter=\"SomeValue\""};

    auto startParamVals(parseStartParams(optParser, startParamDat));
    jsonSharedPtr simConfig = SimulationParams::setWorkingDirectoryAndGetConfigFile(startParamVals);

    EXPECT_NO_THROW(json_utils::validateJson(*simConfig, "json://nrp-core/simulation.json#Simulation"));
    ASSERT_NO_THROW(SimulationParams::parseAndSetCLISimParams(startParamVals[SimulationParams::ParamSimParam.data()].as<SimulationParams::ParamSimParamT>(), *simConfig));
    ASSERT_EQ(simConfig->at("SomeNewParameter").get<std::string>(), "SomeNewValue");
    ASSERT_EQ(simConfig->at("SomeParameter").at("NestedParameter").get<std::string>(), "SomeValue");
}

TEST(SimulationParametersTest, ModifiesParametersInConfig)
{
    auto optParser(SimulationParams::createStartParamParser());

    // Test existing simulation argument
    std::vector<std::string> startParamDat =
        {"nrp_server",
         std::string("-") + SimulationParams::ParamSimCfgFile.data(), TEST_SIM_SIMPLE_CONFIG_FILE,
         std::string("-") + SimulationParams::ParamSimParam.data(), "SimulationName=\"NewName\""};

    auto startParamVals(parseStartParams(optParser, startParamDat));
    jsonSharedPtr simConfig = SimulationParams::setWorkingDirectoryAndGetConfigFile(startParamVals);

    EXPECT_NO_THROW(json_utils::validateJson(*simConfig, "json://nrp-core/simulation.json#Simulation"));
    ASSERT_NO_THROW(SimulationParams::parseAndSetCLISimParams(startParamVals[SimulationParams::ParamSimParam.data()].as<SimulationParams::ParamSimParamT>(), *simConfig));
    ASSERT_EQ(simConfig->at("SimulationName").get<std::string>(), "NewName");
}

TEST(SimulationParametersTest, CLIModifiesNestetParsInConfig)
{
    auto optParser(SimulationParams::createStartParamParser());

    // Test valid nested simulation argument and comma separation
    std::vector<std::string> startParamDat =
        {"nrp_server",
         std::string("-") + SimulationParams::ParamSimCfgFile.data(), TEST_SIM_SIMPLE_CONFIG_FILE,
         std::string("-") + SimulationParams::ParamSimParam.data(), "EngineConfigs.0.EngineName=\"NewName\",EngineConfigs.0.EngineType=\"NewType\"",
         std::string("-") + SimulationParams::ParamSimParam.data(), "EngineConfigs.EngineName:NewName.NewKey=\"NewValue\"",
         std::string("-") + SimulationParams::ParamSimParam.data(), "EngineConfigs.0.NewBool=false",
         std::string("-") + SimulationParams::ParamSimParam.data(), "EngineConfigs.0.NewInt=13"};

    auto startParamVals(parseStartParams(optParser, startParamDat));
    jsonSharedPtr simConfig = SimulationParams::setWorkingDirectoryAndGetConfigFile(startParamVals);

    EXPECT_NO_THROW(json_utils::validateJson(*simConfig, "json://nrp-core/simulation.json#Simulation"));
    ASSERT_NO_THROW(SimulationParams::parseAndSetCLISimParams(startParamVals[SimulationParams::ParamSimParam.data()].as<SimulationParams::ParamSimParamT>(), *simConfig));
    ASSERT_EQ(simConfig->at("EngineConfigs").at(0).at("EngineName").get<std::string>(), "NewName");
    ASSERT_EQ(simConfig->at("EngineConfigs").at(0).at("EngineType").get<std::string>(), "NewType");
    ASSERT_EQ(simConfig->at("EngineConfigs").at(0).at("NewKey").get<std::string>(), "NewValue");
    ASSERT_EQ(simConfig->at("EngineConfigs").at(0).at("NewBool").get<bool>(), false);
    ASSERT_EQ(simConfig->at("EngineConfigs").at(0).at("NewInt").get<int>(), 13);
}

TEST(SimulationParametersTest, ChangeArrayToElementThrows)
{
    auto optParser(SimulationParams::createStartParamParser());

    // Test invalid nested simulation argument
    std::vector<std::string> startParamDat =
        {"nrp_server",
         std::string("-") + SimulationParams::ParamSimCfgFile.data(), TEST_SIM_SIMPLE_CONFIG_FILE,
         std::string("-") + SimulationParams::ParamSimParam.data(), "EngineConfigs.EngineName=\"NewName\""};

    auto startParamVals(parseStartParams(optParser, startParamDat));
    jsonSharedPtr simConfig = SimulationParams::setWorkingDirectoryAndGetConfigFile(startParamVals);

    EXPECT_NO_THROW(json_utils::validateJson(*simConfig, "json://nrp-core/simulation.json#Simulation"));
    ASSERT_THROW(SimulationParams::parseAndSetCLISimParams(startParamVals[SimulationParams::ParamSimParam.data()].as<SimulationParams::ParamSimParamT>(), *simConfig), std::logic_error);
}

TEST(SimulationParametersTest, AddsEmptyObjectToConfig)
{
    auto optParser(SimulationParams::createStartParamParser());

    // Test valid list element override with empty dictionary
    std::vector<std::string> startParamDat =
        {"nrp_server",
         std::string("-") + SimulationParams::ParamSimCfgFile.data(), TEST_SIM_SIMPLE_CONFIG_FILE,
         std::string("-") + SimulationParams::ParamSimParam.data(), "EngineConfigs.0={}"};

    auto startParamVals(parseStartParams(optParser, startParamDat));
    jsonSharedPtr simConfig = SimulationParams::setWorkingDirectoryAndGetConfigFile(startParamVals);

    EXPECT_NO_THROW(json_utils::validateJson(*simConfig, "json://nrp-core/simulation.json#Simulation"));
    ASSERT_NO_THROW(SimulationParams::parseAndSetCLISimParams(startParamVals[SimulationParams::ParamSimParam.data()].as<SimulationParams::ParamSimParamT>(), *simConfig));
    ASSERT_TRUE(simConfig->at("EngineConfigs").at(0).empty());
}

TEST(SimulationParametersTest, EditingNonexistingArrayElementThrows)
{
    auto optParser(SimulationParams::createStartParamParser());

    // Test invalid nested simulation argument list id
    std::vector<std::string> startParamDat =
        {"nrp_server",
         std::string("-") + SimulationParams::ParamSimCfgFile.data(), TEST_SIM_SIMPLE_CONFIG_FILE,
         std::string("-") + SimulationParams::ParamSimParam.data(), "EngineConfigs.10.EngineName=\"NewName\""};

    auto startParamVals(parseStartParams(optParser, startParamDat));
    jsonSharedPtr simConfig = SimulationParams::setWorkingDirectoryAndGetConfigFile(startParamVals);

    EXPECT_NO_THROW(json_utils::validateJson(*simConfig, "json://nrp-core/simulation.json#Simulation"));
    ASSERT_THROW(SimulationParams::parseAndSetCLISimParams(startParamVals[SimulationParams::ParamSimParam.data()].as<SimulationParams::ParamSimParamT>(), *simConfig), std::out_of_range);
}

TEST(SimulationParametersTest, InvalidWorkingDirThrows)
{
    auto optParser(SimulationParams::createStartParamParser());

    // Test invalid experiment directory
    std::vector<std::string> startParamDat =
        {"nrp_server",
         std::string("-") + SimulationParams::ParamExpDir.data(), "non/existing/directory"};

    ASSERT_THROW(parseStartParamsAndGetConfig(optParser, startParamDat), std::invalid_argument);
}

TEST(SimulationParametersTest, ValidWorkingDir)
{
    auto optParser(SimulationParams::createStartParamParser());

    // Test valid example directory
    std::vector<std::string> startParamDat =
        {"nrp_server",
         std::string("-") + SimulationParams::ParamExpDir.data(), std::filesystem::path(TEST_SIM_SIMPLE_CONFIG_FILE).parent_path()};

    ASSERT_NO_THROW(parseStartParamsAndGetConfig(optParser, startParamDat));
}

TEST(SimulationParametersTest, ValidWorkingDirAndConfig)
{
    auto optParser(SimulationParams::createStartParamParser());

    // Test valid JSON config file and valid example directory
    std::vector<std::string> startParamDat =
        {"nrp_server",
         std::string("-") + SimulationParams::ParamSimCfgFile.data(), std::filesystem::path(TEST_SIM_SIMPLE_CONFIG_FILE).filename(),
         std::string("-") + SimulationParams::ParamExpDir.data(), std::filesystem::path(TEST_SIM_SIMPLE_CONFIG_FILE).parent_path()};

    ASSERT_NE(parseStartParamsAndGetConfig(optParser, startParamDat), nullptr);

    startParamDat =
        {"nrp_server",
         std::string("-") + SimulationParams::ParamSimCfgFile.data(), TEST_SIM_SIMPLE_CONFIG_FILE};

    ASSERT_NE(parseStartParamsAndGetConfig(optParser, startParamDat), nullptr);
}

TEST(SimulationParametersTest, ValidWorkingDirAndInvalidConfigThrow)
{
    auto optParser(SimulationParams::createStartParamParser());

    // Test invalid JSON config file and valid example directory
    std::vector<std::string> startParamDat =
        {"nrp_server",
         std::string("-") + SimulationParams::ParamSimCfgFile.data(), "noFile.json",
         std::string("-") + SimulationParams::ParamExpDir.data(), std::filesystem::path(TEST_SIM_SIMPLE_CONFIG_FILE).parent_path()};

    ASSERT_THROW(parseStartParamsAndGetConfig(optParser, startParamDat), std::invalid_argument);
}