/* NRP Core - Backend infrastructure to synchronize simulations
 *
 * Copyright 2020 Michael Zechmair
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

#include "grpc_devices/my_device_grpc.h"

template<>
GRPCDevice DeviceSerializerMethods<GRPCDevice>::serialize<MyDevice>(const MyDevice &device)
{
    static const std::string str = "test";

    // Copy device ID to the gRPC message

    GRPCDevice grpcMessage = serializeID<GRPCDevice>(device.id());

    // Get pointer to the message data

    auto myDevice = grpcMessage.dev().mutable_mydevice();

    myDevice->InitAsDefaultInstance();

    // Copy data from scalar property to the gRPC message

    myDevice->set_scalar(device.getScalar());

    // Copy data from array property to the gRPC message

    myDevice->add_array(device.getArray()[0]);
    myDevice->add_array(device.getArray()[1]);

    myDevice->set_string(str);

    // Copy data from vector property to the gRPC message

    *myDevice->mutable_vector() = {device.getVector().begin(), device.getVector().end()};

    return grpcMessage;
}

template<>
MyDevice DeviceSerializerMethods<GRPCDevice>::deserialize<MyDevice>(DeviceIdentifier &&deviceId, deserialization_t data)
{
    return MyDevice(std::move(deviceId), MyDevice::property_template_t(data->mydevice().scalar(),
                                                                       MyDeviceConsts::MyArray({data->mydevice().array(0), 
                                                                                                data->mydevice().array(1)}),
                                                                       data->mydevice().string(),
                                                                       MyDeviceConsts::MyVector(data->mydevice().vector().begin(),
	                                                                                            data->mydevice().vector().end())));
}

// EOF
