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

#include "proto_python_bindings/proto_field_ops.h"
#include "nrp_protobuf/protobuf_utils.h"

StreamDataPackController::StreamDataPackController( const std::string & datapackName,
                                                    const std::string & engineName)
    : _datapackName(datapackName)
    , _engineName(engineName)
    , _netDump(false)
    , _fileDump(false)
    , _initialized(false)
{
    _fmtCallback = &StreamDataPackController::fmtDummy;
}

StreamDataPackController::StreamDataPackController( const std::string &datapackName,
                                                    const std::string &engineName,
                                                    const std::string &baseDir)
    : StreamDataPackController(datapackName, engineName)
{
    _fileDump = true;
    _fileLogger = spdlog::rotating_logger_mt(datapackName, baseDir + "/" + datapackName + ".data", 1048576 * 5, 3);
    _fileLogger->set_pattern("%T.%e,%v");
    _fileLogger->flush_on(spdlog::level::info);
}

#ifdef MQTT_ON
StreamDataPackController::StreamDataPackController( const std::string &datapackName,
                                                    const std::string &engineName,
                                                    std::shared_ptr<mqtt::async_client> mqttClient)
    : StreamDataPackController(datapackName, engineName)
{
    _netDump = true;
    _mqttDataTopic = new mqtt::topic(*mqttClient, "nrp/data/" + datapackName);
    _mqttTypeTopic = new mqtt::topic(*mqttClient, "nrp/data/" + datapackName + "/type", 1, true);
}

StreamDataPackController::StreamDataPackController( const std::string &datapackName,
                                                    const std::string &engineName,
                                                    const std::string &baseDir,
                                                    std::shared_ptr<mqtt::async_client> mqttClient)
    : StreamDataPackController(datapackName, engineName, baseDir)
{
    _netDump = true;
    _mqttDataTopic = new mqtt::topic(*mqttClient, "nrp/data/" + datapackName);
    _mqttTypeTopic = new mqtt::topic(*mqttClient, "nrp/data/" + datapackName + "/type", 1, true);
}
#endif

void StreamDataPackController::handleDataPackData(const google::protobuf::Message &data)
{
    std::string msg;
    if (!this->_initialized){
        msg = data.GetTypeName();

#ifdef MQTT_ON
        // Stream msg type
        if (_netDump){
            this->_mqttTypeTopic->publish(msg);
        }
#endif

        // Check EngineGrpc.DataPackMessage case. Only relevant for file dump
        if(msg == "EngineGrpc.DataPackMessage") {
            this->_isDataPackMessage = true;
            const auto& d = protobuf_utils::getDataFromDataPackMessage(
                    dynamic_cast<const EngineGrpc::DataPackMessage &>(data));
            msg = d->GetTypeName();
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
        _mqttDataTopic->publish(msg);
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
        const auto& d =protobuf_utils::getDataFromDataPackMessage(
                dynamic_cast<const EngineGrpc::DataPackMessage &>(data));
        data_str = (this->*fmtCallback)(*d);
    }
    else
        data_str = (this->*fmtCallback)(data);

    _fileLogger->info(
        "{},{}",
        fromSimulationTime<float, std::ratio<1>>(DataTransferGrpcServer::_simulationTime),
        data_str
    );
}

std::string StreamDataPackController::fmtMessage(const google::protobuf::Message &data){

    std::stringstream m_data;

    auto n = data.GetDescriptor()->field_count();
    for(int i=0;i<n;++i)
        if(!this->_initialized)
            m_data << data.GetDescriptor()->field(i)->name() << " ";
        else
            m_data << proto_field_ops::GetScalarFieldAsString(data, data.GetDescriptor()->field(i)) << " ";

    return m_data.str();
}

std::string StreamDataPackController::fmtString(const google::protobuf::Message &data){
    const auto& dump = dynamic_cast<const Dump::String &>(data);
    return  dump.string_stream();
}

std::string StreamDataPackController::fmtFloat(const google::protobuf::Message &data){
    const auto& dump = dynamic_cast<const Dump::ArrayFloat &>(data);
    std::string msg;
    if (dump.dims_size() == 0){
        msg = "0";
        for (int i = 0; i < dump.float_stream_size(); i++)
        {
            msg = msg + "," + std::to_string(dump.float_stream(i));
        }
    }
    else if (dump.dims_size() == 2){
        uint ny = dump.dims(0);
        uint nx = dump.dims(1);
        for (uint iy = 0; iy < ny; iy++){
            msg = msg + std::string("\n") + std::to_string(iy);
            for (uint ix = 0; ix < nx; ix++){
                msg = msg + "," + std::to_string(dump.float_stream(ix + iy*nx));
            }
        }
    }
    return msg;
}

std::string StreamDataPackController::fmtDummy(const google::protobuf::Message &data){
    (void) data;
    return "Type is not supported for file logging";
}

// EOF
