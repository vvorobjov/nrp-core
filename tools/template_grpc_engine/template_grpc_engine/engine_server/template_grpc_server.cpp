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

#include "${engine_name_lowercase}_grpc_engine/engine_server/${engine_name_lowercase}_grpc_server.h"
#include "${engine_name_lowercase}_grpc_engine/engine_server/${engine_name_lowercase}_grpc_device_controller.h"

${engine_name}GrpcServer::${engine_name}GrpcServer(const std::string &serverAddress,
                                     const std::string &engineName,
                                     const std::string &registrationAddress)
    : EngineGrpcServer(serverAddress, engineName, registrationAddress),
    _engineName(engineName)
{

}

SimulationTime ${engine_name}GrpcServer::runLoopStep(SimulationTime timeStep)
{
    static SimulationTime simulationTime = SimulationTime::zero();

    std::cout << "Running simulation step" << std::endl;

    simulationTime += timeStep;

    return simulationTime;
}

void ${engine_name}GrpcServer::initialize(const nlohmann::json &/*data*/, EngineGrpcServer::lock_t & /*deviceLock*/)
{
    std::cout << "Initializing simulation" << std::endl;

    const auto deviceName = "test_device";

    this->registerDeviceNoLock(deviceName, new ${engine_name}GrpcDeviceController(deviceName, this->_engineName));

    this->_initRunFlag = true;
}

void ${engine_name}GrpcServer::shutdown(const nlohmann::json &/*data*/)
{
    std::cout << "Shutting down simulation" << std::endl;
    this->_shutdownFlag = true;
}

void ${engine_name}GrpcServer::reset()
{
    std::cout << "Resetting simulation" << std::endl;
}

// EOF
