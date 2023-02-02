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

#include "nrp_general_library/utils/json_schema_utils.h"
#include "nrp_simulation/simulation/simulation_manager.h"
#include "tests/test_files/simulation_manager_test.h"
#include "tests/test_env_cmake.h"

using namespace testing;


TEST(SimulationManagerTest, StateTransitions)
{
    jsonSharedPtr config(new nlohmann::json(json_utils::parseJSONFile(TEST_SIM_SIMPLE_CONFIG_FILE)));
    TestSimManager manager(config);

    EXPECT_CALL(manager, initializeCB).Times(2)
            .WillOnce(
            Invoke([] () { throw std::exception(); }))
            .WillRepeatedly(Return());
    EXPECT_CALL(manager, resetCB).Times(2)
            .WillOnce(Return(false))
            .WillRepeatedly(Return(true));
    EXPECT_CALL(manager, stopCB).Times(1);
    EXPECT_CALL(manager, runCB).Times(1).WillRepeatedly(Return(true));;
    EXPECT_CALL(manager, runUntilDoneOrTimeoutCB).Times(1).WillRepeatedly(Return(true));
    EXPECT_CALL(manager, shutdownCB).Times(2);

    //// FAILED
    ASSERT_EQ(manager.currentState(), SimulationManager::SimState::Created);
    ASSERT_EQ(manager.initializeSimulation().currentState, SimulationManager::SimState::Failed);
    // Invalid actions
    ASSERT_THROW(manager.resetSimulation(), std::logic_error);
    ASSERT_THROW(manager.stopSimulation(), std::logic_error);
    ASSERT_THROW(manager.runSimulation(1), std::logic_error);
    ASSERT_THROW(manager.runSimulationUntilDoneOrTimeout(), std::logic_error);
    ASSERT_THROW(manager.initializeSimulation(), std::logic_error);
    // Current state
    ASSERT_EQ(manager.currentState(), SimulationManager::SimState::Failed);
    // Valid actions
    ASSERT_NO_THROW(manager.shutdownSimulation());

    //// CREATED
    // Current state
    ASSERT_EQ(manager.currentState(), SimulationManager::SimState::Created);
    // Invalid actions
    ASSERT_THROW(manager.resetSimulation(), std::logic_error);
    ASSERT_THROW(manager.stopSimulation(), std::logic_error);
    ASSERT_THROW(manager.runSimulation(1), std::logic_error);
    ASSERT_THROW(manager.runSimulationUntilDoneOrTimeout(), std::logic_error);
    ASSERT_THROW(manager.shutdownSimulation(), std::logic_error);
    // Current state
    ASSERT_EQ(manager.currentState(), SimulationManager::SimState::Created);
    // Valid actions
    ASSERT_NO_THROW(manager.initializeSimulation());

    //// INITIALIZED / STOPPED
    // Current state
    ASSERT_EQ(manager.currentState(), SimulationManager::SimState::Initialized);
    // Invalid actions
    ASSERT_THROW(manager.initializeSimulation(), std::logic_error);
    // Valid actions
    ASSERT_NO_THROW(manager.runSimulation(1));
    ASSERT_NO_THROW(manager.runSimulationUntilDoneOrTimeout());
    ASSERT_NO_THROW(manager.stopSimulation());
    ASSERT_NO_THROW(manager.resetSimulation());
    ASSERT_EQ(manager.currentState(), SimulationManager::SimState::Stopped);
    // Reset succeeds
    manager.resetSimulation();
    ASSERT_EQ(manager.currentState(), SimulationManager::SimState::Initialized);
    // Shutdown
    ASSERT_NO_THROW(manager.shutdownSimulation());
    ASSERT_EQ(manager.currentState(), SimulationManager::SimState::Created);
}