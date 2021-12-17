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

#ifndef FUNCTIONAL_NODE_FACTORY_H
#define FUNCTIONAL_NODE_FACTORY_H

#include "nrp_event_loop/computational_graph/functional_node.h"

template<size_t, typename, typename, size_t>
struct sub_tuple;

// Helper structure defining the type of a sub range of Tuple between indexes IDX1 and IDX2
template<size_t IDX1, typename... Tpack, typename Tuple, size_t IDX2>
struct sub_tuple<IDX1, std::tuple<Tpack...>, Tuple, IDX2>
        : sub_tuple<IDX1 + 1, std::tuple<Tpack..., std::tuple_element_t<IDX1, Tuple>>, Tuple, IDX2>
{ };

// Base case
template<std::size_t IDX, typename... Tpack, typename Tuple>
struct sub_tuple<IDX, std::tuple<Tpack...>, Tuple, IDX>
{ typedef std::tuple<Tpack...> type; };

/*!
 * \brief Creates an instance of FunctionalNode with the right template given a function signature
 *
 * It hides all the complexity related with specifying correctly FunctionalNode template and performs function signature
 * checking ensuring that the instantiated FunctionalNode is correct
 */
class FunctionalNodeFactory {

    // helper type to check if the declared input parameters are correct
    template<class T>
    using is_correct_input = std::conjunction<std::is_pointer<T>,
            std::is_const<std::remove_pointer_t<T>>>; // const T*

    // helper type to check if the declared output parameters are correct
    template<class T>
    using is_correct_output = std::conjunction<std::is_lvalue_reference<T>,
            std::negation<std::is_const<std::remove_reference<T>>>>; // (not const) T&

public:

    /*!
     * \brief Instantiates a FunctionalNode
     *
     * It takes as template parameters the number of inputs 'in_n' and outputs 'out_n' and a parameter pack containing
     * the function arguments
     */
    template<size_t in_n, size_t out_n, typename... args>
    static auto create(const std::string &id, std::function<void(args...)> f) {
        // Check that template types are correct
        static_assert(in_n + out_n == sizeof...(args), "Wrong number of input and/or output parameters in Functional Node");

        // Declare helper types
        using f_params_t = std::tuple<args...>;
        using f_params_no_ref_t = std::tuple<std::remove_reference_t<args>...>;
        using f_params_no_pointer_t = std::tuple<std::remove_const_t<std::remove_pointer_t<args>>...>;
        using inputs_t = typename sub_tuple<0, std::tuple<>, f_params_t, in_n>::type;
        using outputs_t = typename sub_tuple<in_n, std::tuple<>, f_params_t, in_n + out_n>::type;
        using inputs_no_pointer_t = typename sub_tuple<0, std::tuple<>, f_params_no_pointer_t, in_n>::type;
        using outputs_no_ref_t = typename sub_tuple<in_n, std::tuple<>, f_params_no_ref_t, in_n + out_n>::type;

        // Check that function arguments has the correct types
        FunctionalNodeFactory::checkInputs<0, in_n, inputs_t>(); // const T*
        FunctionalNodeFactory::checkOutputs<0, out_n, outputs_t>(); // T&

        // Instantiates FN
        using params_t = decltype(std::tuple_cat(std::declval<inputs_t>(), std::declval<outputs_no_ref_t>()));
        auto f_wrap = [f](params_t &p) { std::apply(f, p); };
        return FunctionalNode<inputs_no_pointer_t, outputs_no_ref_t>(id, f_wrap);
    }

private:

    /*!
     * \brief Checks if each of the elements in tuple_t satisfies the conditions for being a Functional Node input
     */
    template<size_t N, size_t MaxN, typename tuple_t>
    static void checkInputs()
    {
        if constexpr (N < MaxN) {

            static_assert(is_correct_input<typename std::tuple_element<N, tuple_t>::type>::value, "Functional Node inputs must be \"constT*\"");
            checkInputs<N+1, MaxN, tuple_t>();
        }
    }

    /*!
     * \brief Checks if each of the elements in tuple_t satisfies the conditions for being a Functional Node output
     */
    template<size_t N, size_t MaxN, typename tuple_t>
    static void checkOutputs()
    {
        if constexpr (N < MaxN) {
            static_assert(is_correct_output<typename std::tuple_element<N, tuple_t>::type>::value, "Functional Node outputs must be \"T&\"");
            checkOutputs<N+1, MaxN, tuple_t>();
        }
    }
};


#endif //FUNCTIONAL_NODE_FACTORY_H
