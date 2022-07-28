/* * NRP Core - Backend infrastructure to synchronize simulations
 *
 * Copyright 2020-2021 NRP Team
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

#ifndef SIMULATION_MANAGER_TEST_H
#define SIMULATION_MANAGER_TEST_H

#include <gmock/gmock.h>
#include "nrp_simulation/simulation/simulation_manager.h"

using namespace testing;

/*!
 * \brief Implementation of SimulationManager for tests
 */
class TestSimManager
        : public SimulationManager
{
    public:

        TestSimManager(const jsonSharedPtr &simulationConfig) :
                SimulationManager(simulationConfig)
        {}

        const std::string & getStatus() override
        {
            return this->_status;
        }

    MOCK_METHOD(void, initializeCB, (), (override));
    MOCK_METHOD(bool, resetCB, (), (override));
    MOCK_METHOD(void, stopCB, (), (override));
    MOCK_METHOD(bool, runUntilTimeOutCB, (), (override));
    MOCK_METHOD(bool, runCB, (unsigned numIterations), (override));
    MOCK_METHOD(void, shutdownCB, (), (override));

    std::string _status = "";
};

#endif
