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

#ifndef OUTPUT_ROS_NODE_H
#define OUTPUT_ROS_NODE_H

#include <boost/python.hpp>
#include <iostream>
#include <mutex>

#include "nrp_event_loop/computational_graph/output_node.h"
#include "nrp_event_loop/python/output_edge.h"

#include "nrp_ros_proxy/nrp_ros_proxy.h"

/*!
 * \brief Output node used to connect a ROS publisher to the computational graph
 *
 * The template parameter MSG_TYPE is the ROS msg type this node can publish
 */
template<class MSG_TYPE>
class OutputROSNode : public OutputNode<MSG_TYPE> {
public:

    /*!
     * \brief Constructor
     */
    OutputROSNode(const std::string &id,
                  bool publishFromCache = false,
                  unsigned int computePeriod = 1) :
            OutputNode<MSG_TYPE>(id, OutputNodePolicies::PublishFormatPolicy::SERIES, publishFromCache, 0, computePeriod)
    { }

protected:

    void sendSingleMsg(const std::string& /*id*/, const MSG_TYPE* data) override
    {
        // TODO: check that 'id' is equal to the topic address this node publishes to?

        NRPROSProxy* rosProxy = &(NRPROSProxy::getInstance());
        if(rosProxy)
            rosProxy->publish(this->id(), *data);
        else
            NRPLogger::warn("From OutputROSNode \"" + this->id() +
                            "\". NRPCoreSim is not connected to ROS and this node can't publish. Check your experiment configuration");
    }

    // TODO: implement this node in a way that supports ROS msg types that contains a field with an array of another type.
    //  Its input port will accept the type of the array field and in this function the array msg will be composed and
    //  published. Maybe a new class OutputROSArrayNode
    void sendBatchMsg(const std::string& /*id*/, const std::vector<const MSG_TYPE*>& /*data*/) override
    {
        throw NRPException::logCreate("BATCH update policy is not supported by OutputROSNode");
    }

};

template<class MSG_TYPE>
class OutputROSEdge : public SimpleOutputEdge<MSG_TYPE, OutputROSNode<MSG_TYPE>> {

public:

    OutputROSEdge(const std::string &keyword, const std::string &address,
                  bool publishFromCache = false,
                  unsigned int computePeriod = 1) :
            SimpleOutputEdge<MSG_TYPE, OutputROSNode<MSG_TYPE>>(keyword, address, address,
                    publishFromCache, computePeriod)
    {}

protected:

    OutputROSNode<MSG_TYPE>* makeNewNode() override
    { return new OutputROSNode<MSG_TYPE>(this->_id, this->_publishFromCache, this->_computePeriod); }
};


#endif //OUTPUT_ROS_NODE_H
