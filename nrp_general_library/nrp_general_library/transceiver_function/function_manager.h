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

#ifndef FUNCTION_MANAGER_H
#define FUNCTION_MANAGER_H

#include "nrp_general_library/engine_interfaces/engine_client_interface.h"
#include "nrp_general_library/transceiver_function/transceiver_datapack_interface.h"
#include "nrp_general_library/transceiver_function/from_engine_datapack.h"

#include <vector>
#include <map>
#include <memory>
#include <list>
#include <boost/python.hpp>

/*!
* \brief Data associated with a single function
*/
struct FunctionData
{
    /*!
    * \brief Name of the Function
    */
    std::string Name;

    /*!
    * \brief Pointer to the Function
    */
    TransceiverDataPackInterface::shared_ptr Function = nullptr;

    /*!
    * \brief DataPacks requested by the Function
    */
    datapack_identifiers_set_t DataPackIDs;

    FunctionData() = default;
    FunctionData(const std::string &name,
                 const TransceiverDataPackInterface::shared_ptr &function,
                 const datapack_identifiers_set_t &datapackIDs);
};


/*!
 * \brief Manages all types of Python functions that can be run as part of NRP Core
 *
 * This class is responsible for loading and running all Python functions that are
 * supported by NRP Core. The following function types are supported:
 * * Preprocessing Functions
 * * Transceiver Functions
 * * Status Functions
 * Preprocessing and Transceiver Functions will be also refered to as DataPack (Processing) Functions,
 * to distinguish them from Status Functions, which have a slightly different purpose.
 */
class FunctionManager
{
    public:

        using function_datas_t = std::multimap<std::string, FunctionData>;
        using linked_functions_t = std::pair<FunctionManager::function_datas_t::iterator, FunctionManager::function_datas_t::iterator>;
        using status_function_results_t = std::tuple<bool, datapacks_vector_t>;

        FunctionManager();
        FunctionManager(const boost::python::dict &tfGlobals);

        /*!
         * \brief Get DataPack IDs requested by TFs
         * \return
         */
        datapack_identifiers_set_t getRequestedDataPackIDs() const;

        DataPackPassingPolicy getDataPackPassingPolicy() const;

        void setDataPackPassingPolicy(DataPackPassingPolicy method);

        /*!
         * \brief Sets simulation time that will be accessible by all Python Functions
         * \param simulationTime Current simulation time from the simulation manager
         */
        void setSimulationTime(SimulationTime simulationTime);

        /*!
         * \brief Returns simulation time stored by the manager
         */
        const SimulationTime & getSimulationTime() const;

        /*!
         * \brief Sets simulation iteration that will be accessible by all Python Functions
         * \param simulationIteration Current simulation iteration from the simulation manager
         */
        void setSimulationIteration(unsigned long simulationIteration);

        /*!
         * \brief Returns simulation iteration number stored by the manager
         */
        unsigned long getSimulationIteration() const;

        /*!
         * \brief Loads given DataPack Processing Function
         * \param dataPackFunctionName Name of the function, used as function's ID. The name doesn't have to match
         *                             the actual name of the python function.
         * \param dataPackFunctionFilename Name of the file where the function is located.
         *
         * The function runs python code from the given file, executes all decorators found in the function definition,
         * and stores the function with some metadata for future use.
         *
         * DataPack Processing Functions are Transceiver Functions and Preprocessing Functions.
         */
        void loadDataPackFunction(const std::string & dataPackFunctionName, const std::string & dataPackFunctionFilename);

        /*!
         * \brief Loads given status funtion
         * \param statusFunctionName Name of the function, used as function's ID. The name doesn't have to match
         *                           the actual name of the python function.
         * \param statusFunctionFilename Name of the file where the function is located.
         *
         * The function runs python code from the given file, executes all decorators found in the definition of the function,
         * and stores the function with some metadata for future use.
         */
        void loadStatusFunction(const std::string & statusFunctionName, const std::string & statusFunctionFilename);

        /*!
         * \brief Executes all Preprocessing Functions linked to an engine
         * \param engineName Name of engine
         * \return Returns results of executed Preprocessing Functions
         */
        datapacks_vector_t executePreprocessingFunctions(const std::string &engineName, datapacks_set_t dataPacks);

        /*!
         * \brief Executes all Transceiver Functions linked to an engine
         * \param engineName Name of engine
         * \return Returns results of executed Preprocessing Functions
         */
        datapacks_vector_t executeTransceiverFunctions(const std::string &engineName, datapacks_set_t dataPacks);

        /*!
         * \brief Executes Status Function registered loaded by the manager
         * \return Results of the Status Function execution as JSON object
         *
         * This method will run the Status Function that was loaded using `loadStatusFunction` method.
         */
        status_function_results_t executeStatusFunction(datapacks_set_t dataPacks);

    private:
        /*!
         * \brief Registers a new transfer function. Used by TransceiverFunction to automatically register itself with the interpreter upon creation
         * \param linkedEngine Name of linked engine
         * \param transceiverFunction Transfer Function to register
         * \return Returns pointer to stored location. Used by TransceiverDataPackInterfaceGeneral to automatically update the registered function when an upper decorator runs pySetup
         */
        // TODO Why does this return a pointer to pointer?
        TransceiverDataPackInterfaceSharedPtr *registerNewDataPackFunction(const std::string &linkedEngine, const TransceiverDataPackInterfaceSharedPtr &transceiverFunction);

        SimulationTime _simulationTime = SimulationTime::zero();
        unsigned long _simulationIteration = 0;
        DataPackPassingPolicy _DataPackPassingPolicy;

        /*!
         * \brief Registers new Status Function in the manager
         * \param statusFunction Pointer to the function object
         * \return Shared pointer to the function object
         */
        // TODO Why does this return a pointer to pointer?
        TransceiverDataPackInterfaceSharedPtr *registerNewStatusFunction(const TransceiverDataPackInterfaceSharedPtr &statusFunction);

        /*!
         * \brief Global dictionary
         */
        boost::python::dict _globalDict;

        /*!
         * \brief All loaded DataPack Processing Functions
         */
        function_datas_t _dataPackFunctions;

        /*!
         * \brief Loaded Status Function
         */
        std::unique_ptr<FunctionData> _statusFunction = nullptr;

        /*!
         * \brief Internal iterator used for transactional loading of transceiver functions
         *
         * Loading of transceiver functions is performed in several steps:
         * - the proper python file is loaded by loadDataPackFunction method
         * - all decorators are called, as part of the loading process, which result in another C++ method (registerNewDataPackFunction)
         *   being called, interrupting the loadDataPackFunction method
         * - the loadDataPackFunction continues, checks if the loading was successful, and finalizes the loading process
         * This variable is used to synchronize the process and make it transactional.
         */
        function_datas_t::iterator _newDataPackFunctionIt = this->_dataPackFunctions.end();

        /*!
         * \brief Tries to find a DataPack Processing Function with given name
         * \param name Name of the function to find
         * \return Returns iterator to TF. If name not present, returns _dataPackFunctions.end()
         */
        function_datas_t::const_iterator findDataPackFunction(const std::string &name) const;

        /*!
         * \brief Execute one transfer function.
         * \param tfName Name of function to execute
         * \return Returns result of execution. Contains a list of datapack commands
         */
        std::vector<std::shared_ptr<DataPackInterface>> runDataPackFunction(const std::string &tfName, datapacks_set_t dataPacks);

        /*!
         * \brief Get TFs linked to specific engine
         * \param engineName Name of engine
         * \return Returns range of TFs linked to engine name
         */
        linked_functions_t getDataPackFunctions(const std::string &engineName);

        /*!
         * \brief Executes all Transceiver Functions or Preprocessing Functions linked to given engine
         * \param engineName Name of the engine
         * \param preprocessing When true, the method will run all Preprocessing Functions,
         *                      when false, it will run Transceiver Functions
         * \return Returns results of executed DataPack Processing Functions
         */
        datapacks_vector_t executeDataPackFunctions(const std::string &engineName,
                                                    datapacks_set_t dataPacks,
                                                    const bool preprocessing);

        // Give the function classes access to private methods,
        // so that they can register themselves in the manager

        friend class TransceiverFunction;
        friend class PreprocessingFunction;
        friend class StatusFunction;
};

using FunctionManagerSharedPtr = std::shared_ptr<FunctionManager>;
using FunctionManagerConstSharedPtr = std::shared_ptr<const FunctionManager>;

#endif // FUNCTION_MANAGER_H

// EOF
