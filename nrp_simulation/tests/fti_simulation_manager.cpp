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

#include <gtest/gtest.h>

#include "nrp_general_library/utils/python_interpreter_state.h"
#include "nrp_simulation/simulation/simulation_manager.h"
#include "nrp_simulation/simulation/simulation_manager_fti.h"
#include "nrp_simulation/simulation/simulation_parameters.h"
#include "nrp_simulation/config/cmake_conf.h"
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

TEST(FTISimManagerTest, FTISimManagerInitialize)
{
    auto optParser(SimulationParams::createStartParamParser());

    const char *pPyArgv = "simtest";
    PythonInterpreterState pyInterp(1, const_cast<char**>(&pPyArgv));

    std::vector<std::string> startParamDat = {"nrp_server",
                                              std::string("-") + SimulationParams::ParamSimCfgFile.data(), TEST_SIM_CONFIG_FILE};
    std::vector<const char*> startParams = createStartParamPtr(startParamDat);

    auto argc = static_cast<int>(startParams.size());
    auto argv = const_cast<char**>(startParams.data());

    auto startParamVals(optParser.parse(argc, argv));
    jsonSharedPtr simConfig = SimulationParams::setWorkingDirectoryAndGetConfigFile(startParamVals);

    EngineLauncherManagerSharedPtr engines(new EngineLauncherManager());
    MainProcessLauncherManagerSharedPtr processManager(new MainProcessLauncherManager());
    FTILoopSimManager manager(simConfig, engines, processManager);

    // Exception if required engine launchers is not added
    manager.initializeSimulation();
    ASSERT_EQ(manager.currentState(), SimulationManager::SimState::Failed);
    manager.shutdownSimulation();

    // Add launchers
    engines->registerLauncher(EngineLauncherInterfaceSharedPtr(new GazeboEngineGrpcLauncher()));
    engines->registerLauncher(EngineLauncherInterfaceSharedPtr(new NestEngineJSONLauncher()));

    manager.initializeSimulation();
    ASSERT_EQ(manager.currentState(), SimulationManager::SimState::Initialized);
    ASSERT_NO_THROW(manager.runSimulation(1, nlohmann::json()));
    ASSERT_EQ(manager.currentState(), SimulationManager::SimState::Stopped);
}

TEST(FTISimManagerTest, FTISimManagerLoopReset)
{
    auto optParser(SimulationParams::createStartParamParser());

    const char *pPyArgv = "simtest";
    PythonInterpreterState pyInterp(1, const_cast<char**>(&pPyArgv));

    std::vector<std::string> startParamDat = {"nrp_server",
                                              std::string("-") + SimulationParams::ParamSimCfgFile.data(), TEST_SIM_CONFIG_FILE};
    std::vector<const char*> startParams = createStartParamPtr(startParamDat);

    auto argc = static_cast<int>(startParams.size());
    auto argv = const_cast<char**>(startParams.data());

    auto startParamVals(optParser.parse(argc, argv));
    jsonSharedPtr simConfig = SimulationParams::setWorkingDirectoryAndGetConfigFile(startParamVals);

    EngineLauncherManagerSharedPtr engines(new EngineLauncherManager());
    engines->registerLauncher(EngineLauncherInterfaceSharedPtr(new GazeboEngineGrpcLauncher()));
    engines->registerLauncher(EngineLauncherInterfaceSharedPtr(new NestEngineJSONLauncher()));
    MainProcessLauncherManagerSharedPtr processManager(new MainProcessLauncherManager());
    FTILoopSimManager manager(simConfig, engines, processManager);

    manager.initializeSimulation();

    ASSERT_NO_THROW(manager.runSimulation(1, nlohmann::json()));
    ASSERT_EQ(manager.resetSimulation().currentState, SimulationManager::SimState::Initialized);
    ASSERT_NO_THROW(manager.runSimulation(1, nlohmann::json()));
}
