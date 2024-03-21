/* * NRP Core - Backend infrastructure to synchronize simulations
 *
 * Copyright 2020-2023 NRP Team
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

#ifndef JOINT_DATAPACK_CONTROLLER_H
#define JOINT_DATAPACK_CONTROLLER_H

#include "nrp_json_engine_protocol/engine_server/json_datapack_controller.h"
#include "nrp_general_library/datapack_interface/datapack_interface.h"

#include <gazebo/gazebo.hh>
#include <gazebo/physics/JointController.hh>
#include <gazebo/physics/Joint.hh>

#include <nlohmann/json.hpp>

namespace gazebo
{
    /*!
     * \brief Interface for a single joint
     */
    class JointDataPackController
            : public JsonDataPackController
    {
            using fcn_ptr_t = void(physics::JointPtr, double, int);

        public:
            JointDataPackController(const physics::JointPtr &joint, const physics::JointControllerPtr &jointController, const std::string &jointName)
                : JsonDataPackController(JsonDataPack::createID(jointName, "")),
                  _joint(joint),
                  _jointController(jointController)
            {}

            void handleDataPackData(const nlohmann::json &data) override
            {
                setCachedData(data);

                const auto &jointName = this->_datapackId.Name;
                const auto cachedData = getCachedData();

                if(!(*cachedData)["position"].is_null())
                    this->_jointController->SetPositionTarget(jointName, (*cachedData)["position"].get<float>());

                if(!(*cachedData)["velocity"].is_null())
                    this->_jointController->SetVelocityTarget(jointName, (*cachedData)["velocity"].get<float>());

                if(!(*cachedData)["effort"].is_null())
                    this->_joint->SetForce(0, (*cachedData)["effort"].get<float>());
            }

            nlohmann::json * getDataPackInformation() override
            {
                auto data = getCachedData();

                (*data)["position"] = this->_joint->Position(0);
                (*data)["velocity"] = this->_joint->GetVelocity(0);
                (*data)["effort"]   = this->_joint->GetForce(0);

                return &(this->_data);
            }

        private:

            /*!
             * \brief Pointer to joint
             */
            physics::JointPtr _joint;

            /*!
             * \brief Pointer to joint controller of the joint's model
             */
            physics::JointControllerPtr _jointController = nullptr;
    };
}

#endif // JOINT_DATAPACK_CONTROLLER_H
