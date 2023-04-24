#include <string>
#include <iostream>
#include "nrp_protobuf/mypackage.pb.h"
#include "nrp_general_library/datapack_interface/datapack.h"

using namespace std;

bool my_function(const DataPackInterface* i1, DataPackInterface*& o1) {
    if(o1)
        delete o1;

    if(i1 != nullptr && !i1->isEmpty()) {
        auto i1dp = dynamic_cast<const DataPack<MyPackage::MyMessage>*>(i1);

        cout << "got data: " << i1dp->getData().str() << endl;
        auto js = new MyPackage::MyMessage(i1dp->getData());

        // Datapacks are read-only, so a new object must be instantiated every iteration
        o1 = new DataPack<MyPackage::MyMessage>("datapack_1", "datatransfer_engine", js);

        return true;
    }
    else {
        cout << "got null pointer" << endl;
        return false;
    }
}
