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

#ifndef MQTT_CLIENT_H
#define MQTT_CLIENT_H


#include <functional>

#include "nlohmann/json.hpp"

#include "nrp_general_library/utils/nrp_logger.h"
#include "mqtt/async_client.h"

// TODO: QOS and non-clean session are hardcoded, they could be parameters
const int  QOS = 1;
const auto TIMEOUT = std::chrono::seconds(10);

class NRPMQTTProxy;

class NRPMQTTClient {

public:
    /*!
     * Constructor
     */
    NRPMQTTClient(nlohmann::json clientParams);

    NRPMQTTClient() {};

    virtual ~NRPMQTTClient() = default;

    /*!
     * \brief Subscribe to MQTT topic 'address' with callback function 'callback'
     */
    virtual void subscribe(const std::string& address, const std::function<void (const std::string&)>& callback);

    /*!
     * \brief Publishes 'msg' to MQTT topic 'address'
     */
    virtual void publish(const std::string& address, const std::string& msg);

    /*!
     * \brief Disconnects client from MQTT Broker
     */
    virtual void disconnect();

    /*!
     * \brief Check connection status to broker
     */
    virtual bool isConnected();

private:

    /*!
     * \brief MQTT client pointer
     */
    std::shared_ptr< mqtt::async_client> _mqttClient;

    /*!
     * \brief The communicated topics
     */
    std::map<std::string, mqtt::topic> _topics;

    /*!
     * \brief Subscriptions
     */
    std::map<std::string, const std::function<void (const std::string&)>&> _subscribers;

    friend NRPMQTTProxy;
};

#endif //MQTT_CLIENT_H
