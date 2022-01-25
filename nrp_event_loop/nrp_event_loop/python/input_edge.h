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
template <class T_IN, INPUT_C<T_IN> INPUT_CLASS>
class SimpleInputEdge {

public:

    SimpleInputEdge() = delete;

    /*!
     * \brief Constructor
     */
    SimpleInputEdge(std::string keyword, std::string id, std::string port, InputNodePolicies::MsgPublishPolicy msgPublishPolicy,
                    InputNodePolicies::MsgCachePolicy msgCachePolicy) :
            _keyword(std::move(keyword)), _id(std::move(id)), _port(std::move(port)), _msgPublishPolicy(std::move(msgPublishPolicy)),
            _msgCachePolicy(std::move(msgCachePolicy))
    {}

    /*!
     * \brief __call__ function in the decorator
     *
     * It creates and registers an input node. Afterwards add a port to it and registers an edge
     * to 'obj'. 'obj' is expected to be a Python object wrapping a PythonFunctionalNode
     */
    boost::python::object pySetup(const boost::python::object& obj)
    {
        // Register input node
        std::shared_ptr<ComputationalNode> node(makeNewNode());
        ComputationalGraphManager::getInstance().registerNode(node);
        INPUT_CLASS* i_node = dynamic_cast<INPUT_CLASS*>(node.get());
        if(!i_node)
            throw NRPException::logCreate("When creating InputEdge: node with the same name (\"+node->id()+\") already registered with a different type");

        i_node->setMsgPublishPolicy(_msgPublishPolicy);
        i_node->setMsgCachePolicy(_msgCachePolicy);
        i_node->registerOutput(_port);

        // Register edge
        try {
            std::shared_ptr<PythonFunctionalNode> f = boost::python::extract<std::shared_ptr<PythonFunctionalNode> >(
                    obj);

            auto& cgm = ComputationalGraphManager::getInstance();
            if(i_node->msgPublishPolicy() == InputNodePolicies::MsgPublishPolicy::LAST)
                cgm.registerEdge<T_IN, boost::python::object>(
                        i_node->getSinglePort(_port), f->getOrRegisterInput<T_IN>(_keyword));
            else
                cgm.registerEdge<std::vector<const T_IN*>, boost::python::object>(
                        i_node->getListPort(_port), f->getOrRegisterInput<std::vector<const T_IN*>>(_keyword));
        }
        catch (const boost::python::error_already_set&) {
            NRPLogger::error("An error occurred while creating InputEdge. Check that the Functional Node definition is correct");
            PyErr_Print();
            boost::python::throw_error_already_set();
        }

        // Returns FunctionalNode
        return obj;
    }

protected:

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

#endif //INPUT_EDGE_H
