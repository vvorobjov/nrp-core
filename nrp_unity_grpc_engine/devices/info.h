/* * NRP Core - Backend infrastructure to synchronize simulations
 *
 * Copyright 2020-2021 NRP Team
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * This project has received funding from the European Union’s Horizon 2020
 * Framework Programme for Research and Innovation under the Specific Grant
 * Agreement No. 945539 (Human Brain Project SGA3).
 */

#ifndef UNITY_INFO_H
#define UNITY_INFO_H

#include "nrp_general_library/device_interface/device.h"
#include "nrp_grpc_engine_protocol/device_interfaces/grpc_device_serializer.h"

struct GetInfoConst
{
		using vec3_t = std::array<float, 3>;

		static constexpr FixedString BoardPosition = "board_position";
		static constexpr vec3_t DefaultBoardPosition = {0,0,0};

		static constexpr FixedString BoardRotation = "board_rotation";
		static constexpr vec3_t DefaultBoardRotation = {0,0,0};

		static constexpr FixedString JointAngles = "joint_angles";
		static constexpr vec3_t DefaultJointAngles = {0,0,0};

		using JPropNames = PropNames<BoardPosition, BoardRotation, JointAngles>;
};

class GetInfo
        : public GetInfoConst,
          public Device<GetInfo, "GetInfo", GetInfoConst::JPropNames, std::array<float, 3>, std::array<float, 3>, std::array<float, 3> >
{
	public:
		GetInfo(DeviceIdentifier &&devID, property_template_t &&props = property_template_t(DefaultBoardPosition, DefaultBoardRotation, DefaultJointAngles))
		    : Device(std::move(devID), std::move(props))
		{}

		template<class DESERIALIZE_T>
		static auto deserializeProperties(DESERIALIZE_T &&data)
		{	return Device::deserializeProperties(std::forward<DESERIALIZE_T>(data), DefaultBoardPosition, DefaultBoardRotation, DefaultJointAngles);	}

		const vec3_t &getPosition() const;
		void setPosition(const vec3_t &position);

		const vec3_t &getRotation() const;
		void setRotation(const vec3_t &rotation);

        const vec3_t &getJointAngles() const;
		void setJointAngles(const vec3_t &jointAngles);
};

template<>
GRPCDevice DeviceSerializerMethods<GRPCDevice>::serialize<GetInfo>(const GetInfo &dev);

template<>
GetInfo DeviceSerializerMethods<GRPCDevice>::deserialize<GetInfo>(DeviceIdentifier &&devID, deserialization_t data);

struct SetInfoConst
{
		using vec3_t = std::array<float, 3>;

		static constexpr FixedString JointAngles = "joint_angles";
		static constexpr vec3_t DefaultJointAngles = {0,0,0};

		using JPropNames = PropNames<JointAngles>;
};

class SetInfo
        : public SetInfoConst,
          public Device<SetInfo, "SetInfo", SetInfoConst::JPropNames, std::array<float, 3>>
{
	public:
		SetInfo(DeviceIdentifier &&devID, property_template_t &&props = property_template_t(DefaultJointAngles))
		    : Device(std::move(devID), std::move(props))
		{}

		template<class DESERIALIZE_T>
		static auto deserializeProperties(DESERIALIZE_T &&data)
		{	return Device::deserializeProperties(std::forward<DESERIALIZE_T>(data), DefaultJointAngles);	}

        const vec3_t &getJointAngles() const;
		void setJointAngles(const vec3_t &jointAngles);
};

template<>
GRPCDevice DeviceSerializerMethods<GRPCDevice>::serialize<SetInfo>(const SetInfo &dev);

#endif // UNITY_INFO_H

// EOF
