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

#include "nrp_general_library/device_interface/data_device.h"

using namespace testing;

TEST(DataDeviceTest, Constructor)
{
    const std::string deviceName1("deviceName1");
    const std::string engineType1("engine1");

    DataDevice<int> dev(deviceName1, engineType1);

    ASSERT_STREQ(dev.name().data(), deviceName1.data());
    ASSERT_STREQ(dev.type().data(), typeid(int).name());
    ASSERT_STREQ(dev.engineName().data(), engineType1.data());

    int *i = new int(42);
    DataDevice<int> dev2(deviceName1, engineType1, i);
    ASSERT_EQ(dev2.getData(), *i);
}

TEST(DataDeviceTest, GetData)
{
    const std::string deviceName1("deviceName1");
    const std::string engineType1("engine1");

    DataDevice<int> dev(deviceName1, engineType1);

    ASSERT_FALSE(dev.isEmpty());

    dev.getData();
    ASSERT_FALSE(dev.isEmpty());

    auto i2 = dev.releaseData();
    ASSERT_TRUE(dev.isEmpty());
    ASSERT_THROW(dev.getData(), NRPException::exception);
    delete i2;
}
