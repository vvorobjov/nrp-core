#include "test_fn_cpp_node.h"

bool forward_int(const int* i1, int& o1) {
    if(i1 != nullptr) {
        o1 = *i1;
        return true;
    }
    else
        return false;
}

bool python_to_int(const boost::python::object* i1, int& o1) {
    if(i1 != nullptr) {
        if(boost::python::extract<int>(*i1).check()) {
            o1 = boost::python::extract<int>(*i1);
            return true;
        }
    }

    return false;
}

bool int_to_python(const int* i1, boost::python::object& o1) {
    if(i1 != nullptr) {
        o1 = boost::python::object(*i1);
        return true;
    }

    return false;
}
