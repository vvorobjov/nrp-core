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

#ifndef INPUT_TIME_H
#define INPUT_TIME_H

#include "nrp_general_library/utils/time_utils.h"

#include "nrp_event_loop/computational_graph/input_node.h"
#include "nrp_event_loop/python/input_edge.h"

/*!
 * \brief Time base input node which sends the system time related information into the graph as an unsigned long message.
 *
 * Time information is updated externally either by an EventLoop or an FTILoop.
 */
class InputTimeBaseNode : public InputNode<ulong> {
public:

    InputTimeBaseNode(const std::string &id) :
            InputNode(id)
    {}

protected:

    /*! \brief Updates node time info, which is sent to connected nodes */
    void updateTimeInfo(ulong newTime)
    {
        _time = newTime;
        _hasNew = true;
    }

    void configure() override
    {}

    bool updatePortData(const std::string& id) override
    {
        if(!_hasNew)
            return false;

        if(!_portMap.at(id).size())
            _portMap.at(id).addMsg(&_time);

        _hasNew = false;
        return true;
    }

private:

    /*! \brief Stores system time information */
    ulong _time;
    /*! \brief Flag used internally to mark when there is new information available */
    bool _hasNew = false;
};

/*!
 * \brief Specialization of InputTimeBaseNode which stores and sends system clock in milliseconds
 */
class InputClockNode : public InputTimeBaseNode {

public:

    InputClockNode(const std::string &id) :
            InputTimeBaseNode(id)
    {}

    /*! \brief Sets Node clock in milliseconds */
    void updateClock(SimulationTime newTime)
    { this->updateTimeInfo(std::chrono::duration_cast<std::chrono::milliseconds>(newTime).count()); }

};


class InputClockEdge : public SimpleInputEdge<ulong, InputClockNode> {

public:

    InputClockEdge(const std::string& keyword) :
            SimpleInputEdge<ulong, InputClockNode>(keyword, "clock_node", "clock", InputNodePolicies::LAST, InputNodePolicies::KEEP_CACHE)
    {}

protected:

    InputClockNode* makeNewNode()
    { return new InputClockNode(this->_id); }
};

/*!
 * \brief Specialization of InputTimeBaseNode which stores and sends system iteration number
 */
class InputIterationNode : public InputTimeBaseNode {

public:

    InputIterationNode(const std::string &id) :
            InputTimeBaseNode(id)
    {}

    /*! \brief Sets Node iteration number */
    void updateIteration(ulong newIteration)
    { this->updateTimeInfo(newIteration); }

};


class InputIterationEdge : public SimpleInputEdge<ulong, InputIterationNode> {

public:

    InputIterationEdge(const std::string& keyword) :
            SimpleInputEdge<ulong, InputIterationNode>(keyword, "iteration_node", "iteration", InputNodePolicies::LAST, InputNodePolicies::KEEP_CACHE)
    {}

protected:

    InputIterationNode* makeNewNode()
    { return new InputIterationNode(this->_id); }
};


#endif //INPUT_TIME_H
