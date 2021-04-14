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

#ifndef DEVICE_SERIALIZER_METHODS_H
#define DEVICE_SERIALIZER_METHODS_H

#include "nrp_general_library/device_interface/device_interface.h"
#include "nrp_general_library/property_template/serializers/property_serializer.h"

#include <concepts>


//TODO: Currently this is an empty template, specializations are not constraint in any way. DEVICE_SERIALIZER_METHODS_C is not in use either.
template<class OBJECT>
class DeviceSerializerMethods;

/*!
 *  \brief Device serializer concept. Defines necessary functions to de-/serialize devices. Uses PropertySerializers in the background
 */
template<class T, class OBJECT_T, class DEVICE>
concept DEVICE_SERIALIZER_METHODS_C = requires (T &serializer, const DEVICE &device, DeviceIdentifier &&id)
{
    DEVICE_C<DEVICE>;

    typename T::prop_deserialization_t;
    typename T::deserialization_t;

    {	serializer.serialize(device)	} -> std::convertible_to<OBJECT_T>;
    {	serializer.serializeID(std::declval<const DeviceIdentifier&>())	} -> std::convertible_to<OBJECT_T>;

    {	serializer.template deserialize<DEVICE>(std::move(id), std::declval<typename T::deserialization_t>())	} -> std::same_as<DEVICE>;
    {	serializer.deserializeID(std::declval<typename T::deserialization_t>())	} -> std::convertible_to<DeviceIdentifier>;
};

//template<class OBJECT>
//class DeviceSerializerMethods
//{
//	public:
//		using prop_deserialization_t = typename PropertySerializerMethods<OBJECT>::deserialization_t;
//		using deserialization_t = const OBJECT&;

//		template<DEVICE_C DEVICE>
//		static OBJECT serialize(const DEVICE &dev);

//		static OBJECT serializeID(const DeviceIdentifier &devID);

//		template<DEVICE_C DEVICE>
//		static DEVICE deserialize(deserialization_t data);

//		static DeviceIdentifier deserializeID(deserialization_t data);
//};

#endif // DEVICE_SERIALIZER_METHODS_H
