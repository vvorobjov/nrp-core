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

#ifndef COMPUTATIONAL_NODE_H
#define COMPUTATIONAL_NODE_H

#include <string>
#include <map>

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

    const static std::map<NodeType, std::string> nodeTypeStr;

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
     * \brief Returns the node 'type' as a string
     */
    virtual std::string typeStr() const
    { return this->nodeTypeStr.at(this->type()); }

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

    /*!
     * \brief Parses a computational node address returning the node id and the port (if any) contained in the address
     *
     * \param address string containing a computational graph connection address with the format /node_id/port_id
     * \param hasPort if the address contains a port id or just a node id
     * \return a pair with the node and port ids parsed from 'address'
     */
    static std::pair<std::string, std::string> parseNodeAddress(const std::string& address, bool hasPort = true)
    {
        if(address.at(0) != '/')
            throw std::invalid_argument("Error while parsing node address \""+ address +"\". Computational Graph addresses must start with '/'");

        auto n = address.find('/',1);
        if(n == std::string::npos && hasPort)
            throw std::invalid_argument("Error while parsing node address \""+ address +"\". Expected format is '/node_id/port_id'");

        auto node = hasPort ? address.substr(1, n-1) : address.substr(1);
        auto port = hasPort ? address.substr(n+1, address.size()) : node;

        return std::make_pair(node, port);
    }

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

    /*!
     * \brief Function called by the Computational Graph to nodes that the graph has been completely loaded
     */
    virtual void graphLoadedCB()
    { }

    friend class ComputationalGraph;
    friend class ComputationalGraphManager;
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
