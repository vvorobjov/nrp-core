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

#include <functional>
#include <thread>
#include <mutex>

#include <gtest/gtest.h>

#include "nrp_event_loop/computational_graph/computational_node.h"
#include "nrp_event_loop/computational_graph/computational_graph.h"
#include "nrp_event_loop/computational_graph/computational_graph_manager.h"

#include "nrp_event_loop/computational_graph/input_port.h"
#include "nrp_event_loop/computational_graph/output_port.h"

#include "nrp_event_loop/computational_graph/input_node.h"
#include "nrp_event_loop/computational_graph/output_node.h"

#include "tests/test_files/helper_classes.h"

//// Computational Graph

TEST(ComputationalGraph, COMPUTATIONAL_GRAPH)
{
    std::vector<shared_ptr<TestNode>> nodes;
    nodes.push_back(std::make_shared<TestNode>("1", ComputationalNode::Input));
    nodes.push_back(std::make_shared<TestNode>("2", ComputationalNode::Functional));
    nodes.push_back(std::make_shared<TestNode>("3", ComputationalNode::Functional));
    nodes.push_back(std::make_shared<TestNode>("4", ComputationalNode::Functional));
    nodes.push_back(std::make_shared<TestNode>("5", ComputationalNode::Functional));
    nodes.push_back(std::make_shared<TestNode>("6", ComputationalNode::Output));

    ComputationalGraph cg;

    TestNode::compOrder.clear();

    cg.insert_edge(nodes.at(0).get(), nodes.at(1).get());
    cg.insert_edge(nodes.at(0).get(), nodes.at(2).get());
    cg.insert_edge(nodes.at(1).get(), nodes.at(3).get());
    cg.insert_edge(nodes.at(2).get(), nodes.at(4).get());
    cg.insert_edge(nodes.at(3).get(), nodes.at(5).get());
    cg.insert_edge(nodes.at(4).get(), nodes.at(5).get());

    // insert illegal edges
    ASSERT_THROW(cg.insert_edge(nodes.at(1).get(), nodes.at(0).get()), NRPException);
    ASSERT_THROW(cg.insert_edge(nodes.at(5).get(), nodes.at(1).get()), NRPException);
    ASSERT_THROW(cg.insert_edge(nodes.at(0).get(), nodes.at(0).get()), NRPException);

    // Configure
    cg.configure();
    ASSERT_EQ(cg.getState(),ComputationalGraph::READY);
    for(auto& e : nodes)
        ASSERT_EQ(e->isConfigured, true);

    // Compute: node execution order
    cg.compute();
    ASSERT_EQ(cg.getState(),ComputationalGraph::READY);
    ASSERT_EQ(TestNode::compOrder.at(0),"1");
    ASSERT_TRUE(TestNode::compOrder.at(1) == "2" || TestNode::compOrder.at(1) == "3");
    ASSERT_TRUE(TestNode::compOrder.at(2) == "2" || TestNode::compOrder.at(2) == "3");
    ASSERT_TRUE(TestNode::compOrder.at(3) == "4" || TestNode::compOrder.at(3) == "5");
    ASSERT_TRUE(TestNode::compOrder.at(4) == "4" || TestNode::compOrder.at(4) == "5");
    ASSERT_EQ(TestNode::compOrder.at(5),"6");

    // Loop detection
    cg.clear();
    cg.insert_edge(nodes.at(1).get(), nodes.at(2).get());
    cg.insert_edge(nodes.at(2).get(), nodes.at(3).get());
    cg.insert_edge(nodes.at(3).get(), nodes.at(1).get());
    ASSERT_THROW(cg.configure(), NRPException);

    // FSM
    cg.clear();
    cg.insert_edge(nodes.at(0).get(), nodes.at(1).get());
    ASSERT_EQ(cg.getState(),ComputationalGraph::EMPTY);
    ASSERT_THROW(cg.compute(), NRPException);

    nodes.at(0)->doBlockExec = true;
    std::thread t1(&ComputationalGraph::configure, &cg);
    std::unique_lock<std::mutex> lk1(TestNode::m);
    TestNode::cv.wait(lk1, []{return TestNode::isExecuting;});
    lk1.unlock();
    ASSERT_EQ(cg.getState(),ComputationalGraph::CONFIGURING);
    ASSERT_THROW(cg.clear(), NRPException);
    ASSERT_THROW(cg.configure(), NRPException);
    ASSERT_THROW(cg.compute(), NRPException);

    nodes.at(0)->doBlockExec = false;
    t1.join();
    ASSERT_EQ(cg.getState(),ComputationalGraph::READY);
    ASSERT_THROW(cg.configure(), NRPException);

    nodes.at(0)->doBlockExec = true;
    std::thread t2(&ComputationalGraph::compute, &cg);
    std::unique_lock<std::mutex> lk2(TestNode::m);
    TestNode::cv.wait(lk2, []{return TestNode::isExecuting;});
    lk2.unlock();
    ASSERT_EQ(cg.getState(),ComputationalGraph::COMPUTING);
    ASSERT_THROW(cg.clear(), NRPException);
    ASSERT_THROW(cg.configure(), NRPException);
    ASSERT_THROW(cg.compute(), NRPException);

    nodes.at(0)->doBlockExec = false;
    t2.join();
    ASSERT_EQ(cg.getState(),ComputationalGraph::READY);

    cg.clear();
}

TEST(ComputationalGraph, COMPUTATIONAL_GRAPH_MANAGER)
{
    ComputationalGraphManager::resetInstance();
    auto& cgm = ComputationalGraphManager::getInstance();

    std::shared_ptr<ComputationalNode> n1(new TestNode("input", ComputationalNode::Input));
    std::shared_ptr<ComputationalNode> n2(new TestNode("output", ComputationalNode::Output));
    std::shared_ptr<ComputationalNode> n3(new TestNode("functional", ComputationalNode::Functional));
    std::shared_ptr<ComputationalNode> n4(new TestNode("input", ComputationalNode::Output));
    std::shared_ptr<ComputationalNode> n5(new TestNode("functional", ComputationalNode::Functional));
    std::shared_ptr<ComputationalNode> n6(new TestNode("input", ComputationalNode::Input));
    std::shared_ptr<ComputationalNode> n7(new TestNode("output", ComputationalNode::Output));

    cgm.registerNode(n1);
    cgm.registerNode(n2);
    cgm.registerNode(n3);

    //// register node
    // same name different type
    ASSERT_THROW(cgm.registerNode(n4), NRPException);
    // same name functional
    ASSERT_THROW(cgm.registerNode(n5), NRPException);
    // same name input or output
    cgm.registerNode(n6);
    ASSERT_EQ(n1.get(), n6.get());
    cgm.registerNode(n7);
    ASSERT_EQ(n2.get(), n7.get());

    //// get node
    ASSERT_EQ(n1.get(), cgm.getNode("input"));
    ASSERT_EQ(n2.get(), cgm.getNode("output"));
    ASSERT_EQ(n3.get(), cgm.getNode("functional"));
    ASSERT_EQ(nullptr, cgm.getNode("another"));

    //// register edge
    // normal case
    std::shared_ptr<ComputationalNode> n10(new TestInputNode("input_2", InputNodePolicies::MsgPublishPolicy::LAST,
                                                             InputNodePolicies::MsgCachePolicy::KEEP_CACHE, 1));
    std::shared_ptr<ComputationalNode> n11(new TestOutputNode("output_2", OutputNodePolicies::MsgPublishPolicy::SERIES, 1));

    cgm.registerNode(n10);
    cgm.registerNode(n11);

    TestInputNode* n10_p = dynamic_cast<TestInputNode*>(n10.get());
    n10_p->registerOutput("output");
    auto o_p = n10_p->getSinglePort("output");
    TestOutputNode* n11_p = dynamic_cast<TestOutputNode*>(n11.get());
    auto i_p = n11_p->getOrRegisterInput<TestMsg>("input");

    cgm.registerEdge(o_p, i_p);
    cgm.configure();
    cgm.compute();

    ASSERT_EQ(n10_p->msg_v.size(), 1);
    ASSERT_EQ(n11_p->sent_msgs.size(), 1);
    ASSERT_EQ(n10_p->msg_v.at(0).get(), n11_p->sent_msgs.at(0));

    // unregistered nodes
    std::shared_ptr<ComputationalNode> n8(new TestNode("input_other", ComputationalNode::Input));
    std::shared_ptr<ComputationalNode> n9(new TestNode("output_other", ComputationalNode::Output));

    std::function<void(const int*)> f = [](const int* /*a*/) { };
    OutputPort<int> o_p2("output_port", n8.get());
    InputPort<int, int> i_p2("input_port", n9.get(), f);

    ASSERT_THROW(cgm.registerEdge(&o_p2, &i_p2);, NRPException);
}

// EOF
