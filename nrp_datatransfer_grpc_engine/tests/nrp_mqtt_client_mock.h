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

#ifndef MQTT_CLIENT_MOCK_H
#define MQTT_CLIENT_MOCK_H


#include <functional>
#include <gmock/gmock.h>

#include "nlohmann/json.hpp"

#include "datatransfer_grpc_engine/engine_server/nrp_mqtt_client.h"

#include "nrp_general_library/utils/nrp_logger.h"
#include "mqtt/async_client.h"


// Fake NRPMQTTClient
// The execution of NRPMQTTClient methods can be delegated to this class
class NRPMQTTClientFake : public NRPMQTTClient {

public:

    NRPMQTTClientFake(bool conn) : _isConnected(conn) {};
    NRPMQTTClientFake() : _isConnected(false) {};

    void publish(const std::string& , const std::string& ) override {};
    void disconnect() override {_isConnected = false;};
    bool isConnected() override {return _isConnected;}

private:

    bool _isConnected;

};


// Mock NRPMQTTClient
// The wrapper for calls of the NRPMQTTClient in tests
class NRPMQTTClientMock : public NRPMQTTClient {

public:
    /*!
     * Constructor
     */
    NRPMQTTClientMock(nlohmann::json clientParams) : NRPMQTTClient(clientParams) {};

    NRPMQTTClientMock(bool conn) : NRPMQTTClient(),  fake_(conn) {};

    MOCK_METHOD(void, publish, (const std::string& address, const std::string& msg), (override));
    MOCK_METHOD(void, disconnect, (), (override));

    MOCK_METHOD(bool, isConnected, (), (override));

    void DelegateToFake()
    {
        ON_CALL(*this, publish).WillByDefault([this](const std::string &address, const std::string &msg)
                                              { fake_.publish(address, msg); });
        ON_CALL(*this, disconnect).WillByDefault([this]()
                                                 { fake_.disconnect(); });
        ON_CALL(*this, isConnected).WillByDefault([this]()
                                                  { return fake_.isConnected(); });
    }

private:

    NRPMQTTClientFake fake_;
};

#endif //MQTT_CLIENT_MOCK_H
