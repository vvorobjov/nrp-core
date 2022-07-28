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

#include "nrp_general_library/utils/time_utils.h"

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

    std::string timeStamp = getTimestamp();

    bool mqttConnected = false;
#ifdef MQTT_ON
    nlohmann::json mqtt_config;
    // If client doesn't exist yet, then create one
    if (!_mqttClient){
        mqtt_config["MQTTBroker"] = data.at("MQTTBroker");
        mqtt_config["ClientName"] = _engineName;
        _mqttClient = std::make_shared<NRPMQTTClient>(mqtt_config);
    }
    else
    {
        NRPLogger::info("Using preset MQTT client connection");
    }
    mqttConnected = _mqttClient->isConnected();
    if(!mqttConnected)
    {
        NRPLogger::warn("NRPCoreSim is not connected to MQTT, Network data streaming will be disabled. Check your experiment configuration");
    }
    else {      
        _mqttClient->publish(MQTT_WELCOME, "NRP-core is connected!");  
    }
#else
    NRPLogger::info("No MQTT support. Network streaming disabled.");
#endif

    auto &dumps = data.at("dumps");
    std::string dataDir = std::string(data.at("dataDirectory")) + "/" + timeStamp;

    this->_handleDataPackMessage = data.at("streamDataPackMessage") && mqttConnected;

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
        if (_mqttClient->isConnected()){
            _mqttClient->publish("nrp/welcome", "Bye! NRP-core is disconnecting!");
            _mqttClient->disconnect();
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

#ifdef MQTT_ON
bool DataTransferGrpcServer::setNRPMQTTClient(std::shared_ptr< NRPMQTTClient > client)
{
    _mqttClient = client;

    return _mqttClient->isConnected();
}
#endif

// EOF
