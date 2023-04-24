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

#ifndef INPUT_EDGE_H
#define INPUT_EDGE_H

#include <boost/python.hpp>

#include "nrp_event_loop/computational_graph/input_node.h"
#include "nrp_event_loop/python/functional_node.h"
#include "nrp_event_loop/computational_graph/computational_graph_manager.h"

template<class T, class T_IN>
concept INPUT_C = std::is_base_of_v<InputNode<T_IN>, T>;

/*!
 * \brief Helper template class used to implement Python input edge decorators
 */
template <class T_IN, class T_OUT, INPUT_C<T_IN> INPUT_CLASS>
class InputEdge {

public:

    InputEdge() = delete;

    /*!
     * \brief Constructor
     */
    InputEdge(std::string keyword, std::string id, std::string port, InputNodePolicies::MsgPublishPolicy msgPublishPolicy,
                    InputNodePolicies::MsgCachePolicy msgCachePolicy) :
            _keyword(std::move(keyword)), _id(std::move(id)), _port(std::move(port)), _msgPublishPolicy(std::move(msgPublishPolicy)),
            _msgCachePolicy(std::move(msgCachePolicy))
    {}

    /*!
     * \brief __call__ function in associated Python decorator
     *
     * It creates and registers an input node. Afterwards add a port to it and registers an edge
     * to 'obj'. 'obj' is expected to be a Python object wrapping a Functional Node
     */
    boost::python::object pySetup(const boost::python::object& obj)
    {
        // Register input node
        std::shared_ptr<ComputationalNode> node(makeNewNode());
        ComputationalGraphManager::getInstance().registerNode(node);
        INPUT_CLASS* iNode = dynamic_cast<INPUT_CLASS*>(node.get());
        if(!iNode)
            throw NRPException::logCreate("Error in creating Input Node: a node with the same name (\""+node->id()+"\") is already registered with a different type");

        iNode->setMsgPublishPolicy(_msgPublishPolicy);
        iNode->setMsgCachePolicy(_msgCachePolicy);
        iNode->registerOutput(_port);

        // Register edge
        if(boost::python::extract<std::shared_ptr<PythonFunctionalNode>>(obj).check()) {
            // PythonFunctionalNode case
            std::shared_ptr<PythonFunctionalNode> pyFn = boost::python::extract<std::shared_ptr<PythonFunctionalNode>>(obj);
            registerEdgePythonFN(iNode, pyFn);
        }
        else if(boost::python::extract<std::shared_ptr<FunctionalNodeBase>>(obj).check()) {
            // FunctionalNodeBase case
            std::shared_ptr<FunctionalNodeBase> pyFn = boost::python::extract<std::shared_ptr<FunctionalNodeBase>>(obj);
            registerEdgeFNBase(iNode, pyFn);
        }
        else
            throw NRPException::logCreate("InputEdge \""+node->id()+"\" was called with the wrong argument type. Argument must be a FunctionalNode object");

        // Returns FunctionalNode
        return obj;
    }

protected:

    /*!
     * \brief registers an edge between iNode and pyFn. PythonFunctionalNode case
     */
    void registerEdgePythonFN(INPUT_CLASS* iNode, std::shared_ptr<PythonFunctionalNode>& pyFn)
    {
        try {
            auto& cgm = ComputationalGraphManager::getInstance();
            if(iNode->msgPublishPolicy() == InputNodePolicies::MsgPublishPolicy::LAST)
                cgm.registerEdge<T_IN, boost::python::object>(
                        iNode->getSinglePort(_port), pyFn->getOrRegisterInput<T_IN>(_keyword));
            else
                cgm.registerEdge<std::vector<const T_IN*>, boost::python::object>(
                        iNode->getListPort(_port), pyFn->getOrRegisterInput<std::vector<const T_IN*>>(_keyword));
        }
        catch (const NRPException&) {
            throw NRPException::logCreate("An error occurred while creating graph edge to input node \""+iNode->id()+"\"");
        }
    }

    /*!
     * \brief registers an edge between iNode and pyFn. FunctionalNodeBase case
     */
    void registerEdgeFNBase(INPUT_CLASS* iNode, std::shared_ptr<FunctionalNodeBase>& pyFn)
    {
        try {
            auto& cgm = ComputationalGraphManager::getInstance();
            std::string errorMsg = "Port \"" + _keyword + "\" of Node \"" + pyFn->id() +
                                   "\" can't be connected to port \"" + _port + "\" of Node " + iNode->id() +
                                   "\". Probably there is a mismatch between the port types. Review your CG configuration.";
            if(iNode->msgPublishPolicy() == InputNodePolicies::MsgPublishPolicy::LAST) {
                auto iPort = dynamic_cast<InputPort<T_IN, T_OUT>*>(pyFn->getInputById(_keyword));
                if(!iPort)
                    throw NRPException::logCreate(errorMsg);

                cgm.registerEdge<T_IN, T_OUT>(
                        iNode->getSinglePort(_port), iPort);
            }
            else {
                auto iPort = dynamic_cast<InputPort<std::vector<const T_IN *>, std::vector<const T_OUT *>>*>(pyFn->getInputById(_keyword));
                if(!iPort)
                    throw NRPException::logCreate(errorMsg);

                cgm.registerEdge<std::vector<const T_IN *>, std::vector<const T_OUT *>>(
                        iNode->getListPort(_port), iPort);
            }
        }
        catch (const NRPException&) {
            throw NRPException::logCreate("An error occurred while creating graph edge to input node \""+iNode->id()+"\"");
        }
    }

    /*!
     * It instantiates a new node of type INPUT_CLASS
     *
     * To be implemented by any final class derived from this template class
     */
    virtual INPUT_CLASS* makeNewNode() = 0;

    std::string _keyword;
    std::string _id;
    std::string _port;

    InputNodePolicies::MsgPublishPolicy _msgPublishPolicy;
    InputNodePolicies::MsgCachePolicy _msgCachePolicy;
};

template <class T_IN, INPUT_C<T_IN> INPUT_CLASS>
using SimpleInputEdge = InputEdge<T_IN, T_IN, INPUT_CLASS>;

#endif //INPUT_EDGE_H
