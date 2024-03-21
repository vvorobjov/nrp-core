#include <string>
#include <iostream>
#include <boost/python.hpp>

using namespace std;

bool my_function(const boost::python::object* i1, boost::python::object& o1) {
    if(i1 != nullptr) {
        cout << "got data: " << std::to_string(boost::python::extract<int>(*i1)) << endl;
        o1 = *i1;
        return true;
    }
    else {
        cout << "got null pointer" << endl;
        return false;
    }
}
