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

#ifndef PYTHON_FUNCTIONAL_NODE_H
#define PYTHON_FUNCTIONAL_NODE_H

#include <boost/python.hpp>
#include <boost/python/stl_iterator.hpp>

#include "nrp_general_library/utils/nrp_exceptions.h"
#include "nrp_general_library/utils/python_error_handler.h"

#include "nrp_event_loop/utils/graph_utils.h"

#include "nrp_event_loop/computational_graph/functional_node.h"

#include "nrp_event_loop/computational_graph/computational_graph_manager.h"

namespace bpy = boost::python;

// Helper struct to define a tuple type with N elements of type T
template <typename T, size_t N>
struct tuple_array {
    typedef decltype(std::tuple_cat(std::tuple<T>(), typename tuple_array<T, N-1>::type())) type;
};

template <typename T>
struct tuple_array<T, 0> {
    typedef decltype(std::tuple<>()) type;
};

/*!
 * \brief Specialization of FunctionalNode in which _function is a python callable object
 *
 * _function can have a variable number of inputs and outputs between 0 and input_s/output_s
 */
class PythonFunctionalNode : public FunctionalNode<typename tuple_array<bpy::object, 10>::type, typename tuple_array<bpy::object, 10>::type> {

    // TODO: publicly inheriting from FunctionalNode allows to bypass the safety checks when registering port in PythonFunctionalNode

    // TODO: PythonFunctionalNode should be a template with parameter size_t N = number of inputs and outputs. When doing this
    //  FunctionalNode template is not inferred properly. After figuring this out, instances of '3' should be replaced with N

    const static size_t input_s = 10;
    const static size_t output_s = 10;

public:

    /*!
     * \brief Constructor
     */
    PythonFunctionalNode(const std::string &id, const boost::python::list& o_ports, FunctionalNodePolicies::ExecutionPolicy policy = FunctionalNodePolicies::ExecutionPolicy::ON_NEW_INPUT) :
        FunctionalNode(id, [] (params_t&) {}, policy)
    {
        bpy::stl_input_iterator<std::string> begin(o_ports), end;
        _oPortIds.insert(_oPortIds.begin(), begin, end);
    }

    /*!
     * \brief Configure this node
     */
    void configure() override
    {
        // Create edges to other functional nodes
        for (auto& [port_id, address]: _f2fEdges) {
            std::string name, property;
            std::tie(name, property) = extractNodePortFromAddress(address);

            // Get ports
            PythonFunctionalNode* node = dynamic_cast<PythonFunctionalNode*>(ComputationalGraphManager::getInstance().getNode(name));
            if(!node)
                throw NRPException::logCreate("While creating the F2F edge '" + address +
                "'. A Functional node with name '" + name + "' could not be found in the computational graph. Be sure that the edge"
                                                            " address is correctly formatted and the connected node exists.");

            OutputPort<bpy::object>* o_port = node->getOutput(property);
            if(!o_port)
                throw NRPException::logCreate("While creating the F2F edge '" + address +
                                              "'. Functional node '" + name + "' doesn't have a declared output '"+ property +"'. Be sure that the edge"
                                                                                          " address is correctly formatted and the specified output exists.");

            InputPort<bpy::object, bpy::object>* i_port = this->getOrRegisterInput<bpy::object>(port_id);

            // Register edge
            ComputationalGraphManager::getInstance().registerEdge<bpy::object, bpy::object>(o_port, i_port);
        }

        // check unbound inputs and print warning
        for(size_t i=0; i < _iPortIds.size(); ++i)
            if(!getInputByIndex(i)) {
                std::stringstream s;
                s << "In python functional node \"" << this->id() << "\". Input argument \"" << _iPortIds[i] <<
                  "\" is not connected" << std::endl;
                NRPLogger::info(s.str());
            }
    }

    /*!
     * \brief Setup this node with a python callable object and registers it to ComputationalGraphManager
     *
     * After calling this function the object is moved into a shared pointer which is returned by the function.
     * Afterwards this object shouldn't be used. Use the returned shared pointer.
     *
     * It is meant to be used as __call__ function in a Python decorator which creates a PythonFunctionalNode from a given
     * Python function
     */
    boost::python::object pySetup(boost::python::object obj)
    {
        // Create heap allocated shared pointer from 'this' and set it up
        std::shared_ptr<PythonFunctionalNode> self = this->moveToSharedPtr();
        self->_pythonF = obj;
        self->_function = std::bind(&PythonFunctionalNode::pythonCallback, self, std::placeholders::_1 );
        self->setInputPtrs(std::make_index_sequence<input_s>());
        self->setOutputPtrs(std::make_index_sequence<output_s>());

        // Collect function arguments
        auto inspect = bpy::import("inspect");
        bpy::stl_input_iterator<std::string> begin(inspect.attr("getfullargspec")(obj).attr("args")), end;
        self->_iPortIds.insert(self->_iPortIds.begin(), begin, end);

        // Setup ports
        if(self->_oPortIds.size() > output_s || self->_iPortIds.size() > output_s)
            throw NRPException::logCreate("PythonFunctionalNode has not enough input or output ports to wrap the "
                                          "requested python object");

        for(const auto& p : self->_oPortIds)
            self->registerOutput(p);

        // Register node
        std::shared_ptr<ComputationalNode> self_base = std::dynamic_pointer_cast<ComputationalNode>(self);
        ComputationalGraphManager::getInstance().registerNode(self_base);

        return boost::python::object(self);
    }

    /*!
     * \brief Safely get or register an input port
     */
    template<typename T_IN, size_t N = 0>
    InputPort<T_IN, bpy::object>* getOrRegisterInput(const std::string& id)
    {
        auto m_iter = std::find(_iPortIds.begin(), _iPortIds.end(), id);
        if(m_iter == _iPortIds.end())
            throw NRPException::logCreate("\"" + id + "\" does not match any of the declared input arguments in node \"" + this->id() + "\"");

        if constexpr (N < input_s) {
            Port* port = getInputByIndex(N);
            if(port) {
                if(port->id() == id) {
                    if(dynamic_cast< InputPort<T_IN, bpy::object>* >(port))
                        return dynamic_cast< InputPort<T_IN, bpy::object>* >(port);
                    else
                        throw NRPException::logCreate("Input \"" + id + "\" has been registered with a different type in node \"" + this->id() + "\"");
                }
                else
                    return getOrRegisterInput<T_IN, N+1>(id);
            }
            else
                return registerInput<N, T_IN, bpy::object>(id);
        }

        throw NRPException::logCreate("There is no input port with name" + id + "registered to this node and no additional ports can be registered");
    }

    /*!
     * \brief Safely registers an output port
     */
    template<size_t N = 0>
    OutputPort<bpy::object>* registerOutput(const std::string& id)
    {
        if constexpr (N < output_s) {
            if(!getOutputByIndex<N>())
                return FunctionalNode::registerOutput<N, bpy::object>(id);
            else
                return registerOutput<N+1>(id);
        }

        throw NRPException::logCreate("Output can't be registered. All available ports are bound.");
    }

    /*!
     * \brief Safely get an output port
     */
    template<size_t N = 0>
    OutputPort<bpy::object>* getOutput(const std::string& id)
    { return dynamic_cast<OutputPort<bpy::object>*>(getOutputById<N>(id)); }

    /*!
     * \brief Request the registration of an edge between an output port in another functional node an i_port input port in this node
     */
    void registerF2FEdge(const std::string& i_port, const std::string& address)
    { _f2fEdges[i_port] = address; }

private:

    /*!
     * \brief Set the wrapped python function arguments, calls it with them and set _outputs
     */
    void pythonCallback(params_t&)
    {
        boost::python::tuple args;
        boost::python::dict kwargs;

        for(size_t i=0; i < _iPortIds.size(); ++i) {
            auto in = *_inputs[i];
            kwargs[_iPortIds[i]] = in != nullptr ? *in : bpy::object();
        }

        try {
            boost::python::list l_output = boost::python::extract<boost::python::list>(
                    _pythonF(*args, **kwargs));

            if(_oPortIds.size() != (size_t)boost::python::len(l_output)) {
                std::stringstream error_msg;
                error_msg << "Functional Node with id \"" << this->id() << "\" has " << _oPortIds.size() <<
                          " declared outputs, but its Python function returns " << boost::python::len(l_output) << " elements.";
                throw NRPException::logCreate(error_msg.str());
            }


            for(int i=0; i < boost::python::len(l_output); ++i)
                *_outputs[i] = l_output[i];
        }
        catch (const boost::python::error_already_set&) {
            std::string error_msg = "An error occurred while executing Functional Node with id \"" + this->id() + "\"";
            NRPLogger::error(error_msg);
            PyErr_Print();
            boost::python::throw_error_already_set();
        }
    }

    /*!
     * \brief Connect _inputs with _params by setting pointers in _inputs
     */
    template<std::size_t... Ints>
    void setInputPtrs(std::index_sequence<Ints...>)
    { ((_inputs[Ints] = &std::get<Ints>(_params)),...); }

    /*!
     * \brief Connect _outputs with _params by setting pointers in _outputs
     */
    template<std::size_t... Ints>
    void setOutputPtrs(std::index_sequence<Ints...>)
    { ((_outputs[Ints] = &std::get<input_s+Ints>(_params)),...); }

    /*! \brief helper function to move this object to a shared pointer */
    std::shared_ptr<PythonFunctionalNode> moveToSharedPtr()
    { return std::shared_ptr<PythonFunctionalNode>(new PythonFunctionalNode(std::move(static_cast<PythonFunctionalNode&>(*this)))); }

    /*! \brief declared edges from another FunctionalNode to this node */
    std::map<std::string, std::string> _f2fEdges;
    /*! \brief declared inputs in this node */
    std::vector<std::string> _iPortIds;
    /*! \brief declared outputs in this node */
    std::vector<std::string> _oPortIds;
    /*! \brief python callable object wrapped in this node */
    boost::python::object _pythonF;
    /*! \brief array of pointers to the input part of _params. The pointers are set in setInputPtrs */
    std::array< const bpy::object**, input_s > _inputs;
    /*! \brief array of pointers to the output part of _params. The pointers are set in setOutputPtrs */
    std::array< bpy::object*, output_s > _outputs;
};


/*!
 * \brief Helper class used to implement a F2FEdge Python decorator
 */
class F2FEdge {
public:

    F2FEdge(const std::string &keyword, const std::string &address) :
            _keyword(keyword),
            _address(address)
    {  }

    /*!
     * \brief __call__ function in the decorator
     *
     * It receives a Python object wrapping a PythonFunctionalNode and add an F2FEdge to it
     */
    boost::python::object pySetup(const boost::python::object& obj)
    {
        // Register edge in FN
        try {
            std::shared_ptr<PythonFunctionalNode> f = boost::python::extract<std::shared_ptr<PythonFunctionalNode> >(
                    obj);
            f->registerF2FEdge(_keyword, _address);
        }
        catch (const boost::python::error_already_set&) {
            NRPLogger::error("An error occurred while creating the F2FEdge '" + _address +
            "'. Check that Functional Node definition is correct");
            PyErr_Print();
            boost::python::throw_error_already_set();
        }

        // Returns FunctionalNode
        return obj;
    }

protected:

    std::string _keyword;
    std::string _address;
};



#endif //PYTHON_FUNCTIONAL_NODE_H
