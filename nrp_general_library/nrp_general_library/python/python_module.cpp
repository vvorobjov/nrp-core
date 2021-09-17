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

#include "nrp_general_library/python/python_module.h"

#include "nrp_general_library/config/cmake_constants.h"


#include "nrp_general_library/transceiver_function/transceiver_function.h"
#include "nrp_general_library/transceiver_function/transceiver_device_interface.h"
#include "nrp_general_library/transceiver_function/from_engine_device.h"

#include <boost/python.hpp>
#include <boost/python/numpy.hpp>

namespace python = boost::python;

using DeviceIdentifiers = EngineClientInterface::device_identifiers_set_t;

/*!
 * \brief Dummy alias class for FromEngineDevice, mapped to PreprocessedDevice python decorator
 *
 * boost::python doesn't allow to map two different names (FromEngineDevice and
 * PreprocessedDevice in our case) to a single C++ class.
 * This class acts as an 'alias' for FromEngineDevice and allows for two python decorators
 * to be mapped to, effectively, a single class.
 *
 * Although FromEngineDevice and PreprocessedDevice are effectively the same class, they are
 * initialized with different arguments in the python constructors.
 */
class PreprocessedDevice
    : public FromEngineDevice
{
	using FromEngineDevice::FromEngineDevice;
};

/*!
 * \brief Dummy alias class for TransceiverFunction, mapped to PreprocessingFunction python decorator
 *
 * boost::python doesn't allow to map two different names (TransceiverFunction and
 * PreprocessingFunction in our case) to a single C++ class.
 * This class acts as an 'alias' for TransceiverFunction and allows for two python decorators
 * to be mapped to, effectively, a single class.
 *
 * There's no special behaviour of PreprocessingFunction with respect to the regular TransceiverFunction,
 * but the decorator was created for semantical clarity and possible future developments.
 */
class PreprocessingFunction
	: public TransceiverFunction
{
	using TransceiverFunction::TransceiverFunction;
};

struct TransceiverDeviceInterfaceWrapper
        : TransceiverDeviceInterface, python::wrapper<TransceiverDeviceInterface>
{
	python::object runTf(python::tuple &args, python::dict &kwargs) override
	{
		if(python::override runTf = this->get_override("runTf"))
			return runTf(*args, **kwargs);

		return TransceiverDeviceInterface::runTf(args, kwargs);
	}

	python::object defaultRunTf(python::tuple &args, python::dict &kwargs)
	{
		return TransceiverDeviceInterface::runTf(args, kwargs);
	}

	EngineClientInterface::device_identifiers_set_t getRequestedDeviceIDs() const override
	{
		if(python::override getReqIDs = this->get_override("_getRequestedDeviceIDs"))
			return getReqIDs();

		return TransceiverDeviceInterface::getRequestedDeviceIDs();
	}

	EngineClientInterface::device_identifiers_set_t defaultGetRequestedDeviceIDs() const
	{
		return TransceiverDeviceInterface::getRequestedDeviceIDs();
	}
};

inline std::shared_ptr<DeviceIdentifier> genDevID(const std::string &name, const std::string &engineName)
{
	return std::shared_ptr<DeviceIdentifier>(new DeviceIdentifier(name, engineName, ""));
}

inline std::shared_ptr<DeviceInterface> genDevInterface(const std::string &name, const std::string &engineName)
{
	return std::shared_ptr<DeviceInterface>(new DeviceInterface(name, engineName, ""));
}

using namespace boost::python;


BOOST_PYTHON_MODULE(PYTHON_MODULE_NAME)
{
	// DeviceIdentifier
	class_<DeviceIdentifier>("DeviceIdentifier", init<const std::string&, const std::string &, const std::string&>((arg("name"), arg("engine_name"), arg("type") = std::string())))
	        .def("__init__", make_constructor(&genDevID))
	        .def_readwrite("name", &DeviceIdentifier::Name)
	        .def_readwrite("type", &DeviceIdentifier::Type)
	        .def_readwrite("engine_name", &DeviceIdentifier::EngineName);

	register_ptr_to_python<std::shared_ptr<DeviceIdentifier> >();
	register_ptr_to_python<std::shared_ptr<const DeviceIdentifier> >();


	// DeviceInterface
	class_<DeviceInterface>("DeviceInterface", init<const std::string &, const std::string&, const std::string&>())
	        .def("__init__", make_constructor(&genDevInterface))
			.def("isEmpty", &DeviceInterface::isEmpty)
	        .add_property("name", make_function(&DeviceInterface::name, return_value_policy<copy_const_reference>()), &DeviceInterface::setName)
	        .add_property("type", make_function(&DeviceInterface::type, return_value_policy<copy_const_reference>()), &DeviceInterface::setType)
	        .add_property("engine_name", make_function(&DeviceInterface::engineName, return_value_policy<copy_const_reference>()), &DeviceInterface::setEngineName)
	        .add_property("id", make_function(&DeviceInterface::id, return_value_policy<reference_existing_object>()), &DeviceInterface::setID);

	register_ptr_to_python<DeviceInterfaceSharedPtr>();
	register_ptr_to_python<DeviceInterfaceConstSharedPtr>();


	// TransceiverDeviceInterface
	class_<TransceiverDeviceInterfaceWrapper, boost::noncopyable>("TransceiverDeviceInterface", init<>())
	        .def("__call__", &TransceiverDeviceInterface::pySetup<TransceiverDeviceInterface>)
	        .def("runTf", &TransceiverDeviceInterface::runTf, &TransceiverDeviceInterfaceWrapper::defaultRunTf)
	        .def("getRequestedDeviceIDs", &TransceiverDeviceInterface::getRequestedDeviceIDs, &TransceiverDeviceInterfaceWrapper::defaultGetRequestedDeviceIDs);

	register_ptr_to_python<TransceiverDeviceInterface::shared_ptr>();
	register_ptr_to_python<TransceiverDeviceInterface::const_shared_ptr>();


	// FromEngineDevice
	class_<FromEngineDevice, bases<TransceiverDeviceInterface> >("FromEngineDevice", init<const std::string&, const DeviceIdentifier&, bool>( (arg("keyword"), arg("id"), arg("isPreprocessed") = false) ))
	        .def("__call__", &TransceiverDeviceInterface::pySetup<FromEngineDevice>);

	// PreprocessedDevice
	class_<PreprocessedDevice, bases<FromEngineDevice> >("PreprocessedDevice", init<const std::string&, const DeviceIdentifier&, bool>( (arg("keyword"), arg("id"), arg("isPreprocessed") = true) ))
	        .def("__call__", &TransceiverDeviceInterface::pySetup<PreprocessedDevice>);


	// TransceiverFunction
	class_<TransceiverFunction, bases<TransceiverDeviceInterface> >("TransceiverFunction", init<std::string, bool>( (arg("engineName"), arg("isPreprocessing") = false) ))
	        .def("__call__", &TransceiverFunction::pySetup)
	        .def("runTf", &TransceiverFunction::runTf);

	register_ptr_to_python<PtrTemplates<TransceiverFunction>::shared_ptr>();
	register_ptr_to_python<PtrTemplates<TransceiverFunction>::const_shared_ptr>();

	// PreprocessingFunction
	class_<PreprocessingFunction, bases<TransceiverDeviceInterface> >("PreprocessingFunction", init<std::string, bool>( (arg("engineName"), arg("isPreprocessing") = true) ))
	        .def("__call__", &PreprocessingFunction::pySetup)
	        .def("runTf", &PreprocessingFunction::runTf);

	register_ptr_to_python<PtrTemplates<PreprocessingFunction>::shared_ptr>();
	register_ptr_to_python<PtrTemplates<PreprocessingFunction>::const_shared_ptr>();
}
