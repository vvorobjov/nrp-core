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

#ifndef PROTO_OPS_H
#define PROTO_OPS_H

#include <dlfcn.h>
#include "google/protobuf/message.h"

#include "nrp_protobuf/engine_grpc.pb.h"
#include "nrp_protobuf/nrp_server.pb.h"
#include "nrp_general_library/datapack_interface/datapack.h"
#include "nrp_general_library/utils/nrp_exceptions.h"

namespace gpb = google::protobuf;

// Different conversion operations between protobuf messages and datapacks
namespace protobuf_ops {

    /*!
     * @brief Extracts the data field from a datapack protobuf msg, which is of Any type
     *
     * @tparam MSG_TYPE template param containing the list of protobuf types that will be check in the conversion
     * @tparam REMAINING_MSG_TYPES
     * @param from datapack protobuf msg
     * @return protobuf msg which was stored in 'from' data field
     */
    template<class MSG_TYPE, class ...REMAINING_MSG_TYPES>
    std::unique_ptr<gpb::Message> getDataFromDataPackMessageSubset(const EngineGrpc::DataPackMessage & from)
    {
        // If data is of MSG_TYPE type, unpack
        if(from.data().Is<MSG_TYPE>())
        {
            MSG_TYPE * dataPack = new MSG_TYPE();
            from.data().UnpackTo(dataPack);
            return std::unique_ptr<gpb::Message>(dataPack);
        }

        // Try with the remaining types or throw
        if constexpr (sizeof...(REMAINING_MSG_TYPES) > 0)
            return getDataFromDataPackMessageSubset<REMAINING_MSG_TYPES...>(from);
        else
        {
            const auto errorMessage = "Unable to unpack data from Datapack Message '" +
                                      from.datapackid().datapackname() +
                                      "' in engine '"
                                      + from.datapackid().enginename() + "'";

            throw NRPException(errorMessage);
        }
    }


    /*!
     * @brief Finds the type of protobuf msg contained in 'from' data field and creates a datapack from it
     *
     * @tparam MSG_TYPE template param containing the list of protobuf types that will be check in the conversion
     * @tparam REMAINING_MSG_TYPES
     * @param engineName owner of 'from'
     * @param from datapack protobuf msg
     * @return the instantiated datapack
     */
    template<class MSG_TYPE, class ...REMAINING_MSG_TYPES>
    // TODO: use engine name from 'from' after https://hbpneurorobotics.atlassian.net/browse/NRRPLT-8340 is resolved
    DataPackInterfaceConstSharedPtr getDataPackInterfaceFromMessageSubset(const std::string &engineName, const EngineGrpc::DataPackMessage & from)
    {
        const auto & dataPackId = from.datapackid();

        if(!from.has_data())
            return DataPackInterfaceConstSharedPtr(new DataPackInterface(dataPackId.datapackname(),
                                                                         engineName, dataPackId.datapacktype()));
        else if(from.data().Is<MSG_TYPE>())
        {
            MSG_TYPE * data = new MSG_TYPE();
            from.data().UnpackTo(data);

            return DataPackInterfaceConstSharedPtr(
                    new DataPack<MSG_TYPE>(dataPackId.datapackname(), engineName, data));
        }

        if constexpr (sizeof...(REMAINING_MSG_TYPES) > 0)
            return getDataPackInterfaceFromMessageSubset<REMAINING_MSG_TYPES...>(engineName, from);
        else
            throw NRPException::logCreate("Failed to get DataPackInterface from DataPackMessage with name \"" + dataPackId.datapackname() + "\" in engine \"" + engineName + "\"");
    }


    /*!
     * @brief Set a protobuf datapack msg data field from a protobuf message
     *
     * @tparam MSG_TYPE template param containing the list of protobuf types that will be check in the conversion
     * @tparam REMAINING_MSG_TYPES
     * @param from Protobuf message
     * @param to Protobuf datapack message which data will be set
     */
    template<class MSG_TYPE, class ...REMAINING_MSG_TYPES>
    void setDataPackMessageDataSubset(const gpb::Message & from, EngineGrpc::DataPackMessage * to)
    {
        // TODO datapackname and enginename are set outside of this function, should we do it here?
        //  To be considered as part of https://hbpneurorobotics.atlassian.net/browse/NRRPLT-8340
        // Try to cast 'from' to MSG_TYPE
        try
        {
            const auto& message = dynamic_cast<const MSG_TYPE &>(from);
            to->mutable_data()->PackFrom(message);
            return;
        }
        catch(const std::bad_cast& e)
        {
            // Do not process the exception.
            // Bad casts aren't 'bad' in this case, they simply mean that
            // we haven't found the right message type yet.
        }

        // Try with the remaining types or throw
        if constexpr (sizeof...(REMAINING_MSG_TYPES) > 0)
            return setDataPackMessageDataSubset<REMAINING_MSG_TYPES...>(from, to);
        else
        {
            const auto errorMessage = "Unable to pack data into DataPack '" +
                                      to->datapackid().datapackname() +
                                      "' in engine '" +
                                      to->datapackid().enginename() + "'";

            throw NRPException(errorMessage);
        }
    }


    /*!
     * @brief Sets the data field of a datapack protobuf message from a datapack interface
     *
     * @tparam MSG_TYPE template param containing the list of protobuf types that will be check in the conversion
     * @tparam REMAINING_MSG_TYPES
     * @param from input datapack containing data
     * @param to protobuf msg pointer which data field will be set to 'from'
     */
    template<class MSG_TYPE, class ...REMAINING_MSG_TYPES>
    void setDataPackMessageFromInterfaceSubset(const DataPackInterface & from, EngineGrpc::DataPackMessage * to)
    {
        // Try to cast 'from' to DataPack<MSG_TYPE>
        try
        {
            const auto& message = dynamic_cast<const DataPack<MSG_TYPE> &>(from);
            to->mutable_datapackid()->set_datapackname(message.name());
            to->mutable_datapackid()->set_datapacktype(message.type());
            to->mutable_datapackid()->set_enginename(message.engineName());
            to->mutable_data()->PackFrom(message.getData());

            return;
        }
        catch(const std::bad_cast& e)
        {
            // Do not process the exception.
            // Bad casts aren't 'bad' in this case, they simply mean that
            // we haven't found the right message type yet.
        }

        // Try with the remaining types or throw
        if constexpr (sizeof...(REMAINING_MSG_TYPES) > 0)
            return setDataPackMessageFromInterfaceSubset<REMAINING_MSG_TYPES...>(from, to);
        else
            throw NRPException("Failed to set DataPackMessage from DataPackInterface with name \"" + from.name() + "\"");
    }


    /*!
     * @brief Sets the data field of a datapack protobuf message from a datapack interface
     *
     * @tparam MSG_TYPE template param containing the list of protobuf types that will be check in the conversion
     * @tparam REMAINING_MSG_TYPES
     * @param from input datapack containing data
     * @param to protobuf msg pointer which data field will be set to 'from'
     */
    template<class MSG_TYPE, class ...REMAINING_MSG_TYPES>
    void setTrajectoryMessageFromInterfaceSubset(const DataPackInterface & from, NrpCore::TrajectoryMessage * to)
    {
        try
        {
            const auto& message = dynamic_cast<const DataPack<MSG_TYPE> &>(from);
            to->mutable_data()->PackFrom(message.getData());
            return;
        }
        catch(const std::bad_cast& e)
        {
            // Do not process the exception.
            // Bad casts aren't 'bad' in this case, they simply mean that
            // we haven't found the right message type yet.
        }

        if constexpr (sizeof...(REMAINING_MSG_TYPES) > 0)
            return setTrajectoryMessageFromInterfaceSubset<REMAINING_MSG_TYPES...>(from, to);
        else
            throw NRPException::logCreate("DataPack \"" + from.name() + "\" is not supported by engine '" + from.engineName() + "'");
    }


    // Interface containing conversion operations between protobuf messages and datapacks
    class NRPProtobufOpsIface {
    public:
        virtual DataPackInterfaceConstSharedPtr
        getDataPackInterfaceFromMessage(const std::string &engineName, const EngineGrpc::DataPackMessage &from) = 0;

        virtual void
        setDataPackMessageFromInterface(const DataPackInterface &from, EngineGrpc::DataPackMessage *to) = 0;

        virtual std::unique_ptr<gpb::Message> getDataFromDataPackMessage(const EngineGrpc::DataPackMessage &from) = 0;

        virtual void setDataPackMessageData(const gpb::Message &from, EngineGrpc::DataPackMessage *to) = 0;

        virtual void setTrajectoryMessageFromInterface(const DataPackInterface & from, NrpCore::TrajectoryMessage * to) = 0;
    };

    // Template class implementing NRPProtobufOpsIface
    template<class ...MSG_TYPES>
    class NRPProtobufOps : public NRPProtobufOpsIface {
    public:
        DataPackInterfaceConstSharedPtr getDataPackInterfaceFromMessage(const std::string &engineName, const EngineGrpc::DataPackMessage &from) override
        { return getDataPackInterfaceFromMessageSubset<MSG_TYPES...>(engineName, from); }

        void setDataPackMessageFromInterface(const DataPackInterface & from, EngineGrpc::DataPackMessage * to) override
        { setDataPackMessageFromInterfaceSubset<MSG_TYPES...>(from, to); }

        std::unique_ptr<gpb::Message> getDataFromDataPackMessage(const EngineGrpc::DataPackMessage& from) override
        { return getDataFromDataPackMessageSubset<MSG_TYPES...>(from); }

        void setDataPackMessageData(const gpb::Message &from, EngineGrpc::DataPackMessage *to) override
        { setDataPackMessageDataSubset<MSG_TYPES...>(from, to); }

        void setTrajectoryMessageFromInterface(const DataPackInterface & from, NrpCore::TrajectoryMessage * to) override
        {
            setTrajectoryMessageFromInterfaceSubset<MSG_TYPES...>(from, to);
        }
    };
}


// These macros are used to compile protobuf conversion libraries which can be then dynamically loaded and used in
// nrp-core

#define CREATE_PROTOBUF_OPS_FCN_STR "CreateNRPProtobufOps"

/*!
 *  \brief Create a Protobuf operations object
 */
#define CREATE_PROTOBUF_OPS(proto_ops_class)                    \
    extern "C" protobuf_ops::NRPProtobufOpsIface *CreateNRPProtobufOps ();  \
    protobuf_ops::NRPProtobufOpsIface *CreateNRPProtobufOps ()            { \
        return new proto_ops_class();                                  \
    }

#endif // PROTO_OPS_H
