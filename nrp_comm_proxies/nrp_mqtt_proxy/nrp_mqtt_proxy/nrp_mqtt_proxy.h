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

#ifndef MQTT_PROXY_H
#define MQTT_PROXY_H

#include "nrp_mqtt_proxy/nrp_mqtt_client.h"

class NRPMQTTProxy {

public:

    NRPMQTTProxy() = delete;

    // Delete move and copy operators. This ensures this class is a singleton
    NRPMQTTProxy(const NRPMQTTProxy &) = delete;
    NRPMQTTProxy(NRPMQTTProxy &&) = delete;

    NRPMQTTProxy &operator=(const NRPMQTTProxy &) = delete;
    NRPMQTTProxy &operator=(NRPMQTTProxy &&) = delete;

    /*!
     * \brief Get singleton instance of NRPMQTTProxy
     */
    static NRPMQTTProxy &getInstance();

    /*!
     * \brief Reset singleton instance
     */
    static NRPMQTTProxy &resetInstance(const nlohmann::json& clientParams);

    /*!
     * \brief Subscribe to MQTT topic 'address' with callback function 'callback'
     */
    void subscribe(const std::string& address, const std::function<void (const std::string&)>& callback);

    /*!
     * \brief Publishes 'msg' to MQTT topic 'address'
     */
    void publish(const std::string& address, const std::string& msg);

    /*!
     * \brief Disconnects client from MQTT Broker
     */
    void disconnect();

private:

    std::shared_ptr< NRPMQTTClient> _mqttClient;

    /*!
     * Constructor
     */
    NRPMQTTProxy(const nlohmann::json& clientParams);

    static std::unique_ptr<NRPMQTTProxy> _instance;
};

#endif //MQTT_PROXY_H
