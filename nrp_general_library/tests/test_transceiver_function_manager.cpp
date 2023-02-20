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

#include "tests/test_transceiver_function_manager.h"

TestSimpleTransceiverDataPack::TestSimpleTransceiverDataPack(boost::python::api::object fcn)
    : _fcn(fcn)
{}

TestSimpleTransceiverDataPack::~TestSimpleTransceiverDataPack()
{   this->_tfInterpreterRegistry = nullptr; }

TransceiverDataPackInterface::shared_ptr *TestSimpleTransceiverDataPack::getTFInterpreterRegistry()
{   return this->_tfInterpreterRegistry;    }

const std::string &TestSimpleTransceiverDataPack::linkedEngineName() const
{
    return this->_linkedEngine;
}

boost::python::object TestSimpleTransceiverDataPack::runTf(python::tuple &args, python::dict &kwargs, datapacks_set_t/* dataPacks*/)
{   return this->_fcn(*args, **kwargs); }

datapack_identifiers_set_t TestSimpleTransceiverDataPack::updateRequestedDataPackIDs(datapack_identifiers_set_t &&datapackIDs) const
{   return std::move(datapackIDs);  }

// EOF
