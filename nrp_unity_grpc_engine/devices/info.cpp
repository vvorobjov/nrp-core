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

#include "devices/info.h"

const GetInfoConst::vec3_t & GetInfo::getPosition() const
{
    return this->getPropertyByName<BoardPosition>();
}

void GetInfo::setPosition(const vec3_t &position)
{
    this->getPropertyByName<BoardPosition>() = position;
}

const GetInfoConst::vec3_t & GetInfo::getRotation() const
{
    return this->getPropertyByName<BoardRotation>();
}

void GetInfo::setRotation(const vec3_t &rotation)
{
    this->getPropertyByName<BoardRotation>() = rotation;
}

const GetInfoConst::vec3_t & GetInfo::getJointAngles() const
{
    return this->getPropertyByName<JointAngles>();
}

void GetInfo::setJointAngles(const vec3_t &jointAngles)
{
    this->getPropertyByName<JointAngles>() = jointAngles;
}

const SetInfoConst::vec3_t & SetInfo::getJointAngles() const
{
    return this->getPropertyByName<JointAngles>();
}

void SetInfo::setJointAngles(const vec3_t &jointAngles)
{
    this->getPropertyByName<JointAngles>() = jointAngles;
}

template<>
GRPCDevice DeviceSerializerMethods<GRPCDevice>::serialize<GetInfo>(const GetInfo &dev)
{
	GRPCDevice msg = serializeID<GRPCDevice>(dev.id());
	/*msg.dev().mutable_camera()->InitAsDefaultInstance();
	msg.dev().mutable_camera()->set_imagedata(dev.imageData().data(), dev.imageData().size());
	msg.dev().mutable_camera()->set_imagedepth(dev.imagePixelSize());
	msg.dev().mutable_camera()->set_imageheight(dev.imageHeight());
	msg.dev().mutable_camera()->set_imagewidth(dev.imageWidth());*/

	return msg;
}

template<>
GetInfo DeviceSerializerMethods<GRPCDevice>::deserialize<GetInfo>(DeviceIdentifier &&devID, deserialization_t data)
{
	return GetInfo(std::move(devID), GetInfo::property_template_t(GetInfo::vec3_t({data->getinfo().boardposition(0), data->getinfo().boardposition(1), data->getinfo().boardposition(2)}),
                                                                GetInfo::vec3_t({data->getinfo().boardrotation(0), data->getinfo().boardrotation(1), data->getinfo().boardrotation(2)}),
                                                                GetInfo::vec3_t({data->getinfo().boardcranejointangles(0), data->getinfo().boardcranejointangles(1), data->getinfo().boardcranejointangles(2)})));
}

template<>
GRPCDevice DeviceSerializerMethods<GRPCDevice>::serialize<SetInfo>(const SetInfo &dev)
{
    GRPCDevice msg = serializeID<GRPCDevice>(dev.id());
	msg.dev().mutable_setinfo()->InitAsDefaultInstance();

    const auto angles = dev.getJointAngles();

    msg.dev().mutable_setinfo()->add_boardcranejointangles(angles[0]);
    msg.dev().mutable_setinfo()->add_boardcranejointangles(angles[1]);
    msg.dev().mutable_setinfo()->add_boardcranejointangles(angles[2]);

	return msg;
}

// EOF
