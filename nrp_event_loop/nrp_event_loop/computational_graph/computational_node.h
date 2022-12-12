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

#ifndef COMPUTATIONAL_NODE_H
#define COMPUTATIONAL_NODE_H

#include <string>

/*!
 * \brief Base class implementing a node in the computational graph
 */
class ComputationalNode {
public:

    /*! \brief All the possible node types */
    enum NodeType {
        Input, /*!< only can be source in edges  */
        Output, /*!< only can be target in edges  */
        Functional /*!< can be source and target, ie. can receive inputs and send outputs  */
    };

    ComputationalNode() = delete;
    virtual ~ComputationalNode() = default;

    /*!
     * \brief Constructor
     */
    ComputationalNode(std::string id, NodeType type) :
    _id(std::move(id)),
    _type(type)
    { }

    /*!
     * \brief Returns the node 'id'
     */
    const std::string& id() const
    { return this->_id; }

    /*!
     * \brief Returns the node 'type'
     */
    NodeType type() const
    { return this->_type; }

    /*!
     * \brief Sets a value for the node 'visited' property, used for graph traversing
     */
    void setVisited(bool visited)
    { this->_visited = visited; }

    /*!
     * \brief Returns true if the node has been marked as visited, false otherwise
     */
    bool isVisited() const
    { return this->_visited; }

    /*!
     * \brief Sets a value for the node 'doCompute' property, used in some graph execution modes
     */
    void setDoCompute(bool doCompute)
    { _doCompute = doCompute; }

    /*!
     * \brief Tells if this node should be executed in this graph execution cycle, used in some graph execution modes
     *
     * ComputationalNode returns the value of its 'doCompute' property. Subclasses of ComputationalNode can
     * implement further logic that can override this value
     */
    virtual bool doCompute() const
    { return _doCompute; }

protected:

    /*!
     * \brief Configures the node making it ready to execute 'compute'
     */
    virtual void configure() = 0;

    /*!
     * \brief Requests the node to execute its computation
     */
    virtual void compute() = 0;

    /*!
     * \brief Function called by the Computational Graph at the beginning of a new execution cycle
     */
    virtual void graphCycleStartCB()
    { }

    friend class ComputationalGraph;
    friend class ComputationalGraphPythonNodes_PYTHON_DECORATORS_BASIC_Test;

private:

    /*! \brief Node unique identifier */
    std::string _id;
    /*! \brief Node type */
    NodeType _type;
    /*! \brief Visited */
    bool _visited = false;
    /*! \brief Flag storing whether this node should be executed this cycle */
    bool _doCompute = false;
};

#endif //COMPUTATIONAL_NODE_H
