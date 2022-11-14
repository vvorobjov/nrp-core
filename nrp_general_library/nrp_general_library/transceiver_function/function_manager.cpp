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

#include "nrp_general_library/transceiver_function/function_manager.h"

#include "nrp_general_library/utils/nrp_exceptions.h"
#include "nrp_general_library/utils/python_error_handler.h"

#include <iostream>

FunctionData::FunctionData(const std::string &name,
                           const TransceiverDataPackInterface::shared_ptr &function,
                           const EngineClientInterface::datapack_identifiers_set_t &datapackIDs)
    : Name(name),
      Function(function),
      DataPackIDs(datapackIDs)
{}

void DataPackFunctionResult::extractDataPacks()
{
    // Extract pointers to retrieved datapacks
    const auto devListLength = boost::python::len(this->DataPackList);
    this->DataPacks.reserve(devListLength);
    for(unsigned int i = 0; i < devListLength; ++i)
    {
        this->DataPacks.push_back(boost::python::extract<DataPackInterface*>(this->DataPackList[i]));
    }
}

DataPackFunctionResult::DataPackFunctionResult(datapack_list_t &&_datapackList)
    : DataPackList(_datapackList)
{}

FunctionManager::FunctionManager()
    : FunctionManager(static_cast<boost::python::dict>(boost::python::import("__main__").attr("__dict__")))
{}

FunctionManager::FunctionManager(const boost::python::dict &tfGlobals)
    : _globalDict(tfGlobals)
{}


FunctionManager::function_datas_t::const_iterator FunctionManager::findDataPackFunction(const std::string &name) const
{
    for(auto curTFIterator = this->_dataPackFunctions.begin(); curTFIterator != this->_dataPackFunctions.end(); ++curTFIterator)
    {
        if(curTFIterator->second.Name.compare(name) == 0)
            return curTFIterator;
    }

    return this->_dataPackFunctions.end();
}

EngineClientInterface::datapack_identifiers_set_t FunctionManager::updateRequestedDataPackIDs() const
{
    EngineClientInterface::datapack_identifiers_set_t devIDs;

    // Scan all transceiver functions for requested datapacks

    for(const auto &curData : this->_dataPackFunctions)
    {
        auto newDevIDs = curData.second.Function->updateRequestedDataPackIDs();
        devIDs.insert(newDevIDs.begin(), newDevIDs.end());
    }

    return devIDs;
}

void FunctionManager::setEngineDataPacks(FunctionManager::engines_datapacks_t &&engineDataPacks)
{
    this->_engineDataPacks = std::move(engineDataPacks);
}

boost::python::object FunctionManager::runDataPackFunction(const std::string &tfName)
{
    // Find associated TF
    auto tfDataIterator = this->findDataPackFunction(tfName);

    // If TF doesn't exist yet, throw error
    if(tfDataIterator == this->_dataPackFunctions.end())
        throw NRPException::logCreate("TF with name " + tfName + " not loaded");

    try
    {
        boost::python::tuple args;
        boost::python::dict kwargs;

        return tfDataIterator->second.Function->runTf(args, kwargs);
    }
    catch(boost::python::error_already_set &)
    {
        throw NRPException::logCreate("Python error occurred during execution of TF \"" + tfDataIterator->second.Name + "\": " + handle_pyerror());
    }
    catch (NRPException &e)
    {
        std::string function_type = tfDataIterator->second.Function->isPreprocessing() ? "Preprocessing" : "Transceiver";
        throw NRPException::logCreate("Error occurred during execution of " + function_type + " Function \"" + tfDataIterator->second.Name + "\": " + e.what());
    }
}


FunctionManager::status_function_results_t FunctionManager::executeStatusFunction(const nlohmann::json & clientData)
{
    // Early return in case status function is not registered

    if(this->_statusFunction == nullptr)
    {
        return std::make_tuple(nullptr, FunctionManager::tf_results_t());
    }

    // Run the status function

    boost::python::object results;

    try
    {
        boost::python::tuple args;
        boost::python::dict kwargs;

        kwargs["client_data"] = boost::ref(clientData);

        results = this->_statusFunction->Function->runTf(args, kwargs);
    }
    catch(boost::python::error_already_set &)
    {
        throw NRPException::logCreate("Python error occurred during execution of Status Function '" +
                                      this->_statusFunction->Name + "': " + handle_pyerror());
    }

    // Extract the results - a single nlohmann::json object should be returned by the status function

    std::unique_ptr<nlohmann::json> retval;
    FunctionManager::tf_results_t dataPacks;

    try
    {
        boost::python::tuple resultsTuple(results);

        if(boost::python::len(resultsTuple) != 2)
        {
            throw NRPException::logCreate("Python error occurred during extraction of results from Status Function '" +
                                          this->_statusFunction->Name + ". " +
                                          "\nPlease make sure that the function returns a tuple with two elements: a SimulationStatus object,"
                                          "and a (possibly empty) list of DataPacks");
        }

        // The first element in the tuple should be a JSON status object

        retval.reset(boost::python::extract<nlohmann::json *>(resultsTuple[0]));

        // The second element should be a list of DataPacks (it can be empty)

        boost::python::list resultsListDataPacks(resultsTuple[1]);
        DataPackFunctionResult resultDataPacks(std::move(resultsListDataPacks));

        // Extract pointers to retrieved datapacks
        resultDataPacks.extractDataPacks();

        dataPacks.push_back(resultDataPacks);
    }
    catch(boost::python::error_already_set &)
    {
        throw NRPException::logCreate("Python error occurred during extraction of results from Status Function '" +
                                      this->_statusFunction->Name + "': " +
                                      handle_pyerror() +
                                      "\nPlease make sure that the function returns a tuple with two elements: a SimulationStatus object,"
                                      "and a (possibly empty) list of DataPacks");
    }

    return std::make_tuple(std::move(retval), std::move(dataPacks));
}


FunctionManager::linked_functions_t FunctionManager::getDataPackFunctions(const std::string &engineName)
{
    return this->_dataPackFunctions.equal_range(engineName);
}

void FunctionManager::loadStatusFunction(const std::string & statusFunctionName, const std::string & statusFunctionFilename)
{
    if(this->_statusFunction != nullptr)
    {
        throw NRPException::logCreate("Status function already defined, with name '" + this->_statusFunction->Name + "'");
    }

    // Check if the file exists

    if(!std::filesystem::exists(statusFunctionFilename))
    {
        throw NRPException::logCreate("Status function file '" + statusFunctionFilename + "' doesn't exist");
    }

    // Load the code of the status function
    // This should trigger the registerNewStatusFunction method,
    // which is bound to the __call__ method of the StatusFunction decorator

    try
    {
        boost::python::exec_file(statusFunctionFilename.c_str(), this->_globalDict, this->_globalDict);
    }
    catch(boost::python::error_already_set &)
    {
        // Roll back the changes done by the registerNewStatusFunction

        this->_statusFunction.reset(nullptr);

        throw NRPException::logCreate("Loading of StatusFunction file \"" +
                                      statusFunctionFilename +
                                      "\" failed: " +
                                      handle_pyerror());
    }

    // The function field should be set by registerNewStatusFunction method

    assert(this->_statusFunction != nullptr);
    assert(this->_statusFunction->Function != nullptr);

    // Set other status function params

    this->_statusFunction->DataPackIDs = this->_statusFunction->Function->updateRequestedDataPackIDs(EngineClientInterface::datapack_identifiers_set_t());
    this->_statusFunction->Name        = statusFunctionName;
}

void FunctionManager::loadDataPackFunction(const std::string & functionName,
                                           const std::string & functionFilename)
{
    // Throw if there's already a transceiver function registered with that name

    if(this->findDataPackFunction(functionName) != this->_dataPackFunctions.end())
    {
        throw NRPException::logCreate("Transceiver function with name '" +
                                      functionName +
                                      "' is already loaded");
    }

    // Make sure that the previous transceiver function was loaded correctly

    assert(this->_newDataPackFunctionIt == this->_dataPackFunctions.end());

    // Check if the file exists

    if(!std::filesystem::exists(functionFilename))
    {
        throw NRPException::logCreate("DataPack Processing Function file '" + functionFilename + "' doesn't exist");
    }

    // Load the code of the transceiver function
    // This should trigger the registerNewDataPackFunction method,
    // which is bound to the __call__ method of the TransceiverFunction decorator

    try
    {
        boost::python::exec_file(functionFilename.c_str(), this->_globalDict, this->_globalDict);
    }
    catch(boost::python::error_already_set &)
    {
        // Roll back the changes done by the registerNewDataPackFunction:
        // - delete the TrasceiverFunctionData object from the container
        // - reset the internal iterator

        if(this->_newDataPackFunctionIt != this->_dataPackFunctions.end())
        {
            this->_dataPackFunctions.erase(this->_newDataPackFunctionIt);
            this->_newDataPackFunctionIt = this->_dataPackFunctions.end();
        }

        throw NRPException::logCreate("Loading of DataPack Processing Function file \"" +
                                      functionFilename +
                                      "\" failed: " +
                                      handle_pyerror());
    }

    // At this point the internal iterator and the function field of the data structure
    // should be set by registerNewDataPackFunction method

    assert(this->_newDataPackFunctionIt != this->_dataPackFunctions.end());
    assert(this->_newDataPackFunctionIt->second.Function != nullptr);

    // Update transfer function params

    this->_newDataPackFunctionIt->second.DataPackIDs = this->_newDataPackFunctionIt->second.Function->updateRequestedDataPackIDs(EngineClientInterface::datapack_identifiers_set_t());
    this->_newDataPackFunctionIt->second.Name        = functionName;

    // Mark the transceiver function as loaded

    this->_newDataPackFunctionIt = this->_dataPackFunctions.end();
}

TransceiverDataPackInterface::shared_ptr *FunctionManager::registerNewDataPackFunction(const std::string &linkedEngine, const TransceiverDataPackInterface::shared_ptr &transceiverFunction)
{
    // Check that no previous TF has not been processed
    assert(this->_newDataPackFunctionIt == this->_dataPackFunctions.end());

    auto newTFIt = this->_dataPackFunctions.emplace(linkedEngine, FunctionData("", transceiverFunction, {}));
    this->_newDataPackFunctionIt = newTFIt;

    return &(newTFIt->second.Function);
}

TransceiverDataPackInterfaceSharedPtr *FunctionManager::registerNewStatusFunction(const TransceiverDataPackInterfaceSharedPtr &statusFunction)
{
    this->_statusFunction = std::unique_ptr<FunctionData>(new FunctionData("", statusFunction, {}));
    return &this->_statusFunction->Function;
}

FunctionManager::tf_results_t FunctionManager::executeDataPackFunctions(const std::string &engineName, const bool preprocessing)
{
    tf_results_t tfResults;

    const auto linkedTFRange = this->getDataPackFunctions(engineName);

    for(auto curTFIt = linkedTFRange.first; curTFIt != linkedTFRange.second; ++curTFIt)
    {
        if(curTFIt->second.Function->isPreprocessing() == preprocessing)
        {
            // Get datapack outputs from transceiver function
            DataPackFunctionResult::datapack_list_t pyResult(this->runDataPackFunction(curTFIt->second.Name));
            DataPackFunctionResult result(std::move(pyResult));

            // Extract pointers to retrieved datapacks
            result.extractDataPacks();
            tfResults.push_back(result);
        }
    }

    return tfResults;
}

FunctionManager::tf_results_t FunctionManager::executePreprocessingFunctions(const std::string &engineName)
{
    return executeDataPackFunctions(engineName, true);
}

FunctionManager::tf_results_t FunctionManager::executeTransceiverFunctions(const std::string &engineName)
{
    return executeDataPackFunctions(engineName, false);
}

// EOF
