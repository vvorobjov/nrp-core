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

void TFManagerHandle::init(const jsonSharedPtr &simConfig, const engine_interfaces_t &engines)
{
    NRP_LOGGER_TRACE("{} called", __FUNCTION__);

    // Setup engine datapacks and interpreter
    TransceiverFunctionInterpreter::engines_datapacks_t engineDevs;
    for(const auto &engine : engines)
    {
        NRPLogger::debug("Adding {} to TransceiverFunctionManager", engine->engineName());
        engineDevs.emplace(engine->engineName(), &(engine->getCachedDataPacks()));
    }

    this->_tfManager.getInterpreter().setEngineDataPacks(std::move(engineDevs));

    TransceiverDataPackInterface::setTFInterpreter(&(this->_tfManager.getInterpreter()));

    // Load all transceiver functions specified in the config
    const auto &transceiverFunctions = simConfig->at("DataPackProcessingFunctions");
    for(const auto &tf : transceiverFunctions)
    {
        NRPLogger::debug("Adding transceiver function {}", tf.dump());
        this->_tfManager.loadTF(tf);
    }
}

void TFManagerHandle::updateDataPacksFromEngines(const std::vector<EngineClientInterfaceSharedPtr> &engines)
{
    NRP_LOGGER_TRACE("{} called", __FUNCTION__);

    const auto requestedDataPackIDs = this->_tfManager.updateRequestedDataPackIDs();
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

void TFManagerHandle::compute(const std::vector<EngineClientInterfaceSharedPtr> &engines)
{
    NRP_LOGGER_TRACE("{} called", __FUNCTION__);

    executePreprocessingFunctions(this->_tfManager, engines);
    this->_tf_results = executeTransceiverFunctions(this->_tfManager, engines);
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

void TFManagerHandle::executePreprocessingFunctions(TransceiverFunctionManager &tfManager, const std::vector<EngineClientInterfaceSharedPtr> &engines)
{
    NRP_LOGGER_TRACE("{} called", __FUNCTION__);

    for (auto &engine : engines) {
        // Execute all preprocessing functions for this engine

        auto results = tfManager.executeActiveLinkedPFs(engine->engineName());

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

TransceiverFunctionSortedResults TFManagerHandle::executeTransceiverFunctions(TransceiverFunctionManager &tfManager, const std::vector<EngineClientInterfaceSharedPtr> &engines)
{
    TransceiverFunctionSortedResults results;
    for (const auto &engine : engines) {
        auto curResults = tfManager.executeActiveLinkedTFs(engine->engineName());
        results.addResults(curResults);
    }

    return results;
}
