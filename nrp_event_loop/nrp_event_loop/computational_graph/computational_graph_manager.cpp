/* * NRP Core - Backend infrastructure to synchronize simulations
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

#include "nrp_event_loop/computational_graph/computational_graph_manager.h"

std::unique_ptr<ComputationalGraphManager> ComputationalGraphManager::_instance = nullptr;

ComputationalGraphManager::~ComputationalGraphManager()
{
    this->clear();
}

ComputationalGraphManager &ComputationalGraphManager::getInstance()
{
    return *(ComputationalGraphManager::_instance);
}

ComputationalGraphManager &ComputationalGraphManager::resetInstance()
{
    ComputationalGraphManager::_instance.reset(new ComputationalGraphManager());
    return ComputationalGraphManager::getInstance();
}