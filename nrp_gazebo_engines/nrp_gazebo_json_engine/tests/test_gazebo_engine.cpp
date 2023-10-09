
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

#include "nrp_gazebo_json_engine/config/gazebo_json_config.h"
#include "nrp_gazebo_json_engine/config/cmake_constants.h"
#include "nrp_gazebo_json_engine/nrp_client/gazebo_engine_json_nrp_client.h"
#include "nrp_general_library/process_launchers/process_launcher_basic.h"

#include "tests/test_env_cmake.h"

#include <fstream>

static constexpr int MAX_DATA_ACQUISITION_TRIALS = 5;

TEST(TestGazeboJSONEngine, Start)
{
    // Setup config
    nlohmann::json config;
    config["EngineName"] = "engine";
    config["EngineType"] = "gazebo_json";
    config["GazeboWorldFile"] = TEST_EMPTY_WORLD_FILE;
    config["WorldLoadTime"] = 1;
    config["GazeboRNGSeed"] = 12345;

    // Launch gazebo server
    GazeboEngineJSONLauncher launcher;
    PtrTemplates<GazeboEngineJSONNRPClient>::shared_ptr engine = std::dynamic_pointer_cast<GazeboEngineJSONNRPClient>(
            launcher.launchEngine(config, ProcessLauncherInterface::unique_ptr(new ProcessLauncherBasic())));

    ASSERT_NE(engine, nullptr);
    ASSERT_ANY_THROW(engine->initialize());
    sleep(1);
}

TEST(TestGazeboJSONEngine, WorldPlugin)
{
    // Setup config
    nlohmann::json config;
    config["EngineName"] = "engine";
    config["EngineType"] = "gazebo_json";
    config["GazeboWorldFile"] = TEST_WORLD_PLUGIN_FILE;
    config["GazeboRNGSeed"] = 12345;

    // Launch gazebo server
    GazeboEngineJSONLauncher launcher;
    PtrTemplates<GazeboEngineJSONNRPClient>::shared_ptr engine = std::dynamic_pointer_cast<GazeboEngineJSONNRPClient>(
            launcher.launchEngine(config, ProcessLauncherInterface::unique_ptr(new ProcessLauncherBasic())));

    ASSERT_NE(engine, nullptr);
    ASSERT_NO_THROW(engine->initialize());
    sleep(1);

    ASSERT_NO_THROW(engine->runLoopStepAsync(toSimulationTime<int, std::milli>(100)));
    ASSERT_NO_THROW(engine->runLoopStepAsyncGet(toSimulationTimeFromSeconds(5.0)));
    ASSERT_NO_THROW(engine->reset());
}

TEST(TestGazeboJSONEngine, CameraPlugin)
{
    // Setup config
    nlohmann::json config;
    config["EngineName"] = "engine";
    config["EngineType"] = "gazebo_json";
    config["GazeboWorldFile"] = TEST_CAMERA_PLUGIN_FILE;
    config["GazeboRNGSeed"] = 12345;
    std::vector<std::string> env_params ={"GAZEBO_MODEL_PATH=" TEST_GAZEBO_MODELS_DIR ":$GAZEBO_MODEL_PATH"};
    config["EngineEnvParams"] = env_params;

    // Launch gazebo server
    GazeboEngineJSONLauncher launcher;
    PtrTemplates<GazeboEngineJSONNRPClient>::shared_ptr engine = std::dynamic_pointer_cast<GazeboEngineJSONNRPClient>(
            launcher.launchEngine(config, ProcessLauncherInterface::unique_ptr(new ProcessLauncherBasic())));

    ASSERT_NE(engine, nullptr);
    ASSERT_NO_THROW(engine->initialize());
    sleep(1);

    // The data is updated asynchronously, on every new frame. It may happen that on first
    // acquisition there's no camera image yet (isEmpty function returns true), so we allow for few acquisition trials.

    datapacks_vector_t datapacks;
    int trial = 0;

    do
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(200));
        datapacks = engine->getDataPacksFromEngine({DataPackIdentifier("camera::link::camera", engine->engineName(), JsonDataPack::getType())});
        ASSERT_EQ(datapacks.size(), 1);
    }
    while(datapacks.begin()->get()->isEmpty() && trial++ < MAX_DATA_ACQUISITION_TRIALS);

    ASSERT_LE(trial, MAX_DATA_ACQUISITION_TRIALS);

    const JsonDataPack * camDat = dynamic_cast<const JsonDataPack *>(datapacks.begin()->get());

    ASSERT_EQ(camDat->getData()["image_height"], 240);
    ASSERT_EQ(camDat->getData()["image_width" ], 320);
    ASSERT_EQ(camDat->getData()["image_depth" ], 3);
    ASSERT_EQ(camDat->getData()["image_data"  ].size(), 320*240*3);
}


TEST(TestGazeboJSONEngine, JointPlugin)
{
    // Setup config
    nlohmann::json config;
    config["EngineName"] = "engine";
    config["EngineType"] = "gazebo_json";
    config["GazeboWorldFile"] = TEST_JOINT_PLUGIN_FILE;
    config["GazeboSDFModels"] = {{{"Name", "youbot"}, {"File", TEST_YOUBOT_FILE}, {"InitPose", "0 0 1 0 0 0"}}};
    config["GazeboRNGSeed"] = 12345;
    std::vector<std::string> env_params ={"GAZEBO_MODEL_PATH=" TEST_GAZEBO_MODELS_DIR ":$GAZEBO_MODEL_PATH"};
    config["EngineEnvParams"] = env_params;

    // Launch gazebo server
    GazeboEngineJSONLauncher launcher;
    PtrTemplates<GazeboEngineJSONNRPClient>::shared_ptr engine = std::dynamic_pointer_cast<GazeboEngineJSONNRPClient>(
            launcher.launchEngine(config, ProcessLauncherInterface::unique_ptr(new ProcessLauncherBasic())));

    ASSERT_NE(engine, nullptr);
    ASSERT_NO_THROW(engine->initialize());
    sleep(1);

    // Test datapack data getting
    auto datapacks = engine->getDataPacksFromEngine({DataPackIdentifier("youbot::base_footprint_joint", engine->engineName(), JsonDataPack::getType())});
    ASSERT_EQ(datapacks.size(), 1);

    const JsonDataPack *pJointDev = dynamic_cast<const JsonDataPack*>(datapacks.begin()->get());
    ASSERT_EQ(pJointDev->getData()["position"], 0);

    // Test datapack data setting
    const auto newTargetPos = 2.0f;

    std::shared_ptr<DataPackInterface> newJointDataPack = 
        std::shared_ptr<DataPackInterface>(new JsonDataPack(pJointDev->name(),
                                                            pJointDev->engineName(),
                                                            new nlohmann::json({ { "effort", NAN }, { "velocity", NAN }, { "position", newTargetPos} })));
    datapacks_set_t outputDataPacks;
    outputDataPacks.insert(newJointDataPack);

    ASSERT_NO_THROW(engine->sendDataPacksToEngine(outputDataPacks));
}

TEST(TestGazeboJSONEngine, LinkPlugin)
{
    // Setup config
    nlohmann::json config;
    config["EngineName"] = "engine";
    config["EngineType"] = "gazebo_json";
    config["GazeboWorldFile"] = TEST_LINK_PLUGIN_FILE;
    config["GazeboRNGSeed"] = 12345;
    std::vector<std::string> env_params ={"GAZEBO_MODEL_PATH=" TEST_GAZEBO_MODELS_DIR ":$GAZEBO_MODEL_PATH"};
    config["EngineEnvParams"] = env_params;

    // Launch gazebo server
    GazeboEngineJSONLauncher launcher;
    PtrTemplates<GazeboEngineJSONNRPClient>::shared_ptr engine = std::dynamic_pointer_cast<GazeboEngineJSONNRPClient>(
            launcher.launchEngine(config, ProcessLauncherInterface::unique_ptr(new ProcessLauncherBasic())));

    ASSERT_NE(engine, nullptr);
    ASSERT_NO_THROW(engine->initialize());
    sleep(1);

    // Test datapack data getting
    auto datapacks = engine->getDataPacksFromEngine({DataPackIdentifier("youbot::base_footprint", engine->engineName(), JsonDataPack::getType())});
    ASSERT_EQ(datapacks.size(), 1);

    const JsonDataPack *pLinkDev = dynamic_cast<const JsonDataPack*>(datapacks.begin()->get());
    ASSERT_NE(pLinkDev, nullptr);

    // TODO: Check that link state is correct
}
