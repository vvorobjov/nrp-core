#include "test_fn_cpp_node.h"

bool my_function(const int* i1, int& o1) {
    if(i1 != nullptr) {
        o1 = *i1;
        return true;
    }
    else
        return false;
}
