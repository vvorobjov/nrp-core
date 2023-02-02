/* * NRP Core - Backend infrastructure to synchronize simulations
 *
 * Copyright 2020-2021 NRP Team
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * This project has received funding from the European Union’s Horizon 2020
 * Framework Programme for Research and Innovation under the Specific Grant
 * Agreement No. 945539 (Human Brain Project SGA3).
 */

#include "nrp_general_library/transceiver_function/function_manager.h"
#include "nrp_general_library/transceiver_function/transceiver_datapack_interface.h"
#include "nrp_general_library/transceiver_function/from_engine_datapack.h"

#include <boost/python.hpp>
using namespace boost;

struct TestSimpleTransceiverDataPack
        : public TransceiverDataPackInterface
{
    TestSimpleTransceiverDataPack(python::object fcn);
    virtual ~TestSimpleTransceiverDataPack() override;

    datapack_identifiers_set_t getRequestedDataPackIDs() const override
    {   return datapack_identifiers_set_t(); }

    TransceiverDataPackInterface::shared_ptr *getTFInterpreterRegistry() override;

    const std::string &linkedEngineName() const override;

    boost::python::object runTf(boost::python::tuple &args, boost::python::dict &kwargs, datapacks_set_t dataPacks) override;

    datapack_identifiers_set_t updateRequestedDataPackIDs(datapack_identifiers_set_t &&datapackIDs = datapack_identifiers_set_t()) const override;

    TransceiverDataPackInterface::shared_ptr *_tfInterpreterRegistry = nullptr;
    python::object _fcn;

    std::string _linkedEngine = "engine";
};

struct TestOutputDataPack
        : public DataPackInterface
{
    static DataPackIdentifier ID(const std::string & name = "out");

    TestOutputDataPack();

    TestOutputDataPack(const DataPackIdentifier &id)
        : DataPackInterface(id)
    {}

    virtual ~TestOutputDataPack() override;

    int TestValue = 0;
};

struct TestInputDataPack
        : public DataPackInterface
{
    static DataPackIdentifier ID();

    TestInputDataPack();

    TestInputDataPack(const DataPackIdentifier &id)
        : DataPackInterface(id)
    {}

    virtual ~TestInputDataPack() override;

    std::string TestValue;
};
