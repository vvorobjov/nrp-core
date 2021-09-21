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

#ifndef DEVICE_INTERFACE_H
#define DEVICE_INTERFACE_H

#include "nrp_general_library/utils/ptr_templates.h"

#include <string>
#include <string_view>
#include <memory>
#include <type_traits>


/*!
 * \brief Identifies a single device
 */
struct DeviceIdentifier
{
    public:
	/*!
	 * \brief Device Name. Used by simulator to identify source/sink of device
	 */
	std::string Name;

	/*!
	 * \brief Corresponding engine
	 */
	std::string EngineName;

	/*!
	 * \brief Device Type
	 */
	std::string Type;

	DeviceIdentifier() = default;

	DeviceIdentifier(const std::string &_name, const std::string &_engineName, const std::string &_type);

	DeviceIdentifier(std::string &&_name, std::string &&_engineName, std::string &&_type)
	    : Name(std::forward<std::string>(_name)),
	      EngineName(std::forward<std::string>(_engineName)),
	      Type(std::forward<std::string>(_type))
	{}

	bool operator == (const DeviceIdentifier & rhs) const {
	    return (Name == rhs.Name)
     && (EngineName == rhs.EngineName)
     && (Type == rhs.Type);
	}

	bool operator < (const DeviceIdentifier & rhs) const{
	    return (Name < rhs.Name) || (Name == rhs.Name && EngineName < rhs.EngineName) || (Name == rhs.Name &&
	    EngineName == rhs.EngineName && Type < rhs.Type);
	}

};


/*!
 * \brief Interface to devices
 */
class DeviceInterface
        : public PtrTemplates<DeviceInterface>
{
	public:
		DeviceInterface() = default;

		template<class DEV_ID_T>
		DeviceInterface(DEV_ID_T &&id)
		    : _id(std::forward<DEV_ID_T>(id)){
		        static_assert(std::is_same_v<std::remove_reference_t<DEV_ID_T>, const DeviceIdentifier> || std::is_same_v< DeviceIdentifier, std::remove_reference_t<DEV_ID_T> >,"Parameter DEV_ID_T must be type DeviceIdentifier or DeviceIdentifier&");
		        static_assert(std::is_same_v<std::remove_reference_t<DEV_ID_T>, DeviceIdentifier> || std::is_same_v< const DeviceIdentifier, std::remove_reference_t<DEV_ID_T> >,"Parameter DEV_ID_T must be type DeviceIdentifier or DeviceIdentifier&");
		    }

		DeviceInterface(const std::string &name, const std::string &engineName, const std::string &type);
		virtual ~DeviceInterface() = default;

		const std::string &name() const;
		void setName(const std::string &name);

		const std::string &type() const;
		void setType(const std::string &type);

		const std::string &engineName() const;
		void setEngineName(const std::string &engineName);

		const DeviceIdentifier &id() const;
		void setID(const DeviceIdentifier &id);

		virtual DeviceInterface::const_shared_ptr moveToSharedPtr()
		{
			return DeviceInterface::const_shared_ptr(new DeviceInterface(std::move(this->_id)));
		}

		/*!
		 * \brief Indicates if the device contains any data aside from device ID
		 *
		 * The function will return true, if the device is of DeviceInterface type, which
		 * contains only device ID. For any concrete implementation of Device class, it
		 * should return false.
		 */
		bool isEmpty() const;

	protected:

		void setIsEmpty(bool value);

	private:
		/*!
		 * \brief Identifies Device. Contains name and type of this device
		 */
		DeviceIdentifier _id;

		/*!
		 * \brief Indicates if the device contains any data aside from device ID
		 */
		bool _isEmpty = true;
};

using DeviceInterfaceSharedPtr      = DeviceInterface::shared_ptr;
using DeviceInterfaceConstSharedPtr = DeviceInterface::const_shared_ptr;


#endif // DEVICE_INTERFACE_H
