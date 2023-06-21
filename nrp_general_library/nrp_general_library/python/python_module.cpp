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

#include "nrp_general_library/python/python_module.h"

#include "nrp_general_library/config/cmake_constants.h"


#include "nrp_general_library/transceiver_function/transceiver_function.h"
#include "nrp_general_library/transceiver_function/simulation_time_decorator.h"
#include "nrp_general_library/transceiver_function/simulation_iteration_decorator.h"
#include "nrp_general_library/transceiver_function/status_function.h"
#include "nrp_general_library/transceiver_function/transceiver_datapack_interface.h"
#include "nrp_general_library/transceiver_function/from_engine_datapack.h"

#include <boost/python.hpp>
#include <boost/python/numpy.hpp>

namespace python = boost::python;

/*!
 * \brief Dummy alias class for EngineDataPack, mapped to PreprocessedDataPack python decorator
 *
 * boost::python doesn't allow to map two different names (EngineDataPack and
 * PreprocessedDataPack in our case) to a single C++ class.
 * This class acts as an 'alias' for EngineDataPack and allows for two python decorators
 * to be mapped to, effectively, a single class.
 *
 * Although EngineDataPack and PreprocessedDataPack are effectively the same class, they are
 * initialized with different arguments in the python constructors.
 */
class PreprocessedDataPack
    : public EngineDataPack
{
    using EngineDataPack::EngineDataPack;
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
 * but the decorator was created for semantic clarity and possible future developments.
 */
class PreprocessingFunction
    : public TransceiverFunction
{
    using TransceiverFunction::TransceiverFunction;
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
            .def("isUpdated", &DataPackInterface::isUpdated)
            .add_property("name", make_function(&DataPackInterface::name, return_value_policy<copy_const_reference>()), &DataPackInterface::setName)
            .add_property("type", make_function(&DataPackInterface::type, return_value_policy<copy_const_reference>()), &DataPackInterface::setType)
            .add_property("engine_name", make_function(&DataPackInterface::engineName, return_value_policy<copy_const_reference>()), &DataPackInterface::setEngineName)
            .add_property("id", make_function(&DataPackInterface::id, return_value_policy<reference_existing_object>()), &DataPackInterface::setID);

    register_ptr_to_python<DataPackInterfaceSharedPtr>();
    register_ptr_to_python<DataPackInterfaceConstSharedPtr>();


    // TransceiverDataPackInterface
    class_<TransceiverDataPackInterface, boost::noncopyable>("TransceiverDataPackInterface", init<>())
            .def("__call__", &TransceiverDataPackInterface::pySetup<TransceiverDataPackInterface>);

    register_ptr_to_python<TransceiverDataPackInterface::shared_ptr>();
    register_ptr_to_python<TransceiverDataPackInterface::const_shared_ptr>();

    // SimulationTime
    class_<SimulationTimeDecorator, bases<TransceiverDataPackInterface> >("SimulationTime", init<const std::string&>( arg("keyword") ))
            .def("__call__", &TransceiverDataPackInterface::pySetup<SimulationTimeDecorator>);

    // SimulationIteration
    class_<SimulationIterationDecorator, bases<TransceiverDataPackInterface> >("SimulationIteration", init<const std::string&>( arg("keyword") ))
            .def("__call__", &TransceiverDataPackInterface::pySetup<SimulationIterationDecorator>);

    // EngineDataPack
    class_<EngineDataPack, bases<TransceiverDataPackInterface> >("EngineDataPack", init<const std::string&, const DataPackIdentifier&, bool>( (arg("keyword"), arg("id"), arg("isPreprocessed") = false) ))
            .def("__call__", &TransceiverDataPackInterface::pySetup<EngineDataPack>);

    // EngineDataPacks
    class_<EngineDataPacks, bases<TransceiverDataPackInterface> >("EngineDataPacks", init<const std::string&, const boost::python::list&, const std::string &, bool>( (arg("keyword"), arg("datapackNames"), arg("engineName"), arg("isPreprocessed") = false) ))
            .def("__call__", &TransceiverDataPackInterface::pySetup<EngineDataPacks>);

    // PreprocessedDataPack
    class_<PreprocessedDataPack, bases<EngineDataPack> >("PreprocessedDataPack", init<const std::string&, const DataPackIdentifier&, bool>( (arg("keyword"), arg("id"), arg("isPreprocessed") = true) ))
            .def("__call__", &TransceiverDataPackInterface::pySetup<PreprocessedDataPack>);

    // TransceiverFunction
    class_<TransceiverFunction, bases<TransceiverDataPackInterface> >("TransceiverFunction", init<std::string, bool>( (arg("engineName"), arg("isPreprocessing") = false) ))
            .def("__call__", &TransceiverFunction::pySetup);

    register_ptr_to_python<PtrTemplates<TransceiverFunction>::shared_ptr>();
    register_ptr_to_python<PtrTemplates<TransceiverFunction>::const_shared_ptr>();

    // PreprocessingFunction
    class_<PreprocessingFunction, bases<TransceiverDataPackInterface> >("PreprocessingFunction", init<std::string, bool>( (arg("engineName"), arg("isPreprocessing") = true) ))
            .def("__call__", &PreprocessingFunction::pySetup);

    register_ptr_to_python<PtrTemplates<PreprocessingFunction>::shared_ptr>();
    register_ptr_to_python<PtrTemplates<PreprocessingFunction>::const_shared_ptr>();

    // StatusFunction
    class_<StatusFunction, bases<TransceiverDataPackInterface> >("StatusFunction", init<>())
            .def("__call__", &StatusFunction::pySetup);

    register_ptr_to_python<PtrTemplates<StatusFunction>::shared_ptr>();
    register_ptr_to_python<PtrTemplates<StatusFunction>::const_shared_ptr>();
}
