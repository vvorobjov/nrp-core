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
#include "nrp_general_library/utils/nrp_exceptions.h"
#include <boost/python.hpp>


/*!
 * \brief Base device class
 *
 * The class must be specialized by providing a template argument. The argument
 * defines what data class will be stored in the device objects.
 */
template< class DATA_TYPE>
class Device : public DeviceInterface
{
public:

    Device(const std::string &name, const std::string &engineName, DATA_TYPE* data_)
    : DeviceInterface(createID(name, engineName)), data(data_)
    { this->setIsEmpty(false); }

    Device(const std::string &name, const std::string &engineName)
    : DeviceInterface(createID(name, engineName)), data(new DATA_TYPE())
    { this->setIsEmpty(false); }

    Device (const Device&) = delete;
    Device& operator= (const Device&) = delete;

    /*!
     * \brief Returns type of the device class
     *
     * The function returns type of the device class as string.
     * The return value is not human readable. It's an implementation-defined name
     * of the DATA_TYPE used by the device.
     */
    static std::string getType()
    {
        return typeid(DATA_TYPE).name();
    }

    /*!
     * \brief Creates a DeviceIdentifier object with type matching the DATA_TYPE used by the Device class
     *
     * \param[in] name Name of the device
     * \param[in] name Name of the engine to which the device belongs
     *
     * \return A DeviceIdentifier object, with DeviceIdentifier::Name and DeviceIdentifier::EngineName
     *         provided as arguments, and with DeviceIdentifier::Type deduced from DATA_TYPE template argument
     *         of the Device class.
     */
    static DeviceIdentifier createID(const std::string &name, const std::string &engineName)
    {
        return DeviceIdentifier(name, engineName, getType());
    }

    /*!
     * \brief Returns reference to data stored in the object
     *
     * The function returns a read-only reference to the data stored by the object.
     * This is the main accessor function of the Device object.
     *
     * \return Read-only reference to the data stored by the object
     */
    const DATA_TYPE& getData() const
    {
        if(this->isEmpty())
            throw NRPException::logCreate("Attempt to get data from empty Device " + this->name());
        else
            return *data.get();
    }

    /*!
     * \brief Releases ownership of the data stored in the object and returns a raw pointer to the data
     *
     * The caller is responsible for destruction of the released data.
     *
     * \return Raw pointer to the data stored in the object
     */
    DATA_TYPE* releaseData()
    {
        if(this->isEmpty())
            throw NRPException::logCreate("Attempt to get data from empty Device " + this->name());
        else {
            this->setIsEmpty(true);
            return data.release();
        }
    }

    static void create_python(const std::string &name)
    {
        using namespace boost::python;
        class_< Device<DATA_TYPE>, Device<DATA_TYPE> *, bases<DeviceInterface>, boost::noncopyable >
            binder(name.data(), init<const std::string&, const std::string& >((boost::python::arg("name"), boost::python::arg("engine_name"))));
        binder.add_property("data", make_function(&Device<DATA_TYPE>::getData, return_internal_reference<>()));
    }

    DeviceInterfaceConstSharedPtr moveToSharedPtr() final
    {
        return typename PtrTemplates<Device<DATA_TYPE>>::const_shared_ptr(new Device<DATA_TYPE>(this->name(), this->engineName(), this->releaseData()));
    }

private:

    std::unique_ptr<DATA_TYPE> data;
};

#endif // DATA_DEVICE_H
