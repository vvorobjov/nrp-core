//
// NRP Core - Backend infrastructure to synchronize simulations
//
// Copyright 2020-2021 NRP Team
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//   http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//
// This project has received funding from the European Union’s Horizon 2020
// Framework Programme for Research and Innovation under the Specific Grant
// Agreement No. 945539 (Human Brain Project SGA3).
//

#include <chrono>
#include <thread>
#include <mutex>
#include <condition_variable>

#include "nrp_event_loop/computational_graph/computational_node.h"
#include "nrp_event_loop/computational_graph/input_node.h"
#include "nrp_event_loop/computational_graph/output_node.h"

//// Helper classes and functions definitions

struct TestMsg {
    int value;
};

template <>
struct dataConverter<int, TestMsg> {
    static void convert(const int* d1, TestMsg& d2)
    { d2.value = *d1; }
};

class TestNode : public ComputationalNode {
public:

    static std::vector<std::string> compOrder;
    static std::mutex m;
    static std::condition_variable cv;
    static bool isExecuting;

    TestNode(const std::string &id, NodeType type) :
            ComputationalNode(id, type)
    {}

    void configure() override
    {
        if(doBlockExec)
        {
            std::lock_guard<std::mutex> lk(m);
            isExecuting = true;
            cv.notify_one();
        }

        while(doBlockExec)
            std::this_thread::sleep_for(std::chrono::milliseconds(10));

        isConfigured = true;
        isExecuting = false;
    }

    void compute() override
    {
        cycle_calls.push_back("compute");

        if(doBlockExec)
        {
            std::lock_guard<std::mutex> lk(m);
            isExecuting = true;
            cv.notify_one();
        }

        while(doBlockExec)
            std::this_thread::sleep_for(std::chrono::milliseconds(10));

        compOrder.push_back(this->id());
        isExecuting = false;
    }

    void graphCycleStartCB() override
    {
        cycle_calls.push_back("graphCycleStartCB");
    }

    std::vector<std::string> cycle_calls;
    bool doBlockExec = false;
    bool isConfigured = false;
};

class TestInputNode : public InputNode<TestMsg> {
public:

    TestInputNode(const std::string &id, InputNodePolicies::MsgPublishPolicy msgPublishPolicy, InputNodePolicies::MsgCachePolicy msgCachePolicy, size_t queue_size) :
            InputNode(id, msgPublishPolicy, msgCachePolicy, queue_size)
    { }

    void configure() override
    { }

    void stopUpdating()
    { doUpdate = false; }

protected:

    bool updatePortData(const std::string& id) override
    {
        if(_portMap.count(id) && doUpdate) {
            msg_v.push_back(std::make_shared<TestMsg>());
            TestMsg* t = msg_v.back().get();
            t->value = msg_v.size();

            return _portMap.at(id).addMsg(t);
        }

        return false;
    }

public:

    std::vector<std::shared_ptr<TestMsg>> msg_v;
    bool doUpdate = true;

};

class TestOutputNode : public OutputNode<TestMsg> {
public:

    TestOutputNode(const std::string &id, OutputNodePolicies::PublishFormatPolicy publishFormatPolicy,
                   bool publishFromCache, int maxPortConnections, unsigned int computePeriod) :
        OutputNode(id, publishFormatPolicy, publishFromCache, maxPortConnections, computePeriod)
    { }

    void sendSingleMsg(const std::string& /*id*/, const TestMsg* data) override
    {
        sendSingleMsgCalled = true;
        sent_msgs.push_back(data);
    }

    void sendBatchMsg(const std::string& /*id*/, const std::vector<const TestMsg*>& data) override
    {
        sendBatchMsgCalled = true;
        for (auto& m : data)
            sent_msgs.push_back(m);
    }

    void resetCalls()
    {
        sendSingleMsgCalled = false;
        sendBatchMsgCalled = false;
        sent_msgs.clear();
    }

    std::vector<const TestMsg*> sent_msgs;
    bool sendSingleMsgCalled = false;
    bool sendBatchMsgCalled = false;

};

// EOF
