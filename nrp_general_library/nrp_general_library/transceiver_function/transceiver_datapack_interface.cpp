//
// NRP Core - Backend infrastructure to synchronize simulations
//
// Copyright 2020-2023 NRP Team
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

#include "nrp_general_library/transceiver_function/transceiver_datapack_interface.h"

FunctionManager *TransceiverDataPackInterface::_functionManager = nullptr;

const std::string &TransceiverDataPackInterface::linkedEngineName() const
{
    return this->_nextDecorator->linkedEngineName();
}

bool TransceiverDataPackInterface::isPreprocessing() const
{
    return this->_nextDecorator->isPreprocessing();
}

boost::python::object TransceiverDataPackInterface::runTf(boost::python::tuple &args, boost::python::dict &kwargs, datapacks_set_t dataPacks)
{
    return this->_nextDecorator->runTf(args, kwargs, dataPacks);
}

datapack_identifiers_set_t TransceiverDataPackInterface::updateRequestedDataPackIDs(datapack_identifiers_set_t &&datapackIDs) const
{
    auto subDataPackIDs = this->_nextDecorator->updateRequestedDataPackIDs(std::move(datapackIDs));
    auto newDataPackIDs = this->getRequestedDataPackIDs();

    subDataPackIDs.insert(newDataPackIDs.begin(), newDataPackIDs.end());

    return subDataPackIDs;
}

datapack_identifiers_set_t TransceiverDataPackInterface::getRequestedDataPackIDs() const
{
    return datapack_identifiers_set_t();
}

void TransceiverDataPackInterface::setTFInterpreter(FunctionManager *interpreter)
{
    TransceiverDataPackInterface::_functionManager = interpreter;
}

TransceiverDataPackInterface::shared_ptr *TransceiverDataPackInterface::getTFInterpreterRegistry()
{
    return this->_nextDecorator->getTFInterpreterRegistry();
}
