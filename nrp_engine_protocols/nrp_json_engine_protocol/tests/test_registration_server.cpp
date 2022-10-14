//
// NRP Core - Backend infrastructure to synchronize simulations
//
// Copyright 2020-2021 NRP Team
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//   http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//
// This project has received funding from the European Union’s Horizon 2020
// Framework Programme for Research and Innovation under the Specific Grant
// Agreement No. 945539 (Human Brain Project SGA3).
//

#include <gtest/gtest.h>
#include <boost/asio.hpp>

#include "nrp_json_engine_protocol/nrp_client/engine_json_registration_server.h"
#include "nrp_general_library/utils/nrp_exceptions.h"

using namespace boost::asio;


class RegistrationServer : public testing::Test
{
    protected:

        virtual void TearDown()
        {
            EngineJSONRegistrationServer::clearInstance();
        }

        const std::string ipv4 = "127.0.0.1";
        const uint16_t    port = 9001;
        const std::string address = ipv4 + ":" + std::to_string(port);
};


TEST_F(RegistrationServer, TryInitializeSimple)
{
    // The port should be free, and the returned address should equal the requested address

    ASSERT_EQ(EngineJSONRegistrationServer::tryInstantiate(address, 0), address);
}


TEST_F(RegistrationServer, TryInitializePortTaken)
{
    // Bind to the port before creating the registration server

    io_service service;
    ip::tcp::socket socket(service);
    socket.open(ip::tcp::v4());
    socket.bind(ip::tcp::endpoint(ip::address::from_string(ipv4), port));

    // The number of retries is 0, the function should throw after failing to instantiate the registration server

    ASSERT_THROW(EngineJSONRegistrationServer::tryInstantiate(address, 0), NRPExceptionNonRecoverable);
}


TEST_F(RegistrationServer, TryInitializePortTakenRetry)
{
    // Bind to the port before creating the registration server

    io_service service;
    ip::tcp::socket socket(service);
    socket.open(ip::tcp::v4());
    socket.bind(ip::tcp::endpoint(ip::address::from_string(ipv4), port));

    // We ask the function to retry to create the reg server one time with another port in case the first try fails
    // It should find a free port and return it instead of the requested one, but the IP shouldn't change

    const std::string result = EngineJSONRegistrationServer::tryInstantiate(address, 1);
    ASSERT_TRUE(result.find(ipv4) != std::string::npos);
    ASSERT_TRUE(result.find(std::to_string(port)) == std::string::npos);
}

// EOF
