//
// NRP Core - Backend infrastructure to synchronize simulations
//
// Copyright 2020-2023 NRP Team
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

#include <algorithm>
#include "nrp_event_loop/event_loop/event_loop_engine.h"
#include "nrp_general_library/utils/nrp_exceptions.h"

EventLoopEngine::EventLoopEngine(std::chrono::milliseconds timestep, std::chrono::milliseconds rtDeltaThres,
                                 size_t storeCapacity, bool doProcessLast,
                                 const nlohmann::json &engineConfig, EngineProtoWrapper* engineWrapper,
                                 bool delegateRTControl, bool logRTInfo, bool syncTimeRef,
                                 std::vector<std::string> publishDatapacks) :
        EventLoopInterface(timestep, rtDeltaThres, delegateRTControl, logRTInfo, syncTimeRef),
        _datapackPub(new EngineGrpc::DataPackMessage()),
        _storeCapacity(storeCapacity),
        _doProcessLast(doProcessLast),
        _datapackPubNames(publishDatapacks),
        _engineWrapper(engineWrapper),
        _engineConfig(engineConfig)
{ _isTimeSyncMaster = false; }

EventLoopEngine::~EventLoopEngine()
{ this->shutdown(); }

void EventLoopEngine::initializeCB()
{
    lock_t lock(this->_datapackLock);
    
    _mqttProxy = &(NRPMQTTProxy::getInstance());
    if(!_mqttProxy || !_mqttProxy->isConnected())
        throw NRPException::logCreate("EventLoopEngine failed to connect to MQTT. Ensure that the MQTT broker is running and check the engine configuration");

    auto stepConf = this->_engineConfig.at("EngineTimestep").get<float>();
    auto stepELE = fromSimulationTime<float, std::ratio<1>>(std::chrono::duration_cast<SimulationTime>(this->_timestep));
    if(stepConf != stepELE) {
        NRPLogger::info(
                "Engine Timestep parameter " + std::to_string(stepConf) +
                " overwritten to " + std::to_string(stepELE) +
                " to match EventLoop time step");
        this->_engineConfig["EngineTimestep"] = stepELE;
    }

    // TODO: quick hack to let engine wrapper know if it should handle rt execution itself, find a better way
    if(_delegateRTControl)
        this->_engineConfig["HandleRTControl"] = true;

    this->_engineWrapper->initialize(this->_engineConfig);

    using std::placeholders::_1;
    auto datapackNames = this->_engineWrapper->getNamesRegisteredDataPacks();
    for(const auto& dpName : datapackNames) {
        NRPLogger::info("Subscribing to topic: " + datapackTopicSet(dpName));
        _mqttProxy->subscribe(datapackTopicSet(dpName), std::bind(&EventLoopEngine::topic_callback, this, dpName, _1));
        _datapackStore.emplace(dpName, std::vector<EngineGrpc::DataPackMessage>());
        _datapackStore[dpName].reserve(_storeCapacity);
    }

    if(_datapackPubNames.empty())
        _datapackPubNames = datapackNames;
    else
        for(const auto& dpName : _datapackPubNames)
            if ( std::find(datapackNames.begin(), datapackNames.end(), dpName) == datapackNames.end() )
                throw NRPException::logCreate("EventLoopEngine was configured to publish datapack \"" + dpName + 
                            "\" but it is not registered in the Engine.");
}

void EventLoopEngine::runLoopCB()
{
    // Updates clock
    if(_delegateRTControl)
        this->_engineWrapper->rtClockUpdate(std::chrono::duration_cast<SimulationTime>(_currentTime));

    // Set datapacks
    {
        lock_t lock(this->_datapackLock);
        for (auto &dp: _datapackStore) {
            if (_doProcessLast && !dp.second.empty())
                this->_engineWrapper->setDataPack(dp.second.back());
            else {
                for (const auto &msg: dp.second)
                    this->_engineWrapper->setDataPack(msg);
            }

            dp.second.clear();
        }
    }
    // Advance Engine
    this->_engineWrapper->runLoopStep(std::chrono::duration_cast<SimulationTime>(this->_timestep));
    // Get and publish datapacks
    for(const auto& dpName : _datapackPubNames) {
        _datapackPub->clear_datapackid();
        _datapackPub->clear_data();
        if(this->_engineWrapper->getDataPack(dpName, _datapackPub.get()))
            _mqttProxy->publish(datapackTopicGet(dpName), _datapackPub->SerializeAsString());
    }

}

void EventLoopEngine::shutdownCB()
{
    lock_t lock(this->_datapackLock);
    this->_engineWrapper->shutdown();
}

void EventLoopEngine::realtimeDeltaCB(std::chrono::milliseconds deviation)
{ this->_engineWrapper->handleRTDelta(deviation); }

void EventLoopEngine::topic_callback(const std::string& dpName, const std::string& msgStr)
{
    lock_t lock(this->_datapackLock);

    auto infoMsgPrefix = "Engine " + this->_engineWrapper->getEngineName() + " received a new msg. ";
    EngineGrpc::DataPackMessage m;
    if(!m.ParseFromString(msgStr)) {
        NRPLogger::warn(infoMsgPrefix + "But it could not be unpacked to EngineGrpc::DataPackMessage.");
        return;
    }

    const auto& datapackName = m.datapackid().datapackname();

    if(!this->isRunningNotAsync()) {
        NRPLogger::debug(infoMsgPrefix + "But the engine is not running, the msg will be discarded.");
        return;
    }
    else if(datapackName != dpName) {
        NRPLogger::warn(infoMsgPrefix + "Received datapack " + datapackName + " in topic " + dpName +
        ", it will be discarded.");
        return;
    }
    else if(_datapackStore[datapackName].size() >=  _datapackStore[datapackName].capacity()) {
        NRPLogger::debug(infoMsgPrefix + "But datapack queue is full, dropping msg.");
        return;
    }

    _datapackStore[datapackName].push_back(std::move(m));
}

std::string EventLoopEngine::datapackTopicGet(const std::string& dpName)
{ return  this->_engineWrapper->getEngineName() + "/get/" + dpName; }

std::string EventLoopEngine::datapackTopicSet(const std::string& dpName)
{ return  this->_engineWrapper->getEngineName() + "/set/" + dpName; }

std::string EventLoopEngine::getLoggerBaseName()
{ return this->_engineWrapper->getEngineName(); }

