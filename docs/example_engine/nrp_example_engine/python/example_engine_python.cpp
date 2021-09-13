#include <boost/python.hpp>

#include "nrp_general_library/config/cmake_constants.h"
#include "nrp_example_engine/config/cmake_constants.h"


namespace python = boost::python;

BOOST_PYTHON_MODULE(PYTHON_MODULE_NAME)
{
	// Import the base Python module
	python::import(PYTHON_MODULE_NAME_STR);
}
