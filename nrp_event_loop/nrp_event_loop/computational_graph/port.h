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

#ifndef PORT_H
#define PORT_H

#include "nrp_event_loop/computational_graph/computational_node.h"

/*!
 * \brief Base class implementing a port in the computational graph
 */
class Port {
public:

    Port() = delete;
    virtual ~Port() = default;

    /*!
     * \brief Constructor
     */
    Port(std::string id, ComputationalNode* parent):
        _id(std::move(id)),
        _parent(parent)
    { }

    /*!
     * \brief Returns the port 'id'
     */
    const std::string& id()
    { return _id; }

    /*!
     * \brief Returns the port parent node
     */
    ComputationalNode* parent() const
    { return _parent; }

    /*!
     * \brief Return the number of subscriptions of this port
     *
     * This is the number of ports this port is subscribed to in the case of input ports or the number of ports
     * subscribed to this port in the case of output ports
     */
    virtual size_t subscriptionsSize() = 0;

private:

    /*! \brief Port unique identifier */
    std::string _id;
    /*! \brief Port parent node */
    ComputationalNode* _parent;
};

#endif //PORT_H
