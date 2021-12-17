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

#ifndef INPUT_ENGINE_NODE_H
#define INPUT_ENGINE_NODE_H

#include <boost/python.hpp>
#include <mutex>

#include "nrp_general_library/datapack_interface/datapack_interface.h"
#include "nrp_general_library/engine_interfaces/engine_client_interface.h"

#include "nrp_event_loop/computational_graph/input_node.h"
#include "nrp_event_loop/python/input_edge.h"

#include "nrp_event_loop/utils/graph_utils.h"

/*!
 * \brief Input node used to connect an EngineClient with the computational graph
 *
 * It has two methods 'requestedDataPacks' and 'setDataPacks' which allows to update the node externally with the latest
 * datapacks
 */
class InputEngineNode : public InputNode<DataPackInterface> {
public:

    /*!
     * Constructor
     */
    InputEngineNode(const std::string &id, const std::string &engineName) :
            InputNode(id, MsgPublishPolicy::LAST),
            _engineName(engineName)
    {}

    /*!
     * Get the set of DataPackInterfaces that this node requests
     */
    const std::set<DataPackIdentifier>& requestedDataPacks() const
    { return _dataIds; }

    /*!
     * Set datapacks to be published into the graph in the next call to 'compute'
     */
    void setDataPacks(EngineClientInterface::datapacks_set_t dpacks)
    {
        std::lock_guard<std::mutex> lock(_dataMutex);

        // move datapacks into temporary storage without copying the shared pointer
        while(!dpacks.empty()) {
            auto n = dpacks.extract(dpacks.begin());
            if(this->_portMap.count(n.value()->name()) &&
               (!_dataTemp.count(n.value()->name()) || !n.value()->isEmpty()))
                _dataTemp[n.value()->name()] = std::move(n.value());
        }
    }

    void configure() override
    {
        for(auto& [id, port] : _portMap)
            _dataIds.insert(DataPackIdentifier(id, _engineName, ""));
    }

protected:

    bool updatePortData(const std::string& id) override
    {
        std::lock_guard<std::mutex> lock(_dataMutex);

        // move temp datapack to store without copying shared pointer and update pointer
        auto d = _dataTemp.find(id);
        if(d != _dataTemp.end() && (!_dataStore.count(id) || !_dataTemp.at(id)->isEmpty())) {
            _portMap.at(id).clear();

            _dataStore[id] = std::move(d->second);
            _portMap.at(id).addMsg(_dataStore[id].get());

            _dataTemp.erase(d);

            return true;
        }

        return false;
    }

private:
    
    /*! \brief name of the Engine this node is connected to */
    std::string _engineName;
    /*! \brief mutex used in datapacks access operations */
    std::mutex _dataMutex;
    /*! \brief map storing datapacks inserted by 'setDataPacks'  */
    std::map<std::string, DataPackInterfaceConstSharedPtr> _dataTemp;
    /*! \brief map storing datapacks which pointers are connected to this node ports  */
    std::map<std::string, DataPackInterfaceConstSharedPtr> _dataStore;
    /*! \brief set of datapacks identifiers requested by this node  */
    std::set<DataPackIdentifier> _dataIds;
};


class InputEngineEdge : public SimpleInputEdge<DataPackInterface, InputEngineNode> {

public:

    InputEngineEdge(const std::string& keyword, const std::string& address) :
            SimpleInputEdge(keyword, extractNodePortFromAddress(address).first+"_input", extractNodePortFromAddress(address).second),
            _engineName(extractNodePortFromAddress(address).first)
    {}

protected:

    InputEngineNode* makeNewNode() override
    { return new InputEngineNode(this->_id, _engineName); }

private:

    std::string _engineName;
};


#endif //INPUT_ENGINE_NODE_H
