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

#include "nrp_general_library/utils/json_schema_utils.h"
#include "tests/test_helpers.h"

#include <gtest/gtest.h>

#include <stdexcept>
#include <string>

using nrp_test::TempDir;
using nrp_test::writeTextFile;

// validateJson / jsonSchemaLoader both read schema files from
// NRP_CONFIG_INSTALL_DIR at runtime. Those paths are populated by `make
// install`. Rather than couple this test to the install layout, we limit
// coverage here to the two dependency-free entry points: parseJSONFile
// and the header-only setDefault template.

TEST(JsonSchemaUtilsParseJSONFile, ParsesWellFormedObject)
{
    TempDir tmp;
    const auto path = tmp / "config.json";
    writeTextFile(path, R"({"engine":"nest","ids":[1,2,3]})");

    const nlohmann::json cfg = json_utils::parseJSONFile(path.string());
    EXPECT_EQ(cfg.at("engine").get<std::string>(), "nest");
    EXPECT_EQ(cfg.at("ids").size(), 3u);
}

TEST(JsonSchemaUtilsParseJSONFile, ThrowsInvalidArgumentForMissingFile)
{
    TempDir tmp;
    EXPECT_THROW(
        json_utils::parseJSONFile((tmp / "absent.json").string()),
        std::invalid_argument
    );
}

TEST(JsonSchemaUtilsParseJSONFile, ThrowsInvalidArgumentForMalformedJson)
{
    TempDir tmp;
    const auto path = tmp / "bad.json";
    writeTextFile(path, "{ this is not valid json");

    EXPECT_THROW(
        json_utils::parseJSONFile(path.string()),
        std::invalid_argument
    );
}

TEST(JsonSchemaUtilsSetDefault, InsertsKeyWhenAbsent)
{
    nlohmann::json instance = nlohmann::json::object();
    json_utils::setDefault<int>(instance, "step", 42);
    ASSERT_TRUE(instance.contains("step"));
    EXPECT_EQ(instance.at("step").get<int>(), 42);
}

TEST(JsonSchemaUtilsSetDefault, DoesNotOverrideExistingKey)
{
    nlohmann::json instance{{"step", 7}};
    json_utils::setDefault<int>(instance, "step", 42);
    EXPECT_EQ(instance.at("step").get<int>(), 7);
}

TEST(JsonSchemaUtilsSetDefault, AcceptsHeterogeneousValueTypes)
{
    nlohmann::json instance = nlohmann::json::object();
    json_utils::setDefault<std::string>(instance, "name", "alpha");
    json_utils::setDefault<bool>(instance, "enabled", true);
    json_utils::setDefault<double>(instance, "threshold", 0.5);

    EXPECT_EQ(instance.at("name").get<std::string>(), "alpha");
    EXPECT_EQ(instance.at("enabled").get<bool>(), true);
    EXPECT_DOUBLE_EQ(instance.at("threshold").get<double>(), 0.5);
}
