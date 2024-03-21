#include <string>
#include <iostream>
#include <boost/python.hpp>

using namespace std;

// Convert a python object wrapping 'int' to an 'int'
bool from_python(const boost::python::object* i1, int& o1) {
    // Check if 'i1' is null
    if(i1 != nullptr) {
        // Check if 'i1' is convertible to the expected type
        if(boost::python::extract<int>(*i1).check()) {
            // Just copy the content of 'i1' into output 'o1'
            o1 = boost::python::extract<int>(*i1);
            return true;
        }
        else
            std::cout << "Argument Python object 'i1' is not convertible to 'int' in function 'from_python_node'" << std::endl;
    }

    return false;
}

// Convert 'int' to python object
bool to_python(const int* i1, boost::python::object& o1) {
    // Check if 'i1' is null
    if(i1 != nullptr) {
        // Just copy the content of 'i1' into output 'o1'
        o1 = boost::python::object(*i1);
        return true;
    }

    return false;
}

// Print a python object containg 'int'
bool print_python_int(const boost::python::object* i1) {
    // Check if 'i1' is null
    if(i1 != nullptr) {
        // Just copy 'i1' into 'o1'
        if(boost::python::extract<int>(*i1).check())
            std::cout <<  boost::python::extract<int>(*i1) << std::endl;
        return true;
    }

    return false;
}
