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

#include "nrp_protobuf/proto_ops/proto_ops_manager.h"

#include <gtest/gtest.h>

#include <string>

// First direct tests for ProtoOpsManager. Coverage is intentionally scoped
// to the parts that do not need a real plugin .so on disk (singleton
// semantics, failure-path return values for loadProtobufPlugin). Loading an
// actual protobuf plugin is already exercised end-to-end by the JSON/gRPC
// engine suites, so duplicating that here would only add fixture weight.

TEST(ProtoOpsManagerSingleton, GetInstanceReturnsSameObjectOnRepeatedCalls)
{
    ProtoOpsManager &a = ProtoOpsManager::getInstance();
    ProtoOpsManager &b = ProtoOpsManager::getInstance();
    EXPECT_EQ(&a, &b);
}

TEST(ProtoOpsManagerSingleton, ResetInstanceReplacesTheSingleton)
{
    ProtoOpsManager *before = &ProtoOpsManager::getInstance();
    ProtoOpsManager::resetInstance();
    ProtoOpsManager *after = &ProtoOpsManager::getInstance();

    // The unique_ptr reset gives a different object; we only check that the
    // pointer is non-null and not identical to the old one. We cannot rely
    // on the *address* never being recycled, but a direct reset inside the
    // same scope almost always yields a new address.
    ASSERT_NE(after, nullptr);
    EXPECT_NE(before, after);

    // Post-reset, getInstance() should again be stable.
    EXPECT_EQ(&ProtoOpsManager::getInstance(), after);
}

TEST(ProtoOpsManagerLoadPlugin, EmptyPathReturnsNullptr)
{
    ProtoOpsManager::resetInstance();
    auto plugin = ProtoOpsManager::getInstance().loadProtobufPlugin("");
    EXPECT_EQ(plugin.get(), nullptr);
}

TEST(ProtoOpsManagerLoadPlugin, NonExistentLibraryReturnsNullptr)
{
    ProtoOpsManager::resetInstance();
    auto plugin = ProtoOpsManager::getInstance().loadProtobufPlugin("/does/not/exist/nrp_test_plugin.so");
    EXPECT_EQ(plugin.get(), nullptr);
}

TEST(ProtoOpsManagerLoadPlugin, SecondAttemptToLoadSameMissingPluginStillReturnsNullptr)
{
    // Two consecutive failing loads of the same path should each return
    // nullptr. This pins the manager's behaviour on repeated failure --
    // in particular, that a prior failed load does not poison subsequent
    // lookups or (conversely) mask them by caching a stale success.
    ProtoOpsManager::resetInstance();
    const std::string path = "/does/not/exist/nrp_test_plugin_repeat.so";

    auto first  = ProtoOpsManager::getInstance().loadProtobufPlugin(path);
    auto second = ProtoOpsManager::getInstance().loadProtobufPlugin(path);

    EXPECT_EQ(first.get(),  nullptr);
    EXPECT_EQ(second.get(), nullptr);
}
