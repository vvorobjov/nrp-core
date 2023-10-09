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

#include "nrp_general_library/datapack_interface/datapack_interface.h"


DataPackIdentifier::DataPackIdentifier(const std::string &_name, const std::string &_engineName, const std::string &_type)
    : Name(_name), EngineName(_engineName), Type(_type)
{}

DataPackInterface::DataPackInterface(const std::string &name, const std::string &engineName, const std::string &type)
    : DataPackInterface(DataPackIdentifier(name, engineName, type))
{}

const std::string &DataPackInterface::name() const
{
    return this->_id.Name;
}

void DataPackInterface::setName(const std::string &name)
{
    this->_id.Name = name;
}

const std::string &DataPackInterface::type() const
{
    return this->_id.Type;
}

void DataPackInterface::setType(const std::string &type)
{
    this->_id.Type = type;
}

const std::string &DataPackInterface::engineName() const
{
    return this->_id.EngineName;
}

void DataPackInterface::setEngineName(const std::string &engineName)
{
    this->_id.EngineName = engineName;
}

const DataPackIdentifier &DataPackInterface::id() const
{
    return this->_id;
}

void DataPackInterface::setID(const DataPackIdentifier &id)
{
    this->_id = id;
}

bool DataPackInterface::isEmpty() const
{
    return this->_isEmpty;
}

void DataPackInterface::setIsEmpty(bool value)
{
    this->_isEmpty = value;
}

bool DataPackInterface::isUpdated() const
{
    return this->_isUpdated;
}

void DataPackInterface::resetIsUpdated() const
{
    this->_isUpdated = false;
}

// EOF
