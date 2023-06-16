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
// This project has received funding from the European Union�s Horizon 2020
// Framework Programme for Research and Innovation under the Specific Grant
// Agreement No. 945539 (Human Brain Project SGA3).
//

#include "datatransfer_grpc_engine/engine_server/datatransfer_grpc_server.h"
#include "datatransfer_grpc_engine/engine_server/stream_datapack_controller.h"

#include "nrp_general_library/utils/time_utils.h"

int DataTransferEngine::_iteration = 0;
SimulationTime DataTransferEngine::_simulationTime = SimulationTime::zero();

DataTransferEngine::DataTransferEngine(const std::string &engineName,
                                     const std::string &protobufPluginsPath,
                                     const nlohmann::json &protobufPlugins)
    : EngineProtoWrapper(engineName, protobufPluginsPath, protobufPlugins),
    _engineName(engineName)
{
    _dataPacksNames.clear();
}

SimulationTime DataTransferEngine::runLoopStep(SimulationTime timeStep)
{
    NRP_LOGGER_TRACE("DataTransferEngine::runLoopStep called");

    _iteration++;

    this->_simulationTime += timeStep;
    
#ifdef MQTT_ON
    if (_mqttClient->isConnected()){
         _mqttClient->publish(this->_mqttBase + "/time", std::to_string(fromSimulationTime<float, std::ratio<1>>(this->_simulationTime)));
    }
#endif

    return _simulationTime;
}

void DataTransferEngine::initialize(const nlohmann::json &data)
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

    // Topic: MQTT_BASE/simulationID
    this->_mqttBase = std::string(MQTT_BASE) + std::string("/");
    if (data.contains("MQTTPrefix")){
        // Topic: MQTTPrefix/MQTT_BASE/simulationID
        this->_mqttBase = std::string(data.at("MQTTPrefix")) + std::string("/") + this->_mqttBase;
    }
    this->_mqttBase += std::string(data.at("simulationID"));

    if(!mqttConnected)
    {
        NRPLogger::warn("NRPCoreSim is not connected to MQTT, Network data streaming will be disabled. Check your experiment configuration");
    }
    else {      
        _mqttClient->publish(this->_mqttBase + "/welcome", "NRP-core is connected!", true);
    }
#else
    NRPLogger::info("No MQTT support. Network streaming disabled.");
#endif

    auto &dumps = data.at("dumps");
    std::string dataDir = std::string(data.at("dataDirectory")) + "/" + timeStamp;

    this->_handleDataPackMessage = data.at("streamDataPackMessage") && mqttConnected;

    for(auto &dump : dumps){
        const auto datapackName = dump.at("name");
        _dataPacksNames.push_back(datapackName);
        const auto netDump = dump.at("network") && mqttConnected;
        const auto fileDump = dump.at("file");
        if (fileDump && !netDump){
            this->registerDataPack(datapackName, new StreamDataPackController(datapackName, this->_engineName, this->_protoOps, dataDir));
        }
#ifdef MQTT_ON
        else if (fileDump && netDump){
            this->registerDataPack(datapackName, new StreamDataPackController(datapackName, this->_engineName, this->_protoOps, dataDir, _mqttClient, this->_mqttBase));
        }
        else if (!fileDump && netDump){
            this->registerDataPack(datapackName, new StreamDataPackController(datapackName, this->_engineName, this->_protoOps, _mqttClient, this->_mqttBase));
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

void DataTransferEngine::shutdown()
{
    NRPLogger::debug("Shutting down simulation");

#ifdef MQTT_ON
    try{
        if (_mqttClient->isConnected()){
            _mqttClient->publish(this->_mqttBase + "/welcome", "Bye! NRP-core is disconnecting!", true);
            _mqttClient->clearRetained();
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

void DataTransferEngine::reset()
{
    NRPLogger::debug("Resetting simulation");
    this->_simulationTime = SimulationTime::zero();

    for (const auto& datapackName: this->_dataPacksNames){
        auto controller = dynamic_cast<StreamDataPackController *>(this->getDataPackController(datapackName));
        if (controller){
            controller->resetSinks();
            NRPLogger::debug("The data-transfer streams of the DataPack '{}' were reset.", datapackName);
        }
        else {
            NRPLogger::debug("Data-transfer DataPack '{}' doesn't have associated data streams to be reset.");
        }
    }
}

#ifdef MQTT_ON
bool DataTransferEngine::setNRPMQTTClient(std::shared_ptr< NRPMQTTClient > client)
{
    _mqttClient = client;

    return _mqttClient->isConnected();
}
#endif

// EOF
