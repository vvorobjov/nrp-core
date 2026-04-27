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

#include "tests/test_helpers.h"

#include <gtest/gtest.h>

#include <filesystem>
#include <fstream>

using namespace nrp_test;

TEST(TestHelpers, TempDirCreatesFreshDirectoryAndRemovesItOnDestruct)
{
    std::filesystem::path observed;
    {
        TempDir tmp;
        observed = tmp.path();
        ASSERT_FALSE(observed.empty());
        ASSERT_TRUE(std::filesystem::exists(observed));
        ASSERT_TRUE(std::filesystem::is_directory(observed));
    }
    EXPECT_FALSE(std::filesystem::exists(observed));
}

TEST(TestHelpers, TempDirHonoursPrefix)
{
    TempDir tmp("nrp-prefix-check-");
    const std::string stem = tmp.path().filename().string();
    EXPECT_EQ(stem.rfind("nrp-prefix-check-", 0), 0u) << "stem=" << stem;
}

TEST(TestHelpers, TempDirMoveConstructorTransfersOwnership)
{
    TempDir a;
    const std::filesystem::path p = a.path();
    ASSERT_TRUE(std::filesystem::exists(p));

    TempDir b(std::move(a));
    // Source is now "empty" and must not delete the directory when it dies.
    EXPECT_TRUE(a.path().empty());
    EXPECT_EQ(b.path(), p);
    EXPECT_TRUE(std::filesystem::exists(p));
}

TEST(TestHelpers, TempDirSlashOperatorJoinsPaths)
{
    TempDir tmp;
    const auto child = tmp / "nested.txt";
    EXPECT_EQ(child.parent_path(), tmp.path());
    EXPECT_EQ(child.filename(), "nested.txt");
}

TEST(TestHelpers, WriteAndReadTextFileRoundtrip)
{
    TempDir tmp;
    const auto path = tmp / "round.txt";
    writeTextFile(path, "hello\nworld");
    EXPECT_EQ(readTextFile(path), "hello\nworld");
}

TEST(TestHelpers, WriteTextFileCreatesParentDirectories)
{
    TempDir tmp;
    const auto path = tmp.path() / "deep" / "er" / "file.txt";
    writeTextFile(path, "x");
    EXPECT_TRUE(std::filesystem::exists(path));
    EXPECT_EQ(readTextFile(path), "x");
}

TEST(TestHelpers, ReadTextFileThrowsForMissingFile)
{
    TempDir tmp;
    EXPECT_THROW(readTextFile(tmp / "does-not-exist"), std::runtime_error);
}

TEST(TestHelpers, LoadJsonFileParsesWellFormedJson)
{
    TempDir tmp;
    const auto path = tmp / "cfg.json";
    writeTextFile(path, R"({"engine":"nest","step":0.01,"ids":[1,2,3]})");

    const nlohmann::json cfg = loadJsonFile(path);
    EXPECT_EQ(cfg.at("engine").get<std::string>(), "nest");
    EXPECT_DOUBLE_EQ(cfg.at("step").get<double>(), 0.01);
    EXPECT_EQ(cfg.at("ids").size(), 3u);
}

TEST(TestHelpers, LoadJsonFileThrowsForMissingFile)
{
    TempDir tmp;
    EXPECT_THROW(loadJsonFile(tmp / "absent.json"), std::runtime_error);
}

TEST(TestHelpers, LoadJsonFileThrowsForMalformedJson)
{
    TempDir tmp;
    const auto path = tmp / "bad.json";
    writeTextFile(path, "{ this is not json");
    EXPECT_THROW(loadJsonFile(path), nlohmann::json::parse_error);
}
