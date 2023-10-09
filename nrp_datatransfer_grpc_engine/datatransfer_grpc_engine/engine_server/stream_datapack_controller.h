//
// NRP Core - Backend infrastructure to synchronize simulations
//
// Copyright 2020-2023 NRP Team
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//   http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//
// This project has received funding from the European Union’s Horizon 2020
// Framework Programme for Research and Innovation under the Specific Grant
// Agreement No. 945539 (Human Brain Project SGA3).
//

#ifndef STREAM_DATATRANSFER_GRPC_DATAPACK_CONTROLLER_SERVER_H
#define STREAM_DATATRANSFER_GRPC_DATAPACK_CONTROLLER_SERVER_H

#include "nrp_general_library/engine_interfaces/datapack_controller.h"
#include "nrp_protobuf/engine_grpc.grpc.pb.h"
#include "nrp_protobuf/dump.pb.h"
#include "nrp_protobuf/proto_ops/protobuf_ops.h"

#include "spdlog/sinks/rotating_file_sink.h"
#include "nrp_general_library/utils/nrp_logger.h"

#ifdef MQTT_ON
#include "nrp_mqtt_proxy/nrp_mqtt_client.h"

using MQTTTopicsUpdateCallbackType = std::function<void(const std::string&, const std::string&)>;
#endif

/*!
 * \brief DataPackController class for processing streaming messages
 *
 * The instance of the class in created for every DataPack to be processed by the engine.
 * Depending on the DataPack, the controller instance is configured when receiving the first message.
 */
class StreamDataPackController
    : public DataPackController<google::protobuf::Message>
{
    public:
        /*!
         * \brief Default StreamDataPackController constructor 
         *
         * \param[in] datapackName The name of the datapack
         * \param[in] engineName The engine name
         */
        StreamDataPackController(const std::string & datapackName,
                                 const std::string & engineName,
                                 const std::vector<std::unique_ptr<protobuf_ops::NRPProtobufOpsIface>>& protoOps);

#ifdef MQTT_ON
        /*!
         * \brief StreamDataPackController constructor for streaming to file and network
         *
         * \param[in] datapackName The name of the datapack
         * \param[in] engineName The engine name
         * \param[in] baseDir output data files location
         * \param[in] mqttClient initialized MQTT client pointer
         * \param[in] mqttBaseTopic the common MQTT topic name base (prefix)
         */
        StreamDataPackController(const std::string &datapackName,
                                 const std::string &engineName,
                                 const std::vector<std::unique_ptr<protobuf_ops::NRPProtobufOpsIface>>& protoOps,
                                 const std::string &baseDir,
                                 const std::shared_ptr<NRPMQTTClient> &mqttClient,
                                 const std::string &mqttBaseTopic,
                                 MQTTTopicsUpdateCallbackType topicsUpdateCallback);

        /*!
         * \brief StreamDataPackController constructor for streaming to network
         *
         * \param[in] datapackName The name of the datapack
         * \param[in] engineName The engine name
         * \param[in] mqttClient initialized MQTT client pointer
         * \param[in] mqttBaseTopic the common MQTT topic name base (prefix)
         */
        StreamDataPackController(const std::string &datapackName,
                                 const std::string &engineName,
                                 const std::vector<std::unique_ptr<protobuf_ops::NRPProtobufOpsIface>>& protoOps,
                                 const std::shared_ptr<NRPMQTTClient> &mqttClient,
                                 const std::string &mqttBaseTopic,
                                 MQTTTopicsUpdateCallbackType topicsUpdateCallback);
#endif

        /*!
         * \brief StreamDataPackController constructor for streaming to file
         *
         * \param[in] datapackName The name of the datapack
         * \param[in] engineName The engine name
         * \param[in] baseDir output data files location
         */
        StreamDataPackController(const std::string &datapackName,
                                 const std::string &engineName,
                                 const std::vector<std::unique_ptr<protobuf_ops::NRPProtobufOpsIface>>& protoOps,
                                 const std::string &baseDir);

        /*!
         * \brief Processes data coming from the transceiver function
         *
         * \param[in] data The latest data from the transceiver function
         */
        void handleDataPackData(const google::protobuf::Message &data) override;

        /*!
         * \brief Returns the newest simulation data
         *
         * The data will be passed to the engine client through gRPC.
         * There it will be wrapped in a datapack object and passed to the transceiver functions.
         *
         * \return Pointer to the latest simulation data. The returned object should NOT
         *         be cached by the controller, its destruction will be handled by the caller.
         *         nullptr can be returned when no new data is available.
         */
        google::protobuf::Message * getDataPackInformation() override;

        /*!
         * \brief Resets the existing streams of the DataPacks
         * 
         * The file data stream gets the new file with the incremented name.
         * The MQTT data stream gets the 'reset' message.
         *
         */
        void resetSinks();

    private:

        /*!
         * \brief Function for logging to file through spdlog
         *
         * \param[in] data protobuf message t obe logged
         * \param[in] fmtCallback callback function for the formatting protobuf into string
         */
        void streamToFile(const google::protobuf::Message &data, std::string (StreamDataPackController::*fmtCallback) (const google::protobuf::Message &));

        /*!
         * \brief Function for initialization of the file logger
         */
        void initFileLogger();

        /*!
         * \brief Function for formatting Dump::String protobuf to string
         *
         * \param[in] data protobuf message to be formatted
         */
        std::string fmtMessage(const google::protobuf::Message &data);

        /*!
         * \brief Function for formatting Dump::String protobuf to string
         *
         * \param[in] data protobuf message to be formatted
         */
        std::string fmtString(const google::protobuf::Message &data);

        /*!
         * \brief Dummy formatting function for callback in not specified types
         *
         * \param[in] data protobuf message to be formatted
         */
        std::string fmtDummy(const google::protobuf::Message &data);

        /*!
         * \brief Function for formatting Dump::ArrayFloat protobuf to string
         *
         * \param[in] data protobuf message to be formatted
         */
        std::string fmtFloat(const google::protobuf::Message &data);

        /*!
         * \brief Name of the datapack that is handled by this controller object
         */
        std::string _datapackName;

        /*!
         * \brief Name of the directory for the data files storage
         */
        std::string _baseDir;

        /*!
         * \brief Name of the engine to which the controller is bound
         */
        std::string _engineName;

        /*!
         * \brief spdlog logger that is used for saving data to file
         */
        std::shared_ptr<spdlog::logger> _fileLogger;

#ifdef MQTT_ON
        /*!
         * \brief mqtt topic for publishing message contents
         */
        std::string _mqttDataTopic;

        /*!
         * \brief mqtt topics name base
         */
        std::string _mqttBase;

        /*!
         * \brief mqtt topics update callback
         */
        MQTTTopicsUpdateCallbackType _topicsUpdateCallback;

        std::shared_ptr< NRPMQTTClient > _mqttClient;
#endif

        /*!
         * \brief network streaming flag
         */
        bool _netDump;

        /*!
         * \brief file streaming flag
         */
        bool _fileDump;

        /*!
         * \brief the first message flag (`true` if the first message has already been processed)
         */
        bool _initialized;

        /*!
         * \brief the counter for the resets, which is used for the data files naming
         */
        unsigned int _rstCnt;

        /*!
         * \brief true if this controller is receiving messages of type DataPackMessage
         */
        bool _isDataPackMessage = false;

        /*!
         * \brief formatting function that is used to convert protobuf to string, it is initialized when the first message is received
         */
        std::string (StreamDataPackController::*_fmtCallback) (const google::protobuf::Message &data);

        /*!
         * \brief the size of the rotating stream file (5 MB)
         */
        const unsigned int NRP_MAX_LOG_FILE_SIZE = 1024 * 1024 * 5;

        /*!
         * \brief the max number of the rotating stream files (0 - 200000)
         * A number greater than 200000 will throw an exception
         */
        const unsigned int NRP_MAX_LOG_FILE_N = 200000;

        const std::vector<std::unique_ptr<protobuf_ops::NRPProtobufOpsIface>>& _protoOps;
};

#endif // STREAM_DATATRANSFER_GRPC_DATAPACK_CONTROLLER_SERVER_H

// EOF
