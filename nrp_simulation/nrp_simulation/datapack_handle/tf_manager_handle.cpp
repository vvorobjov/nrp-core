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


void TFManagerHandle::init(const jsonSharedPtr &simConfig, const engine_interfaces_t &engines)
{
    NRP_LOGGER_TRACE("{} called", __FUNCTION__);

    // Setup engine datapacks and interpreter
    FunctionManager::engines_datapacks_t engineDevs;
    for(const auto &engine : engines)
    {
        NRPLogger::debug("Adding {} to FunctionManager", engine->engineName());
        engineDevs.emplace(engine->engineName(), &(engine->getCachedDataPacks()));
    }

    this->_functionManager.setEngineDataPacks(std::move(engineDevs));

    TransceiverDataPackInterface::setTFInterpreter(&(this->_functionManager));

    this->loadDataPackFunctions(simConfig);
    this->loadStatusFunction(simConfig);
}

void TFManagerHandle::updateDataPacksFromEngines(const std::vector<EngineClientInterfaceSharedPtr> &engines)
{
    NRP_LOGGER_TRACE("{} called", __FUNCTION__);

    const auto requestedDataPackIDs = this->_functionManager.updateRequestedDataPackIDs();
    try
    {
        for(auto &engine : engines)
        {
            engine->updateDataPacksFromEngine(requestedDataPackIDs);
        }
    }
    catch(std::exception &)
    {
        // TODO: Handle failure on datapack retrieval
        throw;
    }
}

void TFManagerHandle::compute(const std::vector<EngineClientInterfaceSharedPtr> &engines, const nlohmann::json & clientData)
{
    NRP_LOGGER_TRACE("{} called", __FUNCTION__);

    executePreprocessingFunctions(this->_functionManager, engines);
    this->_tf_results = executeTransceiverFunctions(this->_functionManager, engines);

    auto statusTuple = this->_functionManager.executeStatusFunction(clientData);

    // Extract the JSON status object from the returned tuple

    auto statusJson = std::move(std::get<0>(statusTuple));

    // Extract DataPacks from the status function

    this->_tf_results.addResults(std::move(std::get<1>(statusTuple)));

    this->_status = statusJson ? statusJson->dump() : "";
}

void TFManagerHandle::sendDataPacksToEngines(const std::vector<EngineClientInterfaceSharedPtr> &engines)
{
    NRP_LOGGER_TRACE("{} called", __FUNCTION__);

    for(const auto &engine : engines)
    {
        try
        {
            // Find corresponding datapacks
            const auto interfaceResultIterator = this->_tf_results.find(engine->engineName());
            if(interfaceResultIterator != this->_tf_results.end())
                engine->sendDataPacksToEngine(interfaceResultIterator->second);
        }
        catch(std::exception &e)
        {
            throw NRPException::logCreate(e, "Failed to send datapacks to engine \"" + engine->engineName() + "\"");
        }
    }
}

void TFManagerHandle::executePreprocessingFunctions(FunctionManager &tfManager, const std::vector<EngineClientInterfaceSharedPtr> &engines)
{
    NRP_LOGGER_TRACE("{} called", __FUNCTION__);

    for (auto &engine : engines) {
        // Execute all preprocessing functions for this engine

        auto results = tfManager.executePreprocessingFunctions(engine->engineName());

        // Extract datapacks from the function results
        // The datapacks are stack objects, but we want to store pointers to them in engines cache
        // We have to convert them into heap-allocated objects

        EngineClientInterface::datapacks_set_t datapacksHeap;
        for (const auto &result : results) {
            for (const auto &datapack : result.DataPacks) {
                datapacksHeap.emplace(datapack->moveToSharedPtr());
            }
        }

        // Store pointers to datapacks from preprocessing functions in the engines cache

        engine->updateCachedDataPacks(std::move(datapacksHeap));
    }
}

TransceiverFunctionSortedResults TFManagerHandle::executeTransceiverFunctions(FunctionManager &tfManager, const std::vector<EngineClientInterfaceSharedPtr> &engines)
{
    TransceiverFunctionSortedResults results;
    for (const auto &engine : engines) {
        auto curResults = tfManager.executeTransceiverFunctions(engine->engineName());
        results.addResults(curResults);

        // Extract datapacks from the function results
        // The datapacks are stack objects, but we want to store pointers to them in engines cache
        // We have to convert them into heap-allocated objects

        // TODO Review as part of NRRPLT-8589
        /*EngineClientInterface::datapacks_set_t datapacksHeap;
        for (const auto &result : curResults) {
            for (const auto &datapack : result.DataPacks) {
                // moveToSharedPtr 'steals' the data stored in the DataPack.
                // We have to clone it so that the data can still be sent to the engine.
                auto clonedDatapack = datapack->clone();
                datapacksHeap.emplace(clonedDatapack->moveToSharedPtr());
            }
        }

        // Store pointers to datapacks from preprocessing functions in the engines cache

        engine->updateCachedDataPacks(std::move(datapacksHeap));*/
    }

    return results;
}
