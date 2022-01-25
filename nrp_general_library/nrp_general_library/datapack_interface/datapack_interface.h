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

#ifndef DATAPACK_INTERFACE_H
#define DATAPACK_INTERFACE_H

#include "nrp_general_library/utils/ptr_templates.h"

#include <string>
#include <string_view>
#include <memory>
#include <type_traits>


/*!
 * \brief Identifies a single datapack
 */
struct DataPackIdentifier
{
    public:
    /*!
     * \brief DataPack Name. Used by simulator to identify source/sink of datapack
     */
    std::string Name;

    /*!
     * \brief Corresponding engine
     */
    std::string EngineName;

    /*!
     * \brief DataPack Type
     */
    std::string Type;

    DataPackIdentifier() = default;

    DataPackIdentifier(const std::string &_name, const std::string &_engineName, const std::string &_type);

    DataPackIdentifier(std::string &&_name, std::string &&_engineName, std::string &&_type)
        : Name(std::forward<std::string>(_name)),
          EngineName(std::forward<std::string>(_engineName)),
          Type(std::forward<std::string>(_type))
    {}

    bool operator == (const DataPackIdentifier & rhs) const {
        return (Name == rhs.Name)
     && (EngineName == rhs.EngineName)
     && (Type == rhs.Type);
    }

    bool operator < (const DataPackIdentifier & rhs) const{
        return (Name < rhs.Name) || (Name == rhs.Name && EngineName < rhs.EngineName) || (Name == rhs.Name &&
        EngineName == rhs.EngineName && Type < rhs.Type);
    }

};


/*!
 * \brief Interface to datapacks
 */
class DataPackInterface
        : public PtrTemplates<DataPackInterface>
{
    public:
        DataPackInterface() = default;

        template<class DEV_ID_T>
        DataPackInterface(DEV_ID_T &&id)
            : _id(std::forward<DEV_ID_T>(id)){
                static_assert(std::is_same_v<std::remove_reference_t<DEV_ID_T>, const DataPackIdentifier> || std::is_same_v< DataPackIdentifier, std::remove_reference_t<DEV_ID_T> >,"Parameter DEV_ID_T must be type DataPackIdentifier or DataPackIdentifier&");
                static_assert(std::is_same_v<std::remove_reference_t<DEV_ID_T>, DataPackIdentifier> || std::is_same_v< const DataPackIdentifier, std::remove_reference_t<DEV_ID_T> >,"Parameter DEV_ID_T must be type DataPackIdentifier or DataPackIdentifier&");
            }

        DataPackInterface(const std::string &name, const std::string &engineName, const std::string &type);
        virtual ~DataPackInterface() = default;

        const std::string &name() const;
        void setName(const std::string &name);

        const std::string &type() const;
        void setType(const std::string &type);

        const std::string &engineName() const;
        void setEngineName(const std::string &engineName);

        const DataPackIdentifier &id() const;
        void setID(const DataPackIdentifier &id);

        virtual DataPackInterface::const_shared_ptr moveToSharedPtr()
        {
            return DataPackInterface::const_shared_ptr(new DataPackInterface(std::move(this->_id)));
        }

        /*!
        * \brief Virtual clone method to support polymorphic copy
        */
        virtual DataPackInterface* clone() const
        { return new DataPackInterface(this->name(), this->engineName(), this->type()); }

        /*!
         * \brief Indicates if the datapack contains any data aside from datapack ID
         *
         * The function will return true, if the datapack is of DataPackInterface type, which
         * contains only datapack ID. For any concrete implementation of DataPack class, it
         * should return false.
         */
        bool isEmpty() const;

    protected:

        void setIsEmpty(bool value);

    private:
        /*!
         * \brief Identifies DataPack. Contains name and type of this datapack
         */
        DataPackIdentifier _id;

        /*!
         * \brief Indicates if the datapack contains any data aside from datapack ID
         */
        bool _isEmpty = true;
};

using DataPackInterfaceSharedPtr      = DataPackInterface::shared_ptr;
using DataPackInterfaceConstSharedPtr = DataPackInterface::const_shared_ptr;


#endif // DATAPACK_INTERFACE_H
