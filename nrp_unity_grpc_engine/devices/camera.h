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

#ifndef UNITY_CAMERA_H
#define UNITY_CAMERA_H

#include "nrp_general_library/device_interface/device.h"

#include <vector>

struct CameraConst
{
	static constexpr FixedString ImageData = "image_data";

	using cam_data_t = std::vector<unsigned char>;

	using JPropNames = PropNames<ImageData>;
};

/*!
 * \brief Physics Camera Image
 */
class Camera
        : public CameraConst,
          public Device<Camera, "PhysicsCamera", CameraConst::JPropNames, CameraConst::cam_data_t>
{
	public:
		Camera(DeviceIdentifier &&devID, property_template_t &&props = property_template_t(std::vector<unsigned char>({})))
		    : Device(std::move(devID), std::move(props))
		{}

		template<class DESERIALIZE_T>
		static auto deserializeProperties(DESERIALIZE_T &&data)
		{	return Device::deserializeProperties(std::forward<DESERIALIZE_T>(data), std::vector<unsigned char>({}));	}

		const cam_data_t &imageData() const;
		cam_data_t &imageData();
		void setImageData(const cam_data_t &imageData);
};

#endif // UNITY_CAMERA_H
