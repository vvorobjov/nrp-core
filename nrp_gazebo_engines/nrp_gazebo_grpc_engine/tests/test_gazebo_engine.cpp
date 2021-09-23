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

#include "nrp_gazebo_grpc_engine/config/gazebo_grpc_config.h"
#include "nrp_gazebo_grpc_engine/config/cmake_constants.h"
#include "nrp_gazebo_grpc_engine/nrp_client/gazebo_engine_grpc_nrp_client.h"
#include "nrp_general_library/process_launchers/process_launcher_basic.h"

#include "tests/test_env_cmake.h"

#include <fstream>

static constexpr int MAX_DATA_ACQUISITION_TRIALS = 5;

TEST(TestGazeboGrpcEngine, Start)
{
    // Setup config
    nlohmann::json config;
    config["EngineName"] = "engine";
    config["EngineType"] = "gazebo_grpc";
    config["GazeboWorldFile"] = TEST_EMPTY_WORLD_FILE;
    config["WorldLoadTime"] = 1;
    config["GazeboRNGSeed"] = 12345;

    // Launch gazebo server
    GazeboEngineGrpcLauncher launcher;
    PtrTemplates<GazeboEngineGrpcNRPClient>::shared_ptr engine = std::dynamic_pointer_cast<GazeboEngineGrpcNRPClient>(
            launcher.launchEngine(config, ProcessLauncherInterface::unique_ptr(new ProcessLauncherBasic())));

    ASSERT_NE(engine, nullptr);
    sleep(1);

    ASSERT_ANY_THROW(engine->initialize());
}

TEST(TestGazeboGrpcEngine, WorldPlugin)
{
    // Setup config
    nlohmann::json config;
    config["EngineName"] = "engine";
    config["EngineType"] = "gazebo_grpc";
    config["GazeboWorldFile"] = TEST_WORLD_PLUGIN_FILE;
    config["GazeboRNGSeed"] = 12345;

    // Launch gazebo server
    GazeboEngineGrpcLauncher launcher;
    PtrTemplates<GazeboEngineGrpcNRPClient>::shared_ptr engine = std::dynamic_pointer_cast<GazeboEngineGrpcNRPClient>(
            launcher.launchEngine(config, ProcessLauncherInterface::unique_ptr(new ProcessLauncherBasic())));

    ASSERT_NE(engine, nullptr);
    sleep(1);

    ASSERT_NO_THROW(engine->initialize());
    ASSERT_NO_THROW(engine->runLoopStepAsync(toSimulationTime<int, std::milli>(100)));
    ASSERT_NO_THROW(engine->runLoopStepAsyncGet(toSimulationTimeFromSeconds(5.0)));
    ASSERT_NO_THROW(engine->reset());
}

TEST(TestGazeboGrpcEngine, CameraPlugin)
{
    // Setup config
    nlohmann::json config;
    config["EngineName"] = "engine";
    config["EngineType"] = "gazebo_grpc";
    config["GazeboWorldFile"] = TEST_CAMERA_PLUGIN_FILE;
    config["GazeboRNGSeed"] = 12345;
    std::vector<std::string> env_params ={"GAZEBO_MODEL_PATH=" TEST_GAZEBO_MODELS_DIR ":$GAZEBO_MODEL_PATH"};
    config["EngineEnvParams"] = env_params;

    // Launch gazebo server
    GazeboEngineGrpcLauncher launcher;
    PtrTemplates<GazeboEngineGrpcNRPClient>::shared_ptr engine = std::dynamic_pointer_cast<GazeboEngineGrpcNRPClient>(
            launcher.launchEngine(config, ProcessLauncherInterface::unique_ptr(new ProcessLauncherBasic())));

    ASSERT_NE(engine, nullptr);
    sleep(1);

    ASSERT_NO_THROW(engine->initialize());

	// The data is updated asynchronously, on every new frame. It may happen that on first
	// acquisition there's no camera image yet (isEmpty function returns true), so we allow for few acquisition trials.

    const EngineClientInterface::datapacks_t * datapacks;
    int trial = 0;

    do
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(200));
        datapacks = &engine->updateDataPacksFromEngine({DataPackIdentifier("nrp_camera::camera", engine->engineName(), "irrelevant_type")});
        ASSERT_EQ(datapacks->size(), 1);
    }
    while(dynamic_cast<const DataPackInterface&>(*(datapacks->at(0))).isEmpty() && trial++ < MAX_DATA_ACQUISITION_TRIALS);

	ASSERT_LE(trial, MAX_DATA_ACQUISITION_TRIALS);

    const DataPack<Gazebo::Camera>& camDat = dynamic_cast<const DataPack<Gazebo::Camera>&>(*(datapacks->at(0)));

    ASSERT_EQ(camDat.getData().imageheight(), 240);
    ASSERT_EQ(camDat.getData().imagewidth(),  320);
    ASSERT_EQ(camDat.getData().imagedepth(),  3);
    ASSERT_EQ(camDat.getData().imagedata().size(), 320*240*3);
}


TEST(TestGazeboGrpcEngine, JointPlugin)
{
    // Setup config
    nlohmann::json config;
    config["EngineName"] = "engine";
    config["EngineType"] = "gazebo_grpc";
    config["GazeboWorldFile"] = TEST_JOINT_PLUGIN_FILE;
    config["GazeboRNGSeed"] = 12345;
    std::vector<std::string> env_params ={"GAZEBO_MODEL_PATH=" TEST_GAZEBO_MODELS_DIR ":$GAZEBO_MODEL_PATH"};
    config["EngineEnvParams"] = env_params;

    // Launch gazebo server
    GazeboEngineGrpcLauncher launcher;
    PtrTemplates<GazeboEngineGrpcNRPClient>::shared_ptr engine = std::dynamic_pointer_cast<GazeboEngineGrpcNRPClient>(
            launcher.launchEngine(config, ProcessLauncherInterface::unique_ptr(new ProcessLauncherBasic())));

    ASSERT_NE(engine, nullptr);
    sleep(1);

    ASSERT_NO_THROW(engine->initialize());

    // Test datapack data getting
    auto datapacks = engine->updateDataPacksFromEngine({DataPackIdentifier("youbot::base_footprint_joint",
                                                    engine->engineName(), "irrelevant_type")});
    ASSERT_EQ(datapacks.size(), 1);

    const auto *pJointDev = dynamic_cast<const DataPack<Gazebo::Joint> *>(datapacks[0].get());
    ASSERT_NE(pJointDev, nullptr);
    ASSERT_EQ(pJointDev->getData().position(), 0);

    // Test datapack data setting
    const auto newTargetPos = 2.0f;

    auto newJointDev = new Gazebo::Joint();
    newJointDev->set_effort(NAN);
    newJointDev->set_velocity(NAN);
    newJointDev->set_position(newTargetPos);
    DataPack<Gazebo::Joint> dev("youbot::base_footprint_joint", engine->engineName(), newJointDev);

    ASSERT_NO_THROW(engine->sendDataPacksToEngine({&dev}));
}

TEST(TestGazeboGrpcEngine, LinkPlugin)
{
    // Setup config
    nlohmann::json config;
    config["EngineName"] = "engine";
    config["EngineType"] = "gazebo_grpc";
    config["GazeboWorldFile"] = TEST_LINK_PLUGIN_FILE;
    config["GazeboRNGSeed"] = 12345;
    std::vector<std::string> env_params ={"GAZEBO_MODEL_PATH=" TEST_GAZEBO_MODELS_DIR ":$GAZEBO_MODEL_PATH"};
    config["EngineEnvParams"] = env_params;

    // Launch gazebo server
    GazeboEngineGrpcLauncher launcher;
    PtrTemplates<GazeboEngineGrpcNRPClient>::shared_ptr engine = std::dynamic_pointer_cast<GazeboEngineGrpcNRPClient>(
            launcher.launchEngine(config, ProcessLauncherInterface::unique_ptr(new ProcessLauncherBasic())));

    ASSERT_NE(engine, nullptr);
    sleep(1);

    ASSERT_NO_THROW(engine->initialize());

    // Test datapack data getting
    auto datapacks = engine->updateDataPacksFromEngine({DataPackIdentifier("link_youbot::base_footprint",
                                                    engine->engineName(), "irrelevant_type")});
    ASSERT_EQ(datapacks.size(), 1);

    const auto *pLinkDev = dynamic_cast<const DataPack<Gazebo::Link> *>(datapacks[0].get());
    ASSERT_NE(pLinkDev, nullptr);

    // TODO: Check that link state is correct
}
