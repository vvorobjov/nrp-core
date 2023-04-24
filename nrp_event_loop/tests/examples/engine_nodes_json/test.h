#include <string>
#include <iostream>
#include "nrp_general_library/datapack_interface/datapack.h"
#include <nlohmann/json.hpp>

using namespace std;

bool my_function(const DataPackInterface* i1, DataPackInterface*& o1) {
    if(o1)
        delete o1;

    if(i1 != nullptr && !i1->isEmpty()) {
        auto i1dp = dynamic_cast<const DataPack<nlohmann::json>*>(i1);

        cout << "got data: " << i1->name() << endl;
        auto js = new nlohmann::json(i1dp->getData());

        // Datapacks are read-only, so a new object must be instantiated every iteration
        o1 = new DataPack<nlohmann::json>("rec_datapack2", "python_2", js);

        return true;
    }
    else {
        cout << "got null pointer" << endl;
        return false;
    }
}
