#include <boost/python.hpp>
#include <boost/python/scope.hpp>
#include <boost/python/enum.hpp>

#include "nrp_event_loop/computational_graph/computational_node_policies.h"
#include "nrp_event_loop/computational_graph/functional_node.h"
#include "nrp_event_loop/python/functional_node.h"
#include "nrp_event_loop/nodes/dummy/input_dummy.h"
#include "nrp_event_loop/nodes/dummy/output_dummy.h"
#include "nrp_event_loop/nodes/engine/input_node.h"
#include "nrp_event_loop/nodes/engine/output_node.h"
#include "nrp_event_loop/nodes/time/input_time.h"

#include "nrp_event_loop/fn_factory/functional_node_factory_manager.h"

#ifdef SPINNAKER_ON
#include "nrp_event_loop/nodes/spinnaker/input_node.h"
#include "nrp_event_loop/nodes/spinnaker/output_node.h"
#endif

#ifdef ROS_ON
#include "nrp_event_loop/nodes/ros/ros_edge_factory.h"
#endif

#ifdef MQTT_ON
#include "nrp_event_loop/nodes/mqtt/mqtt_edge_factory.h"
#endif

#include "nrp_event_loop/config/cmake_constants.h"


/*!
 * \brief Helper function for instantiating a C++ Functional Node from Python
 *
 * \param fnModuleName name of the module library which will be used to instantiate the node
 * \param functionName name of the C++ function which the node will run
 * \param nodeName name of the FN
 * \param policy exec policy of the FN
 * \return instantiated FN wrapped in a Python object
 */
boost::python::object createFNFromFactoryModule(const std::string& fnModuleName, const std::string& functionName, const std::string &nodeName,
                                           FunctionalNodePolicies::ExecutionPolicy policy)
{
    auto& fn_manager = FunctionalNodeFactoryManager::getInstance();
    fn_manager.loadFNFactoryPlugin(fnModuleName);
    std::shared_ptr<FunctionalNodeBase> fn(fn_manager.createFunctionalNode(functionName, nodeName, policy));

    std::shared_ptr<ComputationalNode> fn_base = std::dynamic_pointer_cast<ComputationalNode>(fn);
    ComputationalGraphManager::getInstance().registerNode(fn_base);

    return boost::python::object(fn);
}

class node_policies_ns{

public:

    class input_node_ns{};
    class functional_node_ns{};
    class output_node_ns{};
};

BOOST_PYTHON_MODULE(EVENT_LOOP_PYTHON_MODULE_NAME)
{
    // node policies enums
    {
        bpy::scope node_policies_scope =  bpy::class_<node_policies_ns>("node_policies");

        {
            bpy::scope input_policies_scope =  bpy::class_<node_policies_ns::input_node_ns>("input_node");

            bpy::enum_<InputNodePolicies::MsgCachePolicy>("msg_cache")
                    .value("clear", InputNodePolicies::MsgCachePolicy::CLEAR_CACHE)
                    .value("keep", InputNodePolicies::MsgCachePolicy::KEEP_CACHE)
                    .export_values();

            bpy::enum_<InputNodePolicies::MsgPublishPolicy>("msg_publish")
                    .value("last", InputNodePolicies::MsgPublishPolicy::LAST)
                    .value("all", InputNodePolicies::MsgPublishPolicy::ALL)
                    .export_values();
        }

        {
            bpy::scope fn_policies_scope =  bpy::class_<node_policies_ns::functional_node_ns>("functional_node");

            bpy::enum_<FunctionalNodePolicies::ExecutionPolicy>("exec_policy")
                    .value("on_new_message", FunctionalNodePolicies::ExecutionPolicy::ON_NEW_INPUT)
                    .value("always", FunctionalNodePolicies::ExecutionPolicy::ALWAYS)
                    .export_values();
        }

        {
            bpy::scope output_policies_scope =  bpy::class_<node_policies_ns::output_node_ns>("output_node");

            bpy::enum_<OutputNodePolicies::PublishFormatPolicy>("publish_format")
                    .value("series", OutputNodePolicies::PublishFormatPolicy::SERIES)
                    .value("batch", OutputNodePolicies::PublishFormatPolicy::BATCH)
                    .export_values();
        }
    }

    // basic decorators
    bpy::class_<FunctionalNodeBase, std::shared_ptr<FunctionalNodeBase>>("FunctionalNodeBase", bpy::no_init);

    bpy::class_<PythonFunctionalNode, bpy::bases<FunctionalNodeBase>>("FunctionalNode",
                                      bpy::init<const std::string &, const boost::python::list &, FunctionalNodePolicies::ExecutionPolicy>(
                                              (bpy::arg("name"),
                                                      bpy::arg("outputs"),
                                                      bpy::arg("exec_policy") = FunctionalNodePolicies::ExecutionPolicy::ON_NEW_INPUT)))
            .def("__call__", &PythonFunctionalNode::pySetup);

    bpy::class_<F2FEdge>("FromFunctionalNode", bpy::init<const std::string &, const std::string &>( (bpy::arg("keyword"), bpy::arg("address")) ))
            .def("__call__", &F2FEdge::pySetup);

    bpy::class_< InputDummyEdge >("InputDummy", bpy::init<const std::string &, const std::string &, boost::python::object>((bpy::arg("keyword"), bpy::arg("address"), bpy::arg("value")) ))
            .def("__call__", &InputDummyEdge::pySetup);

    bpy::class_< OutputDummyEdge >("OutputDummy", bpy::init<const std::string &, const std::string &,
                                   bool, unsigned int>(
                                           (bpy::arg("keyword"), bpy::arg("address"),
                                                   bpy::arg("publish_from_cache") = false,
                                                   bpy::arg("compute_period") = 1) ))
            .def("__call__", &OutputDummyEdge::pySetup);

    bpy::register_ptr_to_python<std::shared_ptr<PythonFunctionalNode> >();
    
    // other node types decorators
    bpy::class_< InputEngineEdge >("FromEngine", bpy::init<const std::string &, const std::string &,
                                   InputNodePolicies::MsgCachePolicy>((bpy::arg("keyword"), bpy::arg("address"), bpy::arg("cache_policy") = InputNodePolicies::KEEP_CACHE) ))
            .def("__call__", &InputEngineEdge::pySetup);

    bpy::class_< OutputEngineEdge >("ToEngine", bpy::init<const std::string &, const std::string &>( (bpy::arg("keyword"), bpy::arg("address")) ))
            .def("__call__", &OutputEngineEdge::pySetup);

    bpy::class_< InputClockEdge >("Clock", bpy::init<const std::string &>((bpy::arg("keyword")) ))
            .def("__call__", &InputClockEdge::pySetup);

    bpy::class_< InputIterationEdge >("Iteration", bpy::init<const std::string &>((bpy::arg("keyword")) ))
            .def("__call__", &InputIterationEdge::pySetup);

    // CPP FN create wrapper
    bpy::def("createFNFromFactoryModule",  createFNFromFactoryModule, bpy::args("module_name", "function_name", "node_name","exec_policy"));

#ifdef ROS_ON
    bpy::class_< RosEdgeFactory >("RosSubscriber", bpy::init<const std::string &, const std::string &,
                                  const bpy::object &, InputNodePolicies::MsgPublishPolicy, InputNodePolicies::MsgCachePolicy>(
                                          (bpy::arg("keyword"), bpy::arg("address"), bpy::arg("type"),
                                                  bpy::arg("publish_policy") =  InputNodePolicies::LAST,
                                                  bpy::arg("cache_policy") =  InputNodePolicies::KEEP_CACHE) ))
            .def("__call__", &RosEdgeFactory::pySetupSelector);

    bpy::class_< RosEdgeFactoryOutput >("RosPublisher", bpy::init<const std::string &, const std::string &,
                                        const bpy::object &, bool, unsigned int>(
                                                (bpy::arg("keyword"), bpy::arg("address"),
                                                        bpy::arg("type"),
                                                        bpy::arg("publish_from_cache") = false,
                                                        bpy::arg("compute_period") = 1) ))
            .def("__call__", &RosEdgeFactoryOutput::pySetupSelector);
#endif

#ifdef MQTT_ON
    bpy::class_< MqttEdgeFactory >("MQTTSubscriber", bpy::init<const std::string &, const std::string &,
            const bpy::object &, InputNodePolicies::MsgPublishPolicy, InputNodePolicies::MsgCachePolicy>(
            (bpy::arg("keyword"), bpy::arg("address"), bpy::arg("type"),
                    bpy::arg("publish_policy") =  InputNodePolicies::LAST,
                    bpy::arg("cache_policy") =  InputNodePolicies::KEEP_CACHE) ))
            .def("__call__", &MqttEdgeFactory::pySetupSelector);

    bpy::class_< MqttEdgeFactoryOutput >("MQTTPublisher", bpy::init<const std::string &, const std::string &,
                                         const bpy::object &, bool, unsigned int>(
                                                 (bpy::arg("keyword"), bpy::arg("address"),
                                                         bpy::arg("type"),
                                                         bpy::arg("publish_from_cache") = false,
                                                         bpy::arg("compute_period") = 1) ))
            .def("__call__", &MqttEdgeFactoryOutput::pySetupSelector);
#endif

#ifdef SPINNAKER_ON
    bpy::class_< InputSpinnakerEdge >("FromSpinnaker", bpy::init<const std::string &, const std::string &,
            InputNodePolicies::MsgPublishPolicy, InputNodePolicies::MsgCachePolicy>(
                    (bpy::arg("keyword"), bpy::arg("address"),
                            bpy::arg("publish_policy") =  InputNodePolicies::LAST,
                            bpy::arg("cache_policy") =  InputNodePolicies::KEEP_CACHE) ))
            .def("__call__", &InputSpinnakerEdge::pySetup);

    bpy::class_< OutputSpinnakerEdge >("ToSpinnaker", bpy::init<const std::string &, const std::string &,
                                       bool, unsigned int>(
                                               (bpy::arg("keyword"), bpy::arg("address"),
                                                       bpy::arg("publish_from_cache") = false,
                                                       bpy::arg("compute_period") = 1) ))
            .def("__call__", &OutputSpinnakerEdge::pySetup);
#endif
}
