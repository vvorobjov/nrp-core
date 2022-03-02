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
#include "nrp_opensim_engine/config/opensim_config.h"
#include "nrp_opensim_engine/nrp_client/opensim_nrp_client.h"
#include "nrp_general_library/process_launchers/process_launcher_basic.h"
#include "tests/test_env_cmake.h"

#include <boost/python.hpp>
#include <future>
#include <restclient-cpp/restclient.h>

namespace python = boost::python;

TEST(TestPySimEngine, TestInitError)
{
    // Setup config
    nlohmann::json config;
    config["EngineName"] = "engine";
    config["EngineType"] = "opensim";
    config["PythonFileName"] = TEST_PYSIM_INIT_ERROR_NAME;
    config["WorldFileName"] = TEST_SIMULATOR_INIT_WORLD_NAME;
    config["Visualizer"] = false;

    // Launch gazebo server
    OpenSimJSONLauncher launcher;
    PtrTemplates<OpenSimNRPClient>::shared_ptr engine = 
        std::dynamic_pointer_cast<OpenSimNRPClient>(
            launcher.launchEngine(config, 
                ProcessLauncherInterface::unique_ptr(new ProcessLauncherBasic())));

    ASSERT_NE(engine, nullptr);
    ASSERT_NO_THROW(engine->initialize());
}

TEST(TestPySimEngine, TestRunLoop)
{
    // Setup config
    nlohmann::json config;
    config["EngineName"] = "engine";
    config["EngineType"] = "opensim";
    config["PythonFileName"] = TEST_PYSIM_INIT_ERROR_NAME;
    config["WorldFileName"] = TEST_SIMULATOR_INIT_WORLD_NAME;
    config["Visualizer"] = false;

    // Launch gazebo server
    OpenSimJSONLauncher launcher;
    PtrTemplates<OpenSimNRPClient>::shared_ptr engine = 
        std::dynamic_pointer_cast<OpenSimNRPClient>(
            launcher.launchEngine(config, 
                ProcessLauncherInterface::unique_ptr(new ProcessLauncherBasic())));

    ASSERT_NE(engine, nullptr);

    ASSERT_NO_THROW(engine->initialize());
    ASSERT_NO_THROW(engine->runLoopStepAsync(toSimulationTime<int, std::milli>(100)));
    ASSERT_NO_THROW(engine->runLoopStepAsyncGet(toSimulationTimeFromSeconds(5.0)));
    ASSERT_NO_THROW(engine->shutdown());
}

TEST(TestPySimEngine, TestDataPack)
{
    // Setup config
    nlohmann::json config;
    config["EngineName"] = "engine";
    config["EngineType"] = "opensim";
    config["PythonFileName"] = TEST_PYSIM_DATAPACK_ERROR_NAME;
    config["WorldFileName"] = TEST_SIMULATOR_INIT_WORLD_NAME;
    config["Visualizer"] = false;

    // Launch gazebo server
    OpenSimJSONLauncher launcher;
    PtrTemplates<OpenSimNRPClient>::shared_ptr engine = 
        std::dynamic_pointer_cast<OpenSimNRPClient>(
            launcher.launchEngine(config, 
                ProcessLauncherInterface::unique_ptr(new ProcessLauncherBasic())));

    ASSERT_NE(engine, nullptr);

    ASSERT_NO_THROW(engine->initialize());

    // Test datapack data getting
    auto datapacks = engine->updateDataPacksFromEngine(
        {DataPackIdentifier("infos", 
            engine->engineName(), JsonDataPack::getType())});
    ASSERT_EQ(datapacks.size(), 1);
    const JsonDataPack *pJointDev = dynamic_cast<const JsonDataPack*>(datapacks[0].get());
    ASSERT_EQ(pJointDev->getData()["init"], 1);

    // Test datapack data setting
    JsonDataPack newJointDev("flag", 
        engine->engineName(), new nlohmann::json({{"val",1}}));
    ASSERT_NO_THROW(engine->sendDataPacksToEngine({&newJointDev}));
    ASSERT_NO_THROW(engine->runLoopStepAsync(toSimulationTime<int, std::milli>(100)));
    ASSERT_NO_THROW(engine->runLoopStepAsyncGet(toSimulationTimeFromSeconds(5.0)));
    ASSERT_NO_THROW(engine->shutdown());
}

TEST(TestPySimEngine, TestLibFunc)
{
    // Setup config
    nlohmann::json config;
    config["EngineName"] = "engine";
    config["EngineType"] = "opensim";
    config["PythonFileName"] = TEST_PYSIM_SENSOR_ERROR_NAME;
    config["WorldFileName"] = TEST_SIMULATOR_SIM_WORLD_NAME;
    config["Visualizer"] = false;

    // Launch gazebo server
    OpenSimJSONLauncher launcher;
    PtrTemplates<OpenSimNRPClient>::shared_ptr engine = 
        std::dynamic_pointer_cast<OpenSimNRPClient>(
            launcher.launchEngine(config, 
                ProcessLauncherInterface::unique_ptr(new ProcessLauncherBasic())));

    ASSERT_NE(engine, nullptr);

    ASSERT_NO_THROW(engine->initialize());
    ASSERT_NO_THROW(engine->runLoopStepAsync(toSimulationTime<int, std::milli>(100)));
    ASSERT_NO_THROW(engine->runLoopStepAsyncGet(toSimulationTimeFromSeconds(5.0)));
    ASSERT_NO_THROW(engine->reset());
    ASSERT_NO_THROW(engine->shutdown());
}