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

#include "nrp_mqtt_proxy/nrp_mqtt_client.h"

#include "nrp_general_library/utils/json_schema_utils.h"

bool NRPMQTTClient::isConnected()
{
    if (_mqttClient)
    {
        return _mqttClient->is_connected();
    }
    else
    {
        return false;
    }
}

void NRPMQTTClient::publish(const std::string& address, const std::string& msg, bool retained)
{
    if(!isConnected())
        return;

    if(!_topics.count(address))
        _topics.emplace(address, mqtt::topic(*_mqttClient, address, QOS, retained));

    if(_topics.at(address).get_retained() != retained) {
        std::string p1 = retained ? "" : "non ";
        std::string p2 = retained ? "non " : "";
        NRPLogger::warn("Attempt to publish a {}retained message to topic \"{}\", but this topic publishes {}retained messages", p1, address, p2);
    }

    _topics.at(address).publish(msg);
}

void NRPMQTTClient::publishDirect(const std::string& address, const std::string& msg)
{
    if(_subscribers.count(address))
        _subscribers.at(address)(msg);
}

void NRPMQTTClient::subscribe(const std::string& address, const std::function<void (const std::string&)>& callback)
{
    if(!_subscribers.count(address)) {
        _subscribers.emplace(address, callback);

        if(isConnected())
            _mqttClient->subscribe(address, QOS);
    }
    else
        NRPLogger::warn("Subscribe to " + address + " failed. The address is already in use.");
}

NRPMQTTClient::NRPMQTTClient(nlohmann::json clientParams) :
        _callback(this)
{
    json_utils::validateJson(clientParams, "json://nrp-core/nrp_connectors.json#/MQTTClient");
    _mqttClient = std::make_shared<mqtt::async_client>(clientParams.at("MQTTBroker"),
                                                       clientParams.at("ClientName"));

    try {
        NRPLogger::info("Connecting to MQTT broker.");

        // In case of re-connection keep session
        mqtt::connect_options connOpts;
        connOpts.set_clean_session(true);

        _mqttClient->set_callback(_callback);
        _mqttClient->connect(connOpts)->wait();

        NRPLogger::info("Connection to MQTT broker is established.");
        NRPLogger::debug("MQTT broker address: {}", _mqttClient->get_server_uri());
    }
    catch(std::exception &e)
    {
        NRPLogger::warn("Connection to MQTT broker failed! All MQTT publish/subscribe operations will be disabled");
        NRPLogger::debug("MQTT connection error: {}", e.what());
    }
}

NRPMQTTClient::NRPMQTTClient() :
        _callback(this)
{}

void NRPMQTTClient::disconnect()
{
    try{
        if (_mqttClient->is_connected()){
            _mqttClient->disconnect()->wait();
        }
    }
    catch(std::exception &e)
    {
        NRPLogger::error("Couldn't gracefully disconnect from MQTT broker.");
    }
}

void NRPMQTTClient::clearRetained()
{
    if(!isConnected())
        return;

    for (auto & [address, topic] : _topics) {
        if(topic.get_retained())
            topic.publish("");
    }
}

////////////////

MQTTCallback::MQTTCallback(NRPMQTTClient* proxy) :
        _proxy(proxy)
{ }

void MQTTCallback::message_arrived(mqtt::const_message_ptr msg)
{
    if(_proxy->_subscribers.count(msg->get_topic()))
        _proxy->_subscribers.at(msg->get_topic())(msg->to_string());
}
