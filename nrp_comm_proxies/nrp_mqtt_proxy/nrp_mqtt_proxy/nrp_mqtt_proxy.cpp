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

#include "nrp_mqtt_proxy/nrp_mqtt_proxy.h"

std::unique_ptr<NRPMQTTProxy> NRPMQTTProxy::_instance = nullptr;

NRPMQTTProxy &NRPMQTTProxy::getInstance()
{
    return *(NRPMQTTProxy::_instance);
}

NRPMQTTProxy &NRPMQTTProxy::resetInstance(const nlohmann::json& clientParams)
{
    NRPMQTTProxy::_instance.reset(new NRPMQTTProxy(clientParams));
    return NRPMQTTProxy::getInstance();
}

void NRPMQTTProxy::publish(const std::string& address, const std::string& msg, bool retained)
{ _mqttClient->publish(address, msg, retained); }

void NRPMQTTProxy::subscribe(const std::string& address, const std::function<void (const std::string&)>& callback)
{ _mqttClient->subscribe(address, callback); }

NRPMQTTProxy::NRPMQTTProxy(const nlohmann::json& clientParams) :
        _mqttClient(new NRPMQTTClient(clientParams))
{
    if(_mqttClient->isConnected())
        this->publish("nrp/welcome", "NRP-core is connected!");
}

void NRPMQTTProxy::disconnect()
{
    if(_mqttClient->isConnected())
        this->publish("nrp/welcome", "Bye! NRP-core is disconnecting!");
    _mqttClient->disconnect();
}

void NRPMQTTProxy::clearRetained()
{ _mqttClient->clearRetained(); }
