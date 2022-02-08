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

#ifndef OUTPUT_NODE_H
#define OUTPUT_NODE_H

#include <map>

#include "nrp_event_loop/computational_graph/computational_node_policies.h"
#include "nrp_event_loop/computational_graph/computational_node.h"
#include "nrp_event_loop/computational_graph/input_port.h"

/*!
 * \brief Implementation of an output node in the computation graph
 *
 * Output nodes are the connection point to send data out of the computation graph. In its 'compute' operation all messages
 * received since last 'compute' are sent using the 'sendSingleMsg' implemented by derived classes.
 */
template <class DATA>
class OutputNode : public ComputationalNode {
public:

    /*!
     * \brief Constructor
     */
    OutputNode(const std::string &id, OutputNodePolicies::MsgPublishPolicy msgPublishPolicy = OutputNodePolicies::MsgPublishPolicy::SERIES, int maxPortConnections = 0) :
        ComputationalNode(id, ComputationalNode::Output),
        _msgPublishPolicy(msgPublishPolicy),
        _maxPortConnections(maxPortConnections)
    { }

    void configure() override
    {
        // Allocate space for _storedMsgs. It is assumed that no new ports are registered nor existing ones are subscribed after this call.
        // To enforce the latter ports can't be accessed or registered after the node is configured.
        for(auto& [port_id, port]: _inputPorts)
            _storedMsgs[port_id].reserve(port->subscriptionsSize());

        _isConfigured = true;
    }

    virtual void compute() override final
    {
        // TODO: this loop could be possibly parallelized
        for (auto &[id, v] : _storedMsgs)
            sendMsgs(id);
    }

    /*!
     * \brief Gets or register input port to this node and returns a pointer to it
     */
    template <class T_IN>
    InputPort<T_IN, DATA>* getOrRegisterInput(const std::string& id)
    {
        InputPort<T_IN, DATA>* port = nullptr;

        if(_isConfigured) {
            std::string s = "Node '" + this->id() + "' is already configured. Ignoring request to get or register port '" + id + "'.";
            NRPLogger::info(s);
        }
        else if(_inputPorts.count(id)) {
            port = dynamic_cast<InputPort<T_IN, DATA>*>(_inputPorts.at(id).get());
            if(!port)
                throw NRPException::logCreate("In Output node '" + this->id() + "', attempt to register port '" + id +
                    "' under a different data type");
        }
        else {
            using std::placeholders::_1;
            std::function<void(const DATA *)> f = std::bind(&OutputNode::storeMsg, this, id, _1);
            port = new InputPort<T_IN, DATA>(id, this, f, _maxPortConnections);
            _inputPorts.emplace(id, std::shared_ptr<Port>(port));
        }


        return port;
    }

    OutputNodePolicies::MsgPublishPolicy msgPublishPolicy()
    { return _msgPublishPolicy; }

protected:

    /*!
     * \brief Stores a new msg in _storedMsgs['id']
     */
    void storeMsg(const std::string& id,const DATA* data)
    {
        if(data)
            _storedMsgs[id].push_back(data);
    }

    /*!
     * \brief Sends all msgs stored in _storedMsgs['id'] and clears the storage
     */
    void sendMsgs(const std::string& id)
    {
        if (this->_msgPublishPolicy == OutputNodePolicies::MsgPublishPolicy::SERIES)
            for (auto m : _storedMsgs[id])
                sendSingleMsg(id, m);
        else
            sendBatchMsg(id, _storedMsgs[id]);

        _storedMsgs[id].clear();
    }

    /*!
     * \brief Sends out a single msg, to be implemented by derived classes
     */
    virtual void sendSingleMsg(const std::string& id, const DATA* data) = 0;

    /*!
     * \brief Sends out a vector of msg as a single batch, to be implemented by derived classes
     */
    virtual void sendBatchMsg(const std::string& id, const std::vector<const DATA*>& data) = 0;

    /*! \brief List of ports owned by this node */
    std::map< std::string, std::shared_ptr<Port> > _inputPorts;
    /*! \brief List of msgs stored in this node*/
    std::map< std::string, std::vector<const DATA*>> _storedMsgs;
    /*! \brief Send policy used by this node */
    OutputNodePolicies::MsgPublishPolicy _msgPublishPolicy;
    /*! \brief Maximum number of subscriptions of ports in this node */
    int _maxPortConnections;
    /*! \brief true if the node has been configured, false otherwise */
    bool _isConfigured = false;
};


#endif //OUTPUT_NODE_H
