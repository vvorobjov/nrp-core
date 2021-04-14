#include <boost/python.hpp>
#include <boost/python/suite/indexing/vector_indexing_suite.hpp>

#include "nrp_general_library/config/cmake_constants.h"
#include "nrp_general_library/device_interface/device_python_wrappers.h"
#include "nrp_example_engine/config/cmake_constants.h"
#include "nrp_example_engine/devices/example_device.h"


namespace python = boost::python;

BOOST_PYTHON_MODULE(PYTHON_MODULE_NAME)
{
	// Import the base Python module
	python::import(PYTHON_MODULE_NAME_STR);

	// Create a Python class for MyDevice type
    python::class_<typename MyDevice::MyVector>("MyVec")
            .def(python::vector_indexing_suite<typename MyDevice::MyVector>());

    python_property_device_class<MyDevice>::create();
}
