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

#include "nrp_general_library/utils/file_finder.h"
#include "tests/test_helpers.h"

#include <gtest/gtest.h>

#include <filesystem>
#include <vector>

using nrp_test::TempDir;
using nrp_test::writeTextFile;

TEST(FileFinderTest, EmptySearchListReturnsEmptyPath)
{
    const std::vector<std::filesystem::path> dirs;
    EXPECT_TRUE(FileFinder::findFile("anything.txt", dirs).empty());
}

TEST(FileFinderTest, MissingFileReturnsEmptyPath)
{
    TempDir a;
    TempDir b;
    const std::vector<std::filesystem::path> dirs{ a.path(), b.path() };
    EXPECT_TRUE(FileFinder::findFile("absent.txt", dirs).empty());
}

TEST(FileFinderTest, FindsFileInFirstDirectory)
{
    TempDir a;
    TempDir b;
    writeTextFile(a / "config.json", "{}");

    const std::vector<std::filesystem::path> dirs{ a.path(), b.path() };
    const auto found = FileFinder::findFile("config.json", dirs);
    EXPECT_EQ(found, a.path() / "config.json");
}

TEST(FileFinderTest, FindsFileInSecondDirectoryWhenFirstMisses)
{
    TempDir a;
    TempDir b;
    writeTextFile(b / "later.json", "{}");

    const std::vector<std::filesystem::path> dirs{ a.path(), b.path() };
    const auto found = FileFinder::findFile("later.json", dirs);
    EXPECT_EQ(found, b.path() / "later.json");
}

TEST(FileFinderTest, ReturnsFirstMatchWhenMultipleDirectoriesContainTheFile)
{
    TempDir a;
    TempDir b;
    writeTextFile(a / "shadowed.json", "from-a");
    writeTextFile(b / "shadowed.json", "from-b");

    const std::vector<std::filesystem::path> dirs{ a.path(), b.path() };
    const auto found = FileFinder::findFile("shadowed.json", dirs);
    EXPECT_EQ(found, a.path() / "shadowed.json");
}

TEST(FileFinderTest, OperatorCallBehavesLikeFindFile)
{
    TempDir a;
    writeTextFile(a / "op.json", "{}");
    const std::vector<std::filesystem::path> dirs{ a.path() };

    FileFinder finder;
    EXPECT_EQ(finder("op.json", dirs), FileFinder::findFile("op.json", dirs));
    EXPECT_TRUE(finder("missing.json", dirs).empty());
}

TEST(FileFinderTest, SkipsNonexistentSearchDirectoriesWithoutThrowing)
{
    TempDir real;
    writeTextFile(real / "here.txt", "x");

    const std::vector<std::filesystem::path> dirs{
        std::filesystem::path("/does/not/exist/nrp-test"),
        real.path()
    };
    const auto found = FileFinder::findFile("here.txt", dirs);
    EXPECT_EQ(found, real.path() / "here.txt");
}
