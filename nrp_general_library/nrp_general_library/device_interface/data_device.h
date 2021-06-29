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

#ifndef DATA_DEVICE_H
#define DATA_DEVICE_H

#include "nrp_general_library/device_interface/device_interface.h"
#include <boost/python.hpp>

template< class DATA_TYPE>
class DataDevice : public DeviceInterface
{
public:

    DataDevice(const std::string &name, const std::string &engineName, DATA_TYPE* data_)
    : DeviceInterface(createID(name, engineName)), data(data_)
    { this->setIsEmpty(false); }

    DataDevice(const std::string &name, const std::string &engineName)
    : DeviceInterface(createID(name, engineName)), data(new DATA_TYPE())
    { this->setIsEmpty(false); }

    DataDevice (const DataDevice&) = delete;
    DataDevice& operator= (const DataDevice&) = delete;

    static DeviceIdentifier createID(const std::string &name, const std::string &engineName)
    { return DeviceIdentifier(name, engineName,typeid(DATA_TYPE).name()); }

    const DATA_TYPE& getData() const
    {
        if(this->isEmpty())
            throw NRPException::logCreate("Attempt to get data from empty DataDevice " + this->name());
        else
            return *data.get();
    }

    DATA_TYPE* releaseData()
    {
        if(this->isEmpty())
            throw NRPException::logCreate("Attempt to get data from empty DataDevice " + this->name());
        else {
            this->setIsEmpty(true);
            return data.release();
        }
    }

    static void create_python(const std::string &name)
    {
        using namespace boost::python;
        class_< DataDevice<DATA_TYPE>, bases<DeviceInterface>, boost::noncopyable > binder(name.data(), init<const std::string&, const std::string& >((arg("name"), arg("engine_name"))));
        binder.add_property("data", make_function(&DataDevice<DATA_TYPE>::getData, return_internal_reference<>()));
    }

    DeviceInterfaceConstSharedPtr moveToSharedPtr() final
    {
        return typename PtrTemplates<DataDevice<DATA_TYPE>>::const_shared_ptr(new DataDevice<DATA_TYPE>(this->name(), this->engineName(), this->releaseData()));
    }

private:

    std::unique_ptr<DATA_TYPE> data;
};

#endif // DATA_DEVICE_H
