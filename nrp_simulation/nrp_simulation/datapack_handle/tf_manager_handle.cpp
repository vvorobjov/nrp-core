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

#include "nrp_simulation/datapack_handle/tf_manager_handle.h"


void TFManagerHandle::loadDataPackFunctions(const jsonSharedPtr &simConfig)
{
    // Load all DataPack Processing Functions specified in the config

    const auto &dataPackConfigs = simConfig->at("DataPackProcessingFunctions");
    for(const auto &dataPackConfig : dataPackConfigs)
    {
        bool isTfActive = (!dataPackConfig.contains("IsActive") || dataPackConfig.at("IsActive"));

        if(isTfActive)
        {
            NRPLogger::debug("Loading transceiver function {}", dataPackConfig.dump());
            this->_functionManager.loadDataPackFunction(dataPackConfig.at("Name"), dataPackConfig.at("FileName"));
        }
        else
        {
            NRPLogger::debug("Omitting transceiver function {}, IsActive flag set to 'false'", dataPackConfig.dump());
        }
    }
}


void TFManagerHandle::loadStatusFunction(const jsonSharedPtr &simConfig)
{
    // Load status function, if it's defined

    const auto &statusFunctionConfig = simConfig->find("StatusFunction");
    if(statusFunctionConfig != simConfig->end())
    {
        bool isStatusFunctionActive = (!statusFunctionConfig->contains("IsActive") || statusFunctionConfig->at("IsActive"));

        if(isStatusFunctionActive)
        {
            NRPLogger::debug("Loading status function {}", statusFunctionConfig->dump());
            this->_functionManager.loadStatusFunction(statusFunctionConfig->at("Name"), statusFunctionConfig->at("FileName"));
        }
        else
        {
            NRPLogger::debug("Omitting status function {}, IsActive flag set to 'false'", statusFunctionConfig->dump());
        }
    }
}


void TFManagerHandle::init(const jsonSharedPtr &simConfig, const engine_interfaces_t &/*engines*/)
{
    NRP_LOGGER_TRACE("{} called", __FUNCTION__);

    TransceiverDataPackInterface::setTFInterpreter(&(this->_functionManager));

    auto DataPackPassingPolicy = ((*simConfig)["DataPackPassingPolicy"] == "value") ? PASS_BY_VALUE : PASS_BY_REFERENCE;
    this->_functionManager.setDataPackPassingPolicy(DataPackPassingPolicy);

    this->loadDataPackFunctions(simConfig);
    this->loadStatusFunction(simConfig);
}

void TFManagerHandle::postEngineActivityHelper(const std::vector<EngineClientInterfaceSharedPtr> &engines)
{
    const auto requestedDataPackIDs = this->_functionManager.getRequestedDataPackIDs();

    for(auto &engine : engines)
    {
        try
        {
            auto dataPacks = engine->getDataPacksFromEngine(requestedDataPackIDs);
            this->_simulationDataManager->pushToTrajectory(dataPacks);
        }
        catch(std::exception &e)
        {
            throw NRPException::logCreate(e, "Failed to get datapacks from engine \"" + engine->engineName() + "\"");
        }
    }
}

void TFManagerHandle::postEngineInit(const std::vector<EngineClientInterfaceSharedPtr> &engines)
{
    NRP_LOGGER_TRACE("{} called", __FUNCTION__);
    this->postEngineActivityHelper(engines);
}

void TFManagerHandle::preEngineReset(const std::vector<EngineClientInterfaceSharedPtr> &engines)
{
    NRP_LOGGER_TRACE("{} called", __FUNCTION__);

    for(const auto &engine : engines)
    {
        try
        {
            auto dataPacks = this->_simulationDataManager->getEngineDataPacks(engine->engineName());
            engine->sendDataPacksToEngine(dataPacks);
            // Prevents "pushing back" the same DataPacks to the main script during postEngineReset()
            this->_simulationDataManager->clear();
        }
        catch(std::exception &e)
        {
            throw NRPException::logCreate(e, "Failed to send datapacks to engine \"" + engine->engineName() + "\"");
        }
    }
}

void TFManagerHandle::postEngineReset(const std::vector<EngineClientInterfaceSharedPtr> &engines)
{
    NRP_LOGGER_TRACE("{} called", __FUNCTION__);
    this->postEngineActivityHelper(engines);
}

void TFManagerHandle::updateDataPacksFromEngines(const std::vector<EngineClientInterfaceSharedPtr> &engines)
{
    NRP_LOGGER_TRACE("{} called", __FUNCTION__);

    const auto requestedDataPackIDs = this->_functionManager.getRequestedDataPackIDs();

    for(auto &engine : engines)
    {
        try
        {
            auto dataPacks = engine->getDataPacksFromEngine(requestedDataPackIDs);
            this->_simulationDataManager->updateEnginePool(dataPacks);
        }
        catch(std::exception &e)
        {
            throw NRPException::logCreate(e, "Failed to get datapacks from engine \"" + engine->engineName() + "\"");
        }
    }
}

void TFManagerHandle::compute(const std::vector<EngineClientInterfaceSharedPtr> &engines)
{
    NRP_LOGGER_TRACE("{} called", __FUNCTION__);

    this->_functionManager.setSimulationTime(this->_simulationTime);
    this->_functionManager.setSimulationIteration(this->_simulationIteration);

    executePreprocessingFunctions(this->_functionManager, engines, this->_simulationDataManager->getPreprocessingDataPacks());
    executeTransceiverFunctions  (this->_functionManager, engines, this->_simulationDataManager->getTransceiverDataPacks());

    auto statusTuple = this->_functionManager.executeStatusFunction(this->_simulationDataManager->getStatusDataPacks());

    // Extract the 'done' flag from the returned tuple

    this->_simulationDataManager->setDoneFlag(std::get<0>(statusTuple));

    // Extract the vector of DataPack proxy objects and push them to the trajectory manager

    this->_simulationDataManager->pushToTrajectory(std::get<1>(statusTuple));
}

void TFManagerHandle::sendDataPacksToEngines(const std::vector<EngineClientInterfaceSharedPtr> &engines)
{
    NRP_LOGGER_TRACE("{} called", __FUNCTION__);

    for(const auto &engine : engines)
    {
        try
        {
            auto dataPacks = this->_simulationDataManager->getEngineDataPacks(engine->engineName());
            engine->sendDataPacksToEngine(dataPacks);
        }
        catch(std::exception &e)
        {
            throw NRPException::logCreate(e, "Failed to send datapacks to engine \"" + engine->engineName() + "\"");
        }
    }

    this->_simulationDataManager->startNewIteration();
}

void TFManagerHandle::executePreprocessingFunctions(FunctionManager &tfManager,
                                                    const std::vector<EngineClientInterfaceSharedPtr> &engines,
                                                    datapacks_set_t dataPacks)
{
    NRP_LOGGER_TRACE("{} called", __FUNCTION__);

    for(auto &engine : engines)
    {
        auto results = tfManager.executePreprocessingFunctions(engine->engineName(), dataPacks);
        this->_simulationDataManager->updatePreprocessingPool(results);
    }
}

void TFManagerHandle::executeTransceiverFunctions(FunctionManager &tfManager,
                                                  const std::vector<EngineClientInterfaceSharedPtr> &engines,
                                                  datapacks_set_t dataPacks)
{
    for(const auto &engine : engines)
    {
        auto results = tfManager.executeTransceiverFunctions(engine->engineName(), dataPacks);
        this->_simulationDataManager->updateTransceiverPool(results);
    }
}
