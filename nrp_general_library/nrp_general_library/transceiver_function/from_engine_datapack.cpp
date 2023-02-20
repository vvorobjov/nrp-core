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

#include "nrp_general_library/transceiver_function/from_engine_datapack.h"

#include "nrp_general_library/transceiver_function/function_manager.h"
#include "nrp_general_library/utils/nrp_exceptions.h"
#include "nrp_general_library/utils/python_error_handler.h"

EngineDataPack::EngineDataPack(const std::string &keyword, const DataPackIdentifier &datapackID, bool isPreprocessed)
    : _keyword(keyword),
      _datapackID(datapackID),
      _isPreprocessed(isPreprocessed)
{
    assert(this->getFunctionManager() != nullptr);
    this->_DataPackPassingPolicy = this->getFunctionManager()->getDataPackPassingPolicy();
}

datapack_identifiers_set_t EngineDataPack::getRequestedDataPackIDs() const
{
    // If this is a preprocessing function this and _datapackID must be linked to the same engine
    if(this->isPreprocessing() && this->_datapackID.EngineName != this->linkedEngineName())
        throw NRPException::logCreate("Preprocessing function is linked to engine \"" + this->linkedEngineName() +
        "\" but its input datapack \""+ this->_datapackID.Name + "\" is linked to engine \"" + this->_datapackID.EngineName +
        "\". Preprocessing functions can just take input datapacks from their linked engines");

    return _isPreprocessed ? datapack_identifiers_set_t() : datapack_identifiers_set_t({this->_datapackID});
}


boost::python::object EngineDataPack::runTf(boost::python::tuple &args, boost::python::dict &kwargs, datapacks_set_t dataPacks)
{
    const auto dataPack = dataPacks.find(this->_datapackID);

    if(dataPack != dataPacks.end())
    {
        if(this->_DataPackPassingPolicy == PASS_BY_VALUE)
            kwargs[this->_keyword] = std::shared_ptr<DataPackInterface>((*dataPack)->clone());
        else
            kwargs[this->_keyword] = *dataPack;
    }
    else
    {
        throw NRPException::logCreate("Couldn't find datapack with ID name \"" + this->_datapackID.Name + "\"");
    }

    return TransceiverDataPackInterface::runTf(args, kwargs, dataPacks);
}


EngineDataPacks::EngineDataPacks(const std::string &keyword, const boost::python::list &datapackListNames, const std::string &engineName, bool isPreprocessed)
        : _keyword(keyword),
          _engineName(engineName),
          _isPreprocessed(isPreprocessed)
{
    for(int i = 0; i<len(datapackListNames); ++i)
    {
        try
        {
            _datapacksIDs.push_back(DataPackIdentifier(boost::python::extract<std::string>(datapackListNames[i]),_engineName, std::string()));
        }
        catch(boost::python::error_already_set &)
        {
            throw NRPException::logCreate("Python error occurred during extraction of DataPack names from Transceiver Function '" +
                                          handle_pyerror() +
                                          "\nPlease make sure that the transceiver function datapackNames is a list of strings");
        }
    }

    assert(this->getFunctionManager() != nullptr);
    this->_DataPackPassingPolicy = this->getFunctionManager()->getDataPackPassingPolicy();
}

datapack_identifiers_set_t EngineDataPacks::getRequestedDataPackIDs() const
{
    datapack_identifiers_set_t datapackIdentifiersSet = datapack_identifiers_set_t();
    // If this is a preprocessing function this and _datapackID must be linked to the same engine
    for(const auto &curDataPackID: this->_datapacksIDs)
    {
        if(this->isPreprocessing() && curDataPackID.EngineName != this->linkedEngineName())
            throw NRPException::logCreate("Preprocessing function is linked to engine \"" + this->linkedEngineName() +
                                              "\" but its input datapack \""+ curDataPackID.Name + "\" is linked to engine \"" + curDataPackID.EngineName +
                                              "\". Preprocessing functions can just take input datapacks from their linked engines");
        else
            datapackIdentifiersSet.insert(curDataPackID);
    }

    return _isPreprocessed ? datapack_identifiers_set_t() : datapackIdentifiersSet;
}

boost::python::object EngineDataPacks::runTf(boost::python::tuple &args, boost::python::dict &kwargs, datapacks_set_t dataPacks)
{
    // Find all datapacks that belong to the requested engine

    datapacks_set_t engineDataPacks;
    for(auto dataPack: dataPacks)
    {
        if(dataPack->id().EngineName == this->_datapacksIDs.begin()->EngineName)
        {
            engineDataPacks.insert(dataPack);
        }
    }

    // Find all requested datapacks

    boost::python::dict dataPackDict;
    for(auto requestedId: this->_datapacksIDs)
    {
        auto dataPack = engineDataPacks.find(requestedId);
        if(dataPack != engineDataPacks.end())
        {
            if(this->_DataPackPassingPolicy == PASS_BY_VALUE)
                dataPackDict[requestedId.Name] = std::shared_ptr<DataPackInterface>((*dataPack)->clone());
            else
                dataPackDict[requestedId.Name] = *dataPack;
        }
        else
        {
            throw NRPException::logCreate("Couldn't find datapack with ID name \"" + requestedId.Name + "\"");
        }
    }

    // Add the datapacks to the arguments

    kwargs[this->_keyword] = dataPackDict;

    return TransceiverDataPackInterface::runTf(args, kwargs, dataPacks);
}

// EOF
