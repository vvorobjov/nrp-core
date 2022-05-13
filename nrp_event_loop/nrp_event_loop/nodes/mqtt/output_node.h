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

#ifndef OUTPUT_MQTT_NODE_H
#define OUTPUT_MQTT_NODE_H

#include <mutex>
#include <functional>
#include "nlohmann/json.hpp"
#include <google/protobuf/message.h>

#include "nrp_event_loop/computational_graph/output_node.h"
#include "nrp_event_loop/python/output_edge.h"

#include "nrp_mqtt_proxy/nrp_mqtt_proxy.h"

/*!
 * \brief Output node used to connect an MQTT publisher to the computational graph
 *
 * The template parameter MSG_TYPE is the MQTT msg type this node can publish
 */
template<class MSG_TYPE>
class OutputMQTTNode : public OutputNode<MSG_TYPE> {
public:

    /*!
     * \brief Constructor
     */
    OutputMQTTNode(const std::string &id, const std::string &address) :
            OutputNode<MSG_TYPE>(id),
            _address(address)
    { }

protected:

    void sendSingleMsg(const std::string& /*id*/, const MSG_TYPE* data) override
    {
        // TODO: check that 'id' is equal to the topic address this node publishes to?

        NRPMQTTProxy* mqttProxy = &(NRPMQTTProxy::getInstance());
        if(mqttProxy) {
            if constexpr (std::is_same_v<MSG_TYPE, std::string>)
                mqttProxy->publish(_address, *data);
            else if constexpr (std::is_same_v<MSG_TYPE, nlohmann::json>)
                mqttProxy->publish(_address, data->dump());
            else if constexpr (std::is_base_of_v<google::protobuf::Message, MSG_TYPE>) {
                if(data->SerializeToString(&_msg))
                    mqttProxy->publish(_address, _msg);
                else
                    NRPLogger::warn("From OutputMQTTNode \""+ this->id() +"\". Failed to serialize protobuf message");
            }
            else
                throw NRPException::logCreate("OutputMQTTNode \""+ this->id() +"\" was instantiated with a non supported type");
        }
        else
            NRPLogger::warn("From OutputMQTTNode \"" + this->id() +
                            "\". NRPCoreSim is not connected to MQTT and this node can't publish. Check your experiment configuration");
    }
    
    void sendBatchMsg(const std::string& /*id*/, const std::vector<const MSG_TYPE*>& /*data*/) override
    {
        throw NRPException::logCreate("BATCH update policy is not supported by OutputMQTTNode");
    }

private:

    /*! \brief address of the MQTT topic this node connects to */
    std::string _address;
    /*! \brief msg to be published. Here to be used when converting from protobuf */
    std::string _msg;

};

template<class MSG_TYPE>
class OutputMQTTEdge : public SimpleOutputEdge<MSG_TYPE, OutputMQTTNode<MSG_TYPE>> {

public:

    OutputMQTTEdge(const std::string &keyword, const std::string &address) :
            SimpleOutputEdge<MSG_TYPE, OutputMQTTNode<MSG_TYPE>>(keyword, address+"_output", address),
            _address(address)
    {}

protected:

    OutputMQTTNode<MSG_TYPE>* makeNewNode() override
    { return new OutputMQTTNode<MSG_TYPE>(this->_id, _address); }

    std::string _address;
};


#endif //OUTPUT_MQTT_NODE_H
