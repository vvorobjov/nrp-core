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

#include <gtest/gtest.h>

#include "nrp_simulation/datapack_handle/simulation_data_manager.h"
#include "nrp_general_library/datapack_interface/datapack.h"


/*!
 * \brief Helper function that generated a shared pointer to a DataPack with given parameters
 */
static std::shared_ptr<const DataPackInterface> generateDataPack(const std::string & name,
                                                                 const std::string & engineName,
                                                                 const std::string & type = "")
{
    return std::shared_ptr<const DataPackInterface>(new DataPackInterface(name, engineName, type));
}


/*!
 * \brief Helper function that populates various DataPack pools of the DataManager used in tests
 */
static void populateDataManager(SimulationDataManager & dataManager)
{
    // Populate the pool of DataPacks coming from the Transceiver Functions

    datapacks_vector_t tfDataPacks;
    tfDataPacks.push_back(generateDataPack("tf_datapack1", "engine_name1"));
    tfDataPacks.push_back(generateDataPack("tf_datapack2", "engine_name2"));
    dataManager.updateTransceiverPool(tfDataPacks);

    // Populate the pool of DataPacks from external sources

    datapacks_vector_t externalDataPacks;
    externalDataPacks.push_back(generateDataPack("ext_datapack1", "engine_name1"));
    externalDataPacks.push_back(generateDataPack("ext_datapack2", "engine_name2"));
    dataManager.updateExternalPool(externalDataPacks);

    // Populate the pool of DataPacks coming from the Preprocessing Functions

    datapacks_vector_t pfDataPacks;
    pfDataPacks.push_back(generateDataPack("pf_datapack1", "engine_name1"));
    pfDataPacks.push_back(generateDataPack("pf_datapack2", "engine_name2"));
    dataManager.updatePreprocessingPool(pfDataPacks);

    // Populate the pool of DataPacks from the Engines

    datapacks_vector_t engineDataPacks;
    engineDataPacks.push_back(generateDataPack("engine_datapack1", "engine_name1"));
    engineDataPacks.push_back(generateDataPack("engine_datapack2", "engine_name2"));
    dataManager.updateEnginePool(engineDataPacks);
}


TEST(SimulationDataManager, Comparator)
{
    DataPackPointerComparator comparator;

    // Test DataPacks with different names (same engine name)

    auto dataPack1 = generateDataPack("name1", "engine_name");
    auto dataPack2 = generateDataPack("name2", "engine_name");

    // comp(a, a) == false

    ASSERT_FALSE(comparator(dataPack1, dataPack1));

    // If comp(a,b) == true then comp(b,a) == false

    ASSERT_TRUE (comparator(dataPack1, dataPack2));
    ASSERT_FALSE(comparator(dataPack2, dataPack1));

    // Test DataPacks with different engine names (same name)

    auto dataPack3 = generateDataPack("name", "engine_name1");
    auto dataPack4 = generateDataPack("name", "engine_name2");

    // comp(a, a) == false

    ASSERT_FALSE(comparator(dataPack3, dataPack3));

    // If comp(a,b) == true then comp(b,a) == false

    ASSERT_TRUE (comparator(dataPack3, dataPack4));
    ASSERT_FALSE(comparator(dataPack4, dataPack3));
}


TEST(SimulationDataManager, GetEngineDataPacks)
{
    SimulationDataManager dataManager;
    populateDataManager(dataManager);

    // Retrieve a set of DataPacks that should be sent to engine 1
    // This should be a combination of external DataPacks and
    // DataPacks coming from the Transceiver Functions

    auto dataPacks = dataManager.getEngineDataPacks("engine_name1");
    ASSERT_EQ(dataPacks.size(), 2);
    ASSERT_EQ(dataPacks.count(DataPackIdentifier("tf_datapack1",  "engine_name1", "")), 1);
    ASSERT_EQ(dataPacks.count(DataPackIdentifier("ext_datapack1", "engine_name1", "")), 1);
}


TEST(SimulationDataManager, GetTransceiverDataPacks)
{
    SimulationDataManager dataManager;
    populateDataManager(dataManager);

    // Retrieve a set of DataPacks that should be accessible to Transceiver Functions
    // This should be a combination of DataPacks coming from the Engines and
    // DataPacks coming from the Preprocessing Functions

    auto dataPacks = dataManager.getTransceiverDataPacks();
    ASSERT_EQ(dataPacks.size(), 4);
    ASSERT_EQ(dataPacks.count(DataPackIdentifier("engine_datapack1", "engine_name1", "")), 1);
    ASSERT_EQ(dataPacks.count(DataPackIdentifier("engine_datapack2", "engine_name2", "")), 1);
    ASSERT_EQ(dataPacks.count(DataPackIdentifier("pf_datapack1",     "engine_name1", "")), 1);
    ASSERT_EQ(dataPacks.count(DataPackIdentifier("pf_datapack2",     "engine_name2", "")), 1);
}


TEST(SimulationDataManager, GetTransceiverDataPacksEmpty)
{
    SimulationDataManager dataManager;

    // Insert an empty DataPack into the Engine DataPacks pool
    // It should be included in the DataPacks for the Transceiver Functions

    datapacks_vector_t engineDataPacks;
    engineDataPacks.push_back(generateDataPack("engine_datapack1", "engine_name1"));
    dataManager.updateEnginePool(engineDataPacks);

    auto dataPacks = dataManager.getTransceiverDataPacks();
    ASSERT_TRUE(dataPacks.begin()->get()->isEmpty());

    // Insert a non-empty DataPack with the same ID into the Engine DataPacks pool
    // It should overwrite the previous DataPack

    engineDataPacks.clear();
    engineDataPacks.push_back(std::shared_ptr<const DataPackInterface>(new DataPack<int>("engine_datapack1", "engine_name1", new int(5))));
    dataManager.updateEnginePool(engineDataPacks);

    dataPacks = dataManager.getTransceiverDataPacks();
    ASSERT_FALSE(dataPacks.begin()->get()->isEmpty());

    // Try to insert an empty DataPack again
    // It should not overwrite the previously inserted non-empty DataPack

    engineDataPacks.clear();
    engineDataPacks.push_back(generateDataPack("engine_datapack1", "engine_name1"));
    dataManager.updateEnginePool(engineDataPacks);

    dataPacks = dataManager.getTransceiverDataPacks();
    ASSERT_FALSE(dataPacks.begin()->get()->isEmpty());
}


TEST(SimulationDataManager, GetPreprocessingDataPacks)
{
    SimulationDataManager dataManager;
    populateDataManager(dataManager);

    // Retrieve a set of DataPacks that should be accessible to Preprocessing Functions
    // The Preprocessing Functions should have access only to DataPacks coming from the Engines

    auto dataPacks = dataManager.getPreprocessingDataPacks();
    ASSERT_EQ(dataPacks.size(), 2);
    ASSERT_EQ(dataPacks.count(DataPackIdentifier("engine_datapack1", "engine_name1", "")), 1);
    ASSERT_EQ(dataPacks.count(DataPackIdentifier("engine_datapack2", "engine_name2", "")), 1);
}


TEST(SimulationDataManager, GetStatusDataPacks)
{
    SimulationDataManager dataManager;
    populateDataManager(dataManager);

    // Retrieve a set of DataPacks that should be accessible to Status Function
    // This should be a combination of DataPacks coming from the Engines,
    // from the Preprocessing Functions, and from the Transceiver Functions

    auto dataPacks = dataManager.getStatusDataPacks();
    ASSERT_EQ(dataPacks.size(), 6);
    ASSERT_EQ(dataPacks.count(DataPackIdentifier("engine_datapack1", "engine_name1", "")), 1);
    ASSERT_EQ(dataPacks.count(DataPackIdentifier("engine_datapack2", "engine_name2", "")), 1);
    ASSERT_EQ(dataPacks.count(DataPackIdentifier("pf_datapack1",     "engine_name1", "")), 1);
    ASSERT_EQ(dataPacks.count(DataPackIdentifier("pf_datapack2",     "engine_name2", "")), 1);
    ASSERT_EQ(dataPacks.count(DataPackIdentifier("tf_datapack1",     "engine_name1", "")), 1);
    ASSERT_EQ(dataPacks.count(DataPackIdentifier("tf_datapack2",     "engine_name2", "")), 1);
}


static void checkIsUpdatedFlags(const datapacks_set_t dataPacks, bool flagValue)
{
    for(auto & dataPack: dataPacks)
    {
        ASSERT_EQ(dataPack->isUpdated(), flagValue);
    }
}


TEST(SimulationDataManager, NewIteration)
{
    SimulationDataManager dataManager;
    populateDataManager(dataManager);

    // After creation, all DataPacks stored in the manager should have
    // the isUpdated flag set to true

    checkIsUpdatedFlags(dataManager.getStatusDataPacks(),        true);
    checkIsUpdatedFlags(dataManager.getPreprocessingDataPacks(), true);
    checkIsUpdatedFlags(dataManager.getTransceiverDataPacks(),   true);
    checkIsUpdatedFlags(dataManager.getStatusDataPacks(),        true);

    // Inform the manager that a new simulation iteration has started
    // This should reset the isUpdated flag of all DataPacks to false

    dataManager.startNewIteration();

    checkIsUpdatedFlags(dataManager.getStatusDataPacks(),        false);
    checkIsUpdatedFlags(dataManager.getPreprocessingDataPacks(), false);
    checkIsUpdatedFlags(dataManager.getTransceiverDataPacks(),   false);
    checkIsUpdatedFlags(dataManager.getStatusDataPacks(),        false);
}

// EOF
