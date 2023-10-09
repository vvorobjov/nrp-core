//
// NRP Core - Backend infrastructure to synchronize simulations
//
// Copyright 2020-2023 NRP Team
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

#include "nrp_link_controller_plugin/nrp_link_controller_plugin.h"

#include "nrp_gazebo_json_engine/engine_server/nrp_communication_controller.h"

#include <gazebo/physics/Model.hh>
#include <gazebo/physics/Link.hh>

void gazebo::NRPLinkControllerPlugin::Load(gazebo::physics::ModelPtr model, sdf::ElementPtr)
{
    try {
        auto &commControl = NRPJSONCommunicationController::getInstance();

        // Register a datapack for each link
        auto links = model->GetLinks();
        for (const auto &link: links) {
            const auto datapackName = NRPJSONCommunicationController::createDataPackName(model->GetName(),
                                                                                         link->GetName());

            NRPLogger::info("Registering Link datapack [ {} ]", datapackName);

            this->_linkInterfaces.push_back(LinkDataPackController(datapackName, link));
            commControl.registerDataPack(datapackName, &(this->_linkInterfaces.back()));
        }

        // Register plugin
        commControl.registerModelPlugin(this);
    }
    catch (NRPException&) {
        throw NRPException::logCreate("Failed to register Link datapack. Ensure that this NRP JSON Link plugin is "
                                      "used in conjunction with a gazebo_json Engine in an NRP Core experiment.");
    }
}
