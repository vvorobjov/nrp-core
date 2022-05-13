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

#ifndef INPUT_SPINNAKER_NODE_H
#define INPUT_SPINNAKER_NODE_H

#include <vector>
#include <nlohmann/json.hpp>

#include "nrp_event_loop/computational_graph/input_node.h"
#include "nrp_event_loop/python/input_edge.h"

#include "nrp_event_loop/nodes/spinnaker/spinnaker_proxy.h"


/*!
 * \brief InputSpinnakerNode
 */
class InputSpinnakerNode : public InputNode<nlohmann::json>, public SpiNNakerJsonReceiveCallbackInterface {
public:

    /*!
     * \brief Constructor
     */
    InputSpinnakerNode(const std::string &id, const std::string &label) :
            InputNode(id),
            _label(label)
    {}

    ~InputSpinnakerNode()
    {
        NRPSpinnakerProxy* spProxy = &(NRPSpinnakerProxy::getInstance());
        if (spProxy)
        {
            spProxy->stopSpinnaker();
        }
    }

    void configure() override
    {
        NRPSpinnakerProxy* spProxy = &(NRPSpinnakerProxy::getInstance());
        if(!spProxy) {
            NRPSpinnakerProxy::resetInstance();
            spProxy = &(NRPSpinnakerProxy::getInstance());
        }

        spProxy->addReceiver(_label, this);

        // TODO: no need to worry about this now, restore in final version
        // reserves memory space for storing incoming msgs.
//        _msgTemp.reserve(InputNode::_queueSize);
//        _msgStore.reserve(InputNode::_queueSize);
    }


protected:

    /*!
     * \brief Called when raw UDP data is received
     */
    void new_msg_callback(nlohmann::json msg)
    {
        std::lock_guard<std::mutex> lock(_msgMutex);
        _msgTemp.push_back(msg);
    }

    bool updatePortData(const std::string& id) override
    {
        NRPSpinnakerProxy* spProxy = &(NRPSpinnakerProxy::getInstance());
        spProxy->startSpinnaker();
        std::lock_guard<std::mutex> lock(_msgMutex);

        if(_msgTemp.size()) {
            _msgStore.clear();
            InputNode::_portMap.at(id).clear();

            for(auto & msg : _msgTemp) {
                _msgStore.push_back(std::move(msg));
                InputNode::_portMap.at(id).addMsg(&_msgStore.back());
            }

            _msgTemp.clear();

            return true;
        }

        return false;
    }

private:

    /*! \brief mutex used in msgs access operations */
    std::mutex _msgMutex;
    /*! \brief vector storing incoming msgs temporarily  */
    std::vector<nlohmann::json> _msgTemp;
    /*! \brief vector storing incoming msgs which pointers are connected to this node ports  */
    std::vector<nlohmann::json> _msgStore;

    std::string _label;


};


class InputSpinnakerEdge : public SimpleInputEdge<nlohmann::json, InputSpinnakerNode> {

public:

    InputSpinnakerEdge(const std::string& keyword, const std::string& label,
                       InputNodePolicies::MsgPublishPolicy msgPublishPolicy,
                       InputNodePolicies::MsgCachePolicy msgCachePolicy) :
        SimpleInputEdge<nlohmann::json, InputSpinnakerNode>(keyword, label+"_input", label,  msgPublishPolicy, msgCachePolicy),
        _label(label)
    {}

protected:

    InputSpinnakerNode* makeNewNode() override
    { return new InputSpinnakerNode(this->_id, _label); }

private:

    std::string _label;
};


#endif //INPUT_SPINNAKER_NODE_H
