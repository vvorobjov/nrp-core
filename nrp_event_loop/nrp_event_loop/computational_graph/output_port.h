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

#ifndef OUTPUT_PORT_H
#define OUTPUT_PORT_H

#include <functional>

#include "nrp_event_loop/computational_graph/port.h"
#include "nrp_event_loop/computational_graph/computational_node.h"

/*!
 * \brief Implementation of an output port in the computation graph
 *
 * It forwards msgs to subscribed ports via its 'publish' method
 */
template <class T>
class OutputPort : public Port {
public:

    /*!
     * \brief Constructor
     */
    OutputPort(const std::string& id, ComputationalNode* parent) :
            Port(id, parent)
    {}

    /*!
     * \brief Publish a msg to all subscribers
     */
    void publish(const T* msg)
    {
        for(auto& f : _subscribers)
            f(msg);
    }

    /*!
     * \brief Return the number the number of ports subscribed to this port
     */
    size_t subscriptionsSize() override
    { return _subscribers.size();}

protected:

    /*!
     * \brief Adds a subscriber to this port
     *
     * InputPorts subscribe themselves to OutputPorts
     */
    void add_subscriber(std::function<void(const T*)> callback)
    { _subscribers.push_back(callback); }

    template<typename, typename> friend class InputPort;

private:

    /*! \brief List of subscribers */
    std::vector< std::function<void(const T*)> > _subscribers;
};


#endif //OUTPUT_PORT_H
