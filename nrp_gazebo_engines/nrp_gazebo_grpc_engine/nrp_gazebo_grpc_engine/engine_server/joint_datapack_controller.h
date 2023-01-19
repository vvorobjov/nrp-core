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

#ifndef JOINT_GRPC_DATAPACK_CONTROLLER_H
#define JOINT_GRPC_DATAPACK_CONTROLLER_H

#include <gazebo/gazebo.hh>
#include <gazebo/physics/JointController.hh>
#include <gazebo/physics/Joint.hh>
#include "nrp_general_library/engine_interfaces/datapack_controller.h"
#include "nrp_protobuf/engine_grpc.grpc.pb.h"
#include "nrp_protobuf/gazebo.pb.h"

namespace gazebo
{
    /*!
     * \brief Interface for a single joint
     */
    class JointGrpcDataPackController
            : public DataPackController<google::protobuf::Message>
    {
        public:
        JointGrpcDataPackController(const std::string &jointName, const physics::JointPtr &joint, const physics::JointControllerPtr &jointController)
                : _name(jointName),
                  _joint(joint),
                  _jointController(jointController)
            {}

            virtual void handleDataPackData(const google::protobuf::Message &data) override
            {
                // throws bad_cast
                const auto &j = dynamic_cast<const Gazebo::Joint &>(data);

                if(!std::isnan(j.position()))
                    this->_jointController->SetPositionTarget(_name, j.position());

                if(!std::isnan(j.velocity()))
                    this->_jointController->SetVelocityTarget(_name, j.velocity());

                if(!std::isnan(j.effort()))
                    this->_joint->SetForce(0, j.effort());
            }

            virtual google::protobuf::Message *getDataPackInformation() override
            {
                auto l = new Gazebo::Joint();
            
                l->set_position(this->_joint->Position(0));
                l->set_velocity(this->_joint->GetVelocity(0));
                l->set_effort(this->_joint->GetForce(0));

                return l;
            }

        private:

            /*!
             * \brief Joint Name
             */
            std::string _name;

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

#endif // JOINT_GRPC_DATAPACK_CONTROLLER_H
