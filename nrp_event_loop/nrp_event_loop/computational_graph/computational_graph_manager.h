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

#ifndef COMPUTATION_GRAPH_MANAGER_H
#define COMPUTATION_GRAPH_MANAGER_H


#include <boost/python.hpp>
#include <nlohmann/json.hpp>

#include "nrp_general_library/utils/nrp_exceptions.h"
#include "nrp_general_library/utils/python_error_handler.h"

#include "nrp_event_loop/computational_graph/computational_graph.h"
#include "nrp_event_loop/computational_graph/computational_node.h"

#include "nrp_event_loop/computational_graph/input_port.h"
#include "nrp_event_loop/computational_graph/output_port.h"

/*!
 * \brief Singleton class managing a computational graph.
 *
 * ComputationalGraph is only concerned about the graph structure and node execution policies while ComputationalGraphManager
 * remains responsible for managing the registration of nodes and edges, establishing the actual connections between
 * ports. It owns a ComputationalGraph and all its nodes.
 */
class ComputationalGraphManager
{
public:

    virtual ~ComputationalGraphManager();

    // Delete move and copy operators. This ensures this class is a singleton
    ComputationalGraphManager(const ComputationalGraphManager &) = delete;
    ComputationalGraphManager(ComputationalGraphManager &&) = delete;

    ComputationalGraphManager &operator=(const ComputationalGraphManager &) = delete;
    ComputationalGraphManager &operator=(ComputationalGraphManager &&) = delete;

    /*!
     * \brief Get singleton instance of ComputationalGraphManager
     */
    static ComputationalGraphManager &getInstance();

    /*!
     * \brief Reset singleton instance
     */
    static ComputationalGraphManager &resetInstance();

    /*!
     * \brief Register a node in the graph
     *
     * If the node is of type 'Functional', an attempt to register a node with an 'id' already existing is considered an
     * error and an exception is thrown. If the type is different to 'Functional' the new node is not registered and
     * the reference obj is shifted to the existing node with the same 'id'
     */
    void registerNode(std::shared_ptr<ComputationalNode>& obj)
    {
        // Registers node
        if(!_nodes.count(obj->id()))
            _nodes.emplace(obj->id(), obj);
        // There is a node with the same name but different type
        else if(obj->type() != _nodes[obj->id()]->type() )
            throw NRPException::logCreate("Attempt to register Node with name \"" + obj->id() + "\". But a node with that name already exists with a different type.");
        // There is a node with the same name and type is Functional
        else if(obj->type() == ComputationalNode::Functional)
            throw NRPException::logCreate("Attempt to register Functional Node with name \"" + obj->id() + "\". But a node with that name already exists.");
        else
            obj = _nodes[obj->id()];
    }

    /*!
     * \brief Retrieve a node from the graph as a pointer
     */
    ComputationalNode* getNode(const std::string& id)
    {
        if(_nodes.count(id))
            return _nodes.at(id).get();
        else
            return nullptr;
    }

    /*!
     * \brief Connects an InputPort to an Output port and registers an edge in the graph between their parent nodes
     */
    template<class T_IN, class T_OUT>
    void registerEdge(OutputPort<T_IN>* source, InputPort<T_IN,T_OUT>* target)
    {
        if(!source || !target)
            throw NRPException::logCreate("Attempt to register edge in computational graph from nullptrs");

        if(!_nodes.count(source->parent()->id()) || !_nodes.count(target->parent()->id()))
            throw NRPException::logCreate("Attempt to register edge between nodes " + source->parent()->id() + " and " +
                target->parent()->id() + ", but they are not registered in the graph");

        // TODO: if just ComputationalGraphManager is allowed to call 'subscribeTo' it would be ensured that all
        //  port connections are reflected in the graph
        target->subscribeTo(source);

        _graph.insert_edge(source->parent(), target->parent());
    }

    /*!
     * \brief Executes ComputationalGraph
     */
    void compute()
    { _graph.compute(); }

    /*!
     * \brief Configure ComputationalGraph
     */
    void configure()
    { _graph.configure(); }

    /*!
     * \brief Resets ComputationalGraphManager
     */
    void clear()
    {
        _graph.clear();
        _nodes.clear();
    }

    void setExecMode(ComputationalGraph::ExecMode mode)
    { _graph.setExecMode(mode); }

    ComputationalGraph::ExecMode getExecMode()
    { return _graph.getExecMode(); }

private:

    ComputationalGraphManager() = default;
    static std::unique_ptr<ComputationalGraphManager> _instance;

    ComputationalGraph _graph;
    std::map<std::string, std::shared_ptr<ComputationalNode> > _nodes;
};


#endif //COMPUTATION_GRAPH_MANAGER_H
