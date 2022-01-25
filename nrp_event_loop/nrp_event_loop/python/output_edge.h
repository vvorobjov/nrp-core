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

#ifndef OUTPUT_EDGE_H
#define OUTPUT_EDGE_H

#include "nrp_event_loop/computational_graph/output_node.h"
#include "nrp_event_loop/python/functional_node.h"
#include "nrp_event_loop/computational_graph/computational_graph_manager.h"

template<class T, class T_OUT>
concept OUTPUT_C = std::is_base_of_v<OutputNode<T_OUT>, T>;

/*!
 * \brief Helper template class used to implement Python output edge decorators
 */
template <class T_OUT, OUTPUT_C<T_OUT> OUTPUT_CLASS>
class SimpleOutputEdge {

public:

    SimpleOutputEdge() = delete;

    /*!
     * \brief Constructor
     */
    SimpleOutputEdge(std::string keyword, std::string id, std::string port) :
            _keyword(std::move(keyword)), _id(std::move(id)), _port(std::move(port))
    {}

    /*!
     * \brief __call__ function in the decorator
     *
     * It creates and registers an output node. Afterwards add a port to it and registers an edge
     * from 'obj'. 'obj' is expected to be a Python object wrapping a PythonFunctionalNode
     */
    boost::python::object pySetup(const boost::python::object& obj)
    {
        // Register output node
        std::shared_ptr<ComputationalNode> node(makeNewNode());
        ComputationalGraphManager::getInstance().registerNode(node);
        OUTPUT_CLASS* o_node = dynamic_cast<OUTPUT_CLASS*>(node.get());
        if(!o_node)
            throw NRPException::logCreate("When creating OutputEdge: node with the same name ("+node->id()+") already registered with a different type");

        // Register edge
        try {
            std::shared_ptr<PythonFunctionalNode> f = boost::python::extract<std::shared_ptr<PythonFunctionalNode> >(
                    obj);
            InputPort<boost::python::object, T_OUT> *i_port = o_node->template getOrRegisterInput<boost::python::object>(
                    _port);
            ComputationalGraphManager::getInstance().registerEdge<boost::python::object, T_OUT>(
                    f->getOutput(_keyword), i_port);
        }
        catch (const boost::python::error_already_set&) {
            NRPLogger::error("An error occurred while creating OutputEdge. Check that Functional Node definition is correct");
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
    virtual OUTPUT_CLASS* makeNewNode() = 0;

    std::string _keyword;
    std::string _id;
    std::string _port;
};


#endif //OUTPUT_EDGE_H
