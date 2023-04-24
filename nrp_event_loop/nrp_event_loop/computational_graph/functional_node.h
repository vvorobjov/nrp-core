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

#ifndef FUNCTIONAL_NODE_H
#define FUNCTIONAL_NODE_H

#include <array>
#include <functional>
#include <tuple>

#include "nrp_event_loop/computational_graph/computational_node_policies.h"
#include "nrp_event_loop/computational_graph/computational_node.h"
#include "nrp_event_loop/computational_graph/input_port.h"
#include "nrp_event_loop/computational_graph/output_port.h"

#include "nrp_general_library/utils/nrp_logger.h"

/*!
 * \brief Non-abstract, non-templated base class for the FunctionalNode class
 *
 * Its existence is necessary for storing and manipulating functional nodes in the creation of Computaional Graph edges
 */
class FunctionalNodeBase : public ComputationalNode {
public:

    FunctionalNodeBase(const std::string &id) :
            ComputationalNode(id, ComputationalNode::Functional)
    {}

    /*!
     * \brief Returns an InputPort by id.
     */
    virtual Port* getInputById(const std::string& /*id*/) { return nullptr; };

    /*!
     * \brief Returns an OutputPort by id.
     */
    virtual Port* getOutputById(const std::string& /*id*/) { return nullptr; };


    void configure() override {};
    void compute() override {};
};


template<typename, typename>
class FunctionalNode;

/*!
 * \brief Implementation of a functional node in the computational graph
 *
 * It stores an std::function object, '_function' which is called in the node 'compute' method and which inputs and
 * outputs can be connected to input and output ports respectively
 *
 * _function return value is bool, which is used to decide whether to send outputs or not
 */
template<typename... INPUT_TYPES, typename... OUTPUT_TYPES>
class FunctionalNode<std::tuple<INPUT_TYPES...>, std::tuple<OUTPUT_TYPES...> > : public FunctionalNodeBase {

protected:

    /*! \brief Type of the nth element in INPUT_TYPES parameter pack */
    template <std::size_t N>
    using input_n_t = typename std::tuple_element<N, std::tuple<INPUT_TYPES...>>::type;

    /*! \brief Type of the nth element in OUTPUT_TYPES parameter pack */
    template <std::size_t N>
    using output_n_t = typename std::tuple_element<N, std::tuple<OUTPUT_TYPES...>>::type;

    using inputs_t = std::tuple<const INPUT_TYPES* ...>;
    using outputs_t = std::tuple<OUTPUT_TYPES ...>;
    
    /*! \brief Type representing the argument of _function. 
     * 
     * It is a tuple containing all input parameters as const pointers and all output as values
     */
    using params_t = decltype(std::tuple_cat(std::declval<inputs_t>(), std::declval<outputs_t>()));


public:

    /*!
     * \brief Creates an InputPort and connect it to an input specified by N. Returns the created port.
     */
    template <std::size_t N, class T_IN, class T_OUT>
    InputPort<T_IN, T_OUT>* registerInput(const std::string& id)
    {
        if constexpr (N >= sizeof...(INPUT_TYPES)) {
            std::stringstream s;
            s << "In Functional node '" << this->id() << "'. Attempt to register input with index " << N <<
            ", which is greater than the number of inputs in this node: " << sizeof...(INPUT_TYPES);
            throw NRPException::logCreate(s.str());
        }
        else if constexpr (!std::is_same_v<input_n_t<N>, T_OUT>) {
            std::stringstream s;
            s << "In Functional node '" << this->id() << "'. Attempt to register input with index " << N << " and type '" << typeid(T_OUT).name() <<
                "', but index " << N << " is of type " << typeid(input_n_t<N>).name();
            throw NRPException::logCreate(s.str());
        }
        else if(_inputPorts.at(N)) {
            std::stringstream s;
            s << "In Functional node '" << this->id() << "'. Attempt to register input with index " << N << ", but index "
            << N << " is already registered.";
            throw NRPException::logCreate(s.str());
        }

        using std::placeholders::_1;
        std::function<void(const T_OUT*)> f = std::bind( &FunctionalNode::newInputCallback<N, T_OUT>, this, _1 );
        InputPort<T_IN, T_OUT>* port = new InputPort<T_IN, T_OUT>(id, this, f, 1);
        _inputPorts.at(N).reset(port);

        return port;
    }

    /*!
     * \brief Returns an InputPort by index.
     */
    Port* getInputByIndex(size_t idx)
    {
        if (idx >= _inputPorts.size()) {
            std::stringstream s;
            s << "In Functional node '" << this->id() << "'. Attempt to get input with index " << idx <<
              ", which is greater than the number of inputs in this node: " << _inputPorts.size() - 1;
            throw NRPException::logCreate(s.str());
        }

        return _inputPorts.at(idx).get();
    }

    /*!
     * \brief Returns an InputPort by id.
     */
    Port* getInputById(const std::string& id) override
    {
        for(auto& e : _inputPorts)
            if (e && e->id() == id)
                return e.get();

        std::stringstream s;
        s << "In Functional node '" << this->id() << "'. Input with id '" << id << "' not found";
        NRPLogger::info(s.str());

        return nullptr;
    }

    /*!
     * \brief Creates an OutputPort and connect it to an output specified by N. Returns the created port
     */
    template <std::size_t N, class T>
    OutputPort<T>* registerOutput(const std::string& id)
    {
        if constexpr (N >= sizeof...(OUTPUT_TYPES)) {
            std::stringstream s;
            s << "In Functional node '" << this->id() << "'. Attempt to register output with index " << N <<
            ", which is greater than the number of outputs in this node: " << sizeof...(OUTPUT_TYPES);
            throw NRPException::logCreate(s.str());
        }
        else if constexpr (!std::is_same_v<output_n_t<N>, T>) {
            std::stringstream s;
            s << "In Functional node '" << this->id() << "'. Attempt to register output with index " << N << " and type '"
            << typeid(T).name() << "', but index " << N << " is of type " << typeid(output_n_t<N>).name();
            throw NRPException::logCreate(s.str());
        }
        else if(std::get<N>(_outputPorts)) {
            std::stringstream s;
            s << "In Functional node '" << this->id() << "'. Attempt to register output with index " << N << ", but index "
              << N << " is already registered.";
            throw NRPException::logCreate(s.str());
        }

        OutputPort<T>* port = new OutputPort<T>(id, this);
        std::get<N>(_outputPorts).reset(port);

        return port;
    }

    /*!
     * \brief Returns an OutputPort by index.
     */
    template <std::size_t N>
    Port* getOutputByIndex()
    {
        if constexpr (N >= sizeof...(OUTPUT_TYPES)) {
            std::stringstream s;
            s << "In Functional node '" << this->id() << "'. Attempt to get or register output with index " << N <<
              ", which is greater than the number of outputs in this node: " << sizeof...(OUTPUT_TYPES);
            throw NRPException::logCreate(s.str());
        }

        return std::get<N>(_outputPorts).get();
    }

    /*!
     * \brief Returns an OutputPort by id.
     */
    Port* getOutputById(const std::string& id) override
    { return getOutputByIdTuple(id); }

    template <std::size_t N = 0>
    Port* getOutputByIdTuple(const std::string& id)
    {
        if constexpr (N < sizeof...(OUTPUT_TYPES)) {
            if (std::get<N>(_outputPorts) && std::get<N>(_outputPorts)->id() == id)
                return std::get<N>(_outputPorts).get();
            else
                return getOutputByIdTuple<N+1>(id);
        }

        std::stringstream s;
        s << "In Functional node '" << this->id() << "'. Attempt to get output with id '" << id << "', but it is not registered.";
        NRPLogger::info(s.str());

        return nullptr;
    }

protected:

    /*!
     * \brief Configure. Print warnings if node is not fully connected.
     */
    void configure() override
    {
        if(boundInputPorts() < std::tuple_size_v<inputs_t>) {
            std::stringstream s;
            s << "Functional node " << this->id() << " has been declared with " << std::tuple_size_v<inputs_t> <<
              " inputs, but only " << boundInputPorts() << " are bounded" << std::endl;
            NRPLogger::info(s.str());
        }

        if(boundOutputPorts() < std::tuple_size_v<outputs_t>) {
            std::stringstream s;
            s << "Functional node " << this->id() << " has been declared with " << std::tuple_size_v<outputs_t>
              <<
              " outputs, but only " << boundOutputPorts() << " are bounded" << std::endl;
            NRPLogger::info(s.str());
        }
    }

    /*!
     * \brief Compute. Execute '_function' and send its outputs out
     */
    void compute() override final
    {
        if(_execPolicy == FunctionalNodePolicies::ExecutionPolicy::ALWAYS || _hasNew) {
            // Execute _function and send outputs if _function returns true
            if(_function(_params))
                sendOutputs();
        }

        _hasNew = false;
    }

    /*!
     * \brief Constructor
     */
    FunctionalNode(const std::string &id, std::function<bool(params_t&)> f, FunctionalNodePolicies::ExecutionPolicy policy = FunctionalNodePolicies::ExecutionPolicy::ON_NEW_INPUT) :
            FunctionalNodeBase(id),
            _function(f),
            _execPolicy(policy)
    { initInputs(); }

    /*!
     * \brief Init all inputs to a nullptr.
     */
    template<size_t N = 0>
    void initInputs()
    {
        if constexpr ( N < sizeof...(INPUT_TYPES)) {
            std::get<N>(_params) = nullptr;
            initInputs<N+1>();
        }
    }

    /*!
     * \brief Process incoming new msg
     */
    template <size_t N, class T>
    void newInputCallback(const T* value)
    {
        std::get<N>(_params) = value;
        this->_hasNew = this->_hasNew || value != nullptr;
    }

    /*!
     * \brief Sends all outputs through connected to ports
     */
    template<size_t N = 0>
    void sendOutputs()
    {
        if constexpr ( N < sizeof...(OUTPUT_TYPES)) {
            if (std::get<N>(_outputPorts))
                std::get<N>(_outputPorts)->publish(&std::get<sizeof...(INPUT_TYPES) + N>(_params));

            sendOutputs<N+1>();
        }
    }

    size_t boundInputPorts()
    {
        size_t n = 0;
        for(auto p : _inputPorts)
            n += p ? 1 : 0;

        return n;
    }

    template<size_t N = 0>
    size_t boundOutputPorts()
    {
        if constexpr (N < sizeof...(OUTPUT_TYPES)) {
            size_t n = std::get<N>(_outputPorts) ? 1 : 0;
            return n + boundOutputPorts<N+1>();
        }

        return 0;
    }

    /*! \brief function performing main computation in this node */
    params_t _params;
    /*! \brief function performing main computation in this node. It sets the output part of _params from its input part */
    std::function<bool(params_t&)> _function;

    friend class FunctionalNodeFactory;
    friend class ComputationalNodes_FUNCTIONAL_NODE_Test;
    friend class ComputationalNodes_FUNCTIONAL_NODE_FACTORY_Test;
    friend class ComputationalGraphPythonNodes_PYTHON_FUNCTIONAL_NODE_Test;
    friend class ComputationalGraphPythonNodes_PYTHON_DECORATORS_BASIC_Test;

private:

    /*! \brief input ports */
    std::array< std::shared_ptr<Port>, sizeof...(INPUT_TYPES) > _inputPorts;
    /*! \brief output ports */
    std::tuple< std::shared_ptr< OutputPort<OUTPUT_TYPES> > ...> _outputPorts;

    bool _hasNew = false;
    FunctionalNodePolicies::ExecutionPolicy _execPolicy;
};

#endif //FUNCTIONAL_NODE_H
