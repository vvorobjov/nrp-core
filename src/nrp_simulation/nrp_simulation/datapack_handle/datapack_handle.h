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

#ifndef DATAPACK_HANDLE_H
#define DATAPACK_HANDLE_H

#include "nrp_general_library/engine_interfaces/engine_client_interface.h"
#include "nrp_general_library/utils/json_schema_utils.h"
#include "nrp_general_library/utils/time_utils.h"
#include "nrp_general_library/transceiver_function/function_manager.h"
#include "nrp_simulation/datapack_handle/simulation_data_manager.h"

/*!
 * \brief Helper class for FTILoop encapsulating the datapack operations between Engines in a simulation loop
 */
class DataPackProcessor {

public:

    using engine_interfaces_t = std::vector<EngineClientInterfaceSharedPtr>;

    DataPackProcessor() = delete;
    DataPackProcessor(SimulationDataManager * simulationDataManager):
        _simulationDataManager(simulationDataManager)
    {}

    virtual ~DataPackProcessor() = default;

    /*!
     * \brief Initializes the handler
     *
     * \param simConfig json object containing configuration information to initialize the handler
     * \param engines list of Engine clients participating in the simulation
     */
    virtual void init(const jsonSharedPtr &simConfig, const engine_interfaces_t &engines) = 0;

    /*!
     * \brief Request datapacks from engines
     *
     * \param engines Engines that are been synchronize in the current loop
     */
    virtual void updateDataPacksFromEngines(const std::vector<EngineClientInterfaceSharedPtr> &engines) = 0;

    /*!
     * \brief Perform computations on datapacks
     *
     * \param engines Engines that are been synchronize in the current loop
     */
    virtual void compute(const std::vector<EngineClientInterfaceSharedPtr> &engines) = 0;

    /*!
     * \brief Send datapacks to engines
     *
     * \param engines Engines that are been synchronize in the current loop
     */
    virtual void sendDataPacksToEngines(const std::vector<EngineClientInterfaceSharedPtr> &engines) = 0;

    /*!
     * \brief Performs post-engine-initialization DataPack operations
     *
     * \param engines Vector of engines for which the post-init step should be performed
     */
    virtual void postEngineInit(const std::vector<EngineClientInterfaceSharedPtr> &/*engines*/)
    {}

    /*!
     * \brief Performs pre-engine-reset DataPack operations
     *
     * \param engines Vector of engines for which the pre-init step should be performed
     */
    virtual void preEngineReset(const std::vector<EngineClientInterfaceSharedPtr> &/*engines*/)
    {}

    /*!
     * \brief Performs post-engine-reset DataPack operations
     *
     * \param engines Vector of engines for which the post-reset step should be performed
     */
    virtual void postEngineReset(const std::vector<EngineClientInterfaceSharedPtr> &/*engines*/)
    {}

    /*!
     * \brief Execute sequentially the update, compute and send operations
     *
     * \param engines Engines that are been synchronize in the current loop
     * \param clientData Extra data coming from the NRP Client, will be passed to the status function
     */
    void datapackCycle(const std::vector<EngineClientInterfaceSharedPtr> &engines)
    {
        updateDataPacksFromEngines(engines);
        NRP_LOG_TIME("after_get_datapacks");
        compute(engines);
        NRP_LOG_TIME("after_run_tfs");
        sendDataPacksToEngines(engines);
        NRP_LOG_TIME("after_send_datapacks");
        this->_simulationDataManager->startNewIteration();
    }

    void setSimulationTime(SimulationTime simulationTime)
    {
        this->_simulationTime = simulationTime;
    }

    void setSimulationIteration(unsigned long simulationIteration)
    {
        this->_simulationIteration = simulationIteration;
    }

protected:

    SimulationDataManager * _simulationDataManager;
    SimulationTime _simulationTime = SimulationTime::zero();
    unsigned long _simulationIteration = 0L;
};

#endif // DATAPACK_HANDLE_H
