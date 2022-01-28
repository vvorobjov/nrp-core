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

#include "datatransfer_grpc_engine/engine_server/datatransfer_grpc_server.h"
#include "datatransfer_grpc_engine/engine_server/stream_datapack_controller.h"

int DataTransferGrpcServer::_iteration = 0;
SimulationTime DataTransferGrpcServer::_simulationTime = SimulationTime::zero();

DataTransferGrpcServer::DataTransferGrpcServer(const std::string &serverAddress,
                                     const std::string &engineName,
                                     const std::string &registrationAddress)
    : EngineGrpcServer(serverAddress, engineName, registrationAddress),
    _engineName(engineName)
{
}

SimulationTime DataTransferGrpcServer::runLoopStep(SimulationTime timeStep)
{
    NRP_LOGGER_TRACE("DataTransferGrpcServer::runLoopStep called");

    _iteration++;

    this->_simulationTime += timeStep;

    return _simulationTime;
}

void DataTransferGrpcServer::initialize(const nlohmann::json &data, EngineGrpcServer::lock_t & /*datapackLock*/)
{
    NRPLogger::info("Initializing Data Transfer Engine");

    const auto t = std::time(nullptr);
    const auto tm = *std::localtime(&t);

    std::ostringstream oss;
    oss << std::put_time(&tm, "/%Y%m%d-%H%M%S-") << getpid();

    std::string timeStamp = oss.str();

    bool mqttConnected = false;
#ifdef MQTT_ON
    NRPLogger::info("Connecting to MQTT broker.");

    _mqttClient = std::make_shared<mqtt::async_client>(std::string(data.at("MQTTBroker")), "NRP-core-client");

    try{
        _mqttClient->connect()->wait();
        mqtt::topic top(*_mqttClient, "nrp/welcome", 1);
        top.publish("NRP-core is connected!");
        mqttConnected = true;
        NRPLogger::info("Connection to MQTT broker is established.");
        NRPLogger::debug("MQTT broker address: {}", _mqttClient->get_server_uri());
    }
    catch(std::exception &e)
    {
        NRPLogger::critical("Connection to MQTT broker is failing! Network data streaming is unavailable.");
    }
#else
    NRPLogger::info("No MQTT support. Network streaming disabled.");
#endif

    auto &dumps = data.at("dumps");
    std::string dataDir = std::string(data.at("dataDirectory")) + timeStamp;

    for(auto &dump : dumps){
        const auto datapackName = dump.at("name");
        const auto netDump = dump.at("network") && mqttConnected;
        const auto fileDump = dump.at("file");
        if (fileDump && !netDump){
            this->registerDataPackNoLock(datapackName, new StreamDataPackController(datapackName, this->_engineName, dataDir));
        }
#ifdef MQTT_ON
        else if (fileDump && netDump){
            this->registerDataPackNoLock(datapackName, new StreamDataPackController(datapackName, this->_engineName, dataDir, _mqttClient));
        }
        else if (!fileDump && netDump){
            this->registerDataPackNoLock(datapackName, new StreamDataPackController(datapackName, this->_engineName, _mqttClient));
        }
#endif
        else {
            NRPLogger::warn("No eligible stream destination was defined for datapack {}.", datapackName);
            continue;
        }
        NRPLogger::info("DataPack {} dump was added", datapackName);
    }

    this->_initRunFlag = true;
}

void DataTransferGrpcServer::shutdown(const nlohmann::json &/*data*/)
{
    NRPLogger::debug("Shutting down simulation");

#ifdef MQTT_ON
    try{
        if (_mqttClient->is_connected()){
            mqtt::topic top(*_mqttClient, "nrp/welcome", 1);
            top.publish("Bye! NRP-core is shutting down!");
            _mqttClient->disconnect()->wait();
        }
    }
    catch(std::exception &e)
    {
        NRPLogger::error("Couldn't gracefully disconnect from MQTT broker.");
    }
#endif

    this->_shutdownFlag = true;
}

void DataTransferGrpcServer::reset()
{
    NRPLogger::debug("Resetting simulation");
}

// EOF
