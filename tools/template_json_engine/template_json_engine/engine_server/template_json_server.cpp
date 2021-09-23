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

#include "${engine_name_lowercase}_json_engine/engine_server/${engine_name_lowercase}_json_server.h"
#include "${engine_name_lowercase}_json_engine/engine_server/${engine_name_lowercase}_json_device_controller.h"

${engine_name}JSONServer::${engine_name}JSONServer(const std::string &serverAddress,
                                     const std::string &engineName,
                                     const std::string &registrationAddress)
    : EngineJSONServer(serverAddress, engineName, registrationAddress),
    _engineName(engineName)
{

}

SimulationTime ${engine_name}JSONServer::runLoopStep(SimulationTime timeStep)
{
    static SimulationTime simulationTime = SimulationTime::zero();

    std::cout << "Running simulation step" << std::endl;

    // Advance the simulation by timeStep

    simulationTime += timeStep;

    // Return current simulation time

    return simulationTime;
}

nlohmann::json ${engine_name}JSONServer::initialize(const nlohmann::json &/*data*/, EngineJSONServer::lock_t &/*deviceLock*/)
{
    std::cout << "Initializing simulation" << std::endl;

    // Register a device with name "test_device"
    // This will allow to send data to and from the transceiver function

    const auto deviceName = "test_device";

    this->registerDeviceNoLock(deviceName, new ${engine_name}JSONDeviceController(deviceName, this->_engineName));

    // Mark initialization as complete

    this->_initRunFlag = true;

    return "";
}

nlohmann::json ${engine_name}JSONServer::shutdown(const nlohmann::json &/*data*/)
{
    std::cout << "Shutting down simulation" << std::endl;

    // Request shutdown of the main loop

    this->_shutdownFlag = true;
    return "";
}

nlohmann::json ${engine_name}JSONServer::reset(EngineJSONServer::lock_t &/*deviceLock*/)
{
    std::cout << "Resetting simulation" << std::endl;
    return "";
}

// EOF
