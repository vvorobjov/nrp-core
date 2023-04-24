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

#ifndef INPUT_DUMMY_H
#define INPUT_DUMMY_H

#include <boost/python.hpp>

#include "nrp_event_loop/computational_graph/input_node.h"
#include "nrp_event_loop/python/input_edge.h"

/*!
 * \brief Dummy input node which always sends the same msg upon request
 */
class InputDummy : public InputNode<boost::python::object> {
public:

    InputDummy(const std::string &id, boost::python::object value) :
            InputNode(id),
            _value(std::move(value))
    {}

protected:

    void configure() override
    {}

    bool updatePortData(const std::string& id) override
    {
        _portMap.at(id).clear();
        _portMap.at(id).addMsg(&_value);

        return true;
    }

private:

    boost::python::object _value;
};


class InputDummyEdge : public SimpleInputEdge<boost::python::object, InputDummy> {

public:

    InputDummyEdge(const std::string& keyword, const std::string& id, boost::python::object value) :
            SimpleInputEdge<boost::python::object, InputDummy>(keyword, id, id, InputNodePolicies::LAST, InputNodePolicies::KEEP_CACHE),
            _value(std::move(value))
    {}

protected:

    boost::python::object _value;

    InputDummy* makeNewNode()
    { return new InputDummy(this->_id, _value); }
};


#endif //INPUT_DUMMY_H
