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

#ifndef NRP_COMMUNICATION_CONTROLLER_H
#define NRP_COMMUNICATION_CONTROLLER_H

#include "nrp_json_engine_protocol/engine_server/engine_json_server.h"
#include "nrp_gazebo_json_engine/engine_server/gazebo_step_controller.h"

#include <pistache/router.h>
#include <pistache/endpoint.h>

#include <gazebo/common/Plugin.hh>
#include <map>
#include <memory>

/*!
 * \brief Manages communication with the NRP. Uses a REST server to send/receive data. Singleton class.
 */
class NRPJSONCommunicationController
        : public EngineJSONServer
{
    public:
        ~NRPJSONCommunicationController() override;

        /*! \brief Delete for singleton */
        NRPJSONCommunicationController(const NRPJSONCommunicationController &other) = delete;

        /*! \brief Delete for singleton */
        NRPJSONCommunicationController &operator=(const NRPJSONCommunicationController &other) = delete;

        /*! \brief Delete for singleton */
        NRPJSONCommunicationController(NRPJSONCommunicationController &&other) = delete;

        /*! \brief Delete for singleton */
        NRPJSONCommunicationController &&operator=(NRPJSONCommunicationController &&other) = delete;

        /*!
         * \brief Get singleton instance
         * \return Gets instance of NRPJSONCommunicationController
         */
        static NRPJSONCommunicationController& getInstance();


        /*!
         * \brief Reset server with the given server URL
         * \param serverURL URL used by server
         * \param engineName Name of this engine
         * \param registrationURL URL used to register this engine server's URL
         * \return Returns reference to server instance
         */
        static NRPJSONCommunicationController& resetInstance(const std::string &serverURL, const std::string &engineName, const std::string &registrationURL);


        /*!
         * \brief Register a step controller
         * \param stepController Pointer to step controller
         */
        void registerStepController(GazeboStepController *stepController);

        /*!
         * \brief Register a sensor plugin
         * \param sensorPlugin Pointer to sensor plugin
         */
        void registerSensorPlugin(gazebo::SensorPlugin *sensorPlugin)
        {
            this->_sensorPlugins.push_back(sensorPlugin);
        };

        /*!
         * \brief Register a model plugin
         * \param sensorPlugin Pointer to model plugin
         */
        void registerModelPlugin(gazebo::ModelPlugin *modelPlugin)
        {
            this->_modelPlugins.push_back(modelPlugin);
        };

        /*!
         * \brief Create datapack name from the given model and sensor/joint/link
         * \param modelName Name of the parent model of the controlled object
         * \param objectName Name of the controlled object (sensor, joint, link, ...)
         * \return Returns datapack name
         */
        static std::string createDataPackName(const std::string &modelName, const std::string &objectName)
        {   return modelName + "::" + objectName;  }

    private:

        /*!
         * \brief Singleton instance of this class
         */
        static std::unique_ptr<NRPJSONCommunicationController> _instance;

        /*!
         * \brief Controlls gazebo stepping
         */
        GazeboStepController *_stepController = nullptr;

        /*!
         * \brief Vector of registered SensorPlugin's.
         * They are kept in order to be available for function calls, like Reset
         * Because the world->Reset doesn't call plugins' corresponding functions.
         */
        std::vector< gazebo::SensorPlugin* >  _sensorPlugins;

        /*!
         * \brief Vector of registered ModelPlugin's
         * They are kept in order to be available for function calls, like Reset
         * Because the world->Reset doesn't call plugins' corresponding functions.
         */
        std::vector< gazebo::ModelPlugin* >  _modelPlugins;

        virtual SimulationTime runLoopStep(SimulationTime timeStep) override;

        virtual nlohmann::json initialize(const nlohmann::json &data, EngineJSONServer::lock_t &lock) override;

        virtual nlohmann::json reset(EngineJSONServer::lock_t &lock) override;

        virtual nlohmann::json shutdown(const nlohmann::json &data) override;

        /*!
         * \brief Make private for singleton
         */
        NRPJSONCommunicationController() = delete;

        /*!
         * \brief Constructor. Private for singleton
         * \param serverURL URL used by server
         * \param engineName Name of this engine
         * \param registrationURL URL used to register this engine server's URL
         * \return Returns reference to server instance
         */
        NRPJSONCommunicationController(const std::string &serverURL, const std::string &engineName, const std::string &registrationURL);
};

#endif
