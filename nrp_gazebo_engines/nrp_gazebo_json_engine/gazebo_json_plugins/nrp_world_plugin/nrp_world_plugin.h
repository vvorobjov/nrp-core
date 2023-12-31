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

#ifndef NRP_WORLD_PLUGIN_H
#define NRP_WORLD_PLUGIN_H

#include "nrp_gazebo_json_engine/engine_server/gazebo_step_controller.h"

#include <gazebo/gazebo.hh>
#include <gazebo/physics/JointController.hh>
#include <gazebo/physics/Joint.hh>
#include <gazebo/physics/WorldState.hh>

namespace gazebo
{
    /*!
     * \brief Interface for a single joint
     */
    class NRPWorldPlugin
            : public GazeboStepController,
              public WorldPlugin
    {
        public:
            virtual ~NRPWorldPlugin() override = default;

            virtual void Load(physics::WorldPtr world, sdf::ElementPtr sdf) override;
            virtual void Reset() override;

            virtual SimulationTime runLoopStep(SimulationTime timeStep) override;

            bool finishWorldLoading(double waitTime) override;

            bool resetWorld() override;

            /*!
             * \brief adds a model name to the set of models this plugin should wait for
             *
             * \param modelName model name
             */
            void addRequiredModel(const std::string &modelName) override;

        private:
            /*!
             * \brief Lock to ensure only one loop is being executed
             */
            std::mutex _lockLoop;

            physics::WorldPtr _world;
            sdf::ElementPtr _worldSDF;

            /*!
             * \brief Contains state of the world loaded with the Load function
             */
            physics::WorldState _initialWorldState;

            /*!
             * \brief Set of model names that this plugin should wait for
             */
            std::set<std::string> _requiredModels;

            /*!
             * \brief Gazebo Event Connection Handle
             */
            event::ConnectionPtr add_entity_connection;

            /*!
             * \brief Start running the sim.
             * \param numIterations Number of iterations to run
             */
            void startLoop(unsigned int numIterations);

            /*!
             * \brief Gazebo Event Connection callback
             * \param name entity name
             */
            void entityAddedCB(const std::string &name);
    };

    GZ_REGISTER_WORLD_PLUGIN(NRPWorldPlugin)
}

#endif // NRP_WORLD_PLUGIN
