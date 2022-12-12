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

#ifndef OUTPUT_SPINNAKER_NODE_H
#define OUTPUT_SPINNAKER_NODE_H

#include <nlohmann/json.hpp>

#include "nrp_event_loop/computational_graph/output_node.h"
#include "nrp_event_loop/python/output_edge.h"

#include "nrp_event_loop/nodes/spinnaker/spinnaker_proxy.h"

/*!
 * \brief OutputSpinnakerNode
 */
class OutputSpinnakerNode : public OutputNode<nlohmann::json> {
public:

    /*!
     * \brief Constructor
     */
    OutputSpinnakerNode(const std::string &id, const std::string &label,
                        bool publishFromCache = false,
                        unsigned int computePeriod = 1) :
            OutputNode(id, OutputNodePolicies::PublishFormatPolicy::SERIES, publishFromCache, 0, computePeriod),
            _label(label)
    { }

    ~OutputSpinnakerNode()
    {
        NRPSpinnakerProxy* spProxy = &(NRPSpinnakerProxy::getInstance());
        if (spProxy)
        {
            spProxy->stopSpinnaker();
        }
    }

    void configure() override
    {
        OutputNode::configure();

        NRPSpinnakerProxy* spProxy = &(NRPSpinnakerProxy::getInstance());
        if(!spProxy) {
            NRPSpinnakerProxy::resetInstance();
            spProxy = &(NRPSpinnakerProxy::getInstance());
        }

        spProxy->addSender(_label);
    }

protected:

    void sendSingleMsg(const std::string& /*id*/, const nlohmann::json* data) override
    {
        NRPSpinnakerProxy* spProxy = &(NRPSpinnakerProxy::getInstance());
        spProxy->startSpinnaker();
        spProxy->send(_label, data);
    }

    void sendBatchMsg(const std::string& /*id*/, const std::vector<const nlohmann::json*>& /*data*/) override
    {
        throw NRPException::logCreate("BATCH update policy is not supported in OutputROSEdge");
    }

private:

    std::string _label;

};

class OutputSpinnakerEdge : public SimpleOutputEdge<nlohmann::json, OutputSpinnakerNode> {

public:

    OutputSpinnakerEdge(const std::string &keyword, const std::string &label,
                        bool publishFromCache = false,
                        unsigned int computePeriod = 1) :
            SimpleOutputEdge<nlohmann::json, OutputSpinnakerNode>(keyword, label+"_output", label,
                                                                  publishFromCache, computePeriod),
            _label(label)
    {}

protected:

    OutputSpinnakerNode* makeNewNode() override
    { return new OutputSpinnakerNode(this->_id, _label, this->_publishFromCache, this->_computePeriod); }

private:

    std::string _label;
};


#endif //OUTPUT_SPINNAKER_NODE_H
