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

#ifndef OUTPUT_EDGE_H
#define OUTPUT_EDGE_H

#include "nrp_event_loop/computational_graph/output_node.h"
#include "nrp_event_loop/python/functional_node.h"
#include "nrp_event_loop/computational_graph/computational_graph_manager.h"

template<class T, class T_OUT>
concept OUTPUT_C = std::is_base_of_v<OutputNode<T_OUT>, T>;

/*!
 * \brief Helper class used to implement Python output edge decorators
 */
template <class T_IN, class T_OUT, OUTPUT_C<T_OUT> OUTPUT_CLASS>
class OutputEdge {

public:

    OutputEdge() = delete;

    /*!
     * \brief Constructor
     */
    OutputEdge(std::string keyword, std::string id, std::string port,
                     bool publishFromCache,
                     unsigned int computePeriod) :
            _keyword(std::move(keyword)), _id(std::move(id)), _port(std::move(port)),
            _publishFromCache(publishFromCache),
            _computePeriod(computePeriod)
    {}

    /*!
     * \brief __call__ function in the decorator
     *
     * It creates and registers an output node. Afterwards add a port to it and registers an edge
     * from 'obj'. 'obj' is expected to be a Python object wrapping a Functional Node
     */
    boost::python::object pySetup(const boost::python::object& obj)
    {
        // Register output node
        std::shared_ptr<ComputationalNode> node(makeNewNode());
        ComputationalGraphManager::getInstance().registerNode(node);
        OUTPUT_CLASS* oNode = dynamic_cast<OUTPUT_CLASS*>(node.get());
        if(!oNode)
            throw NRPException::logCreate("When creating Output Node \""+node->id()+"\": a node with the same name "
                                                                                  "was already registered with a different type");

        // Register edge
        if(boost::python::extract<std::shared_ptr<PythonFunctionalNode>>(obj).check()) {
            // PythonFunctionalNode case
            std::shared_ptr<PythonFunctionalNode> pyFn = boost::python::extract<std::shared_ptr<PythonFunctionalNode>>(obj);
            registerEdgePythonFN(oNode, pyFn);
        }
        else if(boost::python::extract<std::shared_ptr<FunctionalNodeBase>>(obj).check()) {
            // FunctionalNodeBase case
            std::shared_ptr<FunctionalNodeBase> pyFn = boost::python::extract<std::shared_ptr<FunctionalNodeBase>>(obj);
            registerEdgeFNBase(oNode, pyFn);
        }
        else
            throw NRPException::logCreate("OutputEdge received the wrong object type");

        // Returns FunctionalNode
        return obj;
    }

protected:

    /*!
     * \brief registers an edge between oNode and pyFn. PythonFunctionalNode case
     */
    void registerEdgePythonFN(OUTPUT_CLASS* oNode, std::shared_ptr<PythonFunctionalNode>& pyFn)
    {
        try {
            InputPort<boost::python::object, T_OUT> *iPort = oNode->template getOrRegisterInput<boost::python::object>(
                    _port);

            // Check that the edge can be created before trying
            checkPortCanConnectOrThrow<boost::python::object>(iPort, oNode);

            // Register edge
            ComputationalGraphManager::getInstance().registerEdge<boost::python::object, T_OUT>(
                    pyFn->getOutput(_keyword), iPort);
        }
        catch (const NRPException&) {
            std::string error_msg = "An error occurred while creating graph edge to output node \"" + oNode->id() + "\"";
            throw NRPException::logCreate(error_msg);
        }
    }

    /*!
     * \brief registers an edge between oNode and pyFn. FunctionalNodeBase case
     */
    void registerEdgeFNBase(OUTPUT_CLASS* oNode, const std::shared_ptr<FunctionalNodeBase>& pyFn)
    {
        try {
            InputPort<T_IN, T_OUT> *iPort = oNode->template getOrRegisterInput<T_IN>(
                    _port);

            // Check that the edge can be created before trying
            checkPortCanConnectOrThrow<T_IN>(iPort, oNode);

            // Register edge
            OutputPort<T_IN>* oPort = dynamic_cast<OutputPort<T_IN>*>(pyFn->getOutputById(_keyword));
            if(!oPort)
                throw NRPException::logCreate("Port \"" + _keyword + "\" of Node \"" + pyFn->id() +
                                              "\" can't be connected to port \"" + _port + "\" of Node " + oNode->id() +
                                              "\". Probably there is a mismatch between the port types. Review your CG configuration.");

            ComputationalGraphManager::getInstance().registerEdge<T_IN, T_OUT>(
                    oPort, iPort);
        }
        catch (const NRPException&) {
            std::string error_msg = "An error occurred while creating graph edge to output node \"" + oNode->id() + "\"";
            throw NRPException::logCreate(error_msg);
        }
    }

    // Check that the port can be connected or throw an exception
    template<class T_INPUT>
    void checkPortCanConnectOrThrow(InputPort<T_INPUT, T_OUT>  *iPort,OUTPUT_CLASS* oNode)
    {
        std::string error_info = "";
        if(!iPort)
            error_info = "port couldn't be found";
        else if(iPort->subscriptionsSize() == iPort->subscriptionsMax()) {
            error_info = "the maximum limit of connections for this port has been reached";
            if(oNode->getComputePeriod() != 1)
                error_info= "the node has been configured with a compute period different than one and can only have one connection per port";
            else if(oNode->publishFromCache())
                error_info = "the node has been configured to publish from cache and can only have one connection per port";
        }

        if(!error_info.empty()) {
            std::string msg = "Error when creating graph edge to output node \"" + oNode->id() + "\", port \"" + _port + "\": " +
                              error_info + ". Check your graph definition.";

            throw NRPException::logCreate(msg);
        }
    }

    /*!
     * It instantiates a new node of type INPUT_CLASS
     *
     * To be implemented by any final class derived from this template class
     */
    virtual OUTPUT_CLASS* makeNewNode() = 0;

    std::string _keyword;
    std::string _id;
    std::string _port;
    bool _publishFromCache;
    unsigned int _computePeriod;
};

template <class T_OUT, OUTPUT_C<T_OUT> OUTPUT_CLASS>
using SimpleOutputEdge = OutputEdge<T_OUT, T_OUT, OUTPUT_CLASS>;


#endif //OUTPUT_EDGE_H
