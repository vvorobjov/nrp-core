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

#include "nrp_camera_controller_plugin/nrp_camera_controller_plugin.h"

#include "nrp_gazebo_json_engine/engine_server/nrp_communication_controller.h"


void gazebo::NRPCameraController::Load(gazebo::sensors::SensorPtr sensor, sdf::ElementPtr sdf)
{
    NRP_LOGGER_TRACE("{} called", __FUNCTION__);
    
    // Load camera plugin
    this->CameraPlugin::Load(sensor, sdf);

    const auto devName = NRPJSONCommunicationController::createDataPackName(sensor->ParentName(), sensor->Name());
    NRPLogger::info("Registering Camera datapack [ {} ]", devName);

    // Create camera datapack and register it
    this->_cameraInterface.reset(new CameraDataPackController(devName, this->camera, sensor));
    try {
        auto &commControl = NRPJSONCommunicationController::getInstance();
        commControl.registerDataPack(devName, this->_cameraInterface.get());
        // Register plugin in communication controller
        commControl.registerSensorPlugin(this);
    }
    catch(NRPException&) {
        throw NRPException::logCreate("Failed to register Camera datapack. Ensure that this NRP JSON Camera plugin is "
                                      "used in conjunction with a gazebo_json Engine in an NRP Core experiment.");
    }
}

void gazebo::NRPCameraController::OnNewFrame(const unsigned char *image, unsigned int width, unsigned int height, unsigned int depth, const std::string &)
{   this->_cameraInterface->updateCamData(image, width, height, depth); }


void gazebo::NRPCameraController::Reset()
{
    NRP_LOGGER_TRACE("{} called", __FUNCTION__);
    this->_cameraInterface->resetTime();
}