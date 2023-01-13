//
// NRP Core - Backend infrastructure to synchronize simulations
//
// Copyright 2020-2021 NRP Team
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

#include "datatransfer_grpc_engine/engine_server/stream_datapack_controller.h"
#include "datatransfer_grpc_engine/engine_server/datatransfer_grpc_server.h"

#include "spdlog/sinks/rotating_file_sink.h"
#include "nrp_general_library/utils/time_utils.h"

#include "nrp_protobuf/proto_python_bindings/proto_field_ops.h"
#include "nrp_protobuf/proto_ops/proto_ops_manager.h"

StreamDataPackController::StreamDataPackController( const std::string & datapackName,
                                                    const std::string & engineName,
                                                    const std::vector<std::unique_ptr<protobuf_ops::NRPProtobufOpsIface>>& protoOps)
    : _datapackName(datapackName)
    , _engineName(engineName)
    , _netDump(false)
    , _fileDump(false)
    , _initialized(false)
    , _rstCnt(0)
    , _protoOps(protoOps)
{
    _fmtCallback = &StreamDataPackController::fmtDummy;
}

StreamDataPackController::StreamDataPackController( const std::string &datapackName,
                                                    const std::string &engineName,
                                                    const std::vector<std::unique_ptr<protobuf_ops::NRPProtobufOpsIface>>& protoOps,
                                                    const std::string &baseDir)
    : StreamDataPackController(datapackName, engineName, protoOps)
{
    _fileDump = true;
    _baseDir = baseDir;
    this->initFileLogger();
}

#ifdef MQTT_ON
StreamDataPackController::StreamDataPackController( const std::string &datapackName,
                                                    const std::string &engineName,
                                                    const std::vector<std::unique_ptr<protobuf_ops::NRPProtobufOpsIface>>& protoOps,
                                                    const std::shared_ptr<NRPMQTTClient> &mqttClient,
                                                    const std::string &mqttBaseTopic)
    : StreamDataPackController(datapackName, engineName, protoOps)
{
    _netDump = true;
    _mqttClient = mqttClient;
    this->_mqttBase = std::string(mqttBaseTopic);
    _mqttDataTopic = std::string(this->_mqttBase + "/data/" + datapackName);
    _mqttTypeTopic = std::string(this->_mqttBase + "/data/" + datapackName + "/type");
    // announce topic
    _mqttClient->publish(this->_mqttBase + "/data", _mqttDataTopic, true);
}

StreamDataPackController::StreamDataPackController( const std::string &datapackName,
                                                    const std::string &engineName,
                                                    const std::vector<std::unique_ptr<protobuf_ops::NRPProtobufOpsIface>>& protoOps,
                                                    const std::string &baseDir,
                                                    const std::shared_ptr<NRPMQTTClient> &mqttClient,
                                                    const std::string &mqttBaseTopic)
    : StreamDataPackController(datapackName, engineName, protoOps, mqttClient, mqttBaseTopic)
{
    _fileDump = true;
    _baseDir = baseDir;
    this->initFileLogger();
}
#endif

void StreamDataPackController::initFileLogger()
{
    std::string filename = this->_baseDir + "/" + _datapackName + "-" + std::to_string(this->_rstCnt) + ".data";
    _fileLogger = spdlog::rotating_logger_mt(_datapackName, filename, NRP_MAX_LOG_FILE_SIZE, NRP_MAX_LOG_FILE_N);
    _fileLogger->set_pattern("%v");
    _fileLogger->flush_on(spdlog::level::info);
    NRPLogger::debug("DataPack {} is streaming into the file {}", this->_datapackName, filename);
}

void StreamDataPackController::resetSinks()
{
    if (this->_fileDump){
        NRPLogger::debug("Resetting the file stream of the DataPack {}...", this->_datapackName);
        _fileLogger->flush();
        spdlog::drop(_datapackName);
        // Increment the reset counter and switch file directory if we step over max value
        (this->_rstCnt++ < std::numeric_limits<unsigned int>::max()) ? "OK" : this->_baseDir += "-next";
        this->initFileLogger();
        NRPLogger::info("The file stream of the DataPack {} is reset.", this->_datapackName);
    }
#ifdef MQTT_ON
    if (_netDump){
        _mqttClient->publish(_mqttDataTopic, "reset");
        NRPLogger::info("The MQTT stream of the DataPack {} got reset message.", this->_datapackName);
    }
#endif
}

void StreamDataPackController::handleDataPackData(const google::protobuf::Message &data)
{
    std::string msg;
    if (!this->_initialized) {
        msg = data.GetTypeName();

#ifdef MQTT_ON
        // Stream msg type
        if (_netDump){
            _mqttClient->publish(_mqttTypeTopic, msg, true);
        }
#endif

        // Check EngineGrpc.DataPackMessage case. Only relevant for file dump
        if(msg == "EngineGrpc.DataPackMessage") {
            this->_isDataPackMessage = true;
            auto dataPack = dynamic_cast<const EngineGrpc::DataPackMessage &>(data);
            bool isFound = false;
            for(auto& mod : _protoOps) {
                try {
                    const auto& d = mod->getDataFromDataPackMessage(dataPack);
                    msg = d->GetTypeName();
                    isFound = true;
                }
                catch (NRPException &) {
                    // this just means that the module couldn't process the request, try with the next one
                }
            }

            // Check if the extraction operation succeeded
            if(!isFound)
                throw NRPException::logCreate("Unable to unpack data from DataPack '" +
                                              dataPack.datapackid().datapackname() +
                                              "' in engine '" +
                                              dataPack.datapackid().enginename() + "'");
        }

        // Check message type case. Only relevant for file dump
        if (_fileDump){
            if (msg == "Dump.String") {
                _fmtCallback = &StreamDataPackController::fmtString;
            }
            else if (msg == "Dump.ArrayFloat") {
                _fmtCallback = &StreamDataPackController::fmtFloat;            
            }
            else {
                _fmtCallback = &StreamDataPackController::fmtMessage;
                // log msg field names
                this->streamToFile(data, this->_fmtCallback);
            }
        }

        this->_initialized = true;
    }

#ifdef MQTT_ON
    if (_netDump){
        data.SerializeToString(&msg);
        _mqttClient->publish(_mqttDataTopic, msg);
    }
#endif

    // In order to access the data from the message, you need to cast it to the proper type
    if (_fileDump)
        this->streamToFile(data, this->_fmtCallback);

}

google::protobuf::Message * StreamDataPackController::getDataPackInformation()
{
    return nullptr;
}

void StreamDataPackController::streamToFile(const google::protobuf::Message &data, std::string (StreamDataPackController::*fmtCallback) (const google::protobuf::Message&))
{
    std::string data_str;
    if(this->_isDataPackMessage)
    {
        auto dataPack = dynamic_cast<const EngineGrpc::DataPackMessage &>(data);
        bool isFound = false;
        for(auto& mod : _protoOps) {
            try {
                const auto& d = mod->getDataFromDataPackMessage(dataPack);
                data_str = (this->*fmtCallback)(*d);
                isFound = true;
            }
            catch (NRPException &) {
                // this just means that the module couldn't process the request, try with the next one
            }
        }

        // Check if the extraction operation succeeded
        if(!isFound)
            throw NRPException::logCreate("Unable to unpack data from DataPack '" +
                                          dataPack.datapackid().datapackname() +
                                          "' in engine '" +
                                          dataPack.datapackid().enginename() + "'");
    }
    else
        data_str = (this->*fmtCallback)(data);

    _fileLogger->info(data_str);
}

std::string StreamDataPackController::fmtMessage(const google::protobuf::Message &data){

    std::stringstream m_data;

    if(!this->_initialized)
        m_data << "sim_time" << ",";
    else
        m_data << fromSimulationTime<float, std::ratio<1>>(DataTransferGrpcServer::_simulationTime) << ",";

    auto n = data.GetDescriptor()->field_count();
    for(int i = 0; i < n; ++i)
        if(!this->_initialized)
            m_data << data.GetDescriptor()->field(i)->name() << ",";
        else
            m_data << proto_field_ops::GetScalarFieldAsString(data, data.GetDescriptor()->field(i)) << ",";

    return m_data.str().substr(0, m_data.str().size()-1);
}

std::string StreamDataPackController::fmtString(const google::protobuf::Message &data){
    const auto& dump = dynamic_cast<const Dump::String &>(data);
    return  std::to_string(fromSimulationTime<float, std::ratio<1>>(DataTransferGrpcServer::_simulationTime)) +
            + "," + dump.string_stream();
}

std::string StreamDataPackController::fmtFloat(const google::protobuf::Message &data){
    const auto& dump = dynamic_cast<const Dump::ArrayFloat &>(data);
    std::string msg;
    if (dump.dims_size() != 2){
        msg = "0";
        for (int i = 0; i < dump.float_stream_size(); i++)
        {
            msg = msg + "," + std::to_string(dump.float_stream(i));
        }
    }
    else if (dump.dims_size() == 2){
        uint ny = dump.dims(0);
        uint nx = dump.dims(1);
        int i = 0;
        for (uint iy = 0; iy < ny; iy++){
            msg = msg + std::string("\n") + std::to_string(iy);
            for (uint ix = 0; (ix < nx) && (i < dump.float_stream_size()); ix++, i++){
                msg = msg + "," + std::to_string(dump.float_stream(ix + iy*nx));
            }
        }
    }

    return  std::to_string(fromSimulationTime<float, std::ratio<1>>(DataTransferGrpcServer::_simulationTime)) +
            + "," + msg;
}

std::string StreamDataPackController::fmtDummy(const google::protobuf::Message &data){
    (void) data;
    return "Type is not supported for file logging";
}

// EOF
