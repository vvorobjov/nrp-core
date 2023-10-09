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

#include "nrp_general_library/plugin_system/engine_plugin_manager.h"
#include "tests/test_env_cmake.h"

using namespace testing;

TEST(PluginManagerTest, TestLoadPlugin)
{
    EnginePluginManager pluginMan;

    ASSERT_NO_THROW(pluginMan.addPluginPath(TEST_PLUGIN_DIR));

    // Test loading plugin
    auto engineLauncher = pluginMan.loadEnginePlugin(TEST_NRP_PLUGIN);
    ASSERT_NE(engineLauncher, nullptr);
}
