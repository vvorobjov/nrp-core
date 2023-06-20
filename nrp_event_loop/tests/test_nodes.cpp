//
// NRP Core - Backend infrastructure to synchronize simulations
//
// Copyright 2020-2023 NRP Team
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

#include <functional>
#include <mutex>

#include <gtest/gtest.h>

#include "nrp_event_loop/computational_graph/computational_node.h"
#include "nrp_event_loop/computational_graph/computational_graph_manager.h"

#include "nrp_event_loop/computational_graph/input_port.h"
#include "nrp_event_loop/computational_graph/output_port.h"

#include "nrp_event_loop/computational_graph/input_node.h"
#include "nrp_event_loop/computational_graph/output_node.h"

#include "nrp_event_loop/fn_factory/functional_node_factory.h"
#include "nrp_event_loop/fn_factory/functional_node_factory_manager.h"

#include "tests/test_files/helper_classes.h"

//// COMPUTATIONAL NODE

TEST(ComputationalNodes, COMPUTATIONAL_NODE) {
    TestNode n1("node", ComputationalNode::Functional);

    ASSERT_EQ(n1.id(), "node");
    ASSERT_EQ(n1.type(), ComputationalNode::Functional);

    ASSERT_EQ(n1.isVisited(), false);
    n1.setVisited(true);
    ASSERT_EQ(n1.isVisited(), true);
    n1.setVisited(false);
    ASSERT_EQ(n1.isVisited(), false);

    ASSERT_EQ(n1.doCompute(), false);
    n1.setDoCompute(true);
    ASSERT_EQ(n1.doCompute(), true);
    n1.setDoCompute(false);
    ASSERT_EQ(n1.doCompute(), false);
}

//// INPUT NODE

TEST(ComputationalNodes, DATA_PORT_HANDLE)
{
    using vector_test_msg = std::vector<const TestMsg*>;

    TestNode n1("input", ComputationalNode::Input);
    TestNode n2("output", ComputationalNode::Output);

    TestMsg msg_send, msg_send_2;

    const TestMsg* msg_got = nullptr;
    const vector_test_msg* msg_got_list = nullptr;

    std::function<void(const TestMsg*)> f = [&](const TestMsg* a) { msg_got = a; };
    std::function<void(const vector_test_msg*)> f_list = [&](const vector_test_msg* a) { msg_got_list = a; };

    DataPortHandle<TestMsg> p_h("handle", &n1, 2);
    InputPort<TestMsg, TestMsg> i_p("input_port", &n2, f);
    InputPort<vector_test_msg, vector_test_msg> i_p_list("input_port_list", &n2, f_list);

    i_p.subscribeTo(p_h.singlePort.get());
    i_p_list.subscribeTo(p_h.listPort.get());

    // Add msg, clear, size
    ASSERT_EQ(p_h.addMsg(&msg_send), true);
    ASSERT_EQ(p_h.size(), 1);
    ASSERT_EQ(p_h.addMsg(&msg_send_2), true);
    ASSERT_EQ(p_h.size(), 2);
    ASSERT_EQ(p_h.addMsg(&msg_send_2), false); // msg should not be added because the queue is full
    ASSERT_EQ(p_h.size(), 2);
    p_h.clear();
    ASSERT_EQ(p_h.size(), 0);

    // Publish
    p_h.addMsg(&msg_send);
    p_h.publishLast();

    ASSERT_EQ(msg_got, &msg_send);
    ASSERT_EQ(msg_got_list, nullptr);

    p_h.publishAll();

    ASSERT_EQ(msg_got_list->size(), 1);
    ASSERT_EQ((*msg_got_list)[0], &msg_send);

    p_h.addMsg(&msg_send_2);
    p_h.publishLast();
    p_h.publishAll();

    ASSERT_EQ(msg_got, &msg_send_2);
    ASSERT_EQ(msg_got_list->size(), 2);
    ASSERT_EQ((*msg_got_list)[1], &msg_send_2);

    p_h.clear();

    ASSERT_EQ(msg_got, &msg_send_2);
    ASSERT_EQ(msg_got_list->size(), 0);

    p_h.publishNullandClear();

    ASSERT_EQ(msg_got, nullptr);
    ASSERT_EQ(msg_got_list, nullptr);
}

TEST(ComputationalNodes, INPUT_NODE_UPDATE_POLICY_WITH_KEEP_CACHE)
{
    using vector_test_msg = std::vector<const TestMsg*>;

    TestNode n_o("output", ComputationalNode::Output);

    //// LAST
    const TestMsg* msg_got = nullptr;
    const TestMsg* msg_got_back = nullptr;

    std::function<void(const TestMsg*)> f = [&](const TestMsg* a) { msg_got = a; };

    InputPort<TestMsg, TestMsg> i_p("input_port", &n_o, f);

    TestInputNode i_last("i_last", InputNodePolicies::MsgPublishPolicy::LAST,
                             InputNodePolicies::MsgCachePolicy::KEEP_CACHE, 2);

    ASSERT_EQ(i_last.msgPublishPolicy(), InputNodePolicies::MsgPublishPolicy::LAST);

    i_last.registerOutput("output");
    auto o_p = i_last.getSinglePort("output");
    ASSERT_NE(o_p, nullptr);

    i_p.subscribeTo(o_p);

    i_last.compute();
    ASSERT_NE(msg_got, nullptr);
    ASSERT_EQ(msg_got->value, 1);
    msg_got_back = msg_got;

    i_last.compute();
    ASSERT_NE(msg_got, msg_got_back);
    ASSERT_EQ(msg_got->value, 2);

    // msg queue is full in port data handle, so new msg created in compute should be dropped
    msg_got_back = msg_got;
    i_last.compute();
    ASSERT_EQ(msg_got, msg_got_back);
    ASSERT_EQ(msg_got->value, 2);

    //// ALL
    const vector_test_msg* msg_got_list = nullptr;

    std::function<void(const vector_test_msg*)> f_list = [&](const vector_test_msg* a) { msg_got_list = a; };

    InputPort<vector_test_msg, vector_test_msg> i_p_list("input_port_list", &n_o, f_list);

    TestInputNode i_all("i_all", InputNodePolicies::MsgPublishPolicy::ALL,
                             InputNodePolicies::MsgCachePolicy::KEEP_CACHE, 2);

    i_all.registerOutput("output");
    auto o_p_list = i_all.getListPort("output");
    ASSERT_NE(o_p_list, nullptr);
    i_p_list.subscribeTo(o_p_list);

    i_all.compute();
    ASSERT_EQ(msg_got_list->size(), 1);
    ASSERT_EQ(msg_got_list->back()->value, 1);

    i_all.compute();
    ASSERT_EQ(msg_got_list->size(), 2);
    ASSERT_EQ(msg_got_list->back()->value, 2);
}

TEST(ComputationalNodes, INPUT_NODE_UPDATE_POLICY_WITH_CLEAR_CACHE)
{
    using vector_test_msg = std::vector<const TestMsg*>;

    TestNode n_o("output", ComputationalNode::Output);

    //// LAST
    const TestMsg* msg_got = nullptr;
    std::function<void(const TestMsg*)> f = [&](const TestMsg* a) { msg_got = a; };
    InputPort<TestMsg, TestMsg> i_p("input_port", &n_o, f);

    TestInputNode i_last("i_last", InputNodePolicies::MsgPublishPolicy::LAST,
                             InputNodePolicies::MsgCachePolicy::CLEAR_CACHE, 2);

    ASSERT_EQ(i_last.msgCachePolicy(), InputNodePolicies::MsgCachePolicy::CLEAR_CACHE);

    i_last.registerOutput("output");
    i_p.subscribeTo(i_last.getSinglePort("output"));

    i_last.compute();
    ASSERT_NE(msg_got, nullptr);
    ASSERT_EQ(msg_got->value, 1);

    i_last.stopUpdating();

    i_last.compute();
    ASSERT_EQ(msg_got, nullptr);

    //// ALL
    const vector_test_msg* msg_got_list = nullptr;
    std::function<void(const vector_test_msg*)> f_list = [&](const vector_test_msg* a) { msg_got_list = a; };
    InputPort<vector_test_msg, vector_test_msg> i_p_list("input_port_list", &n_o, f_list);

    TestInputNode i_all("i_all", InputNodePolicies::MsgPublishPolicy::ALL,
                            InputNodePolicies::MsgCachePolicy::CLEAR_CACHE, 2);

    i_all.registerOutput("output");
    i_p_list.subscribeTo(i_all.getListPort("output"));

    i_all.compute();
    ASSERT_EQ(msg_got_list->size(), 1);
    ASSERT_EQ(msg_got_list->back()->value, 1);

    i_all.stopUpdating();

    i_all.compute();
    ASSERT_EQ(msg_got_list, nullptr);
}

//// OUTPUT NODE

TEST(ComputationalNodes, OUTPUT_NODE) {
    TestNode n_i("input", ComputationalNode::Input);
    OutputPort<TestMsg> o_p("output_port", &n_i);
    TestMsg msg_send;

    //// getOrRegisterInput
    TestOutputNode n_o("output", OutputNodePolicies::PublishFormatPolicy::SERIES,
                       false, 1, 1);
    auto i_p1 = n_o.getOrRegisterInput<TestMsg>("input");
    auto i_p2 = n_o.getOrRegisterInput<TestMsg>("input");

    ASSERT_NE(i_p1, nullptr);
    ASSERT_EQ(i_p1, i_p2);

    ASSERT_THROW(n_o.getOrRegisterInput<int>("input");, NRPException);

    i_p1->subscribeTo(&o_p);

    ASSERT_THROW(i_p1->subscribeTo(&o_p), NRPException);

    n_o.configure();

    ASSERT_EQ(n_o.getOrRegisterInput<TestMsg>("input"), nullptr);

    //// compute, SERIES
    o_p.publish(&msg_send);

    ASSERT_EQ(n_o.sendSingleMsgCalled, false);
    ASSERT_EQ(n_o.sendBatchMsgCalled, false);
    ASSERT_EQ(n_o.sent_msgs.size(), 0);

    n_o.compute();

    ASSERT_EQ(n_o.sendSingleMsgCalled, true);
    ASSERT_EQ(n_o.sendBatchMsgCalled, false);
    ASSERT_EQ(n_o.sent_msgs.size(), 1);
    ASSERT_EQ(n_o.sent_msgs.at(0), &msg_send);

    //// compute, BATCH
    TestOutputNode n_o2("output", OutputNodePolicies::PublishFormatPolicy::BATCH,
                        false, 1, 1);
    n_o2.getOrRegisterInput<TestMsg>("input")->subscribeTo(&o_p);
    n_o2.configure();

    o_p.publish(&msg_send);

    ASSERT_EQ(n_o2.sendSingleMsgCalled, false);
    ASSERT_EQ(n_o2.sendBatchMsgCalled, false);
    ASSERT_EQ(n_o2.sent_msgs.size(), 0);

    n_o2.compute();

    ASSERT_EQ(n_o2.sendSingleMsgCalled, false);
    ASSERT_EQ(n_o2.sendBatchMsgCalled, true);
    ASSERT_EQ(n_o2.sent_msgs.size(), 1);
    ASSERT_EQ(n_o2.sent_msgs.at(0), &msg_send);

    //// compute period
    ASSERT_EQ(n_o.getComputePeriod(), 1);
    n_o.graphCycleStartCB();
    ASSERT_EQ(n_o.doCompute(), true);
    n_o.setComputePeriod(2);
    ASSERT_EQ(n_o.getComputePeriod(), 2);
    n_o.graphCycleStartCB();
    ASSERT_EQ(n_o.doCompute(), false);
    n_o.graphCycleStartCB();
    ASSERT_EQ(n_o.doCompute(), true);
    n_o.setComputePeriod(0);
    ASSERT_EQ(n_o.doCompute(), false);
    n_o.graphCycleStartCB();
    ASSERT_EQ(n_o.doCompute(), false);
    n_o.setDoCompute(true);
    ASSERT_EQ(n_o.doCompute(), true);

    //// compute period and publish from cache constraints
    TestOutputNode n_o3("output", OutputNodePolicies::PublishFormatPolicy::SERIES,
                        true, 2, 1);

    ASSERT_EQ(n_o3._maxPortConnections, 1);

    TestOutputNode n_o4("output", OutputNodePolicies::PublishFormatPolicy::SERIES,
                        false, 2, 2);

    ASSERT_EQ(n_o4._maxPortConnections, 1);

    TestOutputNode n_o5("output", OutputNodePolicies::PublishFormatPolicy::SERIES,
                        false, 2, 1);
    n_o5.setComputePeriod(2);

    ASSERT_EQ(n_o5._maxPortConnections, 2);
    ASSERT_EQ(n_o5.getComputePeriod(), 1);

    //// compute "publish from cache"
    n_o.resetCalls();
    n_o3.getOrRegisterInput<TestMsg>("input")->subscribeTo(&o_p);
    n_o3.configure();

    o_p.publish(&msg_send);
    n_o.compute();
    n_o3.compute();

    ASSERT_EQ(n_o.sent_msgs.size(), 1);
    ASSERT_EQ(n_o3.sent_msgs.size(), 1);

    n_o.compute();
    n_o3.compute();

    ASSERT_EQ(n_o.sent_msgs.size(), 1);
    ASSERT_EQ(n_o3.sent_msgs.size(), 2);

    o_p.publish(&msg_send);
    o_p.publish(&msg_send);
    n_o.compute();
    n_o3.compute();

    ASSERT_EQ(n_o.sent_msgs.size(), 2);
    ASSERT_EQ(n_o3.sent_msgs.size(), 3);

    //// compute period
    n_o4.getOrRegisterInput<TestMsg>("input")->subscribeTo(&o_p);
    n_o4.configure();

    o_p.publish(&msg_send);
    n_o4.compute();
    ASSERT_EQ(n_o4.sent_msgs.size(), 0);
    n_o4.graphCycleStartCB();
    n_o4.compute();
    ASSERT_EQ(n_o4.sent_msgs.size(), 1);
    o_p.publish(&msg_send);
    n_o4.graphCycleStartCB();
    n_o4.compute();
    ASSERT_EQ(n_o4.sent_msgs.size(), 1);
    n_o4.graphCycleStartCB();
    n_o4.compute();
    ASSERT_EQ(n_o4.sent_msgs.size(), 2);
}

//// FUNCTIONAL NODE

TEST(ComputationalNodes, FUNCTIONAL_NODE)
{
    TestNode n1("input", ComputationalNode::Input);
    TestNode n2("output", ComputationalNode::Output);

    int msg_send = 1;
    const int* msg_got = nullptr;
    int nCalledP = 0;
    std::function<void(const int*)> f = [&](const int* a) { msg_got = a; nCalledP++; };

    OutputPort<int> o_p("output_port", &n1);
    InputPort<int, int> i_p("input_port", &n2, f);

    int nCalledF = 0;
    bool returnF = true;
    std::function<bool(const int*, int&)> f1 = [&](const int*i1, int&o1) {
        if(i1 != nullptr)
            o1 = *i1;
        nCalledF++;
        return returnF;
    };

    // Instantiating FunctionalNode directly, only for testing, usually this is done via FunctionalNodeFactory::create
    auto f_wrap = [f1](std::tuple<const int*, int> &p) {
        return std::apply(f1, p);
    };
    FunctionalNode<std::tuple<int>, std::tuple<int>> f_n("f_node", f_wrap, FunctionalNodePolicies::ON_NEW_INPUT);

    //// Register and get input / output
    auto i_pn = f_n.registerInput<0, int, int>("input");
    auto o_pn = f_n.registerOutput<0, int>("output");

    ASSERT_THROW((f_n.registerInput<0, int, int>("input")), NRPException);
    ASSERT_THROW((f_n.registerOutput<0, int>("output")), NRPException);

    ASSERT_THROW((f_n.registerInput<0, int, int>("input_other")), NRPException);
    ASSERT_THROW((f_n.registerOutput<0, int>("output_other")), NRPException);

    ASSERT_THROW((f_n.getInputByIndex(1)), NRPException);

    auto i_pn_test = f_n.getInputByIndex(0);
    auto o_pn_test = f_n.getOutputByIndex<0>();

    ASSERT_EQ(i_pn, i_pn_test);
    ASSERT_EQ(o_pn, o_pn_test);

    i_pn_test = f_n.getInputById("input_another");
    o_pn_test = f_n.getOutputById("output_another");

    ASSERT_EQ(i_pn_test, nullptr);
    ASSERT_EQ(o_pn_test, nullptr);

    i_pn_test = f_n.getInputById("input");
    o_pn_test = f_n.getOutputById("output");

    ASSERT_EQ(i_pn, i_pn_test);
    ASSERT_EQ(o_pn, o_pn_test);

    //// Compute
    i_p.subscribeTo(o_pn);
    i_pn->subscribeTo(&o_p);

    f_n.compute();
    ASSERT_EQ(nCalledF, 0);
    ASSERT_EQ(nCalledP, 0);

    o_p.publish(&msg_send);
    f_n.compute();
    ASSERT_EQ(nCalledF, 1);
    ASSERT_EQ(nCalledP, 1);
    ASSERT_EQ(*msg_got, msg_send);

    f_n.compute();
    ASSERT_EQ(nCalledF, 1);
    ASSERT_EQ(nCalledP, 1);

    f_n._execPolicy = FunctionalNodePolicies::ALWAYS;
    nCalledP = 0;
    nCalledF = 0;

    f_n.compute();
    ASSERT_EQ(nCalledP, 1);
    ASSERT_EQ(nCalledF, 1);

    o_p.publish(&msg_send);
    f_n.compute();
    ASSERT_EQ(nCalledP, 2);
    ASSERT_EQ(nCalledF, 2);
    ASSERT_EQ(*msg_got, msg_send);

    f_n.compute();
    ASSERT_EQ(nCalledP, 3);
    ASSERT_EQ(nCalledF, 3);

    // case FN function returns false
    returnF = false;
    f_n.compute();
    ASSERT_EQ(nCalledP, 3);
    ASSERT_EQ(nCalledF, 4);

}


TEST(ComputationalNodes, FUNCTIONAL_NODE_FACTORY)
{
    // Not much to test here, because if FunctionalNodeFactory::create is called with the wrong
    // template arguments or function signature the code just won't compile, so just calling the
    // function.

    std::function<bool(const int*, int&)> f1 = [](const int*i1, int&o1) {
        if(i1 != nullptr)
            o1 = *i1;

        return true;
    };

    std::shared_ptr<FunctionalNode<std::tuple<int>, std::tuple<int>>> f_n(FunctionalNodeFactory::create<1, 1, const int*, int&>("f_node", f1));
    f_n->compute();
}

TEST(ComputationalNodes, FN_FACTORY_MANAGER)
{
    auto& fn_manager = FunctionalNodeFactoryManager::getInstance();
    fn_manager.loadFNFactoryPlugin("libFNFactoryModule.so");
    std::shared_ptr<FunctionalNodeBase> f_n(fn_manager.createFunctionalNode("my_function", "my_node",
                                                                           FunctionalNodePolicies::ExecutionPolicy::ON_NEW_INPUT));

    auto i_pn = dynamic_cast<InputPort<int, int>*>(f_n->getInputById("i1"));
    auto o_pn = dynamic_cast<OutputPort<int>*>(f_n->getOutputById("o1"));

    TestNode n1("input", ComputationalNode::Input);
    TestNode n2("output", ComputationalNode::Output);

    int msg_send = 1;
    const int* msg_got = nullptr;
    std::function<void(const int*)> f = [&](const int* a) { msg_got = a; };

    OutputPort<int> o_p("output_port", &n1);
    InputPort<int, int> i_p("input_port", &n2, f);

    i_p.subscribeTo(o_pn);
    i_pn->subscribeTo(&o_p);

    o_p.publish(&msg_send);
    f_n->compute();
    ASSERT_EQ(*msg_got, msg_send);
}

// EOF
