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

#ifndef TF_MANAGER_HANDLE_H
#define TF_MANAGER_HANDLE_H

#include "nrp_general_library/transceiver_function/function_manager.h"

#include "nrp_simulation/datapack_handle/datapack_handle.h"


/*!
 * \brief Uses the TF framework to execute datapack transformation operations
 */
class TFManagerHandle : public DataPackProcessor {

public:

    TFManagerHandle(SimulationDataManager * simulationDataManager):
        DataPackProcessor(simulationDataManager)
        {}

    void init(const jsonSharedPtr &simConfig, const engine_interfaces_t &engines) override;

    /*!
     * \brief Performs post-engine-initialization DataPack operations
     *
     * The method will retrieve DataPacks prepared by the engines' initialize() functions,
     * and push them into the trajectory buffer, so that they can be returned to the
     * main script.
     *
     * \param engines Vector of engines for which the post-init step should be performed
     */
    void postEngineInit(const std::vector<EngineClientInterfaceSharedPtr> &engines) override;

    /*!
     * \brief Performs pre-engine-reset DataPack operations
     *
     * The method will send Engine DataPacks to all engines.
     * This allows the main script to send additional data or commands to the engines on reset().
     *
     * \param engines Vector of engines for which the pre-init step should be performed
     */
    void preEngineReset(const std::vector<EngineClientInterfaceSharedPtr> &engines) override;

    /*!
     * \brief Performs post-engine-reset DataPack operations
     *
     * The method will retrieve DataPacks prepared by the engines' reset() functions,
     * and push them into the trajectory buffer, so that they can be returned to the
     * main script.
     *
     * \param engines Vector of engines for which the post-reset step should be performed
     */
    void postEngineReset(const std::vector<EngineClientInterfaceSharedPtr> &engines) override;

    void updateDataPacksFromEngines(const std::vector<EngineClientInterfaceSharedPtr> &engines) override;

    void compute(const std::vector<EngineClientInterfaceSharedPtr> &engines) override;

    void sendDataPacksToEngines(const std::vector<EngineClientInterfaceSharedPtr> &engines) override;

private:

    /*!
     * \brief Execute PreprocessingFunctions for each engine and place output datapacks in its cache
     *
     * \param functionManager function manager
     * \param engines Engines that are been synchronize in the current loop
     */
    void executePreprocessingFunctions(FunctionManager &functionManager,
                                       const std::vector<EngineClientInterfaceSharedPtr> &engines,
                                       datapacks_set_t dataPacks);

    /*!
     * \brief Execute TransceiverFunctions for each engine
     *
     * \param functionManager tfManager
     * \param engines Engines that are been synchronize in the current loop
     */
    void executeTransceiverFunctions(FunctionManager &functionManager,
                                     const std::vector<EngineClientInterfaceSharedPtr> &engines,
                                     datapacks_set_t dataPacks);

    /*!
     * \brief Loads all DataPack Processing Functions defined in the config
     */
    void loadDataPackFunctions(const jsonSharedPtr &simConfig);

    /*!
     * \brief Loads Status Function defined in the config
     */
    void loadStatusFunction(const jsonSharedPtr &simConfig);

    /*!
     * \brief Helper method for postEngine* methods
     *
     * \param engines Vector of engines for which the post step should be performed
     */
    void postEngineActivityHelper(const std::vector<EngineClientInterfaceSharedPtr> &engines);

    /*! \brief  FunctionManager handling datapack operations */
    FunctionManager _functionManager;
};

#endif // TF_MANAGER_HANDLE_H
