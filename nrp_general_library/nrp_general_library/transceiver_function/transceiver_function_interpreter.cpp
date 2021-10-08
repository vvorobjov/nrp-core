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

#include "nrp_general_library/transceiver_function/transceiver_function_interpreter.h"

#include "nrp_general_library/utils/nrp_exceptions.h"
#include "nrp_general_library/utils/python_error_handler.h"

#include <iostream>

TransceiverFunctionData::TransceiverFunctionData(const std::string &_name, const TransceiverDataPackInterface::shared_ptr &_transceiverFunction, const EngineClientInterface::datapack_identifiers_set_t &_datapackIDs, const boost::python::object &_localVariables)
    : Name(_name),
      TransceiverFunction(_transceiverFunction),
      DataPackIDs(_datapackIDs),
      LocalVariables(_localVariables)
{}

void TransceiverFunctionInterpreter::TFExecutionResult::extractDataPacks()
{
    // Extract pointers to retrieved datapacks
    const auto devListLength = boost::python::len(this->DataPackList);
    this->DataPacks.reserve(devListLength);
    for(unsigned int i = 0; i < devListLength; ++i)
    {
        this->DataPacks.push_back(boost::python::extract<DataPackInterface*>(this->DataPackList[i]));
    }
}

TransceiverFunctionInterpreter::TFExecutionResult::TFExecutionResult(datapack_list_t &&_datapackList)
    : DataPackList(_datapackList)
{}

TransceiverFunctionInterpreter::TransceiverFunctionInterpreter()
    : TransceiverFunctionInterpreter(static_cast<boost::python::dict>(boost::python::import("__main__").attr("__dict__")))
{}

TransceiverFunctionInterpreter::TransceiverFunctionInterpreter(const boost::python::dict &tfGlobals)
    : _globalDict(tfGlobals)
{}

TransceiverFunctionInterpreter::transceiver_function_datas_t::const_iterator TransceiverFunctionInterpreter::findTransceiverFunction(const std::string &name) const
{
    for(auto curTFIterator = this->_transceiverFunctions.begin(); curTFIterator != this->_transceiverFunctions.end(); ++curTFIterator)
    {
        if(curTFIterator->second.Name.compare(name) == 0)
            return curTFIterator;
    }

    return this->_transceiverFunctions.end();
}

const TransceiverFunctionInterpreter::transceiver_function_datas_t &TransceiverFunctionInterpreter::getLoadedTransceiverFunctions() const
{
    return this->_transceiverFunctions;
}

EngineClientInterface::datapack_identifiers_set_t TransceiverFunctionInterpreter::updateRequestedDataPackIDs() const
{
    EngineClientInterface::datapack_identifiers_set_t devIDs;

    // Scan all transceiver functions for requested datapacks

    for(const auto &curData : this->_transceiverFunctions)
    {
        auto newDevIDs = curData.second.TransceiverFunction->updateRequestedDataPackIDs();
        devIDs.insert(newDevIDs.begin(), newDevIDs.end());
    }

    return devIDs;
}

void TransceiverFunctionInterpreter::setEngineDataPacks(TransceiverFunctionInterpreter::engines_datapacks_t &&engineDataPacks)
{
    this->_engineDataPacks = std::move(engineDataPacks);
}

boost::python::object TransceiverFunctionInterpreter::runSingleTransceiverFunction(const std::string &tfName)
{
    // Find associated TF
    auto tfDataIterator = this->findTransceiverFunction(tfName);

    // If TF doesn't exist yet, throw error
    if(tfDataIterator == this->_transceiverFunctions.end())
        throw NRPException::logCreate("TF with name " + tfName + " not loaded");

    return this->runSingleTransceiverFunction(tfDataIterator->second);
}

boost::python::object TransceiverFunctionInterpreter::runSingleTransceiverFunction(const TransceiverFunctionData &tfData)
{
    try
    {
        boost::python::tuple args;
        boost::python::dict kwargs;

        return tfData.TransceiverFunction->runTf(args, kwargs);
    }
    catch(boost::python::error_already_set &)
    {
        throw NRPException::logCreate("Python error occurred during execution of TF \"" + tfData.Name + "\": " + handle_pyerror());
    }
    catch (NRPException &e)
    {
        std::string function_type = tfData.TransceiverFunction->isPrepocessing() ? "Preprocessing" : "Transceiver";
        throw NRPException::logCreate("Error occurred during execution of " + function_type + " Function \"" + tfData.Name + "\": " + e.what());
    }
}

TransceiverFunctionInterpreter::linked_tfs_t TransceiverFunctionInterpreter::getLinkedTransceiverFunctions(const std::string &engineName)
{
    return this->_transceiverFunctions.equal_range(engineName);
}

TransceiverFunctionInterpreter::transceiver_function_datas_t::iterator TransceiverFunctionInterpreter::loadTransceiverFunction(const nlohmann::json &transceiverFunction)
{
    // Make sure no previously loaded TFs have not been handled
    assert(this->_newTransceiverFunctionIt == this->_transceiverFunctions.end());

    // Create dict for new module
    boost::python::dict localVars;

    // Load TF code
    const std::string tf_name = transceiverFunction.at("Name");
    const std::string tf_file = transceiverFunction.at("FileName");
    try
    {
        boost::python::exec_file(tf_file.c_str(), this->_globalDict, this->_globalDict);
    }
    catch(boost::python::error_already_set &)
    {
        const auto err = NRPException::logCreate("Loading of TransceiverFunction file \"" + tf_file + "\" failed: " + handle_pyerror());

        if(this->_newTransceiverFunctionIt != this->_transceiverFunctions.end())
        {
            this->_transceiverFunctions.erase(this->_newTransceiverFunctionIt);
            this->_newTransceiverFunctionIt = this->_transceiverFunctions.end();
        }

        throw err;
    }

    // Check that load resulted in a TF
    if(this->_newTransceiverFunctionIt == this->_transceiverFunctions.end())
        throw NRPException::logCreate("No TF found for " + tf_name);

    // Update transfer function params
    this->_newTransceiverFunctionIt->second.DataPackIDs      = this->_newTransceiverFunctionIt->second.TransceiverFunction->updateRequestedDataPackIDs(EngineClientInterface::datapack_identifiers_set_t());
    this->_newTransceiverFunctionIt->second.LocalVariables = localVars;
    this->_newTransceiverFunctionIt->second.Name           = tf_name;

    const auto retVal = this->_newTransceiverFunctionIt;
    this->_newTransceiverFunctionIt = this->_transceiverFunctions.end();

    return retVal;
}

TransceiverFunctionInterpreter::transceiver_function_datas_t::iterator TransceiverFunctionInterpreter::loadTransceiverFunction(const std::string &tfName,
                                                                                                                                const TransceiverDataPackInterfaceSharedPtr &transceiverFunction,
                                                                                                                                boost::python::object &&localVars)
{
    return this->_transceiverFunctions.emplace(transceiverFunction->linkedEngineName(),
                                                TransceiverFunctionData(tfName,
                                                                        transceiverFunction,
                                                                        transceiverFunction->updateRequestedDataPackIDs(),
                                                                        std::move(localVars)));
}

TransceiverFunctionInterpreter::transceiver_function_datas_t::iterator TransceiverFunctionInterpreter::updateTransceiverFunction(const nlohmann::json &transceiverFunction)
{
    // Erase existing TF if found
    auto tfIterator = this->findTransceiverFunction(transceiverFunction.at("Name"));
    if(tfIterator != this->_transceiverFunctions.end())
        this->_transceiverFunctions.erase(tfIterator);

    // Create new one
    return this->loadTransceiverFunction(transceiverFunction);
}

TransceiverDataPackInterface::shared_ptr *TransceiverFunctionInterpreter::registerNewTransceiverFunction(const std::string &linkedEngine, const TransceiverDataPackInterface::shared_ptr &transceiverFunction)
{
    // Check that no previous TF has not been processed
    assert(this->_newTransceiverFunctionIt == this->_transceiverFunctions.end());

    auto newTFIt = this->_transceiverFunctions.emplace(linkedEngine, TransceiverFunctionData("", transceiverFunction, {}, boost::python::dict()));
    this->_newTransceiverFunctionIt = newTFIt;

    return &(newTFIt->second.TransceiverFunction);
}

// EOF
