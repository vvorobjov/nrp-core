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

#include <gtest/gtest.h>

#include "nrp_event_loop/computational_graph/computational_node.h"

#include "nrp_event_loop/computational_graph/input_port.h"
#include "nrp_event_loop/computational_graph/output_port.h"

#include "tests/test_files/helper_classes.h"

//// PORTS

TEST(ComputationalGraphPorts, PORT_SUBSCRIPTORS)
{
    TestNode n1("input", ComputationalNode::Input);
    TestNode n2("output", ComputationalNode::Output);

    std::function<void(const int*)> f = [&](const int*) { };

    OutputPort<int> o_p("output_port", &n1);
    InputPort<int, int> i_p("input_port", &n2, f, 1);

    ASSERT_EQ(o_p.subscriptionsSize(), 0);
    ASSERT_EQ(i_p.subscriptionsSize(), 0);

    i_p.subscribeTo(&o_p);

    ASSERT_EQ(o_p.subscriptionsSize(), 1);
    ASSERT_EQ(i_p.subscriptionsSize(), 1);

    ASSERT_THROW(i_p.subscribeTo(&o_p), NRPException);
}


TEST(ComputationalGraphPorts, PORT_PUBLISH_NO_CONVERSION)
{
    // Create test nodes
    TestNode n1("input", ComputationalNode::Input);
    TestNode n2("output", ComputationalNode::Output);

    // Create msgs and callback function
    int msg_send = 1;
    const int* msg_got = nullptr;
    std::function<void(const int*)> f = [&](const int* a) { msg_got = a; };

    // Create and connect ports
    OutputPort<int> o_p("output_port", &n1);
    InputPort<int, int> i_p("input_port", &n2, f);
    i_p.subscribeTo(&o_p);

    // Test publishing
    o_p.publish(&msg_send);
    ASSERT_EQ(msg_got, &msg_send);

    // nullptr case
    o_p.publish(nullptr);
    ASSERT_EQ(msg_got, nullptr);
}

TEST(ComputationalGraphPorts, PORT_PUBLISH_CONVERSION)
{
    TestNode n1("input", ComputationalNode::Input);
    TestNode n2("output", ComputationalNode::Output);

    int msg_send = 1;
    const TestMsg* msg_got = nullptr;
    std::function<void(const TestMsg*)> f = [&](const TestMsg* a) { msg_got = a; };

    OutputPort<int> o_p("output_port", &n1);
    InputPort<int, TestMsg> i_p("input_port", &n2, f);
    i_p.subscribeTo(&o_p);

    o_p.publish(&msg_send);
    ASSERT_EQ(msg_got->value, msg_send);

    // nullptr case
    o_p.publish(nullptr);
    ASSERT_EQ(msg_got, nullptr);
}

////////////////////////////////////////////////////////////////////////////////
// Additional coverage added under EBR2-19. The tests above exercise the happy
// 1-to-1 subscribe/publish paths; the tests below pin the accessor methods
// (id, parent, subscriptionsMax) and the fan-out / max-subscribers paths.

TEST(ComputationalGraphPorts, PORT_ID_AND_PARENT_ACCESSORS_RETURN_CONSTRUCTOR_VALUES)
{
    TestNode n("owner", ComputationalNode::Input);
    std::function<void(const int*)> f = [](const int*){};

    OutputPort<int> o_p("out-id", &n);
    InputPort<int, int> i_p("in-id", &n, f);

    EXPECT_EQ(o_p.id(), "out-id");
    EXPECT_EQ(i_p.id(), "in-id");
    EXPECT_EQ(o_p.parent(), &n);
    EXPECT_EQ(i_p.parent(), &n);
}

TEST(ComputationalGraphPorts, INPUT_PORT_SUBSCRIPTIONS_MAX_ZERO_REPORTS_SIZE_MAX)
{
    TestNode n("owner", ComputationalNode::Input);
    std::function<void(const int*)> f = [](const int*){};

    InputPort<int, int> unlimited("p0", &n, f, /*maxSubs=*/0);
    EXPECT_EQ(unlimited.subscriptionsMax(), SIZE_MAX);

    InputPort<int, int> capped("p1", &n, f, /*maxSubs=*/3);
    EXPECT_EQ(capped.subscriptionsMax(), 3u);
}

TEST(ComputationalGraphPorts, INPUT_PORT_ENFORCES_MAX_SUBSCRIBERS)
{
    TestNode source_a("src_a", ComputationalNode::Input);
    TestNode source_b("src_b", ComputationalNode::Input);
    TestNode source_c("src_c", ComputationalNode::Input);
    TestNode sink("sink", ComputationalNode::Output);

    OutputPort<int> a("a", &source_a);
    OutputPort<int> b("b", &source_b);
    OutputPort<int> c("c", &source_c);

    std::function<void(const int*)> f = [](const int*){};
    InputPort<int, int> i_p("sink_port", &sink, f, /*maxSubs=*/2);

    i_p.subscribeTo(&a);
    i_p.subscribeTo(&b);
    ASSERT_EQ(i_p.subscriptionsSize(), 2u);

    EXPECT_THROW(i_p.subscribeTo(&c), NRPException);
    EXPECT_EQ(i_p.subscriptionsSize(), 2u);
}

TEST(ComputationalGraphPorts, OUTPUT_PORT_FANS_OUT_TO_MULTIPLE_INPUT_PORTS)
{
    TestNode src("src", ComputationalNode::Input);
    TestNode snk_a("snk_a", ComputationalNode::Output);
    TestNode snk_b("snk_b", ComputationalNode::Output);

    const int* got_a = nullptr;
    const int* got_b = nullptr;
    std::function<void(const int*)> fa = [&](const int* p){ got_a = p; };
    std::function<void(const int*)> fb = [&](const int* p){ got_b = p; };

    OutputPort<int> out("shared", &src);
    InputPort<int, int> in_a("a", &snk_a, fa);
    InputPort<int, int> in_b("b", &snk_b, fb);

    in_a.subscribeTo(&out);
    in_b.subscribeTo(&out);
    ASSERT_EQ(out.subscriptionsSize(), 2u);

    int payload = 42;
    out.publish(&payload);
    ASSERT_NE(got_a, nullptr);
    ASSERT_NE(got_b, nullptr);
    EXPECT_EQ(*got_a, 42);
    EXPECT_EQ(*got_b, 42);
}

TEST(ComputationalGraphPorts, OUTPUT_PORT_PUBLISH_WITH_ZERO_SUBSCRIBERS_IS_A_NO_OP)
{
    TestNode n("lonely", ComputationalNode::Input);
    OutputPort<int> o_p("solo", &n);
    ASSERT_EQ(o_p.subscriptionsSize(), 0u);

    int v = 7;
    // Must not crash or throw when no subscribers are connected.
    ASSERT_NO_THROW(o_p.publish(&v));
    ASSERT_NO_THROW(o_p.publish(nullptr));
}

// EOF
