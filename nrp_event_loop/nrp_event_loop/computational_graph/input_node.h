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

#ifndef INPUT_NODE_H
#define INPUT_NODE_H

#include <memory>
#include <map>
#include <iostream>

#include "nrp_general_library/utils/nrp_logger.h"

#include "nrp_event_loop/computational_graph/computational_node_policies.h"
#include "nrp_event_loop/computational_graph/computational_node.h"
#include "nrp_event_loop/computational_graph/output_port.h"

/*!
 * \brief Helper structure managing data and ports associated with a port id
 */
template<class DATA>
struct DataPortHandle {

    DataPortHandle() = delete;

    /*!
     * \brief Constructor
     */
    DataPortHandle(const std::string& id, ComputationalNode* parent, size_t queue_size) :
            singlePort(new OutputPort<DATA>(id, parent)),
            listPort(new OutputPort<std::vector<const DATA*>>(id, parent))
    { _data.reserve(queue_size); }

    /*!
     * \brief Publish last item in data
     */
    void publishLast()
    {
        if(_data.size())
            singlePort->publish(_data.back());
    }

    /*!
     * \brief Publish all items in data
     */
    void publishAll()
    {
        if(_data.size())
            listPort->publish(&_data);
    }

    /*!
     * \brief Publish a null pointer
     */
    void publishNullandClear()
    {
        singlePort->publish(nullptr);
        listPort->publish(nullptr);
        _data.clear();
    }

    /*!
     * \brief Add a new message to the stored data
     */
    bool addMsg(const DATA* msg)
    {
        if(_data.size() < _data.capacity()) {
            _data.push_back(msg);
            return true;
        }
        else {
            std::string s = "Port handle '" + singlePort->id() + "' reached max size. Added msg will be dropped.";
            NRPLogger::info(s);
            return false;
        }
    }

    /*!
     * \brief Clear data
     */
    void clear()
    { _data.clear(); }

    /*!
     * \brief Return the size of stored data
     */
    size_t size()
    { return _data.size(); }

    /*! \brief Port used to send a single msg */
    std::shared_ptr<OutputPort<DATA>> singlePort;
    /*! \brief Port used to send a list of msgs */
    std::shared_ptr<OutputPort<std::vector<const DATA*>>> listPort;

private:

    /*! \brief Data stored by this handle*/
    std::vector<const DATA*> _data;
};

/*!
 * \brief Implementation of an input node in the computation graph.
 *
 * Input nodes are the connection points to feed data into the computation graph from outside. It owns output ports
 * which are used to forward incoming data to other nodes. One node can handle multiple ports. The class is templated
 * with the data type the node can handle. Each node implementation can handle only one data type.
 *
 * Derived classes of InputNode remains responsible for the ownership of the data passed to InputNode through the
 * updatePortData() virtual method.
 */
template <class DATA>
class InputNode : public ComputationalNode {
public:

    /*!
     * \brief Constructor
     */
    InputNode(const std::string &id, InputNodePolicies::MsgPublishPolicy msgPublishPolicy = InputNodePolicies::MsgPublishPolicy::LAST, InputNodePolicies::MsgCachePolicy msgCachePolicy = InputNodePolicies::MsgCachePolicy::KEEP_CACHE, size_t queue_size = 10) :
            ComputationalNode(id, ComputationalNode::Input),
            _msgPublishPolicy(msgPublishPolicy),
            _msgCachePolicy(msgCachePolicy),
            _queueSize(queue_size)
    { }

    /*!
     * \brief Registers an Output port with id 'id' with this node
     */
    void registerOutput(const std::string& id)
    {
        if(!_portMap.count(id))
            _portMap.emplace(id, DataPortHandle<DATA>(id, this, _queueSize));
        else {
            std::string s = "A port with id '" + id + "' is already registered with node '" + this->id() +
                    "', ignoring this request.";
            NRPLogger::info(s);
        }
    }

    /*!
     * \brief Returns a pointer to single output port if the port is registered, nullptr otherwise
     */
    OutputPort<DATA>* getSinglePort(const std::string& id)
    { return _portMap.count(id) ? _portMap.at(id).singlePort.get() : nullptr; }

    /*!
     * \brief Returns a pointer to list output port if the port is registered, nullptr otherwise
     */
    OutputPort<std::vector<const DATA*>>* getListPort(const std::string& id)
    { return _portMap.count(id) ? _portMap.at(id).listPort.get() : nullptr; }

    InputNodePolicies::MsgPublishPolicy msgPublishPolicy()
    { return _msgPublishPolicy; }

    InputNodePolicies::MsgCachePolicy msgCachePolicy()
    { return _msgCachePolicy; }

    void setMsgPublishPolicy(InputNodePolicies::MsgPublishPolicy msgPublishPolicy)
    { _msgPublishPolicy = msgPublishPolicy; }

    void setMsgCachePolicy(InputNodePolicies::MsgCachePolicy msgCachePolicy)
    { _msgCachePolicy = msgCachePolicy; }

protected:

    /*!
     * \brief Compute. Updates and sends stored msgs.
     */
    void compute() override final
    {
        for(auto& [id, port]: _portMap) {
            auto hasNewMsgs = this->updatePortData(id);

            if(hasNewMsgs) {
                if(_msgPublishPolicy == InputNodePolicies::MsgPublishPolicy::LAST)
                    port.publishLast();
                else
                    port.publishAll();
            }
            else if(_msgCachePolicy == InputNodePolicies::MsgCachePolicy::CLEAR_CACHE)
                port.publishNullandClear();
        }
    }

    /*!
     * \brief Updates pointers stored in _portMap for port 'id'
     *
     * Expected behavior:
     * Implementations of this function must guarantee that data pointers stored in _portMap[id] are valid and will not
     * change between calls to this function. Even after the function is called again, pointers in _portMap[id] should
     * still be valid and remain unchanged if the function returns false, i.e., if port 'id' data wasn't changed.
     *
     * @param id key in _portMap which data is asked to be updated
     * @return true if port 'id' has new data, false otherwise
     */
    virtual bool updatePortData(const std::string& id) = 0;

    /*! \brief Send policy used by this node */
    InputNodePolicies::MsgPublishPolicy _msgPublishPolicy;
    /*! \brief Msg cache policy used by this node */
    InputNodePolicies::MsgCachePolicy _msgCachePolicy;
    /*! \brief Map containing data to handle topics. Data is guaranteed to be unchanged between 'compute' calls  */
    std::map<std::string, DataPortHandle<DATA>> _portMap;
    /*! \brief Maximum number of msgs that the node can store per port */
    size_t _queueSize;

    friend class ComputationalNodes_INPUT_NODE_UPDATE_POLICY_WITH_KEEP_CACHE_Test;
    friend class ComputationalNodes_INPUT_NODE_UPDATE_POLICY_WITH_CLEAR_CACHE_Test;
};


#endif //INPUT_NODE_H
