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
{}

EngineClientInterface::datapack_identifiers_set_t EngineDataPack::getRequestedDataPackIDs() const
{
    // If this is a preprocessing function this and _datapackID must be linked to the same engine
    if(this->isPreprocessing() && this->_datapackID.EngineName != this->linkedEngineName())
        throw NRPException::logCreate("Preprocessing function is linked to engine \"" + this->linkedEngineName() +
        "\" but its input datapack \""+ this->_datapackID.Name + "\" is linked to engine \"" + this->_datapackID.EngineName +
        "\". Preprocessing functions can just take input datapacks from their linked engines");

    return _isPreprocessed ? EngineClientInterface::datapack_identifiers_set_t() : EngineClientInterface::datapack_identifiers_set_t({this->_datapackID});
}

boost::python::object EngineDataPack::runTf(boost::python::tuple &args, boost::python::dict &kwargs)
{
    const auto engineDevs = TransceiverDataPackInterface::TFInterpreter->getEngineDataPacks();

    bool foundDevID = false;
    auto engDataPacksIt = engineDevs.find(this->_datapackID.EngineName);
    if(engDataPacksIt != engineDevs.end())
    {
        for(const auto &curDataPack : *(engDataPacksIt->second))
        {
            if(curDataPack->id().Name == this->_datapackID.Name)
            {
                kwargs[this->_keyword] = curDataPack;

                foundDevID = true;
                break;
            }
        }
    }

    if(!foundDevID)
        throw NRPException::logCreate("Couldn't find datapack with ID name \"" + this->_datapackID.Name + "\"");

    return TransceiverDataPackInterface::runTf(args, kwargs);
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

}

EngineClientInterface::datapack_identifiers_set_t EngineDataPacks::getRequestedDataPackIDs() const
{
    EngineClientInterface::datapack_identifiers_set_t datapackIdentifiersSet = EngineClientInterface::datapack_identifiers_set_t();
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

    return _isPreprocessed ? EngineClientInterface::datapack_identifiers_set_t() : datapackIdentifiersSet;
}

boost::python::object EngineDataPacks::runTf(boost::python::tuple &args, boost::python::dict &kwargs)
{
    const auto engineDevs = TransceiverDataPackInterface::TFInterpreter->getEngineDataPacks();

    boost::python::dict engDataPacks;

    auto engDataPacksIt = engineDevs.find(this->_datapacksIDs.begin()->EngineName);

    //In order to create the dictionary containing the engine datapacks it is necessary to check if each datapackID is already registered
    if(engDataPacksIt != engineDevs.end())
    {
        for(const auto &regDataPack: _datapacksIDs)
        {
            for(const auto &curDataPack : *(engDataPacksIt->second))
            {
                if(regDataPack.Name == curDataPack->id().Name)
                {
                    engDataPacks[curDataPack->id().Name] = curDataPack;
                    break;
                }
            }
        }
    }

    //Checks if dictionary of datapacks contains all datapacks linked to an engine in TF
    if(this->_datapacksIDs.size() == boost::numeric_cast<long unsigned int>(len(engDataPacks.keys())))
    {
        kwargs[this->_keyword] = engDataPacks;
    }
    else
    {
        for(const auto &curDataPackID : this->_datapacksIDs)
        {
            if(!engDataPacks.has_key(curDataPackID.Name))
                throw NRPException::logCreate("Couldn't find datapack with ID name "+ curDataPackID.Name +
                                          std::string(", DataPacks found ") + std::to_string(len(engDataPacks.keys())));
        }
    }


    return TransceiverDataPackInterface::runTf(args, kwargs);
}
