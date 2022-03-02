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

#ifndef CAMERA_GRPC_DATAPACK_CONTROLLER_H
#define CAMERA_GRPC_DATAPACK_CONTROLLER_H

#include <gazebo/gazebo.hh>
#include <gazebo/sensors/CameraSensor.hh>
#include <gazebo/rendering/Camera.hh>
#include "nrp_general_library/engine_interfaces/datapack_controller.h"
#include "nrp_protobuf/engine_grpc.grpc.pb.h"
#include "nrp_protobuf/gazebo_msgs.pb.h"

#include "nrp_general_library/utils/nrp_logger.h"

namespace gazebo
{
    class CameraGrpcDataPackController
            : public DataPackController<google::protobuf::Message>
    {
        public:
            // TODO: unused parameter
            CameraGrpcDataPackController(const std::string &devName, const rendering::CameraPtr &camera, const sensors::SensorPtr &parent)
                : _name(devName),
                  _parentSensor(parent),
                  _data(new Gazebo::Camera())
            {}

            ~CameraGrpcDataPackController()
            {
                delete _data;
            };

            virtual void handleDataPackData(const google::protobuf::Message &data) override
            {}

            virtual google::protobuf::Message *getDataPackInformation() override
            {
                if(_hasNewData) {
                    auto old = _data;
                    _data = new Gazebo::Camera();
                    _hasNewData = false;
                    return old;
                }
                else
                    return nullptr;
            }

            void updateCamData(const unsigned char *image, unsigned int width, unsigned int height, unsigned int depth)
            {
                const common::Time sensorUpdateTime = this->_parentSensor->LastMeasurementTime();
                if(sensorUpdateTime > this->_lastSensorUpdateTime)
                {
                    this->_lastSensorUpdateTime = sensorUpdateTime;

                    _data->set_imageheight(height);
                    _data->set_imagewidth(width);
                    _data->set_imagedepth(depth);

                    const auto imageSize = width*height*depth;
                    _data->set_imagedata(image, imageSize);

                    _hasNewData = true;
                }
            }

            void resetTime(){
                this->_lastSensorUpdateTime = 0;
                this->_hasNewData = false;
            }

        private:
            std::string _name;

            sensors::SensorPtr _parentSensor;

            common::Time _lastSensorUpdateTime = 0;

            Gazebo::Camera *_data;

            bool _hasNewData = false;
    };
}

#endif // CAMERA_GRPC_DATAPACK_CONTROLLER_H
