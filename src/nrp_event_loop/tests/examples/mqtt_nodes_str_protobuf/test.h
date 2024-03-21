#include <string>
#include <iostream>
#include "nrp_protobuf/dump.pb.h"

using namespace std;

bool my_function(const std::string* i1, Dump::String& o1) {
    if(i1 != nullptr) {
        cout << "got data: " << *i1 << endl;
        o1.set_string_stream(*i1);

        return true;
    }
    else {
        cout << "got null pointer" << endl;
        return false;
    }
}
