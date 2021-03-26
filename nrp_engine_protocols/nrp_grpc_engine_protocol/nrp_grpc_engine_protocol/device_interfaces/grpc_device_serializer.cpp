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

#include "nrp_grpc_engine_protocol/device_interfaces/grpc_device_serializer.h"


template<>
GRPCDevice DeviceSerializerMethods<GRPCDevice>::serializeID<GRPCDevice>(const DeviceIdentifier &devID)
{
	EngineGrpc::DeviceMessage msg;
	msg.mutable_deviceid()->set_devicename(devID.Name);
	msg.mutable_deviceid()->set_enginename(devID.EngineName);
	msg.mutable_deviceid()->set_devicetype(devID.Type);
	return msg;
}

template<>
EngineGrpc::DeviceMessage DeviceSerializerMethods<GRPCDevice>::serializeID<EngineGrpc::DeviceMessage>(const DeviceIdentifier &devID)
{
	EngineGrpc::DeviceMessage msg;
	msg.mutable_deviceid()->set_devicename(devID.Name);
	msg.mutable_deviceid()->set_enginename(devID.EngineName);
	msg.mutable_deviceid()->set_devicetype(devID.Type);
	return msg;
}

DeviceIdentifier DeviceSerializerMethods<GRPCDevice>::deserializeID(DeviceSerializerMethods<GRPCDevice>::deserialization_t data)
{
	const auto &devID = data->deviceid();
	return DeviceIdentifier(devID.devicename(), devID.enginename(), devID.devicetype());
}