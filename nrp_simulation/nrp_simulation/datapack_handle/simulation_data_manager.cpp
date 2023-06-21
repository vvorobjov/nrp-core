/*
 * NRP Core - Backend infrastructure to synchronize simulations
 *
 * Copyright 2020-2023 NRP Team
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * This project has received funding from the European Union’s Horizon 2020
 * Framework Programme for Research and Innovation under the Specific Grant
 * Agreement No. 945539 (Human Brain Project SGA3).
 */

#include "nrp_simulation/datapack_handle/simulation_data_manager.h"
#include "nrp_protobuf/dump.pb.h"
#include "nrp_json_engine_protocol/datapack_interfaces/json_datapack.h"


static void resetIsUpdateFlags(datapacks_set_t & dataPacks)
{
    for(auto &dataPack: dataPacks)
    {
        dataPack->resetIsUpdated();
    }
}


static void updateDataPackPool(datapacks_vector_t dataPacks, datapacks_set_t & pool)
{
    for(auto it = dataPacks.begin(); it != dataPacks.end(); it++)
    {
        pool.erase(*it);
        pool.insert(*it);
    }
}


void SimulationDataManager::pushToTrajectory(datapacks_vector_t dataPackVector)
{
    this->_trajectory.insert(this->_trajectory.end(), dataPackVector.begin(), dataPackVector.end());
}


void SimulationDataManager::clearTrajectory()
{
    this->_trajectory.clear();
}


const datapacks_vector_t & SimulationDataManager::getTrajectory() const
{
    return this->_trajectory;
}


void SimulationDataManager::setDoneFlag(bool doneFlag)
{
    this->_doneFlag = doneFlag;
}


bool SimulationDataManager::getDoneFlag() const
{
    return this->_doneFlag;
}


void SimulationDataManager::clear()
{
    this->_trajectory.clear();
    this->_externalPool.clear();
    this->_preprocessingPool.clear();
    this->_transceiverPool.clear();
    this->_enginePool.clear();
    this->_doneFlag = false;
}


void SimulationDataManager::updateExternalPool(datapacks_vector_t dataPacks)
{
    updateDataPackPool(dataPacks, this->_externalPool);
}


void SimulationDataManager::updatePreprocessingPool(datapacks_vector_t dataPacks)
{
    updateDataPackPool(dataPacks, this->_preprocessingPool);
}


void SimulationDataManager::updateTransceiverPool(datapacks_vector_t dataPacks)
{
    updateDataPackPool(dataPacks, this->_transceiverPool);
}


void SimulationDataManager::updateEnginePool(datapacks_vector_t dataPacks)
{
    for(auto it = dataPacks.begin(); it != dataPacks.end(); it++)
    {
        // If there's no datapack with the name in the cache - insert it
        // It there already is one - replace it, but only with a non-empty datapack

        const int count = this->_enginePool.count(*it);
        if(count == 0 || (count > 0 && !(*it)->isEmpty()))
        {
            this->_enginePool.erase(*it);
            this->_enginePool.insert(*it);
        }
    }
}


datapacks_set_t SimulationDataManager::getEngineDataPacks(const std::string & engineName) const
{
    datapacks_set_t dataPacks;

    for(auto dataPack = this->_transceiverPool.begin(); dataPack != this->_transceiverPool.end(); dataPack++)
    {
        if(dataPack->get()->id().EngineName == engineName)
        {
            dataPacks.insert(*dataPack);
        }
    }

    for(auto dataPack = this->_externalPool.begin(); dataPack != this->_externalPool.end(); dataPack++)
    {
        if(dataPack->get()->id().EngineName == engineName)
        {
            dataPacks.insert(*dataPack);
        }
    }

    return dataPacks;
}


datapacks_set_t SimulationDataManager::getTransceiverDataPacks() const
{
    datapacks_set_t dataPacks = this->_enginePool;
    dataPacks.insert(this->_preprocessingPool.begin(), this->_preprocessingPool.end());

    return dataPacks;
}


datapacks_set_t SimulationDataManager::getPreprocessingDataPacks() const
{
    return this->_enginePool;
}


datapacks_set_t SimulationDataManager::getStatusDataPacks() const
{
    datapacks_set_t dataPacks = this->_enginePool;
    dataPacks.insert(this->_preprocessingPool.begin(), this->_preprocessingPool.end());
    dataPacks.insert(this->_transceiverPool.begin(), this->_transceiverPool.end());

    return dataPacks;
}


void SimulationDataManager::startNewIteration()
{
    resetIsUpdateFlags(this->_enginePool);
    resetIsUpdateFlags(this->_preprocessingPool);
    resetIsUpdateFlags(this->_transceiverPool);
    resetIsUpdateFlags(this->_externalPool);
}

// EOF
