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
#include <fstream>

#include "nrp_general_library/process_launchers/process_launcher_basic.h"
#include "nrp_general_library/utils/python_interpreter_state.h"
#include "nrp_simulation/simulation/fti_loop.h"
#include "tests/test_env_cmake.h"

#define DEBUG_NO_CREATE_ENGINE_LAUNCHER_FCN
#include "nrp_nest_json_engine/nrp_client/nest_engine_json_nrp_client.h"
#include "nrp_gazebo_grpc_engine/nrp_client/gazebo_engine_grpc_nrp_client.h"


using namespace testing;

TEST(FTILoopTest, Constructor)
{
    auto simConfigFile = std::fstream(TEST_SIM_CONFIG_FILE, std::ios::in);

    const char *procName = "test";
    PythonInterpreterState pyState(1, const_cast<char**>(&procName));

    jsonSharedPtr config(new nlohmann::json(nlohmann::json::parse(simConfigFile)));

    EngineClientInterfaceSharedPtr brain(NestEngineJSONLauncher().launchEngine(config->at("EngineConfigs").at(1), ProcessLauncherInterface::unique_ptr(new ProcessLauncherBasic())));
    EngineClientInterfaceSharedPtr physics(GazeboEngineGrpcLauncher().launchEngine(config->at("EngineConfigs").at(0), ProcessLauncherInterface::unique_ptr(new ProcessLauncherBasic())));

    ASSERT_NO_THROW(FTILoop simLoop(config, {brain, physics}));
}

TEST(FTILoopTest, RunLoop)
{
    using namespace std::chrono_literals;

    auto simConfigFile = std::fstream(TEST_SIM_CONFIG_FILE, std::ios::in);
    jsonSharedPtr config(new nlohmann::json(nlohmann::json::parse(simConfigFile)));

    const char *procName = "test";
    PythonInterpreterState pyState(1, const_cast<char**>(&procName));

    const SimulationTime timestep(10);
    const float timeStepFloat = 0.01f;

    {
        nlohmann::json nestCfg(config->at("EngineConfigs").at(1));
        nestCfg["NestInitFileName"] = TEST_NEST_SIM_FILE;
        nestCfg["EngineTimestep"] = timeStepFloat;

        nlohmann::json gazeboCfg(config->at("EngineConfigs").at(0));
        gazeboCfg["GazeboWorldFile"] = TEST_GAZEBO_WORLD_FILE;
        gazeboCfg["EngineTimestep"] = timeStepFloat;

        config->at("EngineConfigs").at(1) = nestCfg;
        config->at("EngineConfigs").at(0) = gazeboCfg;
    }

    EngineClientInterfaceSharedPtr brain(NestEngineJSONLauncher().launchEngine(config->at("EngineConfigs").at(1), ProcessLauncherInterface::unique_ptr(new ProcessLauncherBasic())));
    EngineClientInterfaceSharedPtr physics(GazeboEngineGrpcLauncher().launchEngine(config->at("EngineConfigs").at(0), ProcessLauncherInterface::unique_ptr(new ProcessLauncherBasic())));

    // TODO Without the sleeps between calls, gRPC seems to fail in weird ways...
    std::this_thread::sleep_for(100ms);

    FTILoop simLoop(config, {brain, physics});

    ASSERT_NO_THROW(simLoop.initLoop());

    ASSERT_EQ(simLoop.getSimTime(), SimulationTime::zero());
    ASSERT_NO_THROW(simLoop.runLoop(timestep));
    std::this_thread::sleep_for(100ms);
    ASSERT_EQ(simLoop.getSimTime(), timestep);
    ASSERT_NO_THROW(simLoop.runLoop(timestep));
    std::this_thread::sleep_for(100ms);
    ASSERT_EQ(simLoop.getSimTime(), timestep+timestep);
}
