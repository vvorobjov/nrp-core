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

#ifndef GAZEBO_STEP_CONTROLLER_H
#define GAZEBO_STEP_CONTROLLER_H

#include "nrp_general_library/engine_interfaces/engine_client_interface.h"

/*!
 *  \brief Controlls execution of Gazebo steps. Will be inherited by a Gazebo WorldPlugin
 */
class GazeboStepController
{
    public:
        GazeboStepController();
        virtual ~GazeboStepController();

        virtual SimulationTime runLoopStep(SimulationTime timeStep) = 0;

        virtual bool finishWorldLoading(double waitTime) = 0;

        virtual void addRequiredModel(const std::string &modelName) = 0;

        virtual bool resetWorld() = 0;
};

#endif // GAZEBO_STEP_CONTROLLER_H
