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

#include "nrp_gazebo_grpc_engine/config/gazebo_grpc_config.h"
#include "nrp_gazebo_grpc_engine/engine_server/gazebo_step_controller.h"

#include "nrp_protobuf/gazebo.pb.h"
#include "nrp_grpc_engine_protocol/engine_server/engine_proto_wrapper.h"

#include <pistache/router.h>
#include <pistache/endpoint.h>

#include <gazebo/common/Plugin.hh>
#include <map>
#include <memory>

/*!
 * \brief Manages communication with the NRP
 *
 * Implementation of EngineProtoWrapper to manage a Gazebo Engine
 */
class NRPGazeboCommunicationController
        : public EngineProtoWrapper
{
    public:

        using mutex_t = std::timed_mutex;
        using lock_t  = std::unique_lock<NRPGazeboCommunicationController::mutex_t>;

        NRPGazeboCommunicationController(const std::string &engineName,
                                                            const std::string &protobufPluginsPath,
                                                            const nlohmann::json &protobufPlugins);

        /*!
         * \brief Registers a datapack controller locking the datapack mutex
         *
         * \param[in] datapackName       Name of the datapack to be registered
         * \param[in] datapackController Pointer to the datapack controller object that's supposed to be
         *                             registered in the engine
         */
        void registerDataPackWithLock(const std::string & datapackName, ProtoDataPackController *interface)
        {
            lock_t lock(this->_datapackLock);
            this->registerDataPack(datapackName, interface);
        }

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
         * \brief Create datapack name from the given plugin and sensor/joint/link
         * \param modelName Name of the parent model of the controlled object
         * \param objectName Name of the controlled object (sensor, joint, link, ...)
         * \return Returns datapack name
         */
        static std::string createDataPackName(const std::string &modelName, const std::string &objectName)
        {   return modelName + "::" + objectName;  }

        SimulationTime runLoopStep(SimulationTime timeStep) override;

        void initialize(const nlohmann::json &data) override;

        void reset() override;

        void shutdown() override;

        bool initRunFlag() const override { throw NRPException::logCreate("initRunFlag is not implemented in NRPGazeboCommunicationController"); }

        bool shutdownFlag() const override { throw NRPException::logCreate("shutdownFlag is not implemented in NRPGazeboCommunicationController"); }

private:

        /*!
         * \brief Controls gazebo stepping
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

        mutex_t _datapackLock;
};

/*!
 * \brief Singleton class which stores a pointer to the NRPGazeboCommunicationController being used to manage the simulation
 *
 * so other gazebo plugins can invoke it to register datapacks
 */
class CommControllerSingleton
{
public:

    /*! \brief Delete for singleton */
    CommControllerSingleton(const CommControllerSingleton &other) = delete;

    /*! \brief Delete for singleton */
    CommControllerSingleton &operator=(const CommControllerSingleton &other) = delete;

    /*! \brief Delete for singleton */
    CommControllerSingleton(CommControllerSingleton &&other) = delete;

    /*! \brief Delete for singleton */
    CommControllerSingleton &&operator=(CommControllerSingleton &&other) = delete;

    /*!
     * \brief Get singleton instance
     * \return Gets instance of CommControllerSingleton
     */
    static CommControllerSingleton& getInstance();

    /*!
     * \brief Reset
     */
    static CommControllerSingleton& resetInstance(NRPGazeboCommunicationController* engineController);


    NRPGazeboCommunicationController& engineCommController()
    {
        return *_controller;
    }


private:

    /*!
     * \brief Singleton instance of this class
     */
    static std::unique_ptr<CommControllerSingleton> _instance;

    /*!
     * \brief Pointer to the NRPGazeboCommunicationController
     */
    NRPGazeboCommunicationController* _controller;

    /*!
     * \brief Make private for singleton
     */
    CommControllerSingleton() = delete;

    /*!
     * \brief Constructor. Private for singleton
     */
    CommControllerSingleton(NRPGazeboCommunicationController* engineController);
};

#endif
