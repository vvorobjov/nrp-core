#include <boost/python.hpp>

bool forward_int(const int* i1, int& o1);
bool python_to_int(const boost::python::object* i1, int& o1);
bool int_to_python(const int* i1, boost::python::object& o1);
