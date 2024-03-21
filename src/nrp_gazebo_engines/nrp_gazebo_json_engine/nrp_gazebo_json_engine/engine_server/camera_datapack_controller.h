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

#ifndef CAMERA_DATAPACK_CONTROLLER_H
#define CAMERA_DATAPACK_CONTROLLER_H

#include "nrp_general_library/datapack_interface/datapack_interface.h"
#include "nrp_json_engine_protocol/engine_server/json_datapack_controller.h"

#include <gazebo/gazebo.hh>
#include <gazebo/sensors/CameraSensor.hh>
#include <gazebo/rendering/Camera.hh>

#include <nlohmann/json.hpp>

namespace gazebo
{
    class CameraDataPackController
            : public JsonDataPackController
    {
        public:
            CameraDataPackController(const std::string &devName, const rendering::CameraPtr &camera, const sensors::SensorPtr &parent)
                : JsonDataPackController(JsonDataPack::createID(devName, "")),
                  _parentSensor(parent)
            {}

            void handleDataPackData(const nlohmann::json &data) override
            {
                // Empty
            }

            nlohmann::json * getDataPackInformation() override
            {
                if(this->_newDataAvailable)
                {
                    this->_newDataAvailable = false;
                    return &this->_data;
                }

                return nullptr;
            }

            void updateCamData(const unsigned char *image, unsigned int width, unsigned int height, unsigned int depth)
            {
                const common::Time sensorUpdateTime = this->_parentSensor->LastMeasurementTime();

                if(sensorUpdateTime > this->_lastSensorUpdateTime)
                {
                    //std::cout << "Updating camera data\n";
                    this->_lastSensorUpdateTime = sensorUpdateTime;

                    auto cachedData = getCachedData();

                    (*cachedData)["image_height"] = height;
                    (*cachedData)["image_width" ] = width;
                    (*cachedData)["image_depth" ] = depth;

                    const auto imageSize = width * height * depth;
                    (*cachedData)["image_data"  ] = std::vector<unsigned char>(image, image + imageSize);

                    this->_newDataAvailable = true;
                }
            }

            void resetTime(){
                this->_lastSensorUpdateTime = 0;
                this->_newDataAvailable = false;
            }

        private:

            sensors::SensorPtr _parentSensor;

            common::Time _lastSensorUpdateTime = 0;

            bool _newDataAvailable = false;
    };
}

#endif // CAMERA_DATAPACK_CONTROLLER_H
