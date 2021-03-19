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

#include "devices/camera.h"

const CameraConst::cam_data_t &Camera::imageData() const
{
	return this->getPropertyByName<ImageData>();
}

CameraConst::cam_data_t &Camera::imageData()
{
	return this->getPropertyByName<ImageData>();
}

void Camera::setImageData(const CameraConst::cam_data_t &imageData)
{
	this->getPropertyByName<ImageData>() = imageData;
}

template<>
GRPCDevice DeviceSerializerMethods<GRPCDevice>::serialize<Camera>(const Camera &dev)
{
	GRPCDevice msg = serializeID<GRPCDevice>(dev.id());

	return msg;
}

template<>
Camera DeviceSerializerMethods<GRPCDevice>::deserialize<Camera>(DeviceIdentifier &&devID, deserialization_t data)
{
	return Camera(std::move(devID), Camera::property_template_t(Camera::cam_data_t(data->getcamera().imagedata().begin(),
	                                                                                data->getcamera().imagedata().end())));
}

// EOF
