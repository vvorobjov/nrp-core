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

#ifndef SINGLE_TRANSCEIVER_DATAPACK_H
#define SINGLE_TRANSCEIVER_DATAPACK_H

#include "nrp_general_library/transceiver_function/transceiver_datapack_interface.h"

/*!
 * \brief Class for input datapacks for transceiver functions, mapped to EngineDataPack python decorator
 */
class EngineDataPack
    : public TransceiverDataPackInterface
{
    public:
        EngineDataPack(const std::string &keyword, const DataPackIdentifier &datapackID, bool isPreprocessed);
        virtual ~EngineDataPack() override = default;

        datapack_identifiers_set_t getRequestedDataPackIDs() const override;

        boost::python::object runTf(boost::python::tuple &args, boost::python::dict &kwargs, datapacks_set_t dataPacks) override;

    private:

        std::string _keyword;
        DataPackIdentifier _datapackID;
        bool _isPreprocessed;
};

/*!
 * \brief Class for input multiple datapacks for a single transceiver function, mapped to EngineDataPacks python decorator
 */
class EngineDataPacks
        : public TransceiverDataPackInterface
{
public:
    EngineDataPacks(const std::string &keyword, const boost::python::list &datapackListNames, const std::string &engineName, bool isPreprocessed);
    virtual ~EngineDataPacks() override = default;

    datapack_identifiers_set_t getRequestedDataPackIDs() const override;

    boost::python::object runTf(boost::python::tuple &args, boost::python::dict &kwargs, datapacks_set_t dataPacks) override;

private:

    std::string _keyword;
    std::vector<DataPackIdentifier> _datapacksIDs;
    std::string _engineName;
    bool _isPreprocessed;
};

#endif // SINGLE_TRANSCEIVER_DATAPACK_H
