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
#include <condition_variable>

#include <gtest/gtest.h>
#include <nlohmann/json.hpp>

#include "nrp_event_loop/computational_graph/computational_node.h"
#include "nrp_event_loop/computational_graph/computational_graph_manager.h"

#include "nrp_event_loop/computational_graph/functional_node_factory.h"

#include "nrp_event_loop/python/functional_node.h"

#include "nrp_event_loop/config/cmake_constants.h"

#include "nrp_event_loop/nodes/dummy/output_dummy.h"

#include "nrp_event_loop/nodes/engine/input_node.h"
#include "nrp_event_loop/nodes/engine/output_node.h"

#include "nrp_general_library/utils/utils.h"

//// Python nodes

TEST(ComputationalGraphPythonNodes, PYTHON_FUNCTIONAL_NODE)
{
    // Setup required elements
    namespace bpy = boost::python;
    ComputationalGraphManager::resetInstance();
    Py_Initialize();
    PyImport_ImportModule(EVENT_LOOP_MODULE_NAME_STR);
    // TODO: it would be better to use constant JSON_PYTHON_MODULE_NAME_STR to import the module, but
    //  #include "nrp_json_engine_protocol/config/cmake_constants.h" fails before nrp-core is installed
    //  figure out how to fix cmake so this header file can be found before nrp-core is installed
    // PyImport_ImportModule(JSON_PYTHON_MODULE_NAME_STR);
    PyImport_ImportModule("nrp_core.data.nrp_json");
    appendPythonPath(TEST_EVENT_LOOP_PYTHON_FUNCTIONS_MODULE_PATH);
    bpy::object test_module(bpy::import("test_functions"));
    bpy::dict test_module_dict(test_module.attr("__dict__"));

    //// Test normal case
    // instantiate a setup node
    bpy::object test_function = test_module_dict["test_function"];
    bpy::list outputs;
    outputs.append("o_1");
    outputs.append("o_2");
    outputs.append("o_3");
    PythonFunctionalNode fn("f_n", outputs);
    std::shared_ptr<PythonFunctionalNode> fn_p =
            bpy::extract<std::shared_ptr<PythonFunctionalNode>>(fn.pySetup(test_function));

    // connect input/output with non-class conversion
    const int* msg_got = nullptr;
    std::function<void(const int*)> f = [&](const int* a) { msg_got = a; };
    InputPort<bpy::object, int> i_p("input_port", fn_p.get(), f);
    auto o_p_fn = fn_p->getOutput("o_1");
    i_p.subscribeTo(o_p_fn);

    InputPort<int, bpy::object>* i_p_fn = fn_p->getOrRegisterInput<int>("i_1");
    OutputPort<int> o_p("output_port", fn_p.get());
    i_p_fn->subscribeTo(&o_p);

    // connect input/output with string conversion
    const std::string* msg_got2 = nullptr;
    std::function<void(const std::string*)> f2 = [&](const std::string* a) { msg_got2 = a; };
    InputPort<bpy::object, std::string> i_p2("input_port", fn_p.get(), f2);
    auto o_p_fn2 = fn_p->getOutput("o_2");
    i_p2.subscribeTo(o_p_fn2);

    InputPort<std::string, bpy::object>* i_p_fn2 = fn_p->getOrRegisterInput<std::string>("i_2");
    OutputPort<std::string> o_p2("output_port", fn_p.get());
    i_p_fn2->subscribeTo(&o_p2);

    // connect input/output with class conversion
    const nlohmann::json * msg_got3 = nullptr;
    std::function<void(const nlohmann::json*)> f3 = [&](const nlohmann::json* a) { msg_got3 = a; };
    InputPort<bpy::object, nlohmann::json> i_p3("input_port", fn_p.get(), f3);
    auto o_p_fn3 = fn_p->getOutput("o_3");
    i_p3.subscribeTo(o_p_fn3);

    InputPort<nlohmann::json, bpy::object>* i_p_fn3 = fn_p->getOrRegisterInput<nlohmann::json>("i_3");
    OutputPort<nlohmann::json> o_p3("output_port", fn_p.get());
    i_p_fn3->subscribeTo(&o_p3);

    // connect to another functional node
    bpy::list outputs2;
    outputs2.append("o_1");
    bpy::object test_function_2 = test_module_dict["test_function_2"];
    PythonFunctionalNode fn_2("f_n_2", outputs2);
    std::shared_ptr<PythonFunctionalNode> fn_p_2 =
            bpy::extract<std::shared_ptr<PythonFunctionalNode>>(fn_2.pySetup(test_function_2));

    fn_p_2->registerF2FEdge("i_1", "/f_n/o_2");
    fn_p_2->configure();

    const std::string* msg_got4 = nullptr;
    std::function<void(const std::string*)> f4 = [&](const std::string* a) { msg_got4 = a; };
    InputPort<bpy::object, std::string> i_p4("input_port", fn_p_2.get(), f4);
    auto o_p_fn4 = fn_p_2->getOutput("o_1");
    i_p4.subscribeTo(o_p_fn4);

    // send and compute
    int msg_send = 1;
    o_p.publish(&msg_send);

    std::string msg_send2 = "hi";
    o_p2.publish(&msg_send2);

    nlohmann::json msg_send3;
    msg_send3["field"] = 3;
    o_p3.publish(&msg_send3);

    fn_p->compute();
    fn_p_2->compute();

    ASSERT_EQ(*msg_got, 1);
    ASSERT_EQ(*msg_got2, "hi");
    ASSERT_EQ(msg_got3->at("field"), 3);
    ASSERT_EQ(*msg_got4, "hi");

    //// I/O register/get
    // Get not declared output
    auto null_port = fn_p->getOutput("another_output");
    ASSERT_EQ(null_port, nullptr);
    // Get or register not declared input
    ASSERT_THROW(fn_p->getOrRegisterInput<nlohmann::json>("another_input"), NRPException);

    //// pythonCallback
    // wrong return type from python function
    bpy::object wrong_type_f = test_module_dict["test_wrong_return_type"];
    PythonFunctionalNode fn2("f_n2", outputs2, FunctionalNodePolicies::ExecutionPolicy::ALWAYS);
    std::shared_ptr<PythonFunctionalNode> fn_p2 =
            bpy::extract<std::shared_ptr<PythonFunctionalNode>>(fn2.pySetup(wrong_type_f));

    ASSERT_THROW(fn_p2->compute(), boost::python::error_already_set);

    // python function returns less than declared outputs
    bpy::object less_f = test_module_dict["test_less_elements"];
    PythonFunctionalNode fn3("f_n3", outputs2, FunctionalNodePolicies::ExecutionPolicy::ALWAYS);
    std::shared_ptr<PythonFunctionalNode> fn_p3 =
            bpy::extract<std::shared_ptr<PythonFunctionalNode>>(fn3.pySetup(less_f));

    ASSERT_THROW(fn_p3->compute(), NRPException);

    // python function returns more than declared outputs
    bpy::object more_f = test_module_dict["test_more_elements"];
    PythonFunctionalNode fn4("f_n4", outputs2, FunctionalNodePolicies::ExecutionPolicy::ALWAYS);
    std::shared_ptr<PythonFunctionalNode> fn_p4 =
            bpy::extract<std::shared_ptr<PythonFunctionalNode>>(fn4.pySetup(more_f));

    ASSERT_THROW(fn_p4->compute(), NRPException);

    // error in python function
    bpy::object broken_f = test_module_dict["test_broken_function"];
    PythonFunctionalNode fn5("f_n5", outputs2, FunctionalNodePolicies::ExecutionPolicy::ALWAYS);
    std::shared_ptr<PythonFunctionalNode> fn_p5 =
            bpy::extract<std::shared_ptr<PythonFunctionalNode>>(fn5.pySetup(broken_f));

    ASSERT_THROW(fn_p5->compute(), boost::python::error_already_set);

    //// register F2F edge
    // wrong node
    PythonFunctionalNode fn_3("f_n_3", outputs2);
    std::shared_ptr<PythonFunctionalNode> fn_p_3 =
            bpy::extract<std::shared_ptr<PythonFunctionalNode>>(fn_3.pySetup(test_function_2));

    fn_p_3->registerF2FEdge("i_1", "/wrong_node/o_2");
    ASSERT_THROW( fn_p_3->configure();, NRPException);

    // wrong port
    PythonFunctionalNode fn_4("f_n_4", outputs2);
    std::shared_ptr<PythonFunctionalNode> fn_p_4 =
            bpy::extract<std::shared_ptr<PythonFunctionalNode>>(fn_4.pySetup(test_function_2));

    fn_p_4->registerF2FEdge("i_1", "/f_n/wrong_port");
    ASSERT_THROW( fn_p_4->configure();, NRPException);

}

TEST(ComputationalGraphPythonNodes, PYTHON_DECORATORS_BASIC)
{
    // Setup required elements
    namespace bpy = boost::python;
    ComputationalGraphManager::resetInstance();
    Py_Initialize();
    PyImport_ImportModule(EVENT_LOOP_MODULE_NAME_STR);
    appendPythonPath(TEST_EVENT_LOOP_PYTHON_FUNCTIONS_MODULE_PATH);

    //// Test normal case
    // load and configure nodes
    bpy::object test_module(bpy::import("test_decorators"));
    bpy::dict test_module_dict(test_module.attr("__dict__"));

    ComputationalGraphManager::getInstance().configure();

    // FN decorator
    auto fn_p = dynamic_cast<PythonFunctionalNode*>(ComputationalGraphManager::getInstance().getNode("function"));

    const int* msg_got = nullptr;
    std::function<void(const int*)> f = [&](const int* a) { msg_got = a; };
    InputPort<bpy::object, int> i_p("input_port", fn_p, f);
    auto o_p_fn = fn_p->getOutput("output1");
    i_p.subscribeTo(o_p_fn);

    InputPort<int, bpy::object>* i_p_fn = fn_p->getOrRegisterInput<int>("input1");
    OutputPort<int> o_p("output_port", fn_p);
    i_p_fn->subscribeTo(&o_p);

    fn_p->compute();
    ASSERT_EQ(bpy::len(test_module_dict["msgs"]), 0);
    int msg_send = 1;
    o_p.publish(&msg_send);
    fn_p->compute();
    ASSERT_EQ(bpy::len(test_module_dict["msgs"]), 1);
    ASSERT_EQ(*msg_got, 1);
    fn_p->compute();
    ASSERT_EQ(bpy::len(test_module_dict["msgs"]), 1);

    // return None case
    msg_got = nullptr;
    test_module_dict["set_return_none"](bpy::object(true));
    o_p.publish(&msg_send);
    ASSERT_NO_THROW(fn_p->compute());
    ASSERT_EQ(msg_got, nullptr);

    // 'always' policy
    auto fn_p_always = dynamic_cast<PythonFunctionalNode*>(ComputationalGraphManager::getInstance().getNode("function_always"));
    fn_p_always->compute();
    ASSERT_EQ(bpy::len(test_module_dict["msgs"]), 2);

    // SimpleInputEdge, SimpleOutputEdge
    ComputationalGraphManager::getInstance().getNode("idummy1")->compute();
    ComputationalGraphManager::getInstance().getNode("function2")->compute();
    auto odummy_p = dynamic_cast<OutputDummy*>(ComputationalGraphManager::getInstance().getNode("odummy1"));
    odummy_p->compute();
    ASSERT_EQ(bpy::extract<int>(*(odummy_p->lastData)), 10);

    // F2F Edge
    ComputationalGraphManager::getInstance().getNode("function3")->compute();
    auto odummy_p2 = dynamic_cast<OutputDummy*>(ComputationalGraphManager::getInstance().getNode("odummy2"));
    odummy_p2->graphCycleStartCB();
    odummy_p2->compute();
    ASSERT_EQ(bpy::extract<int>(*(odummy_p2->lastData)), 10);

    // execution period
    ASSERT_EQ(odummy_p->getComputePeriod(), 1);
    ASSERT_EQ(odummy_p2->getComputePeriod(), 2);
    odummy_p2->call_count = 0;
    int n = 0;
    while(n++ < 4)
        ComputationalGraphManager::getInstance().compute();
    ASSERT_EQ(odummy_p2->call_count, 2); // Called 2 times

    // 'publish from cache' policy
    ASSERT_EQ(odummy_p->publishFromCache(), true);
    ASSERT_EQ(odummy_p2->publishFromCache(), false);

    //// Failing cases
    ComputationalGraphManager::getInstance().clear();
    ASSERT_THROW(bpy::import("wrong_i_port"), boost::python::error_already_set);
    ComputationalGraphManager::getInstance().clear();
    ASSERT_THROW(bpy::import("wrong_o_port"), boost::python::error_already_set);
    ComputationalGraphManager::getInstance().clear();
    ASSERT_THROW(bpy::import("input_no_node"), boost::python::error_already_set);
    ComputationalGraphManager::getInstance().clear();
    ASSERT_THROW(bpy::import("output_no_node"), boost::python::error_already_set);
    ComputationalGraphManager::getInstance().clear();
    ASSERT_THROW(bpy::import("wrong_period_connections"), boost::python::error_already_set);
    ComputationalGraphManager::getInstance().clear();
    ASSERT_THROW(bpy::import("wrong_from_cache_connections"), boost::python::error_already_set);
}

TEST(ComputationalGraphPythonNodes, ENGINE_NODES) {
    // Setup required elements
    namespace bpy = boost::python;
    ComputationalGraphManager::resetInstance();
    Py_Initialize();
    PyImport_ImportModule(EVENT_LOOP_MODULE_NAME_STR);
    appendPythonPath(TEST_EVENT_LOOP_PYTHON_FUNCTIONS_MODULE_PATH);

    //// Test normal case
    // load and configure nodes
    try {
        bpy::import("test_engine_nodes");
    }
    catch (const boost::python::error_already_set &) {
        NRPLogger::error("Test failed when loading test_engine_nodes.py");
        PyErr_Print();
        boost::python::throw_error_already_set();
    }

    ComputationalGraphManager::getInstance().configure();

    // check results
    auto input_p = dynamic_cast<InputEngineNode*>(ComputationalGraphManager::getInstance().getNode("fake_engine_input"));
    auto output_p = dynamic_cast<OutputEngineNode*>(ComputationalGraphManager::getInstance().getNode("fake_engine_output"));

    auto req_devs = input_p->requestedDataPacks();
    ASSERT_EQ(req_devs.size(),1);
    ASSERT_EQ(req_devs.begin()->Name, "my_data_pack");
    ASSERT_EQ(req_devs.begin()->EngineName, "fake_engine");

    datapacks_vector_t dpacks_sent;
    dpacks_sent.push_back(DataPackInterfaceSharedPtr(new DataPackInterface(*(req_devs.begin()))));
    input_p->setDataPacks(dpacks_sent);

    ComputationalGraphManager::getInstance().compute();
    auto dpacks_received = output_p->getDataPacks();
    ASSERT_EQ(dpacks_received.size(),1);
    ASSERT_EQ(dpacks_received.begin()->get()->name(), "my_data_pack");
    ASSERT_EQ(dpacks_received.begin()->get()->engineName(), "fake_engine");

    // cache policy 'keep'
    ComputationalGraphManager::getInstance().compute();
    ASSERT_EQ(output_p->getDataPacks().size(),1);

    // datapack sent with wrong engine name
    auto output_p2 = dynamic_cast<OutputEngineNode*>(ComputationalGraphManager::getInstance().getNode("another_fake_engine_output"));
    ASSERT_EQ(output_p2->getDataPacks().size(),0);

    // cache policy 'clear'
    auto input_p_clear = dynamic_cast<InputEngineNode*>(ComputationalGraphManager::getInstance().getNode("fake_engine_2_input"));
    auto output_p_clear = dynamic_cast<OutputEngineNode*>(ComputationalGraphManager::getInstance().getNode("fake_engine_2_output"));

    datapacks_vector_t dpacks_sent_clear;
    dpacks_sent_clear.push_back(DataPackInterfaceSharedPtr(new DataPackInterface(*(input_p_clear->requestedDataPacks().begin()))));
    input_p_clear->setDataPacks(dpacks_sent_clear);

    ComputationalGraphManager::getInstance().compute();
    ASSERT_EQ(output_p_clear->getDataPacks().size(),1);
    ComputationalGraphManager::getInstance().compute();
    ASSERT_EQ(output_p_clear->getDataPacks().size(),0);

    // check that execution period was set correctly, always 1 for output Engine nodes
    ASSERT_EQ(output_p->getComputePeriod(), 1);
}

// EOF
