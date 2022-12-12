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

#ifndef INPUT_MQTT_NODE_H
#define INPUT_MQTT_NODE_H

#include <mutex>
#include <functional>
#include "nlohmann/json.hpp"
#include <google/protobuf/message.h>

#include "nrp_event_loop/computational_graph/input_node.h"
#include "nrp_event_loop/python/input_edge.h"

#include "nrp_mqtt_proxy/nrp_mqtt_proxy.h"

/*!
 * \brief Input node used to connect a ROS subscriber to the computational graph
 * 
 * The template parameter MSG_TYPE is the ROS msg type this node subscribes to
 */
template<class MSG_TYPE>
class InputMQTTNode : public InputNode<MSG_TYPE> {
public:

    /*!
     * \brief Constructor
     */
    InputMQTTNode(const std::string &id, const std::string &address) :
            InputNode<MSG_TYPE>(id),
            _address(address)
    {}


protected:

    void configure() override
    {
        // Set msg conversion function
        if constexpr (std::is_same_v<MSG_TYPE, std::string>)
            _msgFromString = [] (const std::string& s) { return s; };
        else if constexpr (std::is_same_v<MSG_TYPE, nlohmann::json>)
            _msgFromString = [] (const std::string& s) {
                try {
                    return nlohmann::json::parse(s);
                }
                catch(const std::exception &e) {
                    std::stringstream error_msg;
                    error_msg << "Error in InputMQTTNode while converting incoming msg into json format: " << e.what();
                    throw NRPException::logCreate(error_msg.str());
                }
            };
        else if constexpr (std::is_base_of_v<google::protobuf::Message, MSG_TYPE>)
            _msgFromString = [] (const std::string& s) {
                MSG_TYPE m;
                if(m.ParseFromString(s))
                    return m;
                else
                    throw NRPException::logCreate("Error in InputMQTTNode while converting incoming msg into protobuf format");
            };
        else
            throw NRPException::logCreate("InputMQTTNode \""+ this->id() +"\" was instantiated with a non supported type");

        // Subscribes to mqtt topic
        NRPMQTTProxy* mqttProxy = &(NRPMQTTProxy::getInstance());
        if(mqttProxy) {
            using std::placeholders::_1;
            _callback = std::bind(&InputMQTTNode::topic_callback, this, _1);
            mqttProxy->subscribe(_address, _callback);
        }
        else
            NRPLogger::warn("From InputMQTTNode \"" + this->id() +
                            "\". NRPCoreSim is not connected to MQTT and this node can't subscribe to topics. Check your experiment configuration");

        // reserves memory space for storing incoming msgs
        _msgTemp.reserve(InputNode<MSG_TYPE>::_queueSize);
        _msgStore.reserve(InputNode<MSG_TYPE>::_queueSize);
    }

    /*!
     * \brief callback function used in the MQTT subscriber
     */
    void topic_callback(const std::string& msg)
    {
        std::lock_guard<std::mutex> lock(_msgMutex);

        // store msg pointer
        if(_msgTemp.size() < _msgTemp.capacity())
            _msgTemp.push_back(_msgFromString(msg));
        else
            NRPLogger::debug("'"+this->id()+"' node capacity is full. Dropping message...");
    }

    bool updatePortData(const std::string& id) override
    {
        std::lock_guard<std::mutex> lock(_msgMutex);

        // TODO: check that 'id' is equal to the topic address this node subscribes to?

        if(_msgTemp.size()) {
            _msgStore.clear();
            InputNode<MSG_TYPE>::_portMap.at(id).clear();

            for(auto & msg : _msgTemp) {
                _msgStore.push_back(std::move(msg));
                InputNode<MSG_TYPE>::_portMap.at(id).addMsg(&_msgStore.back());
            }

            _msgTemp.clear();

            return true;
        }

        return false;
    }

private:

    /*! \brief mutex used in msg access operations */
    std::mutex _msgMutex;
    /*! \brief vector storing incoming msgs temporarily  */
    std::vector<MSG_TYPE> _msgTemp;
    /*! \brief vector storing incoming msgs which pointers are connected to this node ports  */
    std::vector<MSG_TYPE> _msgStore;
    /*! \brief address of the MQTT topic this node connects to */
    std::string _address;
    /*! \brief callback for new msgs */
    std::function<void (const std::string&)> _callback;
    /*! \brief converts msgs in string format to MSG_TYPE */
    std::function<MSG_TYPE (const std::string&)> _msgFromString;
};


template<class MSG_TYPE>
class InputMQTTEdge : public SimpleInputEdge<MSG_TYPE, InputMQTTNode<MSG_TYPE>> {

public:

    InputMQTTEdge(const std::string& keyword, const std::string& address,
                 InputNodePolicies::MsgPublishPolicy msgPublishPolicy,
                 InputNodePolicies::MsgCachePolicy msgCachePolicy) :
            SimpleInputEdge<MSG_TYPE, InputMQTTNode<MSG_TYPE>>(keyword, address+"_input", address, msgPublishPolicy, msgCachePolicy),
            _address(address)
    {}

protected:

    InputMQTTNode<MSG_TYPE>* makeNewNode() override
    { return new InputMQTTNode<MSG_TYPE>(this->_id, _address); }

    std::string _address;
};


#endif //INPUT_MQTT_NODE_H
