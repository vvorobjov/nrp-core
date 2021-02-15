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

#include "nrp_general_library/device_interface/device.h"
#include "nrp_general_library/property_template/serializers/json_property_serializer.h"
#include "nrp_json_engine_protocol/device_interfaces/json_device_serializer.h"

using namespace testing;

using std::string_view_literals::operator""sv;

struct TestJSONDevice
        : public Device<TestJSONDevice, "type", PropNames<"int", "string">, int, std::string>
{
	template<DEV_ID_C DEV_ID_T, SAME_AS_C<property_template_t> PROP_T>
	TestJSONDevice(DEV_ID_T &&id, PROP_T &&props)
	    : Device(std::forward<DEV_ID_T>(id), std::forward<PROP_T>(props))
	{}

	template<class DESERIALIZER_T>
	static auto deserializeProperties(DESERIALIZER_T &&data)
	{	return Device::deserializeProperties(std::forward<DESERIALIZER_T>(data));	}
};

TEST(JSONDeviceMethodsTest, IDFunctions)
{
	auto data = nlohmann::json({{"", {{"int", 6}, {"string", "otherData"}}}});
	auto dev1 = DeviceSerializerMethods<nlohmann::json>::deserialize<TestJSONDevice>(TestJSONDevice::createID("dev2", "engine"), data.begin());

	// Test serialization
	const nlohmann::json serializedData = DeviceSerializerMethods<nlohmann::json>::serializeID(dev1.id());
	ASSERT_EQ(serializedData.size(), 1);
	ASSERT_EQ(serializedData.front().size(), 2);
	ASSERT_STREQ(serializedData.begin().key().data(), dev1.name().data());
	ASSERT_STREQ(serializedData.front().at(DeviceSerializerMethods<nlohmann::json>::JSONTypeID.data()).get<std::string>().data(), dev1.type().data());
	ASSERT_STREQ(serializedData.front().at(DeviceSerializerMethods<nlohmann::json>::JSONEngineNameID.data()).get<std::string>().data(), dev1.engineName().data());

	// Test deserialization
	DeviceIdentifier deserializedID = DeviceSerializerMethods<nlohmann::json>::deserializeID(serializedData.begin());
	ASSERT_EQ(dev1.id(), deserializedID);

	// Test exception throwing
	nlohmann::json empty;
	empty["test"] = nlohmann::json();
	ASSERT_THROW(DeviceSerializerMethods<nlohmann::json>::deserializeID(empty.begin()), std::exception);
}

TEST(JSONDeviceMethodsTest, ConversionFunctions)
{
	auto data = nlohmann::json({{"", {{"int", 6}, {"string", "otherData"}}}});
	auto dev1 = DeviceSerializerMethods<nlohmann::json>::deserialize<TestJSONDevice>(TestJSONDevice::createID("dev2", "engine"), data.begin());

	// Test serialization
	const nlohmann::json serializedData = DeviceSerializerMethods<nlohmann::json>::serialize(dev1);
	ASSERT_EQ(serializedData.size(), 1);
	ASSERT_EQ(serializedData.front().size(), 4);
	ASSERT_STREQ(serializedData.begin().key().data(), dev1.name().data());
	ASSERT_STREQ(serializedData.front().at(DeviceSerializerMethods<nlohmann::json>::JSONTypeID.data()).get<std::string>().data(), dev1.type().data());
	ASSERT_STREQ(serializedData.front().at(DeviceSerializerMethods<nlohmann::json>::JSONEngineNameID.data()).get<std::string>().data(), dev1.engineName().data());
	ASSERT_EQ(serializedData.front().at("int").get<int>(), (dev1.getPropertyByName<"int">()));
	ASSERT_STREQ(serializedData.front().at("string").get<std::string>().data(), (dev1.getPropertyByName<"string">().data()));

	auto devid = DeviceSerializerMethods<nlohmann::json>::deserializeID(serializedData.begin());
    auto deserializedDev = DeviceSerializerMethods<nlohmann::json>::deserialize<TestJSONDevice>(std::move(devid), serializedData.begin());
    ASSERT_EQ(dev1.id(), deserializedDev.id());
	ASSERT_EQ((dev1.getPropertyByName<"int">()), (deserializedDev.getPropertyByName<"int">()));
	ASSERT_STREQ((dev1.getPropertyByName<"string">().data()), (deserializedDev.getPropertyByName<"string">().data()));
}
