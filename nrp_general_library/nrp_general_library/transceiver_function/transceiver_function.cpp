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

#include "nrp_general_library/transceiver_function/transceiver_function.h"

TransceiverFunction::TransceiverFunction(std::string linkedEngine, bool isPreprocessing)
    : _linkedEngine(linkedEngine),
      _isPreprocessing(isPreprocessing)
{}

const std::string &TransceiverFunction::linkedEngineName() const
{	return this->_linkedEngine;	}

bool TransceiverFunction::isPrepocessing() const
{   return this->_isPreprocessing; }

TransceiverDataPackInterface::shared_ptr TransceiverFunction::pySetup(boost::python::object transceiverFunction)
{
	this->_function = transceiverFunction;

	auto tf = this->moveToSharedPtr<TransceiverFunction>();

	assert(tf->_tfInterpreterRegistryPtr == nullptr);
	if(tf->_tfInterpreterRegistryPtr == nullptr)
		tf->_tfInterpreterRegistryPtr = TransceiverFunction::TFInterpreter->registerNewTransceiverFunction(this->linkedEngineName(), tf);

	return tf;
}

boost::python::object TransceiverFunction::runTf(boost::python::tuple &args, boost::python::dict &kwargs)
{
    boost::python::object retVal = this->_function(*args, **kwargs);
    this->checkTFOutputIsCorrectOrRaise(retVal);
    return retVal;
}

void TransceiverFunction::checkTFOutputIsCorrectOrRaise(const boost::python::object &tfOutput) const
{
    // error msg
    std::string function_type = this->isPrepocessing() ? "Preprocessing" : "Transceiver";
    std::string error_msg = function_type + " functions must return a list of DataPacks";

    // TFs must returns a list
    if(!boost::python::extract<boost::python::list>(tfOutput).check())
        throw NRPException::logCreate(error_msg);

    const auto devListLength = boost::python::len(tfOutput);
    for(unsigned int i = 0; i < devListLength; ++i)
    {
        // All elements in the list must be datapacks
        if (!boost::python::extract<DataPackInterface *>(tfOutput[i]).check())
            throw NRPException::logCreate(error_msg);
        // If PF, DataPack and PF have the same Engine
        else if(this->isPrepocessing())
        {
            DataPackInterface *dev = boost::python::extract<DataPackInterface *>(tfOutput[i]);
            if (dev->engineName() != this->linkedEngineName())
                throw NRPException::logCreate("Preprocessing function is linked to engine \"" + this->linkedEngineName() +
                                              "\" but its output datapack \""+ dev->name() + "\" is linked to engine \"" + dev->engineName() +
                                              "\". Preprocessing functions can just return datapacks to their linked engines");
        }
    }
}

EngineClientInterface::datapack_identifiers_set_t TransceiverFunction::getRequestedDataPackIDs() const
{
	return EngineClientInterface::datapack_identifiers_set_t();
}

EngineClientInterface::datapack_identifiers_set_t TransceiverFunction::updateRequestedDataPackIDs(EngineClientInterface::datapack_identifiers_set_t &&datapackIDs) const
{
	return std::move(datapackIDs);
}

TransceiverDataPackInterface::shared_ptr *TransceiverFunction::getTFInterpreterRegistry()
{
	return this->_tfInterpreterRegistryPtr;
}
