/* * NRP Core - Backend infrastructure to synchronize simulations
 *
 * Copyright 2020-2023 NRP Team
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

#ifndef DATA_DATAPACK_H
#define DATA_DATAPACK_H

#include "nrp_general_library/datapack_interface/datapack_interface.h"
#include "nrp_general_library/utils/nrp_exceptions.h"
#include <boost/python.hpp>

/*!
 * \brief Base datapack class
 *
 * The class must be specialized by providing a template argument. The argument
 * defines what data class will be stored in the datapack objects.
 */
template< class DATA_TYPE>
class DataPack : public DataPackInterface
{
public:

    DataPack(const std::string &name, const std::string &engineName, DATA_TYPE* data_)
    : DataPackInterface(createID(name, engineName)), data(data_)
    { this->setIsEmpty(false); }

    DataPack(const std::string &name, const std::string &engineName)
    : DataPackInterface(createID(name, engineName)), data(new DATA_TYPE())
    { this->setIsEmpty(false); }

    DataPack (const DataPack&) = delete;
    DataPack& operator= (const DataPack&) = delete;

    DataPack(DataPack&& obj) = default;
    DataPack& operator = (DataPack&&) = default;

    /*!
     * \brief Returns type of the datapack class
     *
     * The function returns type of the datapack class as string.
     * The return value is not human readable. It's an implementation-defined name
     * of the DATA_TYPE used by the datapack.
     */
    static std::string getType()
    {
        return typeid(DATA_TYPE).name();
    }

    /*!
     * \brief Creates a DataPackIdentifier object with type matching the DATA_TYPE used by the DataPack class
     *
     * \param[in] name Name of the datapack
     * \param[in] name Name of the engine to which the datapack belongs
     *
     * \return A DataPackIdentifier object, with DataPackIdentifier::Name and DataPackIdentifier::EngineName
     *         provided as arguments, and with DataPackIdentifier::Type deduced from DATA_TYPE template argument
     *         of the DataPack class.
     */
    static DataPackIdentifier createID(const std::string &name, const std::string &engineName)
    {
        return DataPackIdentifier(name, engineName, getType());
    }

    /*!
     * \brief Returns reference to data stored in the object
     *
     * The function returns a read-only reference to the data stored by the object.
     * This is the main accessor function of the DataPack object.
     *
     * \return Read-only reference to the data stored by the object
     */
    const DATA_TYPE& getData() const
    {
        if(this->isEmpty())
            throw NRPException::logCreate("Attempt to get data from empty DataPack " + this->name());
        else
            return *data.get();
    }

    /*!
     * \brief Returns a python string representation of this object content
     *
     * \return Python string
     */
    PyObject * toPythonString()
    {
        std::string dataStr = boost::python::extract<std::string>(boost::python::str(this->data.get()));
        std::string dataPackStr = "- name: '" + this->name() + "'\n- engine: '" + this->engineName() + "'\n- data:\n" + dataStr;
        return PyUnicode_FromString(dataPackStr.c_str());
    }

    static void create_python(const std::string &name)
    {
        using namespace boost::python;
        class_< DataPack<DATA_TYPE>, DataPack<DATA_TYPE> *, bases<DataPackInterface>, boost::noncopyable >
            binder(name.data(), init<const std::string&, const std::string& >((boost::python::arg("name"), boost::python::arg("engine_name"))));
        binder.add_property("data", make_function(&DataPack<DATA_TYPE>::getData, return_internal_reference<>()));
        binder.def("__str__", &DataPack<DATA_TYPE>::toPythonString);
        binder.def("getType", &DataPack<DATA_TYPE>::getType).staticmethod("getType");
    }

    DataPackInterface* clone() const override
    { return new DataPack<DATA_TYPE>(this->name(), this->engineName(), new DATA_TYPE(*data), isUpdated()); }

private:

    DataPack(const std::string &name, const std::string &engineName, DATA_TYPE* data_, bool isUpdated) :
        DataPackInterface(name, engineName, getType(), isUpdated),
        data(data_)
    {
        this->setIsEmpty(false);
    }

    std::unique_ptr<DATA_TYPE> data;
};


/*!
 * \brief Wrapper class for DataPacks with no name and no engine association
 *
 * Technically it's possible to alias the DataPack class with two different Python names,
 * for example JsonDataPack and JsonRawData, but in that case boost::python will complain about
 * duplicated converter functions. Having this thin wrapper/alias class prevents that.
 */
template< class DATA_TYPE>
class RawData : public DataPack<DATA_TYPE>
{
public:

    RawData()
    : DataPack<DATA_TYPE>("", "")
    {}

    static void create_python(const std::string &name)
    {
        using namespace boost::python;
        class_< RawData<DATA_TYPE>, RawData<DATA_TYPE> *, bases<DataPack<DATA_TYPE>>, boost::noncopyable>
            binder(name.data(), init<>());
        binder.add_property("data", make_function(&RawData<DATA_TYPE>::getData, return_internal_reference<>()));
        binder.def("__str__", &RawData<DATA_TYPE>::toPythonString);
        binder.def("getType", &RawData<DATA_TYPE>::getType).staticmethod("getType");
    }
};

#endif // DATA_DATAPACK_H
