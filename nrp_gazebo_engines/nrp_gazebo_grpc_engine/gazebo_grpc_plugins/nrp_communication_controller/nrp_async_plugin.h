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

#ifndef NRP_GAZEBO_ASYNC_PLUGIN_H
#define NRP_GAZEBO_ASYNC_PLUGIN_H

#include <gazebo/gazebo.hh>
#include <string_view>
#include <nlohmann/json.hpp>

#include "nrp_event_loop/event_loop/event_loop_engine.h"

namespace gazebo
{
    /*!
     * \brief Plugin that creates a REST server for NRP communication
     */
    class NRPGazeboAsyncPlugin
            : public SystemPlugin
    {
    public:
        virtual ~NRPGazeboAsyncPlugin();

        /*!
         * \brief Load the communication server
         */
        virtual void Load(int argc = 0, char **argv = nullptr);

        virtual void Reset();

    private:
        /*!
         * \brief ELE config
         */
        nlohmann::json _eleConfig;
        std::chrono::milliseconds _timestep, _timeout, _timestepWarn;

        /*!
         * \brief Engine name, read from program opts
         */
        std::string _engineName;

        /*!
         * \brief Path to Protobuf Plugins, read from program opts
         */
        std::string _protobufPluginsPath;

        /*!
         * \brief List of Protobuf Plugins, read from program opts
         */
        nlohmann::json _protobufPlugins;

        /*!
         * \brief Pointer to the EventLoopEngine managing the simulation
         */
        std::unique_ptr<EventLoopEngine> _ele;

        std::shared_ptr<NRPLogger> _logger;

        void startELE();

        void shutdown();
    };

    GZ_REGISTER_SYSTEM_PLUGIN(NRPGazeboAsyncPlugin)
}

#endif
