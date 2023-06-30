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

#ifndef EVENT_LOOP_ENGINE_H
#define EVENT_LOOP_ENGINE_H

#include "nrp_mqtt_proxy/nrp_mqtt_proxy.h"
#include "nrp_event_loop/event_loop/event_loop_interface.h"
#include "nrp_grpc_engine_protocol/engine_server/engine_proto_wrapper.h"
#include "nrp_protobuf/engine_grpc.pb.h"

#include <nlohmann/json.hpp>

/*!
 * \brief EventLoop implementation which runs an EngineProtoWrapper
 */
class EventLoopEngine : public EventLoopInterface
{
    using mutex_t = std::timed_mutex;
    using lock_t  = std::unique_lock<EventLoopEngine::mutex_t>;

    public:

        /*!
         * \brief Constructor
         */
        EventLoopEngine(std::chrono::milliseconds timestep, std::chrono::milliseconds rtDeltaThres,
                        size_t storeCapacity, bool doProcessLast,
                        const nlohmann::json &engineConfig, EngineProtoWrapper* engineWrapper,
                        bool delegateRTControl = false,
                        bool logRTInfo = false,
                        bool syncTimeRef = false);

        ~EventLoopEngine();

    protected:

        void initializeCB() override;

        void runLoopCB() override;

        void shutdownCB() override;

        void realtimeDeltaCB(std::chrono::milliseconds deviation) override;

    private:

        /*!
         * \brief Callback function used to subscribe to mqtt topics
         *
         * \param msgStr received mqtt msg
         */
        void topic_callback(const std::string& dpName, const std::string& msgStr);

        std::map<std::string, std::vector<EngineGrpc::DataPackMessage>> _datapackStore;
        std::shared_ptr<EngineGrpc::DataPackMessage> _datapackPub;
        size_t _storeCapacity;
        bool _doProcessLast;
        std::vector<std::string> _datapackNames;
        NRPMQTTProxy* _mqttProxy;

        /*!
         * \brief Pointer to the Engine Wrapper object
         */
        std::unique_ptr<EngineProtoWrapper> _engineWrapper;

        /*!
         * \brief helper function to construct topic name for setting datapacks
         * \return topic string
         */
        std::string datapackTopicSet(const std::string& dpName);

        /*!
         * \brief helper function to construct topic name for getting datapacks
         * \return topic string
         */
        std::string datapackTopicGet(const std::string& dpName);

        /*! \brief Configuration of the Engine run by this EventLoop  */
        nlohmann::json _engineConfig;
        /*! \brief mutex object used to protect data read/write operations */
        mutex_t _datapackLock;

};


#endif // EVENT_LOOP_ENGINE_H
