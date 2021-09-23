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
#include "nrp_general_library/transceiver_function/transceiver_datapack_interface.h"
#include "nrp_general_library/transceiver_function/from_engine_datapack.h"

#include <boost/python.hpp>
#include <boost/python/numpy.hpp>

namespace python = boost::python;

using DataPackIdentifiers = EngineClientInterface::datapack_identifiers_set_t;

/*!
 * \brief Dummy alias class for FromEngineDataPack, mapped to PreprocessedDataPack python decorator
 *
 * boost::python doesn't allow to map two different names (FromEngineDataPack and
 * PreprocessedDataPack in our case) to a single C++ class.
 * This class acts as an 'alias' for FromEngineDataPack and allows for two python decorators
 * to be mapped to, effectively, a single class.
 *
 * Although FromEngineDataPack and PreprocessedDataPack are effectively the same class, they are
 * initialized with different arguments in the python constructors.
 */
class PreprocessedDataPack
    : public FromEngineDataPack
{
	using FromEngineDataPack::FromEngineDataPack;
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

struct TransceiverDataPackInterfaceWrapper
        : TransceiverDataPackInterface, python::wrapper<TransceiverDataPackInterface>
{
	python::object runTf(python::tuple &args, python::dict &kwargs) override
	{
		if(python::override runTf = this->get_override("runTf"))
			return runTf(*args, **kwargs);

		return TransceiverDataPackInterface::runTf(args, kwargs);
	}

	python::object defaultRunTf(python::tuple &args, python::dict &kwargs)
	{
		return TransceiverDataPackInterface::runTf(args, kwargs);
	}

	EngineClientInterface::datapack_identifiers_set_t getRequestedDataPackIDs() const override
	{
		if(python::override getReqIDs = this->get_override("_getRequestedDataPackIDs"))
			return getReqIDs();

		return TransceiverDataPackInterface::getRequestedDataPackIDs();
	}

	EngineClientInterface::datapack_identifiers_set_t defaultGetRequestedDataPackIDs() const
	{
		return TransceiverDataPackInterface::getRequestedDataPackIDs();
	}
};

inline std::shared_ptr<DataPackIdentifier> genDevID(const std::string &name, const std::string &engineName)
{
	return std::shared_ptr<DataPackIdentifier>(new DataPackIdentifier(name, engineName, ""));
}

inline std::shared_ptr<DataPackInterface> genDevInterface(const std::string &name, const std::string &engineName)
{
	return std::shared_ptr<DataPackInterface>(new DataPackInterface(name, engineName, ""));
}

using namespace boost::python;


BOOST_PYTHON_MODULE(PYTHON_MODULE_NAME)
{
	// DataPackIdentifier
	class_<DataPackIdentifier>("DataPackIdentifier", init<const std::string&, const std::string &, const std::string&>((arg("name"), arg("engine_name"), arg("type") = std::string())))
	        .def("__init__", make_constructor(&genDevID))
	        .def_readwrite("name", &DataPackIdentifier::Name)
	        .def_readwrite("type", &DataPackIdentifier::Type)
	        .def_readwrite("engine_name", &DataPackIdentifier::EngineName);

	register_ptr_to_python<std::shared_ptr<DataPackIdentifier> >();
	register_ptr_to_python<std::shared_ptr<const DataPackIdentifier> >();


	// DataPackInterface
	class_<DataPackInterface>("DataPackInterface", init<const std::string &, const std::string&, const std::string&>())
	        .def("__init__", make_constructor(&genDevInterface))
			.def("isEmpty", &DataPackInterface::isEmpty)
	        .add_property("name", make_function(&DataPackInterface::name, return_value_policy<copy_const_reference>()), &DataPackInterface::setName)
	        .add_property("type", make_function(&DataPackInterface::type, return_value_policy<copy_const_reference>()), &DataPackInterface::setType)
	        .add_property("engine_name", make_function(&DataPackInterface::engineName, return_value_policy<copy_const_reference>()), &DataPackInterface::setEngineName)
	        .add_property("id", make_function(&DataPackInterface::id, return_value_policy<reference_existing_object>()), &DataPackInterface::setID);

	register_ptr_to_python<DataPackInterfaceSharedPtr>();
	register_ptr_to_python<DataPackInterfaceConstSharedPtr>();


	// TransceiverDataPackInterface
	class_<TransceiverDataPackInterfaceWrapper, boost::noncopyable>("TransceiverDataPackInterface", init<>())
	        .def("__call__", &TransceiverDataPackInterface::pySetup<TransceiverDataPackInterface>)
	        .def("runTf", &TransceiverDataPackInterface::runTf, &TransceiverDataPackInterfaceWrapper::defaultRunTf)
	        .def("getRequestedDataPackIDs", &TransceiverDataPackInterface::getRequestedDataPackIDs, &TransceiverDataPackInterfaceWrapper::defaultGetRequestedDataPackIDs);

	register_ptr_to_python<TransceiverDataPackInterface::shared_ptr>();
	register_ptr_to_python<TransceiverDataPackInterface::const_shared_ptr>();


	// FromEngineDataPack
	class_<FromEngineDataPack, bases<TransceiverDataPackInterface> >("FromEngineDataPack", init<const std::string&, const DataPackIdentifier&, bool>( (arg("keyword"), arg("id"), arg("isPreprocessed") = false) ))
	        .def("__call__", &TransceiverDataPackInterface::pySetup<FromEngineDataPack>);

	// PreprocessedDataPack
	class_<PreprocessedDataPack, bases<FromEngineDataPack> >("PreprocessedDataPack", init<const std::string&, const DataPackIdentifier&, bool>( (arg("keyword"), arg("id"), arg("isPreprocessed") = true) ))
	        .def("__call__", &TransceiverDataPackInterface::pySetup<PreprocessedDataPack>);


	// TransceiverFunction
	class_<TransceiverFunction, bases<TransceiverDataPackInterface> >("TransceiverFunction", init<std::string, bool>( (arg("engineName"), arg("isPreprocessing") = false) ))
	        .def("__call__", &TransceiverFunction::pySetup)
	        .def("runTf", &TransceiverFunction::runTf);

	register_ptr_to_python<PtrTemplates<TransceiverFunction>::shared_ptr>();
	register_ptr_to_python<PtrTemplates<TransceiverFunction>::const_shared_ptr>();

	// PreprocessingFunction
	class_<PreprocessingFunction, bases<TransceiverDataPackInterface> >("PreprocessingFunction", init<std::string, bool>( (arg("engineName"), arg("isPreprocessing") = true) ))
	        .def("__call__", &PreprocessingFunction::pySetup)
	        .def("runTf", &PreprocessingFunction::runTf);

	register_ptr_to_python<PtrTemplates<PreprocessingFunction>::shared_ptr>();
	register_ptr_to_python<PtrTemplates<PreprocessingFunction>::const_shared_ptr>();
}
