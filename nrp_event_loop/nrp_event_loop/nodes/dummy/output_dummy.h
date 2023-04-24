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

#ifndef OUTPUT_DUMMY_H
#define OUTPUT_DUMMY_H

#include <boost/python.hpp>
#include <iostream>

#include "nrp_event_loop/computational_graph/output_node.h"
#include "nrp_event_loop/python/output_edge.h"

/*!
 * \brief Dummy output node which just stores the last received msgs
 */
class OutputDummy : public OutputNode<boost::python::object> {
public:

    OutputDummy(const std::string &id,
                bool publishFromCache = false,
                unsigned int computePeriod = 1) :
            OutputNode(id, OutputNodePolicies::PublishFormatPolicy::SERIES, publishFromCache, 0, computePeriod)
    { }

    size_t call_count = 0;
    const boost::python::object* lastData = nullptr;
    bool debugPrint = false;

protected:

    void sendSingleMsg(const std::string& /*id*/, const boost::python::object* data) override
    {
        if(debugPrint) {
            std::string o_s = boost::python::extract<std::string>(boost::python::str(*data));
            std::stringstream msg;
            msg << "Dummy output node " << this->id() << " got msg " << o_s;
            NRPLogger::info(msg.str());
        }

        lastData = data;
        call_count++;
    }

    void sendBatchMsg(const std::string& id, const std::vector<const boost::python::object*>& data) override
    {
        for (auto d : data) {
            sendSingleMsg(id, d);
            lastData = d;
        }
    }
};


class OutputDummyEdge : public SimpleOutputEdge<boost::python::object, OutputDummy> {

public:

    OutputDummyEdge(const std::string &keyword, const std::string &id,
                    bool publishFromCache = false,
                    unsigned int computePeriod = 1) :
            SimpleOutputEdge<boost::python::object, OutputDummy>(keyword, id, id, publishFromCache, computePeriod)
    {}

protected:

    OutputDummy* makeNewNode() override
    { return new OutputDummy(this->_id, this->_publishFromCache, this->_computePeriod); }
};

#endif //OUTPUT_DUMMY_H
