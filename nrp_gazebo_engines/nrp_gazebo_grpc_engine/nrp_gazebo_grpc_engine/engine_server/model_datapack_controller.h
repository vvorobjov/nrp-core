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

#ifndef MODEL_GRPC_DATAPACK_CONTROLLER_H
#define MODEL_GRPC_DATAPACK_CONTROLLER_H

#include <gazebo/gazebo.hh>
#include <gazebo/physics/Model.hh>
#include "nrp_general_library/engine_interfaces/datapack_controller.h"
#include "nrp_general_library/utils/nrp_exceptions.h"
#include "nrp_protobuf/engine_grpc.grpc.pb.h"
#include "nrp_protobuf/gazebo_msgs.pb.h"

namespace gazebo
{
    /*!
     * \brief Interface for Models
     */
    class ModelGrpcDataPackController
            : public DataPackController<google::protobuf::Message>
    {
            template<class T>
            constexpr static float ToFloat(const T &val)
            {   return static_cast<float>(val); }

        public:
        
            ModelGrpcDataPackController(const std::string &modelName, const physics::ModelPtr &model)
                : _name(modelName),
                  _model(model)
            {}

            virtual void handleDataPackData(const google::protobuf::Message &data) override
            {
                // throws bad_cast
                const auto &l = dynamic_cast<const Gazebo::Model &>(data);

                // Set Pose
                if(l.position_size() || l.rotation_size()) {
                    auto pose = this->_model->WorldPose();

                    if(l.position_size() && l.position_size() == 3) {
                        pose.Pos().X(l.position().Get(0));
                        pose.Pos().Y(l.position().Get(1));
                        pose.Pos().Z(l.position().Get(2));
                    }
                    else if(l.position_size())
                        throw NRPException::logCreate("Model msg position wrong");

                    if(l.rotation_size() && l.rotation_size() == 4) {
                        pose.Rot().X(l.rotation().Get(0));
                        pose.Rot().Y(l.rotation().Get(1));
                        pose.Rot().Z(l.rotation().Get(2));
                        pose.Rot().W(l.rotation().Get(3));
                    }
                    else if(l.rotation_size())
                        throw NRPException::logCreate("Model msg rotation wrong");

                    this->_model->SetWorldPose(pose);
                }

                // Set Linear velocity
                if(l.linearvelocity_size() && l.linearvelocity_size() == 3) {
                    auto linvel = this->_model->WorldLinearVel();
                    linvel.X(l.linearvelocity().Get(0));
                    linvel.Y(l.linearvelocity().Get(1));
                    linvel.Z(l.linearvelocity().Get(2));
                    this->_model->SetLinearVel(linvel);
                }
                else if(l.linearvelocity_size())
                    throw NRPException::logCreate("Model msg linear velocity wrong");

                // Set Angular velocity
                if(l.angularvelocity_size() && l.angularvelocity_size() == 3) {
                    auto angvel = this->_model->WorldAngularVel();
                    angvel.X(l.angularvelocity().Get(0));
                    angvel.Y(l.angularvelocity().Get(1));
                    angvel.Z(l.angularvelocity().Get(2));
                    this->_model->SetAngularVel(angvel);
                }
                else if(l.angularvelocity_size())
                    throw NRPException::logCreate("Model msg angular velocity wrong");
            }

            virtual google::protobuf::Message *getDataPackInformation() override
            {
                auto l = new Gazebo::Model();

                const auto &pose = this->_model->WorldPose();
                l->add_position(ToFloat(pose.Pos().X()));
                l->add_position(ToFloat(pose.Pos().Y()));
                l->add_position(ToFloat(pose.Pos().Z()));

                l->add_rotation(ToFloat(pose.Rot().X()));
                l->add_rotation(ToFloat(pose.Rot().Y()));
                l->add_rotation(ToFloat(pose.Rot().Z()));
                l->add_rotation(ToFloat(pose.Rot().W()));

                const auto &linVel = this->_model->WorldLinearVel();
                l->add_linearvelocity(ToFloat(linVel.X()));
                l->add_linearvelocity(ToFloat(linVel.Y()));
                l->add_linearvelocity(ToFloat(linVel.Z()));

                const auto &angVel = this->_model->WorldAngularVel();
                l->add_angularvelocity(ToFloat(angVel.X()));
                l->add_angularvelocity(ToFloat(angVel.Y()));
                l->add_angularvelocity(ToFloat(angVel.Z()));

                return l;
            }

        private:
            /*!
             * \brief Model Name
             */
            std::string _name;

            /*!
             * \brief Pointer to model
             */
            physics::ModelPtr _model;
    };
}

#endif // MODEL_GRPC_DATAPACK_CONTROLLER_H
