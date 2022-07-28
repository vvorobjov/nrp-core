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

#include "nrp_general_library/transceiver_function/status_function.h"

StatusFunction::StatusFunction()
{}

TransceiverDataPackInterface::shared_ptr StatusFunction::pySetup(boost::python::object statusFunction)
{
    this->_function = statusFunction;

    auto statusFunctionPtr = this->moveToSharedPtr<StatusFunction>();

    assert(statusFunctionPtr->_tfInterpreterRegistryPtr == nullptr);

    statusFunctionPtr->_tfInterpreterRegistryPtr = StatusFunction::TFInterpreter->registerNewStatusFunction(statusFunctionPtr);

    return statusFunctionPtr;
}

const std::string & StatusFunction::linkedEngineName() const
{
    return this->name;
}

bool StatusFunction::isPreprocessing() const
{
    return false;
}

boost::python::object StatusFunction::runTf(boost::python::tuple &args, boost::python::dict &kwargs)
{
    boost::python::object retVal = this->_function(*args, **kwargs);

    return retVal;
}

EngineClientInterface::datapack_identifiers_set_t StatusFunction::getRequestedDataPackIDs() const
{
    return EngineClientInterface::datapack_identifiers_set_t();
}

EngineClientInterface::datapack_identifiers_set_t StatusFunction::updateRequestedDataPackIDs(EngineClientInterface::datapack_identifiers_set_t &&datapackIDs) const
{
    return std::move(datapackIDs);
}

TransceiverDataPackInterface::shared_ptr *StatusFunction::getTFInterpreterRegistry()
{
    return this->_tfInterpreterRegistryPtr;
}

// EOF
