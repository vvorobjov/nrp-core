//
// NRP Core - Backend infrastructure to synchronize simulations
//
// Copyright 2020-2022 NRP Team
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
#include <gmock/gmock.h>

#include "datatransfer_grpc_engine/config/datatransfer_config.h"
#include "datatransfer_grpc_engine/config/cmake_constants.h"
#include "datatransfer_grpc_engine/engine_client/datatransfer_grpc_client.h"
#include "datatransfer_grpc_engine/engine_server/datatransfer_grpc_server.h"
#include "nrp_general_library/process_launchers/process_launcher_basic.h"

#include "tests/test_env_cmake.h"


TEST(TestDatatransferGrpcEngine, Launcher)
{
    // Engine config
    auto simConfigFile = std::fstream(TEST_ENGINE_SIMPLE_CONFIG_FILE, std::ios::in);
    nlohmann::json config(nlohmann::json::parse(simConfigFile));

    // Launch DataStream server
    DataTransferEngineGrpcLauncher launcher;
    PtrTemplates<DataTransferEngineGrpcClient>::shared_ptr engine = std::dynamic_pointer_cast<DataTransferEngineGrpcClient>(
            launcher.launchEngine(config, ProcessLauncherInterface::unique_ptr(new ProcessLauncherBasic())));

    ASSERT_NE(engine, nullptr);
    sleep(1);

    // Initialization should be without errors
    ASSERT_NO_THROW(engine->initialize());
}


TEST(TestDatatransferGrpcEngine, LauncherLog)
{
    // Engine config
    auto simConfigFile = std::fstream(TEST_ENGINE_SIMPLE_CONFIG_FILE, std::ios::in);
    nlohmann::json config(nlohmann::json::parse(simConfigFile));

    // capture console logs
    testing::internal::CaptureStdout();

    // Launch DataStream server (the MQTT broker should be unavailable)
    DataTransferEngineGrpcLauncher launcher;
    PtrTemplates<DataTransferEngineGrpcClient>::shared_ptr engine = std::dynamic_pointer_cast<DataTransferEngineGrpcClient>(
            launcher.launchEngine(config, ProcessLauncherInterface::unique_ptr(new ProcessLauncherBasic())));

    ASSERT_NE(engine, nullptr);
    sleep(1);

    // Initialization should be without errors
    ASSERT_NO_THROW(engine->initialize());

    // Get captured logs
    std::string output = testing::internal::GetCapturedStdout();

    // Check that expected log is present
    EXPECT_THAT(output, testing::HasSubstr("datapack1 dump was added"));
}
