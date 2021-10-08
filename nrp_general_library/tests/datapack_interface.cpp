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

#include "nrp_general_library/datapack_interface/datapack_interface.h"

using namespace testing;

TEST(DataPackIdentifierTest, Constructor)
{
    const std::string datapackName1("datapackName1");
    const std::string datapackType1("datapackType1");
    const std::string engineType1("engine1");

    DataPackIdentifier identifier1(datapackName1, engineType1, datapackType1);

    ASSERT_STREQ(identifier1.Name.data(), datapackName1.data());
    ASSERT_STREQ(identifier1.Type.data(), datapackType1.data());
    ASSERT_STREQ(identifier1.EngineName.data(), engineType1.data());
}

TEST(DataPackIdentifierTest, Operators)
{
    const std::string datapackName1("datapackName1");
    const std::string datapackType1("datapackType1");
    const std::string engineType1("engine1");

    const std::string datapackName2("datapackName2");
    const std::string datapackType2("datapackType2");
    const std::string engineType2("engine2");

    DataPackIdentifier identifier1(datapackName1, engineType1, datapackType1);
    DataPackIdentifier identifier2(datapackName2, engineType2, datapackType2);

    DataPackIdentifier identifier3 = identifier1;

    // Check equality
    ASSERT_FALSE(identifier1 == identifier2);
    ASSERT_TRUE(identifier1 == identifier3);

    // Check less than operator (for set placement)
    ASSERT_TRUE(identifier1 < identifier2);
    ASSERT_FALSE(identifier2 < identifier1);

    ASSERT_FALSE(identifier1 < identifier3);
    ASSERT_FALSE(identifier3 < identifier1);
}

TEST(DataPackInterfaceTest, Constructor)
{
    const std::string datapackName1("datapackName1");
    const std::string datapackType1("datapackType1");
    const std::string engineType1("engine1");

    const std::string datapackName2("datapackName2");
    const std::string datapackType2("datapackType2");
    const std::string engineType2("engine2");

    DataPackInterface interface(datapackName1, engineType1, datapackType1);

    ASSERT_STREQ(interface.name().data(), datapackName1.data());
    ASSERT_STREQ(interface.type().data(), datapackType1.data());
    ASSERT_STREQ(interface.engineName().data(), engineType1.data());

    interface.setName(datapackName2);
    ASSERT_STREQ(interface.name().data(), datapackName2.data());

    interface.setType(datapackType2);
    ASSERT_STREQ(interface.type().data(), datapackType2.data());

    interface.setEngineName(engineType2);
    ASSERT_STREQ(interface.engineName().data(), engineType2.data());

    const DataPackIdentifier id1(datapackName1, engineType1, datapackType1);
    interface.setID(id1);

    ASSERT_STREQ(interface.name().data(), id1.Name.data());
    ASSERT_STREQ(interface.type().data(), id1.Type.data());
    ASSERT_STREQ(interface.engineName().data(), id1.EngineName.data());

    ASSERT_EQ(interface.id(), id1);
}