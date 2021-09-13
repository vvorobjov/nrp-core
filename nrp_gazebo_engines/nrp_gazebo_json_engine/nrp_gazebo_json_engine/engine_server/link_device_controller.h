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

#ifndef LINK_DEVICE_CONTROLLER_H
#define LINK_DEVICE_CONTROLLER_H

#include "nrp_json_engine_protocol/engine_server/json_device_controller.h"
#include "nrp_general_library/device_interface/device_interface.h"

#include <gazebo/gazebo.hh>
#include <gazebo/physics/Link.hh>

#include <nlohmann/json.hpp>

namespace gazebo
{
	/*!
	 * \brief Interface for links
	 */
	class LinkDeviceController
	        : public JsonDeviceController
	{
			template<class T>
			constexpr static float ToFloat(const T &val)
			{	return static_cast<float>(val);	}

		public:
			LinkDeviceController(const std::string &linkName, const physics::LinkPtr &link)
			    : JsonDeviceController(JsonDevice::createID(linkName, "")),
			      _link(link)
			{}

			void handleDeviceData(const nlohmann::json &data) override
			{}

			virtual nlohmann::json * getDeviceInformation() override
			{
				const auto &pose = this->_link->WorldCoGPose();
				auto cachedData = getCachedData();

				(*cachedData)["pos"] = { ToFloat(pose.Pos().X()), ToFloat(pose.Pos().Y()), ToFloat(pose.Pos().Z())	};
				(*cachedData)["rot"] = { ToFloat(pose.Rot().X()), ToFloat(pose.Rot().Y()), ToFloat(pose.Rot().Z())	};

				const auto &linVel = this->_link->WorldLinearVel();
				(*cachedData)["lin_vel"] = { ToFloat(linVel.X()), ToFloat(linVel.Y()), ToFloat(linVel.Z())	};

				const auto &angVel = this->_link->WorldAngularVel();
				(*cachedData)["ang_vel"] = { ToFloat(angVel.X()), ToFloat(angVel.Y()), ToFloat(angVel.Z())	};

				return &(this->_data);
			}

		private:

			/*!
			 * \brief Pointer to link
			 */
			physics::LinkPtr _link;
	};
}

#endif // LINK_DEVICE_CONTROLLER_H
