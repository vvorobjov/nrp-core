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

#ifndef OUTPUT_ENGINE_NODE_H
#define OUTPUT_ENGINE_NODE_H

#include <boost/python.hpp>
#include <iostream>
#include <mutex>

#include "nrp_general_library/datapack_interface/datapack_interface.h"

#include "nrp_event_loop/computational_graph/output_node.h"
#include "nrp_event_loop/python/output_edge.h"

#include "nrp_event_loop/utils/graph_utils.h"

/*!
 * \brief Output node used to connect the computational graph with an EngineClient
 */
class OutputEngineNode : public OutputNode<DataPackInterface*> {
public:

    using DataPackInterfacePtr = DataPackInterface*;

    /*!
     * Constructor
     */
    OutputEngineNode(const std::string &id, const std::string &engineName,
                     bool publishFromCache = false,
                     unsigned int computePeriod = 1) :
            OutputNode(id, OutputNodePolicies::PublishFormatPolicy::SERIES, publishFromCache, 1, computePeriod),
            _engineName(engineName)
    { }

    /*!
     * Returns all datapacks stored in the node and clears the cache
     */
    datapacks_set_t getDataPacks()
    {
        std::lock_guard<std::mutex> lock(_dataMutex);

        datapacks_set_t dataPacks;
        for(auto &[id, dataPackRawPtr] : _dataStore)
            dataPacks.insert(DataPackInterfaceConstSharedPtr(dataPackRawPtr));
        _dataStore.clear();

        return dataPacks;
    }

protected:

    void sendSingleMsg(const std::string& id, const DataPackInterfacePtr * data) override
    {
        std::lock_guard<std::mutex> lock(_dataMutex);

        // OutputNode already checks for nullptr, but since we have a double pointer here an extra check is needed
        if(!(*data))
            return;
        if(id != (*data)->name())
            NRPLogger::info("In OutputEngineNode '" + this->_engineName + "'. Datapack with Id '" + (*data)->name() +
            "' was sent to port '" + id + "' and will not be accepted due to this mismatch. Please check your graph configuration ");
        else if(_engineName != (*data)->engineName())
            NRPLogger::info("In OutputEngineNode '" + this->_engineName + "'. Received datapack with Id '" + (*data)->name() +
                            "' linked to Engine '" + (*data)->engineName() + "'. This node only accept datapacks linked to Engine '" +
                            this->_engineName +"'. Please check your graph configuration ");
        else
            // Datapacks are copied to preserve graph integrity and to ensure that the pointer is valid in next graph cycles
            _dataStore[(*data)->name()] = (*data)->clone();
    }

    void sendBatchMsg(const std::string& /*id*/, const std::vector<const DataPackInterfacePtr*>& /*data*/) override
    {
        throw NRPException::logCreate("BATCH update policy is not supported in OutputEngineNode");
    }

private:

    /*! \brief name of the Engine this node is connected to */
    std::string _engineName;
    /*! \brief mutex used in datapacks access operations */
    std::mutex _dataMutex;
    /*! \brief map storing datapacks received by this node */
    std::map<std::string, DataPackInterface*> _dataStore;

};

class OutputEngineEdge : public SimpleOutputEdge<DataPackInterface*, OutputEngineNode> {

public:

    OutputEngineEdge(const std::string &keyword, const std::string &address) :
            SimpleOutputEdge(keyword, extractNodePortFromAddress(address).first+"_output",
                             extractNodePortFromAddress(address).second, false, 1),
            _engineName(extractNodePortFromAddress(address).first)
    {}

protected:

    OutputEngineNode* makeNewNode() override
    { return new OutputEngineNode(this->_id, _engineName); }

private:

    std::string _engineName;
};


#endif //OUTPUT_ENGINE_NODE_H
