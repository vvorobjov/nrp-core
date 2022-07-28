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
#include "nrp_general_library/transceiver_function/transceiver_function_sorted_results.h"

#include "nrp_simulation/datapack_handle/datapack_handle.h"


/*!
 * \brief Uses the TF framework to execute datapack transformation operations
 */
class TFManagerHandle : public DataPackProcessor {

public:

    void init(const jsonSharedPtr &simConfig, const engine_interfaces_t &engines) override;

    void updateDataPacksFromEngines(const std::vector<EngineClientInterfaceSharedPtr> &engines) override;

    void compute(const std::vector<EngineClientInterfaceSharedPtr> &engines) override;

    void sendDataPacksToEngines(const std::vector<EngineClientInterfaceSharedPtr> &engines) override;

    /*!
     * \brief Execute PreprocessingFunctions for each engine and place output datapacks in its cache
     *
     * \param functionManager function manager
     * \param engines Engines that are been synchronize in the current loop
     */
    static void executePreprocessingFunctions(FunctionManager &functionManager,
                                              const std::vector<EngineClientInterfaceSharedPtr> &engines);

    /*!
     * \brief Execute TransceiverFunctions for each engine
     *
     * \param functionManager tfManager
     * \param engines Engines that are been synchronize in the current loop
     */
    static TransceiverFunctionSortedResults executeTransceiverFunctions(FunctionManager &functionManager,
                                                                        const std::vector<EngineClientInterfaceSharedPtr> &engines);

private:

    /*!
     * \brief Loads all DataPack Processing Functions defined in the config
     */
    void loadDataPackFunctions(const jsonSharedPtr &simConfig);

    /*!
     * \brief Loads Status Function defined in the config
     */
    void loadStatusFunction(const jsonSharedPtr &simConfig);

    /*! \brief  FunctionManager handling datapack operations */
    FunctionManager _functionManager;
    /*! \brief  tf results */
    TransceiverFunctionSortedResults _tf_results;
};

#endif // TF_MANAGER_HANDLE_H
