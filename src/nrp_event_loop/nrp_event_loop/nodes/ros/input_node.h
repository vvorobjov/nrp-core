/* * NRP Core - Backend infrastructure to synchronize simulations
 *
 * Copyright 2020-2023 NRP Team
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

#ifndef INPUT_ROS_NODE_H
#define INPUT_ROS_NODE_H

#include <boost/python.hpp>
#include <functional>
#include <memory>
#include <mutex>

#include "nrp_event_loop/computational_graph/input_node.h"
#include "nrp_event_loop/python/input_edge.h"

#include "nrp_ros_proxy/nrp_ros_proxy.h"

/*!
 * \brief Input node used to connect a ROS 2 subscriber to the computational graph
 *
 * The template parameter MSG_TYPE is the ROS 2 msg type this node subscribes
 * to (e.g. nrp_ros_msgs::msg::Test). Callbacks receive a
 * std::shared_ptr<const MSG_TYPE> as per rclcpp conventions.
 */
template<class MSG_TYPE>
class InputROSNode : public InputNode<MSG_TYPE> {
public:

    /*!
     * \brief Constructor
     */
    InputROSNode(const std::string &id) :
            InputNode<MSG_TYPE>(id)
    {}

    std::string typeStr() const override
    { return "RosSubscriber"; }

protected:

    void configure() override
    {
        // creates ROS 2 subscriber via NRPROSProxy
        std::function<void(std::shared_ptr<const MSG_TYPE>)> callback =
            [this](std::shared_ptr<const MSG_TYPE> msg) { this->topic_callback(msg); };
        NRPROSProxy* rosProxy = &(NRPROSProxy::getInstance());
        if(rosProxy)
            rosProxy->subscribe<MSG_TYPE>(this->id(), callback);
        else
            NRPLogger::warn("From InputROSNode \"" + this->id() +
                            "\". NRPCoreSim is not connected to ROS and this node can't subscribe to topics. Add \"ROSNode\" parameter to your experiment configuration");

        // reserves memory space for storing incoming msgs
        _msgTemp.reserve(InputNode<MSG_TYPE>::_queueSize);
        _msgStore.reserve(InputNode<MSG_TYPE>::_queueSize);
    }

    /*!
     * \brief callback function used in the ROS 2 subscription
     */
    void topic_callback(std::shared_ptr<const MSG_TYPE> msg)
    {
        std::lock_guard<std::mutex> lock(_msgMutex);

        // store msg pointer
        if(_msgTemp.size() < _msgTemp.capacity())
            _msgTemp.push_back(std::move(msg));
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
                InputNode<MSG_TYPE>::_portMap.at(id).addMsg(_msgStore.back().get());
            }

            _msgTemp.clear();

            return true;
        }

        return false;
    }

private:

    /*! \brief mutex used in ROS msgs access operations */
    std::mutex _msgMutex;
    /*! \brief vector storing incoming ROS 2 msgs temporarily  */
    std::vector<std::shared_ptr<const MSG_TYPE>> _msgTemp;
    /*! \brief vector storing incoming ROS 2 msgs which pointers are connected to this node ports  */
    std::vector<std::shared_ptr<const MSG_TYPE>> _msgStore;


};


template<class MSG_TYPE>
class InputROSEdge : public SimpleInputEdge<MSG_TYPE, InputROSNode<MSG_TYPE>> {

public:

    InputROSEdge(const std::string& keyword, const std::string& address,
                 InputNodePolicies::MsgPublishPolicy msgPublishPolicy,
                 InputNodePolicies::MsgCachePolicy msgCachePolicy) :
            SimpleInputEdge<MSG_TYPE, InputROSNode<MSG_TYPE>>(keyword, address, address, msgPublishPolicy, msgCachePolicy)
    {}

protected:

    InputROSNode<MSG_TYPE>* makeNewNode() override
    { return new InputROSNode<MSG_TYPE>(this->_id); }
};


#endif //INPUT_ROS_NODE_H
